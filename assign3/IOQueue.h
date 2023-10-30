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

// void FreeLinkedList(IO_Queue *list);

void enlist_to_IO_queue(IO_Queue *list, PCB *element);

PCB *delist_from_IO_queue(IO_Queue *list);

int IO_Q_is_empty(IO_Queue *list);

int IO_queue_length(IO_Queue *list);

// int LinkedListIsFull(IO_Queue *list);

// int LinkedListLength(IO_Queue *list);

// PCB *GetLinkedListElement(IO_Queue *list, int index);

#endif