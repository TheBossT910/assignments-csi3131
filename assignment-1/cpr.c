/* ------------------------------------------------ ------------
File: cpr.c

Partner 1
Last name: Rashid, Taha
Student number: 300403833

Partner 2
Last name: Leone-Ganado, Samuel
Student number: 300405286

Description: This program contains the code for creation
 of a child process and attach a pipe to it.
	 The child will send messages through the pipe
	 which will then be sent to standard output.

Explanation of the zombie process
(point 5 of "To be completed" in the assignment):

	(please complete this part);

    A zombie process occurs when a child process has terminated, but its parent
    has not yet collected the child's termination status (by calling wait or pipe
    EOF). sleep(10) lets us observe the terminated children remaining as zombie 
    processes until their parent terminates.

------------------------------------------------------------- */
#include <stdio.h>
#include <sys/select.h>
// added libraries
#include <sys/types.h>
#include "unistd.h"

/* Prototype */
void createChildAndRead (int);

/* -------------------------------------------------------------
Function: main
Arguments: 
	int ac	- number of command arguments
	char **av - array of pointers to command arguments
Description:
	Extract the number of processes to be created from the
	Command line. If an error occurs, the process ends.
	Call createChildAndRead to create a child, and read
	the child's data.
-------------------------------------------------- ----------- */

int main (int ac, char **av)
{
 int processNumber; 

 if (ac == 2)
 {
 if (sscanf (av [1], "%d", &processNumber)== 1)
 {
    createChildAndRead(processNumber);
 }
    else fprintf(stderr, "Cannot translate argument\n");
 }
    else fprintf(stderr, "Invalid arguments\n");
    return (0);
}


/* ------------------------------------------------ -------------
Function: createChildAndRead
Arguments: 
	int prcNum - the process number
Description:
	Create the child, passing prcNum-1 to it. Use prcNum
	as the identifier of this process. Also, read the
	messages from the reading end of the pipe and sends it to 
	the standard output (df 1). Finish when no data can
	be read from the pipe.
-------------------------------------------------- ----------- */

void createChildAndRead(int prcNum)
{
    printf("Process %i begins\n", prcNum);
    fflush(stdout);

    // base case
    if (prcNum == 1)
    {
        sleep(5);
        printf("Process 1 ends\n");
        fflush(stdout);
        //sleep(10);
        return;
    }

    // make pipe, [0] = stdin, [1] = stdout */
    int std_pipe[2];
    if (pipe(std_pipe) == -1)
    {
        fprintf(stderr, "pipe() failed\n");
        return;
    }

	// make child
    int pid = fork();

	// fork failed
    if (pid == -1)
    {
        fprintf(stderr, "fork() failed\n");
        return;
    }
	// is child process
    else if (pid == 0)
    {
		// attach writing end of pipe, std_pipe[1], to standard output of child, 1
        dup2(std_pipe[1], 1);

		// close the original (parent's) read/write ends
        close(std_pipe[0]);
        close(std_pipe[1]);

        // execute next recursive call (NOTE: img replaces the child)
        char val_str[12];
        snprintf(val_str, sizeof(val_str), "%i", prcNum - 1);
        char *args[] = {"./cpr", val_str, NULL};
        execvp(args[0], args);
    }
	// is parent process
    else
    {
        // close write end of the pipe
        close(std_pipe[1]);

        // read and print the "read" end of the pipe (to get the children process' data)
        char buffer[256];
        int n;
        while ((n = read(std_pipe[0], buffer, sizeof(buffer))) > 0)
        {
            write(1, buffer, n);
        }

		// close read end of pipe
        close(std_pipe[0]);

        printf("Process %d ends\n", prcNum);
        fflush(stdout);
    }
}
