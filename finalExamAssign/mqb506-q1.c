#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Flag to indicate if SIGUSR2 has been received
volatile sig_atomic_t sigusr2_received = 0;

// Signal handler for SIGINT (Ctrl+C)
volatile sig_atomic_t sigint_count = 0;
void sigint_handler(int signum)
{
    // Increment the SIGINT count
    sigint_count++;
    // Print a custom message
    printf("Caught SIGINT. Total count: %d\n", sigint_count);
}

// Signal handler for SIGUSR2
// <CODE>

int main()
{
    // Print process id (PID)                         (1pt)
    printf("Process ID (PID): %d\n", getpid());

    // Block the SIGUSR1 signal                            (1pt)
    sigset_t blockmask;
    sigemptyset(&blockmask);
    sigaddset(&blockmask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &blockmask, NULL) < 0) {
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

    printf("Program starts waiting for SIGUSR2...\n"); //  (5pt)

    // Set up the SIGUSR2 signal handler and
    // Check flag in a while loop using sigsuspend , but make sure you will not
    // miss the signal (so, block SIGUSR2 before while loop, then check cond and use sigsuspend)
    // <CODE>
    printf("Got SIGUSR2, terminating the program.\n");

    // Restore the oldmask which was obtained when blocking SIGUSR2
    // <CODE>

    return 0;
}
