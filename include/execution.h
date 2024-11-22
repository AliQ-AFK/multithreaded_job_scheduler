#ifndef EXECUTION_H
#define EXECUTION_H

#include "job.h"
#include <semaphore.h>
#include <pthread.h>
//this header is for code reusability and readibility :)
// Struct to hold execution arguments
typedef struct
{
    job *jobs;              // Pointer to job array, the other struct created 
    int *num_jobs;           // total
    pthread_mutex_t* mutex; // Pointer to a mutex (can be NULL for unsynced)
    sem_t* semaphore;       // Pointer to a semaphore (can be NULL for unsynced or mutex-based)
} execution_args;

// Function prototypes for execution logic
void* job_execution(void* arg); // Generic execution logic for print/scan jobs
//void execute_all_jobs(job* jobs, int *num_jobs);
void execute_mutex_jobs(job* jobs, int* num_jobs);
#endif