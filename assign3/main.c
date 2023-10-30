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
#include <time.h>
#include "readyQueue.h"
#include "IOQueue.h"

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

	sem_init(&sem_cpu, 0, 0);
	sem_init(&sem_io, 0, 0);

	ready_queue = new_ready_queue();
	pthread_mutex_init(&ready_queue_mutex, NULL);

	IO_queue = new_IO_queue();
	pthread_mutex_init(&io_queue_mutex, NULL);

	char *algorithmType;
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
	} // RR
	else if (argc == 7)
	{
		algorithmType = argv[2];
		quantum = argv[4];
		filename = argv[6];

		printf("Executing: %s\n", argv[0]);
		printf("%s type: %s\n", argv[1], algorithmType);
		printf("%s %s\n", argv[3], quantum);
		printf("%s %s\n", argv[5], filename);
	}
	else
	{
		// printf("invalid number of arguments");
	}
	printf("\n\n");

	pthread_create(&tid_file_reader, NULL, file_reading_thread, filename);
	pthread_create(&tid_cpu_scheduler, NULL, cpu_scheduler_thread, algorithmType);
	pthread_create(&tid_io_system, NULL, IO_system_thread, NULL);

	pthread_join(tid_file_reader, (void **)&thread_status);
	pthread_join(tid_cpu_scheduler, NULL);
	pthread_join(tid_io_system, NULL);
	// printf("blerp\n");
	// print_PCBs_in_list(ready_queue);

	return 0;
}

void *file_reading_thread(void *arg)
{
	printf("reading file\n");
	char *filename = (char *)arg;
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("could not open file");
		return NULL;
	}

	char buffer[300];
	int currPID = 0;

	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		char *first_word = strtok(buffer, " \n\t");
		if (strcmp(first_word, "proc") == 0)
		{
			PCB *newPCB;
			newPCB = malloc(sizeof(PCB));

			struct timespec ts_start;
			clock_gettime(CLOCK_MONOTONIC, &newPCB->ts_begin);

			currPID++;
			newPCB->PID = currPID;

			// These calls are order sensitive, they invoke strtok, so changing their order changes thier values
			newPCB->PR = get_next_token();
			int remaining_instructions = get_next_token();
			set_PCB_burst_values(newPCB, remaining_instructions);

			pthread_mutex_lock(&ready_queue_mutex);
			printf("Adding PCB to ready queue from file reading thread\n");
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
			printf("invalid word\n");
		}
	}
	// sem_post(&sem_cpu);
	file_read_done = 1;
	printf("\n");
	fclose(file);

	return NULL;
}

void *cpu_scheduler_thread(void *args)
{
	printf("\n\ncpu scheduler\n\n");
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

			pthread_mutex_lock(&ready_queue_mutex);
			PCB *pcb = delist_from_ready_queue(ready_queue);
			pthread_mutex_unlock(&ready_queue_mutex);

			if (pcb != NULL)
			{
				// print_PCB(pcb);
				// printf("\n");
				usleep(pcb->CPUBurst[pcb->cpuindex] * 1000);
				pcb->cpuindex++;
				// printf("cpu index %d\n", pcb->cpuindex);
				// printf("num cpu burst %d\n", pcb->numCPUBurst);
				if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					printf("Turnaround time for PID %d: %f ms\n", pcb->PID, elapsed * 1000);
					// This is the last CPU burst, terminate the PCB
					free(pcb);
					cpu_busy = 0;
				}
				else
				{
					// Insert PCB into IO_Q
					pthread_mutex_lock(&io_queue_mutex);
					enlist_to_IO_queue(IO_queue, pcb);
					pthread_mutex_unlock(&io_queue_mutex);
					io_busy = 0;
					cpu_busy = 0;
					sem_post(&sem_io);
				}
			}
		}
		else if (strcmp(scheduler_alg, "PR") == 0)
		{
			// printf("PR scheduler activated\n");
			int res = sem_timedwait(&sem_cpu, &atimespec);
			if (res == -1 && errno == ETIMEDOUT)
			{
				continue;
			}
			cpu_busy = 1;

			// pthread_mutex_lock(&ready_queue_mutex);
			// PCB *pcb = get_highest_priority_pcb(ready_queue);
			// delist_specific_pcb(ready_queue, pcb);
			// pthread_mutex_unlock(&ready_queue_mutex);

			pthread_mutex_lock(&ready_queue_mutex);
			PCB *pcb = findAndRemoveHighestPriority(ready_queue);
			pthread_mutex_unlock(&ready_queue_mutex);

			// pthread_mutex_lock(&ready_queue_mutex);
			// PCB *pcb = delist_from_ready_queue(ready_queue);
			// pthread_mutex_unlock(&ready_queue_mutex);

			// printf("\n");
			if (pcb != NULL)
			{
				// printf("\n");
				usleep(pcb->CPUBurst[pcb->cpuindex] * 1000); // Convert to microseconds
				pcb->cpuindex++;
				if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					printf("Turnaround time for PID %d: %f ms\n", pcb->PID, elapsed * 1000);
					// This is the last CPU burst, terminate the PCB
					free(pcb);
					cpu_busy = 0;
				}
				else
				{
					// Insert PCB into IO_Q
					pthread_mutex_lock(&io_queue_mutex);
					printf("\n\nEnlisting to IO queue from CPU scheduler\n");
					print_PCB(pcb);
					enlist_to_IO_queue(IO_queue, pcb);
					pthread_mutex_unlock(&io_queue_mutex);
					io_busy = 0;
					cpu_busy = 0;
					sem_post(&sem_io);
				}
			}
		}
		else if (strcmp(scheduler_alg, "SJF") == 0)
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

void *IO_system_thread(void *args)
{
	// printf("IO system\n");
	struct timespec atimespec;
	atimespec.tv_sec = 1;
	while (1)
	{
		// Check conditions for breaking the loop
		if (ready_queue_is_empty(ready_queue) && !cpu_busy && IO_Q_is_empty(IO_queue) && file_read_done)
		{
			break;
		}

		// Wait for a PCB to be available in IO_Q
		int res = sem_timedwait(&sem_io, &atimespec /* say 1 sec */);
		if (res == -1 && errno == ETIMEDOUT)
		{
			continue; // Timed out, check conditions again
		}

		io_busy = 1;

		// Get (remove) the first PCB from IO_Q
		pthread_mutex_lock(&io_queue_mutex);
		// int length = IO_queue_length(IO_queue);
		// printf("IO queue length, io thread %d\n", length);
		PCB *pcb = delist_from_IO_queue(IO_queue);
		pthread_mutex_unlock(&io_queue_mutex);

		// printf("\n\nPCB from io thread: \n");
		// print_PCB(pcb);

		// Simulate I/O by sleeping
		usleep(pcb->IOBurst[pcb->ioindex] * 1000); // Convert to microseconds

		pcb->ioindex++;

		// Insert the PCB into Ready_Q
		pthread_mutex_lock(&ready_queue_mutex);
		printf("enlist TO ready queue FROM io thread\n\n");
		// print_PCBs_in_list(ready_queue);
		enlist_to_ready_queue(ready_queue, pcb);
		pthread_mutex_unlock(&ready_queue_mutex);
		io_busy = 0;
		sem_post(&sem_cpu);
	}
	io_sys_done = 1;
	return NULL;
}

void set_PCB_burst_values(PCB *newPCB, int remaining_instructions)
{

	newPCB->CPUBurst = (int *)malloc((remaining_instructions / 2 + 1) * sizeof(int));
	newPCB->IOBurst = (int *)malloc((remaining_instructions / 2) * sizeof(int));

	if (newPCB->CPUBurst == NULL || newPCB->IOBurst == NULL)
	{
		printf("MEMORY FAILURE\n");
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
		}
		else
		{
			instruction = strtok(NULL, " ");
			io_burst = atoi(instruction);
			newPCB->IOBurst[m] = io_burst;
			m++;
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
