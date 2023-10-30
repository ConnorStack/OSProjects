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
    // printf("Adding element to the ready queue\n");
    if (list->head != NULL)
    {
        printf("Adding element to ready queue. Elements exists already\n");
        list->tail->next = element;
        list->tail = element;
        element->next = NULL;
    }
    else
    {
        printf("Adding element to ready queue. Head is null\n");
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
        return NULL; 
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

// void delistByPR(PCB **head, int targetPR) {
//     // Pointer to traverse the linked list
//     PCB *current = *head;
//     PCB *prev = NULL;

//     // Traverse the list to find the node with the target PR value
//     while (current != NULL && current->PR != targetPR) {
//         prev = current;
//         current = current->next;
//     }

//     // If the node with the target PR is found
//     if (current != NULL) {
//         // Update the pointers to remove the node
//         if (prev == NULL) {
//             // If the target node is the head of the list
//             *head = current->next;
//         } else {
//             prev->next = current->next;
//         }

//         // Free the memory of the removed node
//         free(current);
//     } else {
//         // Node with the target PR not found
//         printf("Node with PR %d not found\n", targetPR);
//     }
// }

// PCB *findHighestPriority(PCB *head) {
//     PCB *current = head;
//     PCB *highestPriorityPCB = NULL;

//     while (current != NULL) {
//         if (highestPriorityPCB == NULL || current->PR > highestPriorityPCB->PR) {
//             highestPriorityPCB = current;
//         }
//         current = current->next;
//     }

//     return highestPriorityPCB;
// }
PCB *findAndRemoveHighestPriority(ready_Queue *ready_queue) {
    PCB **head = &(ready_queue->head);
    PCB *current = *head;
    PCB *prev = NULL;
    PCB *highestPriorityPCB = NULL;
    PCB *prevHighestPriorityPCB = NULL;

    while (current != NULL) {
        if (highestPriorityPCB == NULL || current->PR > highestPriorityPCB->PR) {
            prevHighestPriorityPCB = prev;
            highestPriorityPCB = current;
        }
        prev = current;
        current = current->next;
    }

    if (highestPriorityPCB != NULL) {
        if (prevHighestPriorityPCB == NULL) {
            *head = highestPriorityPCB->next;
        } else {
            prevHighestPriorityPCB->next = highestPriorityPCB->next;
        }
    }

    return highestPriorityPCB;
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

void print_PCB(const PCB *pcb)
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
