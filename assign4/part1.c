#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    unsigned long LA, PA;
    unsigned long pnum, fnum, dnum;
    int PT[32] = {2, 4, 1, 7, 3, 5, 6, -1};
    unsigned int p = 5, f = 3, d = 7;
    char *infile_arg = argv[1];
    char *outfile_arg = argv[2];

    FILE *infile = fopen(infile_arg, "rb");
    FILE *outfile = fopen(outfile_arg, "wb");

    if (infile == NULL || outfile == NULL)
    {
        perror("Failed to open files\n");
        return 1;
    }

    unsigned char buffer[16]; 
    size_t buffer_size = sizeof(buffer);

    while (1)
    {
        unsigned long bytes_read = fread(buffer, 1, buffer_size, infile); 

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
            LA = logical_address_array[i];

            if (LA == 0)
            {
                continue;
            }

            pnum = LA >> d;  
            dnum = LA & 0x7F;

            fnum = PT[pnum];
            PA = (fnum << 7) + dnum;

            printf("The LA is %lx and Translated PA is %lx\n", LA, PA);
            fwrite(&PA, sizeof(PA), 1, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}