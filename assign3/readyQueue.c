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

void enlist_to_ready_queue(ready_Queue *list, PCB *element)
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

PCB *delist_from_ready_queue(ready_Queue *list)
{
    if (ready_queue_is_empty(list))
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

PCB *get_highest_priority_pcb(ready_Queue *list)
{
    // printf("getting highest priority pcb\n");
    if (list->head == NULL) {
        return NULL; 
    }

    PCB *current = list->head;
    PCB *highest_pr_pcb = current;

    while (current != NULL)
    {
        if (current->PR < highest_pr_pcb->PR)
        {
            highest_pr_pcb = current;
        }
        current = current->next;
    }

    // print_PCB(highest_pr_pcb);
    return highest_pr_pcb;
}

PCB *delist_specific_pcb(ready_Queue *list, PCB *pcb)
{
    if (pcb == NULL)
    {
        return NULL; // Invalid PCB to remove
    }

    // PCB *current = list->head;
    // PCB *prev = NULL;

    if (pcb == NULL)
    {
        return NULL; // Invalid PCB to remove
    }

    if (list->head == pcb) {
        list->head = pcb->next;
    }
    if (list->tail == pcb) {
        list->tail = pcb->prev;
    }

    if (pcb->prev) {
        pcb->prev->next = pcb->next;
    }
    if (pcb->next) {
        pcb->next->prev = pcb->prev;
    }

    pcb->next = pcb->prev = NULL;
    return pcb;
}


int ready_queue_is_empty(ready_Queue *list)
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

int ready_queue_length(ready_Queue *list)
{
    int length = 0;
    PCB *current = list->head;

    while (current != NULL)
    {
        length++;
        current = current->next;
    }

    return length;
}

// ready_Q *NewLinkedList(void){}

// void FreeLinkedList(ready_Queue *list){}

// int LinkedListIsFull(ready_Queue *list){}

// PCB *GetLinkedListElement(ready_Queue *list, int index){}

void print_PCB(const PCB *pcb)
{
    printf("Printing pcb\n");
    if (pcb != NULL)
    {
        printf("PID: %d\n", pcb->PID);
        printf("PR: %d\n", pcb->PR);
        printf("numCPUBurst: %d\n", pcb->numCPUBurst);
        printf("numIOBurst: %d\n", pcb->numIOBurst);

        printf("CPUBurst: ");
        for (int i = 0; i < pcb->numCPUBurst; i++)
        {
            printf("%d ", pcb->CPUBurst[i]);
        }
        printf("\n");

        printf("IOBurst: ");
        for (int i = 0; i < pcb->numIOBurst; i++)
        {
            printf("%d ", pcb->IOBurst[i]);
        }
        printf("\n");

        // Print other fields as needed
    }
    else
    {
        printf("PCB is NULL\n");
    }
}

void print_PCBs_in_list(const ready_Queue *list)
{
    PCB *current = list->head;
    printf("attempting print ...\n");
    while (current != NULL)
    {
        // Call the print_PCB function to print the current PCB
        print_PCB(current);

        // Move to the next PCB in the list
        current = current->next;
        printf("\n");
    }
}