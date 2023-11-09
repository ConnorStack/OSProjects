#include <stdlib.h>
#include <stdio.h>

typedef struct page_table_entry
{
    int frame_number;
    int valid_bit;
} page_table_entry;

int find_empty_frame(int[], int);
int select_LRU_victim(int[], int);

void print_freeframes(int[], int);
void print_LRU_count(int[], int);
void print_reverse_map(int[], int);

int main(int argc, char *argv[])
{
    unsigned long LA, PA;

    unsigned int p = 5, f = 3, d = 7;
    unsigned int pnum, fnum, dnum;
    int CLK = 0;
    int freeframes[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    int LRUcount[8] = {0};
    int revmap[8] = {-1};

    page_table_entry PTE[32];
    for (int i = 0; i < 32; i++)
    {
        PTE[i].valid_bit = 0;
    }

    char *infile_arg = argv[1];
    char *outfile_arg = argv[2];

    FILE *infile = fopen(infile_arg, "rb");
    FILE *outfile = fopen(outfile_arg, "wb");

    if (infile == NULL || outfile == NULL)
    {
        perror("Failed to open files\n");
    }

    int freeframes_length = sizeof(freeframes) / sizeof(freeframes[0]);
    int LRUcount_length = sizeof(LRUcount) / sizeof(LRUcount[0]);

    while (1)
    {

        if (fread(&LA, sizeof(unsigned long), 1, infile) != 1)
        {
            if (feof(infile))
            {
                perror("feof found\n");
                break;
            }
            else
            {
                perror("Error reading file\n");
                break;
            }
        }

        CLK++;

        pnum = LA >> d;
        dnum = LA & 0x07F;

        if (PTE[pnum].valid_bit == 1)
        { // there is a free page available
            fnum = PTE[pnum].frame_number;
            PA = (fnum << d) + dnum;
            printf("(valid bit set condition) The LA is %lx\n", LA);
            printf("pnum: %d, dnum: %d, fnum: %d\n", pnum, dnum, fnum);
            printf("PA: %lx\n", PA);
            fwrite(&PA, sizeof(PA), 1, outfile);

            LRUcount[fnum] = CLK;
        }
        else
        { // we
            int empty_frame = find_empty_frame(freeframes, freeframes_length);
            printf("Empty frame found at: %d\n", empty_frame);
            if (empty_frame > 0)
            { // frame found in freeframes
                PTE[pnum].frame_number = empty_frame;
                PTE[pnum].valid_bit = 1;
                fnum = PTE[pnum].frame_number;
                PA = (fnum << d) + dnum;
                fwrite(&PA, sizeof(PA), 1, outfile);
                printf("(empty frame found condition) The LA is %lx\n", LA);
                revmap[empty_frame] = pnum;
                LRUcount[fnum] = CLK;

                printf("Frame allocated: %d\n", empty_frame);
                printf("Updated PTE[pnum]: frame_number=%d, valid_bit=%d\n", PTE[pnum].frame_number, PTE[pnum].valid_bit);
                printf("fnum: %d\n", fnum);
                printf("PA: %lx\n", PA);
            }
            else
            { // freeframes is full, determine a victim to free space
                int victim = select_LRU_victim(LRUcount, LRUcount_length);
                PTE[revmap[victim]].valid_bit = 0;
                PTE[pnum].frame_number = victim;
                PTE[pnum].valid_bit = 1;
                fnum = PTE[pnum].frame_number;
                PA = (fnum << d) + dnum;
                printf("(LRU victim condition) The LA is %lx\n", LA);
                printf("pnum: %d, dnum: %d, fnum: %d\n", pnum, dnum, fnum);
                printf("PA: %lx\n", PA);
                fwrite(&PA, sizeof(PA), 1, outfile);
                LRUcount[fnum] = CLK;
                revmap[fnum] = pnum;
            }
        }
        print_freeframes(freeframes, 8);
        print_LRU_count(LRUcount, 8);
        print_reverse_map(revmap, 8);
        printf("\n\n");
    }

    return 0;
}

int find_empty_frame(int freeframes[], int freeframes_length)
{
    int free_index = -1;

    for (int i = 1; i < freeframes_length; i++)
    {
        // printf("%d\n", i);
        if (freeframes[i] == 1)
        {
            free_index = i;
            freeframes[i] = 0;
            break;
        }
    }

    return free_index;
}

int select_LRU_victim(int LRUcount[], int LRUcount_length)
{
    int min_value = LRUcount[0];
    int min_index = 0;

    for (int i = 1; i < LRUcount_length; i++)
    {
        if (LRUcount[i] < min_value)
        {
            min_value = LRUcount[i];
            min_index = i;
        }
    }

    return min_index;
}

void print_freeframes(int freeframes[], int size)
{
    printf("Free Frames: ");
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", freeframes[i]);
    }
    printf("\n");
}

void print_LRU_count(int LRUcount[], int size)
{
    printf("LRU Count: ");
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", LRUcount[i]);
    }
    printf("\n");
}

void print_reverse_map(int revmap[], int size)
{
    printf("Reverse Map: ");
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", revmap[i]);
    }
    printf("\n");
}