/*
 * File: wordcount.c
 * Author: Connor Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Wordcount.c takes one file argument and counts the number of words in that file.
 */

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
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
  // Move pointer back to the beginning so we can start reading
  fseek(file_ptr, 0, SEEK_SET);

  // Allocate based on the calculated file size, +1 for null terminator
  char *file_contents = (char *)malloc(file_size + 1);
  // Catch bad memory allocation, close file
  if (file_contents == NULL)
  {
    perror("could not allocate memory for file contents");
    fclose(file_ptr);
    exit(1);
  }

  // Read the file contents into memory
  // fread(write to file_contents, inc by 1 byte, for length of file_size, from file_ptr)
  // close the file
  fread(file_contents, 1, file_size, file_ptr);
  fclose(file_ptr);

  // Add null termination to the end of file contents
  file_contents[file_size] = '\0';

  // loop until 'token' is null (end of file) increment for each token
  // spaces, newlines, carriage returns should be deliminated
  char *token = strtok(file_contents, " \t\n\r");
  int count = 0;
  while (token != NULL)
  {
    count++;
    token = strtok(NULL, " \t\n\r");
  }

  // Compose output string with argument, process id, and count
  char *filename = argv[1];
  pid_t pid = getpid();
  printf("\nWordcount with process: %d counted words in file %s: number of words is... %d", pid, filename, count);
  free(file_contents);

  exit(0);
}
