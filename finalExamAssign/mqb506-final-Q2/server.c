/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

struct student_info
{
  char ID[7];
  char name[20];
  double grade;
};

main(argc, argv) int argc;
char *argv[];
{
  struct sockaddr_in sad; /* structure to hold server's address  */
  int port;               /* protocol port number                */

  struct sockaddr_in cad; /* structure to hold client's address  */
  int alen;               /* length of address                   */

  int welcomeSocket, connectionSocket; /* socket descriptors  */

  char clientSentence[128];
  char capitalizedSentence[128];
  char buff[128];
  int i, n;

  struct student_info DB[10] = {
      {"abc123", "Turgay", 99.0},
      {"def456", "Alice", 87.5},
      {"ghi789", "Bob", 92.3},
      {"jkl123", "Charlie", 78.9},
      {"mno456", "Diana", 94.7},
      {"pqr789", "Eva", 85.2},
      {"stu123", "Frank", 88.6},
      {"vwx456", "Grace", 91.8},
      {"yza789", "Hank", 79.4},
      {"bcd123", "Ivy", 96.5}};

  /* Check command-line argument for protocol port and extract   */
  /* port number if one is specified. Otherwise, give error      */
  if (argc > 1)
  {                       /* if argument specified        */
    port = atoi(argv[1]); /* convert argument to binary   */
  }
  else
  {
    fprintf(stderr, "Usage: %s port-number\n", argv[0]);
    exit(1);
  }

  /* Create a socket */

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0); /* CREATE SOCKET */
  if (welcomeSocket < 0)
  {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }

  /* Bind a local address to the socket */

  memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure   */
  sad.sin_family = AF_INET;             /* set family to Internet     */
  sad.sin_addr.s_addr = INADDR_ANY;     /* set the local IP address   */
  sad.sin_port = htons((u_short)port);  /* set the port number        */

  if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0)
  {
    fprintf(stderr, "bind failed\n");
    exit(1);
  }

  /* Specify the size of request queue */

  if (listen(welcomeSocket, 10) < 0)
  {
    fprintf(stderr, "listen failed\n");
    exit(1);
  }

  /* Main server loop - accept and handle requests */

  while (1)
  {
    alen = sizeof(cad);
    if ((connectionSocket = accept(welcomeSocket, (struct sockaddr *)&cad, &alen)) < 0)
    {
      fprintf(stderr, "accept failed\n");
      exit(1);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
      // Handle fork error
    }
    else if (pid == 0)
    {
      // Child process
      close(welcomeSocket); // Child doesn't need the welcome socket

      // Process client requests
      while (1)
      {
        // Receive message from the client
        n = read(connectionSocket, buff, sizeof(buff));
        buff[n] = '\0'; // Ensure null-termination
        if (strcmp(buff, "GET_GRADE ID") == 0)
        {
          printf("Getting grade id\n");
          // Process "GET_GRADE ID"
          // Look up student in DB and send reply
        }
        else if (strcmp(buff, "GET_MAX") == 0)
        {
          printf("Getting max\n");
          // Process "GET_MAX"
          // Calculate maximum grade and send reply
        }
        else if (strcmp(buff, "GET_MIN") == 0)
        {
          printf("Getting min\n");
          // Process "GET_MIN"
          // Calculate minimum grade and send reply
        }
        else if (strcmp(buff, "GET_AVG") == 0)
        {
          printf("Getting avg\n");
          // Process "GET_AVG"
          // Calculate average grade and send reply
        }
        else if (strcmp(buff, "STOP") == 0)
        {
          printf("Stopping\n");
          // Process "STOP"
          // Close connection and terminate
          close(connectionSocket);
          exit(0);
        }
        else
        {
          printf("unknown message\n");
        }

        // Send the appropriate response back to the client
        // Continue this loop until "STOP" is received
      }

      // Close the connection socket in the child process
      close(connectionSocket);

      // Terminate the child process
      exit(0);
    }
    else
    {
      // Parent process
      close(connectionSocket); // Parent doesn't need the connection socket
    }
    /* Get the sentence from the client */

    // clientSentence[0] = '\0';
    // n = read(connectionSocket, buff, sizeof(buff));
    // while (n > 0)
    // {
    //   strncat(clientSentence, buff, n);
    //   if (buff[n - 1] == '\0')
    //     break;
    //   n = read(connectionSocket, buff, sizeof(buff));
    // }

    // /* Capitalize the sentence */

    // for (i = 0; i <= strlen(clientSentence); i++)
    // {
    //   if ((clientSentence[i] >= 'a') && (clientSentence[i] <= 'z'))
    //     capitalizedSentence[i] = clientSentence[i] + ('A' - 'a');
    //   else
    //     capitalizedSentence[i] = clientSentence[i];
    // }

    /* send it to the client */

    write(connectionSocket, capitalizedSentence, strlen(capitalizedSentence) + 1);

    close(connectionSocket);
  }

  return 0;
}
