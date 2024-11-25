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

void process_job(job* jobs, int* num_jobs, const char* job_type, unsigned int* elapsed_time, FILE* log_file, pthread_t thread_id);

void* mutex_job_execution(void* arg);
void* semaphore_job_execution(void* arg);
void* unsynced_job_execution(void* arg);
void execute_all_jobs(job* jobs, int* num_jobs);
void get_current_time(char* buffer, size_t size);




#endif