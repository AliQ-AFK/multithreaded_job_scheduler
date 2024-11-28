#ifndef EXECUTION_H
#define EXECUTION_H

#include "job.h"
#include <semaphore.h>
#include <pthread.h>
#include "utils.h"
//this header is for code reusability and readibility :)
// Struct to hold execution arguments
typedef struct {
    job* jobs;               // Pointer to the array of jobs
    int* num_jobs;           // Pointer to the number of jobs
    pthread_mutex_t* print_mutex;    // Mutex for print jobs (if using mutexes)
    pthread_mutex_t* scan_mutex;     // Mutex for scan jobs (if using mutexes)
    sem_t* print_semaphore;          // Semaphore for print jobs (if using semaphores)
    sem_t* scan_semaphore;           // Semaphore for scan jobs (if using semaphores)
} execution_args;


// Function prototypes for execution logic

void process_job(job* jobs, int* num_jobs, const char* job_type, unsigned int* elapsed_time, FILE* log_file, pthread_t thread_id);

void* mutex_job_execution(void* arg);
void* semaphore_job_execution(void* arg);
void* unsynced_job_execution(void* arg);
void execute_all_jobs(job* jobs, int* num_jobs);
void get_current_time(char* buffer, size_t size);




#endif