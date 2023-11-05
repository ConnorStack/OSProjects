#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    printf("Hello world\n");
    // FILE * fptr = fopen("part1testsequence", "rb");
    FILE * fptr = fopen("part1sequence", "rb");
    if(fptr == NULL){
        perror("could not open file\n");
    }

    unsigned char buffer[1024];
    size_t buffer_size = sizeof(buffer);

    while(1){
        // size_t bytes_read = fread(buffer, 1, sizeof(buffer), fptr);
        unsigned long bytes_read = (unsigned long)fread(buffer, 1, sizeof(buffer), fptr);

        if(bytes_read == 0){
            if(feof(fptr)){
                break;
            }else{
                perror("error reading file\n");
                break;
            }
        }

        size_t num_ulongs = bytes_read / sizeof(unsigned long);
        unsigned long *ulongs = (unsigned long *)buffer;

        for(size_t i = 0; i < num_ulongs; i++){
            printf("#%zu : %lx\n", i+1, ulongs[i]);
        }
        
    }

    fclose(fptr);

    return 0;
}   