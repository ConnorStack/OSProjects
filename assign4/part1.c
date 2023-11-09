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

    if (infile == NULL)
    {
        perror("could not open file\n");
    }

    unsigned char buffer[16]; // probably shorten this
    size_t buffer_size = sizeof(buffer);

    while (1)
    {
        unsigned long bytes_read = fread(buffer, 1, buffer_size, infile); // does this need to be unsigned long?

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

            // Extract page number (pnum) and offset (dnum)
            pnum = LA >> d;   // Right shift to get the page number
            dnum = LA & 0x7F; // Mask to get the offset

            // Use page number (pnum) to find the frame number (fnum) in the page table
            fnum = PT[pnum];

            // Calculate the physical address (PA)
            PA = (fnum << 7) + dnum;

            // Write the physical address (PA) to the output file in binary format
            printf("The LA is %lx and Translated PA is %lx\n", LA, PA);
            fwrite(&PA, sizeof(PA), 1, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);

    //-------------------------------------------
    FILE *verify_file = fopen(outfile_arg, "rb");

    if (verify_file == NULL)
    {
        perror("could not open output file for verification\n");
        return 1;
    }

    printf("Verifying contents of the output file:\n");
    while (1)
    {
        unsigned long read_PA;
        size_t read_result = fread(&read_PA, sizeof(read_PA), 1, verify_file);
        if (read_result == 0)
        {
            if (feof(verify_file))
            {
                break;
            }
            else
            {
                perror("Error reading output file for verification\n");
                break;
            }
        }
        printf("Read PA: %lx\n", read_PA);
    }

    fclose(verify_file);
    //-------------------------------------------

    return 0;
}