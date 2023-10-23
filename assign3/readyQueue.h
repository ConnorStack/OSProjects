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
void Enlist(ready_Queue *list, PCB *element);
int LinkedListIsEmpty(ready_Queue *list);


// PCB *Delist(ready_Queue *list);

// ready_Q *NewLinkedList(void);

// void FreeLinkedList(ready_Queue *list);

// int LinkedListIsFull(ready_Queue *list);

// int LinkedListLength(ready_Queue *list);

// PCB *GetLinkedListElement(ready_Queue *list, int index);