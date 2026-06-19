/* -------------------------------------------------------------
File: university.c

Partner 1
Last name: Rashid, Taha
Student number: 300403833

Partner 2
Last name: Leone-Ganado, Samuel
Student number: 300405286

Description: This program contains the code for assignment 3, 
covering POSIX threads, mutexes, and semaphores.
------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_CHAIRS 3

sem_t sem_students;         // tells TA a student is waiting
sem_t sem_ta;               // tells a student that the TA is ready
pthread_mutex_t mutex;      // protect the students_waiting counter
int students_waiting = 0;   // track students in hallway

void* student(void* arg)
{
    int id = *(int*)arg;

    while (1)
    {
        // student is programming
        printf("Student %d is programming.\n", id);
        // sleep for 1-5 seconds
        sleep(rand() % 5 + 1);

        // student needs help
        pthread_mutex_lock(&mutex);

        // CS, check chairs
        if (students_waiting < MAX_CHAIRS)
        {
            // chair is available
            students_waiting++;
            printf("Student %d takes a seat waiting for the TA. (Students waiting: %d)\n", id, students_waiting);
            
            // notify the TA that a student has arrived
            sem_post(&sem_students);
            
            // unlock CS
            pthread_mutex_unlock(&mutex);

            // wait for TA to be ready for one-on-one
            sem_wait(&sem_ta);
            printf("Student %d is now getting help from the TA.\n", id);
        }
        else
        {
            // no chairs available
            printf("Student %d found no empty chairs and will try again later.\n", id);
            // unlock CS
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

void* teachingAssistant(void* arg)
{
    while (1)
    {
        // TA waits for a student to arrive
        sem_wait(&sem_students);

        // student has arrived, lock CS to modify the waiting count
        pthread_mutex_lock(&mutex);

        // student leaves the waiting room chair to enter the office
        students_waiting--;
        printf("TA is ready and calls a student into the office. (Students waiting: %d)\n", students_waiting);
        sem_post(&sem_ta);

        // unlock CS
        pthread_mutex_unlock(&mutex);

        // TA is helping the student
        printf("TA is currently helping a student.\n");
        // sleep for 1-3 seconds (help)
        sleep(rand() % 3 + 1);
    }
    return NULL;
}

int main(int ac, char **av)
{
    int num_students;
    if (ac == 2)
    {
        if (sscanf(av[1], "%d", &num_students) == 1 && num_students > 0)
        {
            srand(time(NULL));

            // init mutex/semaphores
            pthread_mutex_init(&mutex, NULL);
            sem_init(&sem_students, 0, 0);
            sem_init(&sem_ta, 0, 0);

            pthread_t ta_tid;
            pthread_t student_tids[num_students];
            int student_ids[num_students];

            printf("Creating TA thread\n");
            pthread_create(&ta_tid, NULL, teachingAssistant, NULL);

            // create n student threads
            printf("Creating %d student threads\n", num_students);
            for (int i = 0; i < num_students; i++)
            {
                student_ids[i] = i + 1;
                pthread_create(&student_tids[i], NULL, student, &student_ids[i]);
            }

            // join threads
            pthread_join(ta_tid, NULL);
            for (int i = 0; i < num_students; i++)
            {
                pthread_join(student_tids[i], NULL);
            }
        }
        else
        {
            fprintf(stderr, "Please provide a valid positive integer for the number of students.\n");
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s <number_of_students>\n", av[0]);
    }
    
    return 0;
}