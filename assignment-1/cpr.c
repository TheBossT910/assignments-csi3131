/* ------------------------------------------------ ------------
File: cpr.c

Partner 1
Last name: Rashid, Taha
Student number: 300403833

Partner 2
Last name: Leone-Ganado, Samuel
Student number: ?

Description: This program contains the code for creation
 of a child process and attach a pipe to it.
	 The child will send messages through the pipe
	 which will then be sent to standard output.

Explanation of the zombie process
(point 5 of "To be completed" in the assignment):

	(please complete this part);

	// Answer
	Zombie = process is hanging, i.e. it is waiting for an operation to complete, but it is not completing,
	resulting in the program being in this Zombie state???

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
	// Learned from video: https://www.youtube.com/watch?v=EYI7wsAdWWo

	// DEBUG
	fprintf(stderr, "prcNum is (start) %i \n", prcNum);

	// create pipe
	// [0] = read, [1] = write
	int std_pipe[2];
	pipe(std_pipe);

	// create a child process
	int pid = fork();

	if (!pid)	// child process
	{
		// exit when prcNum <= 0
		if (prcNum <= 0)
		{
			return;
		}

		// attach writing end of pipe, std_pipe[1], to standard output of child, 1
		dup2(std_pipe[1], 1);

		// TODO: figure out why this isn't writing to pipe...
		fprintf(stdout, "\nProcess - %i", prcNum);

		// close both read/write ends of the pipe
		close(std_pipe[0]);
		close(std_pipe[1]);

		// execute next recursive call
		int newPrcNum = prcNum - 1;
		char val_str[12];
		snprintf(val_str, sizeof(val_str), "%i", newPrcNum);

		char *args[] = {"./cpr", val_str, NULL};
		execvp(args[0], args);
	}

	// close writing end of the pipe
	close(std_pipe[1]);

	// reading the pipe
	char buffer[100];
	int n = read(std_pipe[0], buffer, 99);
	buffer[n] = 0;
	fprintf(stderr, "PIPE OUTPUT: %s\n", buffer);
}


/* Example output so far
taharashid@Tahas-MacBook-Pro assignment-1 % make cpr && ./cpr 3
cc     cpr.c   -o cpr
prcNum is (start) 3 
prcNum is (start) 2 
prcNum is (start) 1 
prcNum is (start) 0 
PIPE OUTPUT: 
PIPE OUTPUT: 
PIPE OUTPUT: 
PIPE OUTPUT: 
*/

