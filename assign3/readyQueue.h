#ifndef _READY_Q_H
#define _READY_Q_H

#include <stdio.h>
#include "pcb.h"

typedef struct ready_Queue
{
    PCB *head;
    PCB *tail;

} ready_Queue;

ready_Queue *new_ready_queue();

void enlist_to_ready_queue(ready_Queue *list, PCB *element);

int ready_queue_is_empty(ready_Queue *list);

PCB *delist_from_ready_queue(ready_Queue *list);

PCB *get_highest_priority_pcb(ready_Queue *list);

void remove_PCB_from_queue(ready_Queue* queue, PCB* pcb);

PCB* find_lowest_CPU_burst_PCB(ready_Queue* queue);

PCB* find_highest_PR_PCB(ready_Queue* queue);

void print_PCB_rq(const PCB *pcb);

void print_rq_PCBs_in_list(const ready_Queue *list);

#endif