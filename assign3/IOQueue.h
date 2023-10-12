#ifndef _IO_QUEUE_H
#define _IO_QUEUE_H

#include "PCB.h"

typedef struct IO_Queue
{
    PCB *head;
    PCB *tail;

} IO_Queue;


IO_Queue *new_IO_queue();

// IO_Q *NewLinkedList(void);

// void FreeLinkedList(IO_Q *list);

// void Enlist(IO_Q *list, PCB *element);

// PCB *Delist(IO_Q *list);

// int LinkedListIsEmpty(IO_Q *list);

// int LinkedListIsFull(IO_Q *list);

// int LinkedListLength(IO_Q *list);

// PCB *GetLinkedListElement(IO_Q *list, int index);

#endif