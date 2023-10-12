
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

// void FreeLinkedList(IO_Q *list){}

// void Enlist(IO_Q *list, PCB *element){}

// PCB *Delist(IO_Q *list){}

// int LinkedListIsEmpty(IO_Q *list){}

// int LinkedListIsFull(IO_Q *list){}

// int LinkedListLength(IO_Q *list){}

// PCB *GetLinkedListElement(IO_Q *list, int index){}