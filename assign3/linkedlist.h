

#ifndef _linkedlist_h
#define _linkedlist_h
#include <time.h>


/*
 * Type: PCB
 * -----------
 */
typedef struct PCB {
 int PID, PR;
 int numCPUBurst, numIOBurst;
 int *CPUBurst, *IOBurst;
 int cpuindex, ioindex;
 struct timespec ts_begin, ts_end;
 struct PCB *prev, *next;
 // more fields for performance measures
// use the system time to determine how much waited etc.
} PCB;

/*
 * Type: linked_list_T
 * --------------
 * This type defines the concrete representation of a linked list of student cells.
 * The head field points to the first element in the linked list.
 * The tail field points to the last element in the linked list.
 * The empty linked list is indicated by a NULL head pointer.
 */
typedef struct linked_list_T
{
    PCB *head;
    PCB *tail;
} linked_list_T;

PCB *NewPCB();

linked_list_T *NewLinkedList(void);

void FreeLinkedList(linked_list_T *list);

void Enlist(linked_list_T *list, PCB *element);

PCB *Delist(linked_list_T *list);

int LinkedListIsEmpty(linked_list_T *list);

int LinkedListIsFull(linked_list_T *list);

int LinkedListLength(linked_list_T *list);

PCB *GetLinkedListElement(linked_list_T *list, int index);

//... more methods

#endif