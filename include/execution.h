#ifndef EXECUTION_H
#define EXECUTION_H

#include "job.h"
#include <semaphore.h>
#include <pthread.h>
#include "utils.h"
//this header is for code reusability and readibility :)
// Add these at the top after the includes
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"

// Log level colors
#define DEBUG_COLOR   COLOR_CYAN
#define INFO_COLOR    COLOR_BLUE
#define WARNING_COLOR COLOR_YELLOW
#define ERROR_COLOR   COLOR_RED
#define SUCCESS_COLOR COLOR_GREEN

// First define execution_summary struct
typedef struct
{
    int total_jobs_processed;
    int print_jobs_completed;
    int scan_jobs_completed;
    unsigned int total_time;
    pthread_t thread_id;
} execution_summary;

// Then use it in execution_args struct
typedef struct
{
    job* jobs;               
    int* num_jobs;           
    pthread_mutex_t* print_mutex;    
    pthread_mutex_t* scan_mutex;     
    sem_t* print_semaphore;          
    sem_t* scan_semaphore;           
    execution_summary* summary;
} execution_args;

// Function prototypes for execution logic

void process_job(job* jobs, int* num_jobs, const char* job_type, unsigned int* elapsed_time, FILE* log_file, pthread_t thread_id, execution_summary* summary);

void* mutex_job_execution(void* arg);
void* semaphore_job_execution(void* arg);
void* unsynced_job_execution(void* arg);
void execute_all_jobs(job* jobs, int* num_jobs);
void get_current_time(char* buffer, size_t size);




#endif