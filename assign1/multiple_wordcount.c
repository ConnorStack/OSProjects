/*
 * File: multiple_wordcount.c
 * Author: Connor Stack
 *
 * ....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// include other standard header files or your own user defined libraries needed

/*
 * YOUR COMMENTS
 */

int main(int argc, char *argv[])
{
  int result;
  int status;
  pid_t cpid;
  if (!(argc >= 2))
  {
    perror("Program must take one or more input files");
    exit(1);
  }

  // For each file, fork() another process, have that child execute wordcount
  // int n = atoi(argv[1]);
  for (int i = 1; i < argc; i++)
  {
    cpid = fork();
    if (cpid == -1)
    {
      perror("Fork failed");
      exit(1);
    }
    if (cpid == 0)
    {
      char cmd_line_input[500];
      strcpy(cmd_line_input, "./wordcount ");
      strcat(cmd_line_input, argv[i]);
      printf("Command to run: %s\n", cmd_line_input);
      result = system(cmd_line_input);

      if (result == -1)
      {
        perror("wordcount failed");
        exit(1);
      }
      exit(0);
    }
    else
    {
      // This code executes in the parent process
      cpid = wait(&status);

      pid_t status = wait(&cpid);
      if(cpid == -1){
        perror("something went wrong while waiting");
      }
      // waitpid(cpid, &status, 0);

      else if (WIFEXITED(status))
      {
        printf("Child %ld terminated with return status: %d\n", (long)cpid, WEXITSTATUS(status));
      }
    }
  }

  return 0;
}

/*
pid_t r_wait(int *stat_loc) {
  int retval;
  while(((retval = wait(stat_loc)) == -1) && (errno == EINTR)) ; //intentional space
  return retval;
}

*/