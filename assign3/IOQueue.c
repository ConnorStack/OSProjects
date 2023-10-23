
#include <stdlib.h>
#include "IOQueue.h"

IO_Queue *new_IO_queue(){
    IO_Queue * list;
    list = (IO_Queue*)malloc(sizeof(IO_Queue));

    if(list == NULL){
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

// IO_Q *NewLinkedList(void){}

// void FreeLinkedList(IO_Queue *list){}

void enlist_to_IO_queue(IO_Queue *list, PCB *element){
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

// PCB *Delist(IO_Queue *list){}

int IO_Q_is_empty(IO_Queue *list){}

// int LinkedListIsFull(IO_Queue *list){}

// int LinkedListLength(IO_Queue *list){}

// PCB *GetLinkedListElement(IO_Queue *list, int index){}