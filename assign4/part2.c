#include <stdlib.h>
#include <stdio.h>

typedef struct page_table_entry
{
    int frame_number;
    int valid_bit;
} page_table_entry;

int find_empty_frame(int[], int);
int select_LRU_victim();

int main(int argc, char *argv[])
{
    unsigned long LA, PA;

    unsigned int p = 5, f = 3, d = 7;
    unsigned int pnum, fnum, dnum;
    int CLK = 0;
    // int freeframes[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    int freeframes[4] = {0, 3, 2, 1};
    int LRUcount[8] = {0};
    int revmap[8] = {-1};

    printf("working\n");

    char *infile_arg = argv[1];
    FILE *infile = fopen(infile_arg, "rb");

    char *buffer[1024];

    int freeframes_length = sizeof(freeframes) / sizeof(freeframes[0]);
    printf("free frames length: %d\n", freeframes_length);

    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%d\n", freeframes[i]);
    // }

    int emptyframe = find_empty_frame(freeframes, freeframes_length);
    printf("empty frame: %d\n", emptyframe);

    // while (feof(infile))
    // {
    //     size_t infile_count = fread(buffer, 1, sizeof(buffer), infile);
    //     CLK++;
    //     pnum = LA >> d;
    //     dnum = LA & 0x07F;
    // }

    return 0;
}

int find_empty_frame(int freeframes[], int freeframes_length)
{
    int free_index = -1;

    for (int i = 1; i < freeframes_length; i++)
    {
        printf("%d\n", i);
        if (freeframes[i] == 1)
        {
            free_index = i;
            break;
        }
    }

    return free_index;
}

int select_LRU_victim()
{
    int victim = 0;

    return victim;
}