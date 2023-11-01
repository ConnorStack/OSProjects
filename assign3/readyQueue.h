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

PCB *delist_specific_pcb(ready_Queue *list, PCB *pcb);

PCB *get_highest_pr_remove_from_list(ready_Queue *ready_queue);

void remove_PCB_from_queue(ready_Queue* queue, PCB* pcb);

PCB* find_lowest_CPU_burst_PCB(ready_Queue* queue);

PCB* find_highest_PR_PCB(ready_Queue* queue);

// ready_Q *NewLinkedList(void);

// void FreeLinkedList(ready_Queue *list);

// int LinkedListIsFull(ready_Queue *list);

// int LinkedListLength(ready_Queue *list);

// PCB *GetLinkedListElement(ready_Queue *list, int index);

void print_PCB_rq(const PCB *pcb);

void print_rq_PCBs_in_list(const ready_Queue *list);

#endif