#ifndef PCB_H
#define PCB_H

#include <time.h> 

typedef struct PCB {
    int PID, PR;
    int numCPUBurst, numIOBurst;
    int *CPUBurst, *IOBurst;
    int cpuindex, ioindex;
    struct timespec ts_begin, ts_end;
    struct PCB *prev, *next;

    int total_processes;
    double total_cpu_time;
    double total_io_time;
    double total_time;
} PCB;

#endif