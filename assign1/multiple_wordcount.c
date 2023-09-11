/*
 * File: multiple_wordcount.c
 * Author: Connor Stack
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// include other standard header files or your own user defined libraries needed 

 

/*
 * YOUR COMMENTS
 */


int main(int argc, char *argv[])
{
  // YOU NEED TO IMPLEMENT THIS + some other functions if needed! 
  printf("\nInside multiple word count main\n");

  //this program will take multiple input files as command line arguments
  //use argc - 1 do determine how many files will be processed

  //For each file, fork() another process, have that child execute wordcount
  char command[500];
  for (int i = 1; i < argc; i++){
    strcpy(command, "./wordcount ");
    strcat(command, argv[i]);
    printf("Command to run: %s\n", command);
    system(command);
  }

  return 0;
}
   