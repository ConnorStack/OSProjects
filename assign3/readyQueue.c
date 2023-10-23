#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readyQueue.h"


ready_Queue *new_ready_queue()
{
    ready_Queue *list;

    list = (ready_Queue *)malloc(sizeof(ready_Queue));
    if (list == NULL)
    {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void Enlist(ready_Queue *list, PCB *element)
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

PCB *Delist(ready_Queue *list)
{
    if (LinkedListIsEmpty(list))
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

int LinkedListIsEmpty(ready_Queue *list)
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

// ready_Q *NewLinkedList(void){}

// void FreeLinkedList(ready_Q *list){}

// int LinkedListIsFull(ready_Q *list){}

// int LinkedListLength(ready_Q *list){}

// PCB *GetLinkedListElement(ready_Q *list, int index){}