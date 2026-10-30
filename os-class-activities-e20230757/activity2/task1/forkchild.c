/* forkchild.c — Process creation with fork() and exec() */
#include <stdio.h>      /* Include standard I/O library for printf, perror, and fflush */
#include <stdlib.h>     /* Include standard library for exit() and EXIT_FAILURE */
#include <unistd.h>     /* Include POSIX API header for fork(), sleep(), getpid(), and execlp() */
#include <sys/wait.h>   /* Include process-waiting declarations and macros (waitpid, WIFEXITED) */

int main(void) {        /* Entry point of the program; takes no command-line arguments */
    pid_t pid;          /* Declare variable of type pid_t to store the process ID returned by fork() */

    printf("Parent process (PID: %d) — creating child...\n", getpid()); /* Print initial message showing the current parent PID */
    fflush(stdout);     /* Flush stdout buffer so message appears immediately before fork duplicates buffers */

    pid = fork();       /* Duplicate the calling process into parent and child execution paths */

    if (pid < 0) {      /* Check if fork() returned a negative value, which indicates system call failure */
        perror("fork"); /* Print system error message describing why fork() failed */
        exit(EXIT_FAILURE); /* Terminate the process immediately with an error status code */
    }                   /* Close the fork failure check block */
    else if (pid == 0) {/* Check if PID is 0, which identifies the branch executed by the newly created child */
        /* Child process */
        printf("Child process (PID: %d, Parent PID: %d)\n", /* Print greeting with child PID and parent PID */
               getpid(), getppid());                        /* Pass child PID from getpid() and parent PID from getppid() */

        printf("Child: sleeping 30s — observe the process tree now!\n"); /* Inform user of 30-second pause to view process tree */
        fflush(stdout); /* Force the sleep notice to flush out to the console terminal immediately */

        sleep(30);      /* Pause the child process for 30 seconds so it can be inspected via ps/pstree */

        printf("Child: now executing 'ls -la'...\n"); /* Inform user that sleep is over and exec is about to occur */
        fflush(stdout); /* Ensure message is displayed before current program image is overwritten */

        /* Replace the child process with ls -la */
        execlp("ls", "ls", "-la", (char *)NULL); /* Overwrite process image with 'ls -la'; NULL terminates argument list */

        /* Executed only if execlp fails */
        perror("execlp");   /* Print error if execlp returns, which only happens on replacement failure */
        exit(EXIT_FAILURE); /* Exit with failure status if execlp failed to execute the command */
    }                   /* Close the child process branch */
    else {              /* Enter parent process branch; pid > 0 holding the child's process ID */
        /* Parent process */
        int status;     /* Declare an integer to store termination status information filled by waitpid() */

        printf("Parent: waiting for child (PID: %d) to finish...\n", pid); /* Log parent waiting for specific child PID */
        fflush(stdout); /* Flush the parent's waiting message to output right away */

        if (waitpid(pid, &status, 0) == -1) { /* Suspend parent execution until child finishes; return -1 on error */
            perror("waitpid");                /* Print error message if waitpid system call failed */
            exit(EXIT_FAILURE);               /* Terminate parent process with failure code */
        }                                     /* Close waitpid error handling check */

        if (WIFEXITED(status)) {              /* Evaluate to true if child terminated normally (e.g. exit/return) */
            printf("Parent: child exited with status %d\n", /* Print formatted message reporting normal exit */
                   WEXITSTATUS(status));                    /* Extract the low-order 8 bits representing exit status code */
        }                                     /* Close normal exit condition check */
        else if (WIFSIGNALED(status)) {       /* Evaluate to true if child was terminated abruptly by a signal */
            printf("Parent: child terminated by signal %d\n", /* Print formatted message reporting signal termination */
                   WTERMSIG(status));                         /* Extract the number of the signal that caused child termination */
        }                                     /* Close signal termination condition check */

        printf("Parent: done.\n");            /* Print final message confirming parent has finished execution */
    }                   /* Close the parent process branch */

    return 0;           /* Return 0 to OS indicating successful execution of the main program */
}                       /* Close the main function block */
