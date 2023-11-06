#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned long LA, PA;
    int PT[8] = {2, 4, 1, 7, 3, 5, 6, -1};
    unsigned int p=5, f=3, d=7;

    FILE *infile = fopen("infiletest", "rb");
    FILE *outfile = fopen("outfile", "wb");
    // FILE * fptr = fopen("infile", "rb");

    if (infile == NULL)
    {
        perror("could not open file\n");
    }

    unsigned char buffer[1024];
    size_t buffer_size = sizeof(buffer);

    while (1)
    {
        unsigned long bytes_read = (unsigned long)fread(buffer, 1, buffer_size, infile);

        if (bytes_read == 0)
        {
            if (feof(infile))
            {
                break;
            }
            else
            {
                perror("Error reading file\n");
                break;
            }
        }

        size_t logical_address_count = bytes_read / sizeof(unsigned long);
        unsigned long *logical_address_array = (unsigned long *)buffer;

        for (size_t i = 0; i < logical_address_count; i++)
        {
            unsigned long LA = logical_address_array[i];

            if (LA == 0) {
            continue;
        }

            // Extract page number (pnum) and offset (dnum)
            unsigned long pnum = LA >> 7;   // Right shift to get the page number
            unsigned long dnum = LA & 0x7F; // Mask to get the offset

            // Use page number (pnum) to find the frame number (fnum) in the page table
            unsigned long fnum = PT[pnum];

            // Calculate the physical address (PA)
            unsigned long PA = (fnum << 7) + dnum;

            // Write the physical address (PA) to the output file in binary format
            printf("The LA is %lx and Translated PA is %lx\n", LA, PA);
            fwrite(&PA, sizeof(PA), 1, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}