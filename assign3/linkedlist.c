#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

PCB *NewPCB(){}

linked_list_T *NewLinkedList(void){}

void FreeLinkedList(linked_list_T *list){}

void Enlist(linked_list_T *list, PCB *element){}

PCB *Delist(linked_list_T *list){}

int LinkedListIsEmpty(linked_list_T *list){}

int LinkedListIsFull(linked_list_T *list){}

int LinkedListLength(linked_list_T *list){}

PCB *GetLinkedListElement(linked_list_T *list, int index){}