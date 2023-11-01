#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

    return highest_pr_pcb;
}

// PCB *delist_specific_pcb(ready_Queue *list, PCB *pcb)
// {
//     if (pcb == NULL)
//     {
//         return NULL; 
//     }

//     if (list->head == pcb) {
//         list->head = pcb->next;
//     }
//     if (list->tail == pcb) {
//         list->tail = pcb->prev;
//     }

//     if (pcb->prev) {
//         pcb->prev->next = pcb->next;
//     }
//     if (pcb->next) {
//         pcb->next->prev = pcb->prev;
//     }

//     pcb->next = pcb->prev = NULL;
//     return pcb;
// }

void remove_PCB_from_queue(ready_Queue* list, PCB* pcb) {
    if (list == NULL) {
        return; 
    }

    if(pcb == NULL){
        return;
    }

    PCB* currentPCB = list->head;
    PCB* previousPCB = NULL;

    while (currentPCB != NULL && currentPCB != pcb) {
        previousPCB = currentPCB;
        currentPCB = currentPCB->next;
    }

    if (currentPCB == NULL) {
        return; 
    }

    if (previousPCB != NULL) {
        previousPCB->next = currentPCB->next;
    } else {
        list->head = currentPCB->next;
    }

    if (currentPCB == list->tail) {
        list->tail = previousPCB;
    }
}

PCB* find_highest_PR_PCB(ready_Queue* list) {
    if (ready_queue_is_empty(list)) {
        return NULL; 
    }

    PCB* highestPRPCB = NULL;
    int maxPR = INT_MIN; 


    PCB* currentPCB = list->head;

    while (currentPCB != NULL) {
        if (currentPCB->PR > maxPR) {
            maxPR = currentPCB->PR;
            highestPRPCB = currentPCB;
        }

        currentPCB = currentPCB->next;
    }

    return highestPRPCB;
}

PCB* find_lowest_CPU_burst_PCB(ready_Queue* queue) {
    if (ready_queue_is_empty(queue)) {
        return NULL; 
    }

    PCB* lowestCPUBurstPCB = NULL;
    int minCPUBurst = INT_MAX; 

    PCB* currentPCB = queue->head;

    while (currentPCB != NULL) {
        if (currentPCB->numCPUBurst < minCPUBurst) {
            minCPUBurst = currentPCB->numCPUBurst;
            lowestCPUBurstPCB = currentPCB;
        }

        currentPCB = currentPCB->next;
    }

    return lowestCPUBurstPCB;
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

// void FreeLinkedList(ready_Queue *list){}

// PCB *GetLinkedListElement(ready_Queue *list, int index){}

void print_PCB_rq(const PCB *pcb)
{

    if (pcb != NULL)
    {
        printf("PID: %d\n", pcb->PID);
        printf("PR: %d\n", pcb->PR);
        printf("numCPUBurst: %d\n", pcb->numCPUBurst);
        printf("numIOBurst: %d\n", pcb->numIOBurst);
        printf("cpuindex: %d\n", pcb->cpuindex);
        printf("ioindex: %d\n", pcb->ioindex); 

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

void print_rq_PCBs_in_list(const ready_Queue *list)
{
    PCB *current = list->head;
    printf("Printing ready queue ...\n");
    while (current != NULL)
    {
        // Call the print_PCB function to print the current PCB
        print_PCB_rq(current);

        // Move to the next PCB in the list
        current = current->next;
        printf("\n");
    }
}

