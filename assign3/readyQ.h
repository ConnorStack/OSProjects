#ifndef _ready_Q
#define _ready_Q

#include "pcb.h"

typedef struct ready_Q
{
    PCB *head;
    PCB *tail;
} ready_Q;

#endif

// PCB *NewPCB();

// ready_Q *NewLinkedList(void);

// void FreeLinkedList(ready_Q *list);

// void Enlist(ready_Q *list, PCB *element);

// PCB *Delist(ready_Q *list);

// int LinkedListIsEmpty(ready_Q *list);

// int LinkedListIsFull(ready_Q *list);

// int LinkedListLength(ready_Q *list);

// PCB *GetLinkedListElement(ready_Q *list, int index);