/*
 * File: multiple_wordcount.c
 * Author: Connor Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>

/*
 * This program takes n number of file inputs, a for loop will iterate n times
 * each iteration will call fork, each child process will call wordcount.c
 */

int main(int argc, char *argv[])
{
  int last_child_status;
  int programs_ran = 0;
  int programs_failed = 0;

  if (!(argc >= 2))
  {
    perror("Program must take one or more input files");
    exit(1);
  }

  // For each file, fork() another process, have that child execute wordcount
  for (int i = 1; i < argc; i++)
  {
    pid_t cpid = fork();

    //fork failed
    if (cpid == -1)
    {
      perror("Failed to fork");
      exit(1);
    }

    //child process
    if (cpid == 0)
    {

      char *program_to_execute = "./wordcount";
      char *filename_argument = argv[i];
      // Last element must be null for execvp to work
      char *args[] = {program_to_execute, filename_argument, NULL};
      // From what I understand, we pass the program to execute as the first parameter,
      // and an array of arguments as the second parameter, last element = NULL
      execvp(program_to_execute, args);
    }

    // run as many times as needed to wait for all children and avoid zombies
    for (;;)
    {
      //If I understand correctly, wait_status holds one integer value, either -1 as failure, or pid of child
      //last_child_status, is given a different integer value to represent the termination status of the child
      //so we can use the last_child_status to determine its status with macros, 
      //and we can check if wait_status failed outright, and can break immediately 
      pid_t wait_status = wait(&last_child_status);

      //break if there was an error waiting
      if ((wait_status == -1) && (errno != EINTR))
      {
        perror("something went wrong while waiting");
        break;
      }

      //use WIFEXITED macro with last_child_status to determine if child exited 
      if (WIFEXITED(last_child_status))
      {
        //now use WEXITSTATUS since we know the child exited and determine if it exited successfully
        int wait_exit_status = WEXITSTATUS(last_child_status);
        if (wait_exit_status == 0)
        {
          programs_ran++;
        }
        else
        {
          programs_failed++;
        }
      }
      break;
    }
  }
  printf("\n%d files counted successfully!\n", programs_ran);
  printf("%d files did not exist\n", programs_failed);
  return 0;
}
