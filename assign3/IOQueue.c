#include <stdlib.h>
#include <stdio.h>
#include "IOQueue.h"

IO_Queue *new_IO_queue()
{
    IO_Queue *list;
    list = (IO_Queue *)malloc(sizeof(IO_Queue));

    if (list == NULL)
    {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

// void FreeLinkedList(IO_Queue *list){}

void enlist_to_IO_queue(IO_Queue *list, PCB *element)
{
    if (list->head != NULL)
    {
        list->tail->next = element;
        list->tail = element;
        element->next = NULL;
    }
    else
    {
        list->head = element;
        list->tail = element;
        element->next = NULL;
    }
}

PCB *delist_from_IO_queue(IO_Queue *list)
{
    if (IO_Q_is_empty(list))
    {
        printf("\n Error, List is empty\n");
        return NULL;
    }

    PCB *removedElement = list->head;
    list->head = list->head->next;

    removedElement->next = NULL;

    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    return removedElement;
}

int IO_Q_is_empty(IO_Queue *list)
{
    if (list->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// int LinkedListIsFull(IO_Queue *list){}

// int LinkedListLength(IO_Queue *list){}

// PCB *GetLinkedListElement(IO_Queue *list, int index){}