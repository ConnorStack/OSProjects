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

typedef struct
{
	char *algorithm_type;
	char *filename;
	int quantum_value;
} Scheduler_info;

int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;
sem_t sem_cpu;
sem_t sem_io;

double total_cpu_time = 0;
double total_io_time = 0;
double total_time = 0;
int process_count = 0;
double total_cpu_utilization = 0;

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

	char *algo_type;
	char *filename;

	Scheduler_info scheduler_info;

	// if FIFO, SJF, PRiority
	if (argc == 5)
	{
		algo_type = argv[2];
		filename = argv[4];
		scheduler_info.algorithm_type = algo_type;
		scheduler_info.filename = filename;
		scheduler_info.quantum_value = 0;

		printf("Executing: %s\n", argv[0]);
		printf("%s type: %s\n", argv[1], algo_type);
		printf("%s %s\n", argv[3], filename);
	} // RR
	else if (argc == 7)
	{
		algo_type = argv[2];
		int quantum_value = atoi(argv[4]);
		filename = argv[6];

		scheduler_info.algorithm_type = algo_type;
		scheduler_info.filename = filename;
		scheduler_info.quantum_value = quantum_value;

		printf("Executing: %s\n", argv[0]);
		printf("%s type: %s\n", argv[1], algo_type);
		printf("%s %d\n", argv[3], quantum_value);
		printf("%s %s\n", argv[5], filename);
	}
	else
	{
		printf("invalid number of arguments");
	}
	printf("\n\n");
	printf("Alg type: %s\n", scheduler_info.algorithm_type);

	pthread_create(&tid_file_reader, NULL, file_reading_thread, filename);
	pthread_create(&tid_cpu_scheduler, NULL, cpu_scheduler_thread, &scheduler_info);
	pthread_create(&tid_io_system, NULL, IO_system_thread, NULL);

	pthread_join(tid_file_reader, (void **)&thread_status);
	pthread_join(tid_cpu_scheduler, NULL);
	pthread_join(tid_io_system, NULL);

	printf("-------------------------------------------------------\n");
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
			pthread_mutex_lock(&ready_queue_mutex);

			currPID++;
			newPCB->PID = currPID;

			// These calls are order sensitive, they invoke strtok, so changing their order changes thier values
			newPCB->PR = get_next_token();
			int remaining_instructions = get_next_token();
			set_PCB_burst_values(newPCB, remaining_instructions);

			newPCB->total_processes = remaining_instructions;

			int n = 0;
			int m = 0;
			for (int i = 0; i < remaining_instructions; i++)
			{
				if (i % 2 == 0)
				{
					newPCB->total_cpu_time += newPCB->CPUBurst[n];
					n++;
				}
				else
				{
					newPCB->total_io_time += newPCB->IOBurst[m];
					m++;
				}
			}

			newPCB->total_time = newPCB->total_io_time + newPCB->total_cpu_time;

			enlist_to_ready_queue(ready_queue, newPCB);
			pthread_mutex_unlock(&ready_queue_mutex);
			sem_post(&sem_cpu);
		}
		else if (strcmp(first_word, "sleep") == 0)
		{
			int milliseconds = get_next_token();
			usleep(milliseconds * 1000);
		}
		else if (strcmp(first_word, "stop") == 0)
		{
			break;
		}
		else
		{
			printf("invalid word\n");
		}
	}
	file_read_done = 1;
	fclose(file);

	return NULL;
}

void *cpu_scheduler_thread(void *args)
{
	Scheduler_info *scheduler_info = (Scheduler_info *)args;

	char *scheduler_alg = scheduler_info->algorithm_type;
	int quantum_time = scheduler_info->quantum_value;

	struct timespec atimespec;
	atimespec.tv_sec = 1;

	while (1)
	{
		if (ready_queue_is_empty(ready_queue) && !cpu_busy && IO_Q_is_empty(IO_queue) && !io_busy && file_read_done == 1)
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
				usleep(pcb->CPUBurst[pcb->cpuindex] * 1000);
				pcb->cpuindex++;

				if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					double utilization = (pcb->total_io_time / pcb->total_time) * 100;
					double throughput = pcb->total_processes / pcb->total_time;
					double turnaround_time = pcb->total_time / pcb->total_processes;
					double rtime = pcb->total_time - pcb->total_io_time / pcb->total_processes;
					printf("-------------------------------------------------------\n");
					printf("Input File Name             : %s\n", scheduler_info->filename);
					printf("CPU Scheduling Alg          : %s\n", scheduler_info->algorithm_type);
					printf("CPU Utilization             : %.2lf%%\n", utilization);
					printf("Throughput                  : %.3lf processes / ms\n", throughput);
					printf("Avg Turnaround              : %f ms\n", pcb->PID, elapsed * 1000);
					printf("Avg waiting time in R queue : %.1lf ms\n", rtime);

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

			pthread_mutex_lock(&ready_queue_mutex);
			// PCB *pcb = get_highest_pr_remove_from_list(ready_queue);
			PCB *pcb = findHighestPRPCB(ready_queue);
			removePCBFromQueue(ready_queue, pcb);
			pthread_mutex_unlock(&ready_queue_mutex);

			// printf("\n");
			if (pcb != NULL)
			{
				// printf("\n");
				usleep(pcb->CPUBurst[pcb->cpuindex] * 1000); // Convert to microseconds
				pcb->cpuindex++;
				// printf("PCB id: %d, pcb->cpuindex: %d, pcb->numCPUBurst: %d\n", pcb->PID, pcb->cpuindex, pcb->numCPUBurst);
				if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					// printf("Final cycle of PCB PR\n");
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					// double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					double utilization = (pcb->total_io_time / pcb->total_time) * 100;
					double throughput = pcb->total_processes / pcb->total_time;
					double turnaround_time = pcb->total_time / pcb->total_processes;
					double rtime = pcb->total_time - pcb->total_io_time / pcb->total_processes;
					printf("-------------------------------------------------------\n");
					printf("Input File Name             : %s\n", scheduler_info->filename);
					printf("CPU Scheduling Alg          : %s\n", scheduler_info->algorithm_type);
					printf("CPU Utilization             : %.2lf%%\n", utilization);
					printf("Throughput                  : %.3lf processes / ms\n", throughput);
					printf("Avg Turnaround              : %f ms\n", pcb->PID, elapsed * 1000);
					printf("Avg waiting time in R queue : %.1lf ms\n", rtime);

					free(pcb);
					cpu_busy = 0;
				}
				else
				{
					pthread_mutex_lock(&io_queue_mutex);
					enlist_to_IO_queue(IO_queue, pcb);
					pthread_mutex_unlock(&io_queue_mutex);
					io_busy = 0;
					cpu_busy = 0;
					sem_post(&sem_io);
				}
			}
			else
			{
				printf("UNEXPECTED NULL PCB\n");
			}
		}
		else if (strcmp(scheduler_alg, "SJF") == 0)
		{
			int res = sem_timedwait(&sem_cpu, &atimespec);
			if (res == -1 && errno == ETIMEDOUT)
			{
				continue;
			}
			cpu_busy = 1;

			pthread_mutex_lock(&ready_queue_mutex);
			PCB *pcb = findLowestCPUBurstPCB(ready_queue);
			removePCBFromQueue(ready_queue, pcb);
			// PCB *pcb = delist_from_ready_queue(ready_queue);
			pthread_mutex_unlock(&ready_queue_mutex);
			// Priority scheduling algorithm
			if (pcb != NULL)
			{

				usleep(pcb->CPUBurst[pcb->cpuindex] * 1000);
				pcb->cpuindex++;

				if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					printf("Final cycle of PCB\n");
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;

					double utilization = (pcb->total_io_time / pcb->total_time) * 100;
					double throughput = pcb->total_processes / pcb->total_time;
					double turnaround_time = pcb->total_time / pcb->total_processes;
					double rtime = pcb->total_time - pcb->total_io_time / pcb->total_processes;
					printf("-------------------------------------------------------\n");
					printf("Input File Name             : %s\n", scheduler_info->filename);
					printf("CPU Scheduling Alg          : %s\n", scheduler_info->algorithm_type);
					printf("CPU Utilization             : %.2lf%%\n", utilization);
					printf("Throughput                  : %.3lf processes / ms\n", throughput);
					printf("Avg Turnaround              : %f ms\n", pcb->PID, elapsed * 1000);
					printf("Avg waiting time in R queue : %.1lf ms\n", rtime);
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
		else if (strcmp(scheduler_alg, "RR") == 0)
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
				if (pcb->cpuindex < pcb->numCPUBurst)
				{
					int cpu_burst = pcb->CPUBurst[pcb->cpuindex];
					if (quantum_time < cpu_burst)
					{
						pcb->CPUBurst[pcb->cpuindex] -= quantum_time;
						usleep(quantum_time * 1000);
					}
					else
					{
						usleep(cpu_burst * 1000);
						pcb->CPUBurst[pcb->cpuindex] = 0;
					}
					if (pcb->CPUBurst[pcb->cpuindex] == 0)
					{
						pcb->cpuindex++;
						pthread_mutex_lock(&io_queue_mutex);
						enlist_to_IO_queue(IO_queue, pcb);
						pthread_mutex_unlock(&io_queue_mutex);
						io_busy = 0;
						sem_post(&sem_io);
					}
					else
					{
						// PCB still has CPU bursts left, move to the back of the ready queue
						pthread_mutex_lock(&ready_queue_mutex);
						enlist_to_ready_queue(ready_queue, pcb);
						pthread_mutex_unlock(&ready_queue_mutex);
						cpu_busy = 0;
						sem_post(&sem_cpu);
					}
				}
				else if (pcb->cpuindex >= pcb->numCPUBurst)
				{
					printf("Final cycle of PCB\n");
					struct timespec ts_end;
					clock_gettime(CLOCK_MONOTONIC, &pcb->ts_end);
					double elapsed = (pcb->ts_end.tv_sec - pcb->ts_begin.tv_sec) + (pcb->ts_end.tv_nsec - pcb->ts_begin.tv_nsec) / 1000000000.0;
					double utilization = (pcb->total_io_time / pcb->total_time) * 100;
					double throughput = pcb->total_processes / pcb->total_time;
					double turnaround_time = pcb->total_time / pcb->total_processes;
					double rtime = pcb->total_time - pcb->total_io_time / pcb->total_processes;
					printf("-------------------------------------------------------\n");
					printf("Input File Name             : %s\n", scheduler_info->filename);
					printf("CPU Scheduling Alg          : %s\n", scheduler_info->algorithm_type);
					printf("CPU Utilization             : %.2lf%%\n", utilization);
					printf("Throughput                  : %.3lf processes / ms\n", throughput);
					printf("Avg Turnaround              : %f ms\n", pcb->PID, elapsed * 1000);
					printf("Avg waiting time in R queue : %.1lf ms\n", rtime);
					free(pcb);
					cpu_busy = 0;
				}
			}
		}
		else
		{
			perror("Invalid algorithm.\n");
		}
		cpu_sch_done = 1;
	}

	return NULL;
}

void *IO_system_thread(void *args)
{
	struct timespec atimespec;
	atimespec.tv_sec = 1;
	while (1)
	{
		// Check conditions for breaking the loop
		if (ready_queue_is_empty(ready_queue) && !cpu_busy && IO_Q_is_empty(IO_queue) && file_read_done == 1)
		{
			break;
		}

		// Wait for a PCB to be available in IO_Q
		int res = sem_timedwait(&sem_io, &atimespec);
		if (res == -1 && errno == ETIMEDOUT)
		{
			continue; 
		}

		io_busy = 1;
		pthread_mutex_lock(&io_queue_mutex);
		PCB *pcb = delist_from_IO_queue(IO_queue);
		pthread_mutex_unlock(&io_queue_mutex);
		double io_time = pcb->IOBurst[pcb->ioindex] * 0.001;
		total_io_time += io_time;
		pcb->ioindex++;

		// Simulate I/O by sleeping
		usleep(pcb->IOBurst[pcb->ioindex] * 1000);

		// Insert the PCB into Ready_Q
		pthread_mutex_lock(&ready_queue_mutex);
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
