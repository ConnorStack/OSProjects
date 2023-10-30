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
		printf("Adding element to the beginning of the IO_queue\n");
	if (list->head == NULL) {
		// If the list is empty, simply set the head and tail to the element
		list->head = element;
		list->tail = element;
		element->next = NULL;
	} else {
		// If the list is not empty, set the element's next to the current head
		element->next = list->head;
		list->head = element;
	}

}

// void enlist_to_IO_queue(IO_Queue *list, PCB *element)
// {
	// if (list->head != NULL)
	// {
	//      printf("Step 1: Adding element to the non-empty list\n");
	//     list->tail->next = element;
	//     list->tail = element;
	//     element->next = NULL;
	// }
	// else
	// {
	//     printf("Step 2: Adding element as the first node in an empty list\n");
	//     list->head = element;
	//     list->tail = element;
	//     element->next = NULL;
	// }
// }

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

int IO_queue_length(IO_Queue *list)
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

// int LinkedListIsFull(IO_Queue *list){}

// int LinkedListLength(IO_Queue *list){}

// PCB *GetLinkedListElement(IO_Queue *list, int index){}