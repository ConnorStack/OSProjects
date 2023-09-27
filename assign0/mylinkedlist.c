/*
 * File: mylinkedlist.c
 * Connor Stack
 * YOU NEED TO IMPLEMENT THE FUNCTIONS  here
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylinkedlist.h"

/*
 * Function: NewStudentCell
 * Usage: student_cell_T *element;
 *        element = NewStudentCell(int id, double gpa, char *name);
 * --------------------------
 * This function allocates space for a student cell and intilize its fileds
 */
student_cell_T *NewStudentCell(int id, double gpa, char *name)
{
  student_cell_T *element;

  element = (student_cell_T *)malloc(sizeof(student_cell_T));
  if (!element)
  {
    fprintf(stderr, "NewStudentCell cannot allocate memory\n");
    return NULL;
  }
  element->id = id;
  element->gpa = gpa;
  element->name = strdup(name);

  return element;
}

/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void)
{
  linked_list_T *list;

  list = (linked_list_T *)malloc(sizeof(linked_list_T));
  if (!list)
  {
    fprintf(stderr, "NewLinkedList cannot allocate memory\n");
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;

  return list;
}

/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list)
{
  student_cell_T *current = list->head;
  student_cell_T *next;

  while (current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }

  free(list);
}

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, student_cell_T *element)
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

/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
student_cell_T *Delist(linked_list_T *list)
{
  if (LinkedListIsEmpty(list))
  {
    printf("\n Error, List is empty\n");
    return NULL;
  }

  student_cell_T *removedElement = list->head;
  list->head = list->head->next;

  removedElement->next = NULL;

  if (list->head == NULL)
  {
    list->tail = NULL;
  }

  return removedElement;
}

/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list)
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

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T *list)
{
  int length = 0;
  student_cell_T *current = list->head;

  while (current != NULL)
  {
    length++;
    current = current->next;
  }

  return length;
}

/*
 * Function: GetLinkedListElement
 * Usage: element = GetLinkedListElement(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetLinkedListElement(list, 0) returns the initial
 * element from the list without removing it.  If the caller tries
 * to select an element that is out of range, GetLinkedListElement prints
 * Error and returns NULL.  Note: This function is not a fundamental list operation
 * and is instead provided mainly to facilitate debugging.
 */
student_cell_T *GetLinkedListElement(linked_list_T *list, int index)
{
  student_cell_T *current = list->head;
  int count = 0;
  int linkedListLength = LinkedListLength(list);
  if (linkedListLength > index)
  {
    while (current != NULL)
    {
      if (count == index)
      {
        printf("\nIndex found!\n");
        return current;
      }
      count++;
      current = current->next;
    }
  }
  else
  {
    printf("\n---------------------------\n");
    printf(" --- Index Not Found --- ");
    printf("\n---------------------------\n");
  }

  return NULL;
}

/* OTHER FUNCTIONS YOU WOULD NEED....
 * EXPORT THEM HERE, BUT IMPLMENT THEM in mylinkedlist.c
 */

/*
* Not the best way, but a 'bool' function that determines if
* the studentId already exists in the linkedList
* there is probably a smarter way to do this
*/
int doesContainStudent(linked_list_T *list, int studentId)
{
  int doesContain = 0;
  student_cell_T *current = list->head;

  while (current != NULL)
  {
    if (current->id == studentId)
    {
      doesContain = 1;
      return doesContain;
    }

    current = current->next;
  }

  return doesContain;
}

/*
* Trying to keep main less cluttered with this one
*/
void getMenuSelection(int *select)
{
  printf("\nMenu:\n\n");
  printf("1 : Create/Add student\n");
  printf("2 : Remove the first student in the list\n");
  printf("3 : Print the total number of students\n");
  printf("4 : Print student info at given index\n");
  printf("5 : Print all students\n");
  printf("6 : Print MIN, AVG, and MAX GPAs\n");
  printf("7 : Remove student with highest GPA\n");
  printf("8 : Exit\n\n");
  printf("Enter your choice: \n");

  scanf("%d", select);
}

/* 
* When main needs an integer value read, this is called
* I was having trouble with scanf and leftovers in the input buffer
* So I added this extra while loop to scoop up the extras
*/
int readInt()
{
  int intValue;
  printf("Entry must be of integer type: ");
  if (scanf("%d", &intValue) != 1)
  {
    printf("Invalid Input \n---Make sure to enter an integer value \n");
    exit(1);
  }
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  return intValue;
}

/* 
* When main needs an double value read, this is called
* I was having trouble with scanf and leftovers in the input buffer
* So I added this extra while loop to scoop up the extras
*/
double readDouble()
{
  double doubleValue;
  printf("Entry must be of double type: ");
  if (scanf("%lf", &doubleValue) != 1)
  {
    printf("Invalid Input \n---Make sure to enter a number value \n");
    exit(1);
  }
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  return doubleValue;
}

/*
* Goes through the linked list and prints the contents of each cell
*/
void printEntireLinkedlist(linked_list_T *list)
{
  if (!LinkedListIsEmpty(list))
  {
    student_cell_T *current = list->head;

    while (current != NULL)
    {
      printf("\n----------------------------------\n");
      printf("Student ID: %d\n", current->id);
      printf("Student GPA: %.2lf\n", current->gpa);
      printf("Student Name: %s\n", current->name);
      printf("----------------------------------\n");

      current = current->next;
    }
  }
  else
  {
    printf("List is empty! Nothing to print");
  }
}

/*
* Goes through the whole linked list to find the minimum gpa
* Set to 4.0 first, so the first comparison will be either <= 4
* In the case of an empty linked list, minGpa is set to 0
*/
double getMinGpa(linked_list_T *list)
{
  double minGpa = 4.0;

  if (!LinkedListIsEmpty(list))
  {
    student_cell_T *current = list->head;
    while (current != NULL)
    {
      if (current->gpa < minGpa)
      {
        minGpa = current->gpa;
      }
      current = current->next;
    }
  }
  else{
    minGpa = 0.0;
  }
  return minGpa;
}

/*
* Goes through the whole linked list to find the maximum gpa
* Set to 0.0 first, so the first comparison will be either >= 0
* In the case of an empty linked list, maxGpa is still set to 0
*/
double getMaxGpa(linked_list_T *list)
{
  double maxGpa = 0.0;
  if (!LinkedListIsEmpty(list))
  {
    student_cell_T *current = list->head;
    while (current != NULL)
    {
      if (current->gpa > maxGpa)
      {
        maxGpa = current->gpa;
      }
      current = current->next;
    }
  }
  return maxGpa;
}

/*
* Goes through the whole linked list to find the average gpa
* Find the sum and divide by number of elements
*/
double getAvgOfAllGpa(linked_list_T *list)
{
  double avgGpa = 0.0;
  double totalGpa = 0.0;
  int count = 0;
  if (!LinkedListIsEmpty(list))
  {
    student_cell_T *current = list->head;
    while (current != NULL)
    {
      totalGpa += current->gpa;
      count++;
      current = current->next;
    }
    avgGpa = totalGpa / count;
  }

  return avgGpa;
}

/*
* In Main, the gpa being passed to this method came from getMaxGpa
* So maxGpa is implied, but any gpa could be passed it and it would be removed
* If not found, return null
*/
student_cell_T *delistByGpa(linked_list_T *list, double gpa)
{

  if (LinkedListIsEmpty(list))
  {
    printf("\n Error, List is empty\n");
    return NULL;
  }

  student_cell_T *current = list->head;
  student_cell_T *previous = NULL;

  while (current != NULL)
  {

    if (current->gpa == gpa)
    {
      if (previous == NULL)
      {
        list->head = current->next;
      }
      else
      {
        previous->next = current->next;
      }

      current->next = NULL;
      return current;
    }
    previous = current;
    current = current->next;
  }

  return NULL;
}