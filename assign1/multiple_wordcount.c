/*
 * File: multiple_wordcount.c
 * Author: Connor Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * This program takes n number of .txt file inputs, a for loop will iterate n times
 * each iteration will call fork, each child process will call wordcount.c
 * #include <sys/wait.h> gave me some trouble on my windows, but using wsl as my terminal was fine
 */

int main(int argc, char *argv[])
{
  int status;

  if (!(argc >= 2))
  {
    perror("Program must take one or more input files");
    exit(1);
  }

  // For each file, fork() another process, have that child execute wordcount
  for (int i = 1; i < argc; i++)
  {
    pid_t cpid = fork();
    if (cpid == -1)
    {
      perror("Failed to fork");
      exit(1);
    }
    if (cpid == 0)
    {

      char *program_to_execute = "./wordcount";
      char *filename_argument = argv[i];
      //Last element must be null for execvp to work
      char *args[] = {program_to_execute, filename_argument, NULL};
      //From what I understand, we pass the program to execute as the first parameter, 
      // and an array of arguments as the second parameter, must be null terminated
      execvp(program_to_execute, args);
    }
    else
    {
      // This code executes in the parent process, which needs to wait 
      // for the child processes to complete to avoid zombies
      cpid = wait(&status);

      pid_t status = wait(&cpid);
      if(cpid == -1){
        perror("something went wrong while waiting");
      }
      else if (WIFEXITED(status))
      {
        printf("Child %ld terminated with return status: %d\n", (long)cpid, WEXITSTATUS(status));
      }
    }
  }

  return 0;
}
