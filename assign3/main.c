/*
 *File:
 *Connor Stack
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "readyQ.h"
#include "IOQ.h"
void *file_reading_thread(void *arg);

int main(int argc, char *argv[])
{
    pthread_t tid_file_reader, tid_readyq, tid_ioq;
    int thread_status;
    char *schedule_type = argv[2];
    char *filename = NULL;
    //temporary
    filename = argv[1];
    if (argc == 5)
    {
        filename = argv[4];

        printf("5\n");
    }
    else if (argc == 7)
    {
        filename = argv[6];

        printf("7\n");
    }
    else
    {
        // printf("invalid number of arguments");
    }

    // ready_Q readyq;

    pthread_create(&tid_file_reader, NULL, file_reading_thread, (void *)filename);
    pthread_join(tid_file_reader, (void**)&thread_status);

    return 0;
}


void *file_reading_thread(void *arg)
{
    char *filename = (char *)arg;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("could not open file");
        return NULL;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Process the data as needed
        printf("Read: %s", buffer);
    }

    // Close the file
    fclose(file);

    return NULL;
}

//