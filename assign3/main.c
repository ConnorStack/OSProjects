/*
 *File:
 *Connor Stack
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "readyQueue.h"
#include "IOQueue.h"

int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;

void *file_reading_thread(void *arg);

int main(int argc, char *argv[])
{
    // allocate appropriate thread ID's
    pthread_t tid_file_reader, tid_readyq, tid_ioq;
    int thread_status;
    char *schedule_type = argv[2];
    char *filename = NULL;

    ready_Queue *ready_queue;
    ready_queue = new_ready_queue();

    IO_Queue *IO_queue;
    IO_queue = new_IO_queue();

    // temporary
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
    pthread_join(tid_file_reader, (void **)&thread_status);

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

    char buffer[200];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *first_word = strtok(buffer, " \n\t");
        printf("First word: %s ", first_word);


        if (strcmp(first_word, "proc") == 0)
        {
            printf("proc found\n");
        }
        else if (strcmp(first_word, "sleep") == 0)
        {
            printf("sleep found\n");
        }
        else if (strcmp(first_word, "stop") == 0)
        {
            printf("stop found\n");
        }
        else
        {
            printf("invalid word");
        }

    }
    printf("\n");
    fclose(file);

    return NULL;
}

