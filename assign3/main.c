/*
 *File:
 *Connor Stack
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include "readyQueue.h"
#include "IOQueue.h"

// typedef struct cpu_scheduler_args
// {
//     // this may be a mistake to make a struct
//     char *scheduler_alg;
// } cpu_scheduler_args;

int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;
sem_t sem_cpu;
sem_t sem_io;

ready_Queue *ready_queue;
IO_Queue *IO_queue;
pthread_mutex_t ready_queue_mutex;
pthread_mutex_t io_queue_mutex;

void *file_reading_thread(void *arg);
void *cpu_scheduler_thread(void *args);
void *IO_system_thread(void *args);

void set_PCB_burst_values(PCB *newPCB, int remaining_instructions);
int get_next_token();

int main(int argc, char *argv[])
{
    // allocate appropriate thread ID's
    pthread_t tid_file_reader, tid_cpu_scheduler, tid_io_system;
    int thread_status;
    pthread_mutex_t ready_queue_mutex;

    sem_init(&sem_cpu, 0, 0);
    sem_init(&sem_io, 0, 0);

    ready_queue = new_ready_queue();
    pthread_mutex_init(&ready_queue_mutex, NULL);

    IO_queue = new_IO_queue();
    pthread_mutex_init(&io_queue_mutex, NULL);

    char *algorithmType;
    //--------------------------------------------

    char *filename;
    char *quantum;
    // if FIFO, SJF, PRiority
    if (argc == 5)
    {
        filename = argv[4];
        algorithmType = argv[2];
        printf("Executing: %s\n", argv[0]);
        printf("%s type: %s\n", argv[1], algorithmType);
        printf("%s %s\n", argv[3], filename);

        // printf("Executing: %s \n%s type: %s \n%s %s\n", argv[0], argv[1], algorithmType, argv[3], filename);
    } // RR
    else if (argc == 7)
    {
        // file_reading_args.filename = argv[6];
        algorithmType = argv[2];
        quantum = argv[4];
        filename = argv[6];

        printf("Executing: %s\n", argv[0]);
        printf("%s type: %s\n", argv[1], algorithmType);
        printf("%s %s\n", argv[3], quantum);
        printf("%s %s\n", argv[5], filename);
        // printf("Executing: %s \n %s type: %s \n %s %s \n %s", argv[0], argv[1], algorithmType, argv[3], quantum, filename);
    }
    else
    {
        // printf("invalid number of arguments");
    }

    pthread_create(&tid_file_reader, NULL, file_reading_thread, filename);
    pthread_create(&tid_cpu_scheduler, NULL, cpu_scheduler_thread, algorithmType);
    pthread_create(&tid_io_system, NULL, IO_system_thread, NULL);

    pthread_join(tid_file_reader, (void **)&thread_status);
    pthread_join(tid_cpu_scheduler, NULL);
    // print_PCBs_in_list(ready_queue);

    return 0;
}

void *IO_system_thread(void *args){
    return NULL;
}

void *cpu_scheduler_thread(void *args)
{
    char *scheduler_alg = (char *)args;
    struct timespec atimespec;
    atimespec.tv_sec = 1;

    while (1)
    {
        if (ready_queue_is_empty(ready_queue) && !cpu_busy && IO_Q_is_empty(IO_queue) && !io_busy && file_read_done)
        {
            break;
        }

        if (strcmp(scheduler_alg, "FIFO") == 0)
        {
            int res = sem_timedwait(&sem_cpu, &atimespec);
            if (res == -1 && errno == ETIMEDOUT)
            {
                continue; 
            }
            cpu_busy = 1;

            // sem_wait(&sem_cpu);
            // pthread_mutex_lock(&ready_queue_mutex);
            // pthread_mutex_unlock(&ready_queue_mutex);

            PCB *pcb = delist_from_ready_queue(ready_queue);
            if (pcb != NULL)
            {
                printf("cpu scheduler sleeping");
                usleep(pcb->CPUBurst[pcb->cpuindex] * 1000); // Convert to microseconds
                pcb->cpuindex++;

                if (pcb->cpuindex >= pcb->numCPUBurst)
                {
                    // This is the last CPU burst, terminate the PCB
                    free(pcb);
                    cpu_busy = 0;
                }
                else
                {
                    // Insert PCB into IO_Q
                    enlist_to_IO_queue(IO_queue, pcb);
                    io_busy = 0;
                    cpu_busy = 0;
                    sem_post(&sem_io);
                }
                
            }
        }
        else if (strcmp(scheduler_alg, "SJF") == 0)
        {
            // Shortest Job First scheduling algorithm
        }
        else if (strcmp(scheduler_alg, "PR") == 0)
        {
            // Priority scheduling algorithm
        }
        else if (strcmp(scheduler_alg, "RR") == 0)
        {
            // Round Robin scheduling algorithm
        }
        else
        {
            // Handle the case where the algorithm is not recognized
        }
    }

    cpu_sch_done = 1;
    return NULL;
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
    int currPID = 0;
    // when should currPID increment? after each proc, or each line in the file?

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *first_word = strtok(buffer, " \n\t");

        // If the instruction is proc, create a new PCB and add it to the linked_list
        if (strcmp(first_word, "proc") == 0)
        {

            PCB *newPCB;
            newPCB = malloc(sizeof(PCB));

            newPCB->PID = currPID;

            // These calls are order sensitive, they invoke strtok, so changing their order changes thier values
            newPCB->PR = get_next_token();
            int remaining_instructions = get_next_token();
            set_PCB_burst_values(newPCB, remaining_instructions);

            pthread_mutex_lock(&ready_queue_mutex);
            enlist_to_ready_queue(ready_queue, newPCB);
            pthread_mutex_unlock(&ready_queue_mutex);
            sem_post(&sem_cpu);
        }
        else if (strcmp(first_word, "sleep") == 0)
        {
            int milliseconds = get_next_token();
            usleep(milliseconds * 1000);
            printf("sleep found\n");
        }
        else if (strcmp(first_word, "stop") == 0)
        {
            printf("stop found\n");
            break;
        }
        else
        {
            printf("invalid word");
        }
    }

    file_read_done = 1;
    printf("\n");
    fclose(file);

    return NULL;
}

void set_PCB_burst_values(PCB *newPCB, int remaining_instructions)
{

    newPCB->CPUBurst = (int *)malloc((remaining_instructions / 2 + 1) * sizeof(int));
    newPCB->IOBurst = (int *)malloc((remaining_instructions / 2) * sizeof(int));

    if (newPCB->CPUBurst == NULL || newPCB->IOBurst == NULL)
    {
        printf("MEMORY FAILURE");
    }

    int cpu_burst = 0;
    int io_burst = 0;
    char *instruction = NULL;

    int n = 0;
    int m = 0;

    for (int i = 0; i < remaining_instructions; i++)
    {
        if (i % 2 == 0)
        {
            instruction = strtok(NULL, " ");
            cpu_burst = atoi(instruction);
            newPCB->CPUBurst[n] = cpu_burst;
            n++;
            // printf("CPU burst: %d\n", cpu_burst);
        }
        else
        {
            instruction = strtok(NULL, " ");
            io_burst = atoi(instruction);
            newPCB->IOBurst[m] = io_burst;
            m++;
            // printf("IO burst: %d\n", io_burst);
        }
        newPCB->numCPUBurst = n;
        newPCB->numIOBurst = m;
    }
}

int get_next_token()
{
    char *file_token = strtok(NULL, " ");
    int next_token = atoi(file_token);

    return next_token;
}
