#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Flag to indicate if SIGUSR2 has been received
volatile sig_atomic_t sigusr2_received = 0;
volatile sig_atomic_t sigint_count = 0;

void sigint_handler(int signum)
{
    sigint_count++;
    printf(" Caught SIGINT. Total count: %d\n", sigint_count);
}

// Signal handler for SIGUSR2
void sigusr2_handler(int signum)
{
    sigusr2_received = 1;
}

int main()
{
    // Print process id (PID)                         (1pt)
    printf("Process ID (PID): %d\n", getpid());

    // Block the SIGUSR1 signal                            (1pt)
    sigset_t blockmask;
    sigemptyset(&blockmask);
    sigaddset(&blockmask, SIGUSR1);
    sigaddset(&blockmask, SIGUSR2);
    sigaddset(&blockmask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &blockmask, NULL) < 0)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    // Set up the SIGINT (Ctrl+C) signal handler         (3pt)
    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    if (sigaction(SIGINT, &sa_int, NULL) < 0)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up the SIGUSR2 signal handler 
    // Check flag in a while loop using sigsuspend , but make sure you will not
    // miss the signal (so, block SIGUSR2 before while loop, then check cond and use sigsuspend)
    struct sigaction sa_usr2;
    sa_usr2.sa_handler = sigusr2_handler;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;

    if (sigaction(SIGUSR2, &sa_usr2, NULL) < 0)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigdelset(&blockmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &blockmask, NULL) < 0)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    printf("Program starts waiting for SIGUSR2...\n"); 
    
    
    sigdelset(&blockmask, SIGINT);

    while (!sigusr2_received)
    {
        sigsuspend(&blockmask);
        if (sigusr2_received)
        {
            printf("Got SIGUSR2, terminating the program.\n");
            break; 
        }
        
    }
    // Restore the oldmask which was obtained when blocking SIGUSR2
    if (sigprocmask(SIG_UNBLOCK, &blockmask, NULL) < 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    return 0;
}
