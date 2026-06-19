/* -------------------------------------------------------------
File: university.c

Partner 1
Last name: Rashid, Taha
Student number: 300403833

Partner 2
Last name: Leone-Ganado, Samuel
Student number: 300405286

Description: This program contains the code for [xyz]
------------------------------------------------------------- */
#include <stdio.h>

// Psuedo code
queue students;

// students program, and request help
void student()
{
    while (true)
    {
        // student programming
        int randNum = rand();
        sleep(rand);

        // student needing TA help
        requestHelp();
    }
    return;
}

// TA sleeps, does one-on-one with students
void teachingAssistant()
{
    while(true)
    {
        // trigger via semaphore
        while (nobodyWaiting)
        {
            sleep();
        }
    
        // one-on-one with student
        semaphoreInc; // waiting room
        student = pop();

        mutex lock;
        // critical section. TA helping students
        int randNum = rand()
            sleep(rand);
        mutex unlock;
    }
}

// students perform the action of requesting help
void requestHelp()
{
    if (semaphoreHasSpace)
    {
        semaphoreDec;
        push(student);
        teachingAssistant();
    }

    // return programming either after done with TA, or no space
    return;
}

// actually run the program
int main(int ac, char **av)
{
    int processNumber;

    if (ac == 2)
    {
        if (sscanf(av[1], "%d", &processNumber) == 1)
        {
            // create TA thread
            makeTAThread();

            // create n amount of children threads
            for (x, y, z)
            {
                makeChildThread()
            }
        }
        else
            fprintf(stderr, "Cannot translate argument\n");
    }
    else
        fprintf(stderr, "Invalid arguments\n");
    return (0);
}
