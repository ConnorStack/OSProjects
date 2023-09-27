/*
 * File: driver1.c
 * Connor Stack
 * YOUR NAME ... YOU NEED TO IMPLEMENT THE main() + additional functions if needed
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylinkedlist.h"

/*
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 */
char *ReadLine(void);

int main(int argc, char *arvg[]) // I think this is a typo, argv
{
  // YOU NEED TO IMPLEMENT THIS driver1.c USING FUNCTIONS FROM mylinkedlist.h
  // But before that, implement your ReadLine() function, and test it as shown below.
  // IF NEEDED, YOU CAN ALSO IMLEMENT YOUR OWN FUNCTIONS HERE

  char *name;
  // name = ReadLine();
  // printf("User entered : %s \n", name);
  // free(name);
  // printf("Enter a name to test your ReadLine function  : ");
  // printf("User entered : %s \n", name);
  // free(name);

  linked_list_T *linkedList = NewLinkedList();

  student_cell_T *delistedItem;
  student_cell_T *searchByIndex;
  student_cell_T *delistByMaxGpa;

  int listLength = 0;
  int select = 0;
  int index;
  int studentId;
  double GPA;
  double minGpa;
  double maxGpa;
  double avgGpa;

  while (1)
  {
    getMenuSelection(&select);
    // printf("You chose: %d", select);

    switch (select)
    {
    // case 1 uses custom methods readInt(), readDouble, and ReadLine() to gather input
    case 1:
      printf("\nEnter Student ID ... ");
      studentId = readInt();
      printf("Enter GPA ... ");
      GPA = readDouble();
      if (GPA < 0.0 || GPA > 4.0)
      {
        printf("Invalid GPA, breaking out of menu");
        break;
      }

      if (doesContainStudent(linkedList, studentId) == 1)
      {
        printf("\n-------------------------------------------------------------");
        printf("\n\nTHAT STUDENT ALREADY EXISTS, THIS ENTRY WAS NOT ADDED\n\n");
        printf("-------------------------------------------------------------");
      }
      //This is where we add the cell to our linked list
      else
      {
        printf("Enter student name: ");
        name = ReadLine();
        if (name != NULL)
        {
          //create a new cell
          student_cell_T *newCell = NewStudentCell(studentId, GPA, name);
          free(name);

          if (newCell != NULL)
          {
            //enlist it to our linked list
            Enlist(linkedList, newCell);
          }
          else
          {
            fprintf(stderr, "Could not create new student cell");
          }
        }
        else{
          printf("Error with name entered");
        }
      }
      break;

    //Delist the first student from the linkedlist and display its contents
    case 2:
      delistedItem = Delist(linkedList);
      if (delistedItem != NULL)
      {
        printf("\n---------------------------------------\n");
        printf("REMOVING THIS STUDENT FROM THE LIST\n");
        printf("Student ID: %d\n", delistedItem->id);
        printf("Student GPA: %.2lf\n", delistedItem->gpa);
        printf("Student Name: %s\n", delistedItem->name);
        printf("---------------------------------------\n");
      }
      else
      {
        printf("\nError removing student");
      }
      break;

    //Print the length of the linked list
    case 3:
      listLength = LinkedListLength(linkedList);
      printf("\n-------------------------------\n");
      printf("Linked List Length : %d", listLength);
      printf("\n-------------------------------\n");
      break;

    //Print student cell by index, 0-indexed
    case 4:
      printf("\nEnter the index you to search ... ");
      index = readInt();
      searchByIndex = GetLinkedListElement(linkedList, index);
      if (searchByIndex != NULL)
      {
        printf("\n---------------------------------------\n");
        printf("Student ID: %d\n", searchByIndex->id);
        printf("Student GPA: %.2lf\n", searchByIndex->gpa);
        printf("Student Name: %s\n", searchByIndex->name);
        printf("---------------------------------------\n");
      }
      break;

    //print all students 
    case 5:
      printf("\n----------------------------------\n");
      printf("----Printing all student info-----");
      printf("\n----------------------------------\n");
      printEntireLinkedlist(linkedList);
      break;

    //find and display each category 
    case 6:
      minGpa = getMinGpa(linkedList);
      maxGpa = getMaxGpa(linkedList);
      avgGpa = getAvgOfAllGpa(linkedList);
      printf("\n-----------------------------\n");
      printf("Min gpa is : %.2lf\n", minGpa);
      printf("Max gpa is : %.2lf\n", maxGpa);
      printf("Avg gpa is : %.2lf\n", avgGpa);
      printf("-----------------------------\n");
      break;

    //Delist student with highest GPA from the linkedlist and display its contents
    case 7:
      maxGpa = getMaxGpa(linkedList);
      delistByMaxGpa = delistByGpa(linkedList, maxGpa);
      if(delistByMaxGpa != NULL){
      printf("\n---------------------------------------\n");
      printf("REMOVING THIS STUDENT FROM THE LIST\n");
      printf("Student ID: %d\n", delistByMaxGpa->id);
      printf("Student GPA: %.2lf\n", delistByMaxGpa->gpa);
      printf("Student Name: %s\n", delistByMaxGpa->name);
      printf("---------------------------------------\n");
      }
      break;

    //Free memory and exit program
    case 8:
      printf("\n\nENDING PROGRAM\n\n");
      FreeLinkedList(linkedList);
      exit(1);
      break;
    }
  }

  return 0;
}

/*
 * IMPLEMENTATION of ReadLine();
 * Function: ReadLine
 * Usage: s = ReadLine();
 * ---------------------
 * ReadLine reads a line of text from standard input and returns
 * the line as a string.  The newline '\n' character that terminates
 * the input is not stored as part of the string.
 *
 * In contrast to standard I/O functions (e.g., scanf with "%s", fgets)
 * that can read strings into a given static size buffer, ReadLine function
 * should read the given input line of characters terminated by a newline
 * character ('\n') into a dynamically allocated and resized buffer based on
 * the length of the given input line.
 *
 * When implementing this function you can use standard I/O functions.
 * We just want you to make sure you allocate enough space for the entered data.
 * So don't simply allocate 100 or 1000 bytes every time.
 * If the given input has 5 characters, you need to allocate space for 6 characters.
 *
 * Hint: initially dynamically allocate an array of char with size 10.
 * Then, read data into that array character by charecter (e.g, you can use getchar()).
 * If you see '\n' char before reading 10th character, you are done. And you know the
 * exact length of the input string. So, accordingly allocate enough space and copy the
 * data into new char array, insert '\0' instead of '\n' and free the original array.
 * Then return the new string. However, if you DO NOT see '\n' char after 10th character,
 * then you need larger space. Accordingly, resize your original array and double its size
 * and continue reading data character by character as in the first step...
 * Hope you got the idea!
 *
 * Also please check for possible errors (e.g., no memory etc.) and appropriately handle
 * them. For example, if malloc returns NULL, free partially allocated space and return
 * NULL from this function. The program calling this function may take other actions,
 * e.g., stop the program!
 */
char *ReadLine()
{
  //allocate a default size of memory, and use memory error check
  size_t bufferSize = 10;
  char *buffer = (char *)malloc(bufferSize * sizeof(char));
  if (buffer == NULL)
  {
    fprintf(stderr, "Insufficient Memory\n");
    return (NULL);
  }

  int currentPosition = 0; 
  int charIntValue = getchar();
  while (charIntValue != '\n' && charIntValue != EOF)
  {
    //typecase getchar for char value and use it in our buffers current position
    char inputCharValue = (char)charIntValue; 
    buffer[currentPosition] = inputCharValue;
    currentPosition++;

 
    if (currentPosition >= bufferSize)
    {
      //if we exceed our buffer size, double it with realloc, use memory error check
      bufferSize *= 2;
      char *doubledBuffer = (char *)realloc(buffer, bufferSize * sizeof(char));

      if (doubledBuffer == NULL)
      {
        perror("Insufficient memory for reallocation ");
        fprintf(stderr, "Insufficient Memory\n");
        free(buffer);
        return (NULL);
      }
      //reset our buffer 
      buffer = doubledBuffer;
    }
    //read next getchar and start again
    charIntValue = getchar();
  }

  //based on our final positions size + 1 for null terminator, 
  //make a final buffer with appropriate size, use mem error check
  char *finalBuffer = (char *)malloc((currentPosition + 1) * sizeof(char));

  if (finalBuffer == NULL)
  {
    free(buffer);
    perror("Insufficient Memory");
    fprintf(stderr, "Insufficient Memory\n");
    return (NULL);
  }

  //read from buffer into our final buffer with garunteed correct size
  for (int i = 0; i < currentPosition; i++)
  {
    finalBuffer[i] = buffer[i];
  }
  //set the last char as null-terminator
  int finalLength = currentPosition;
  finalBuffer[finalLength] = '\0';

  //free old buffer, return final buffer
  free(buffer);

  return finalBuffer;
}
