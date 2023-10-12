#ifndef _READY_Q_H
#define _READY_Q_H

#include "pcb.h"

typedef struct ready_Queue
{
    PCB *head;
    PCB *tail;

} ready_Queue;

#endif

ready_Queue *new_ready_queue();

// ready_Q *NewLinkedList(void);

// void FreeLinkedList(ready_Q *list);

// void Enlist(ready_Q *list, PCB *element);

// PCB *Delist(ready_Q *list);

// int LinkedListIsEmpty(ready_Q *list);

// int LinkedListIsFull(ready_Q *list);

// int LinkedListLength(ready_Q *list);

// PCB *GetLinkedListElement(ready_Q *list, int index);