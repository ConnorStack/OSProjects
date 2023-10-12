#ifndef PCB_H
#define PCB_H

#include <time.h> // Include necessary headers

typedef struct PCB {
    int PID, PR;
    int numCPUBurst, numIOBurst;
    int *CPUBurst, *IOBurst;
    int cpuindex, ioindex;
    struct timespec ts_begin, ts_end;
    struct PCB *prev, *next;
    char* scheduling_algorithm;
    // more fields for performance measures
    // use the system time to determine how much waited, etc.
} PCB;

#endif