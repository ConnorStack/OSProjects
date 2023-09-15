/*
 * File: wordcount.c
 * Author: Connor Stack
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// include other standard header files or your own user defined libraries needed

/*
 * YOUR COMMENTS
 */

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage %s processes\n", argv[0]);
    perror("Program only takes one and only one file input");
    exit(1);
  }

  FILE *file_ptr = fopen(argv[1], "r");
  // Catch failed fopen
  if (file_ptr == NULL)
  {
    perror("Either file doesnt exist, or file could not be opened");
    exit(1);
  }
  
  // Move pointer to end of file so we can use ftell to find the byte count
  fseek(file_ptr, 0, SEEK_END);
  int file_size = ftell(file_ptr);
  //Move pointer back to the beginning so we can start reading
  fseek(file_ptr, 0, SEEK_SET);

  // Allocate based on the calculated file size, +1 for null terminator
  char *file_contents = (char *)malloc(file_size + 1);
  // Catch bad memory allocation, close file
  if (file_contents == NULL)
  {
    perror("Error allocating memory");
    fclose(file_ptr);
    exit(1);
  }

  //Read the file contents into memory 
  //fread(write to file_contents, inc by 1 byte, for length of file_size, from file_ptr)
  // close the file
  fread(file_contents, 1, file_size, file_ptr);
  fclose(file_ptr);

  //Add null termination to the end of file contents
  file_contents[file_size] = '\0';


  //If this token is not null, increment count
  //then loop until it is null, incrementing count accordingly
  int count = 0;
  char *token = strtok(file_contents, " ");
  if (token != NULL)
  {
    count++;
    while (token != NULL)
    {
      // printf("\nCounting %s", token);
      count++;
      token = strtok(NULL, " ");
    }
  }

  char *filename = argv[1];
  pid_t pid = getpid();
  // printf("\n\nWord count with process %d counted words in file %s: number of words is... %d\n", pid, filename, count);
  // printf("\n\nCounted words in file %s: number of words is... %d\n", filename, count);
  printf("\n\nWordcount with process: %d counted words in file %s: number of words is... %d\n", pid, filename, count);
  free(file_contents);

  exit(0);
}
