
#include <stdlib.h>
#include "readyQueue.h"

ready_Queue *new_ready_queue(){
    ready_Queue *list;

    list = (ready_Queue *)malloc(sizeof(ready_Queue));
    if(list == NULL){
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

// ready_Q *NewLinkedList(void){}

// void FreeLinkedList(ready_Q *list){}

// void Enlist(ready_Q *list, PCB *element){}

// PCB *Delist(ready_Q *list){}

// int LinkedListIsEmpty(ready_Q *list){}

// int LinkedListIsFull(ready_Q *list){}

// int LinkedListLength(ready_Q *list){}

// PCB *GetLinkedListElement(ready_Q *list, int index){}