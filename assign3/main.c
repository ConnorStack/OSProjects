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

typedef struct file_reading_args{
    ready_Queue *ready_queue;
    pthread_mutex_t *ready_queue_mutex;
    char *filename;

}file_reading_args;

typedef struct cpu_scheduler_args{
    char * scheduler_alg;
}cpu_scheduler_args;

int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;

void *file_reading_thread(void *arg);

int main(int argc, char *argv[])
{
    // allocate appropriate thread ID's
    pthread_t tid_file_reader, tid_cpu_scheduler, tid_io_system;
    int thread_status;
    pthread_mutex_t ready_queue_mutex;

    pthread_mutex_init(&ready_queue_mutex, NULL);

    file_reading_args file_reading_args;
    cpu_scheduler_args cpu_scheduler_args;

    ready_Queue *ready_queue = new_ready_queue();
    IO_Queue *IO_queue = new_IO_queue();

    // temporary, move to appropriate logic gate
    file_reading_args.filename = argv[1];
    file_reading_args.ready_queue = ready_queue;
    file_reading_args.ready_queue_mutex = &ready_queue_mutex;

    cpu_scheduler_args.scheduler_alg = argv[2];
    //--------------------------------------------

    if (argc == 5)
    {
        file_reading_args.filename = argv[4];

        printf("5\n");
    }
    else if (argc == 7)
    {
        file_reading_args.filename = argv[6];

        printf("7\n");
    }
    else
    {
        // printf("invalid number of arguments");
    }

    pthread_create(&tid_file_reader, NULL, file_reading_thread, &file_reading_args);
    // pthread_create($tid_cpu_scheduler, NULL, cpu_scheduler_thread, (void *))

    pthread_join(tid_file_reader, (void **)&thread_status);

    return 0;
}

void *file_reading_thread(void *arg)
{
    file_reading_args *args = (file_reading_args *)arg;
    char *filename = args->filename;
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

        //If the instruction is proc, create a new PCB and add it to the linked_list
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

void *cpu_scheduler_thread(void *args){

}

