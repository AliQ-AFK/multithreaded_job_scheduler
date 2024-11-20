#include "utils.h"
#include <pthread.h>

// Declare and initialize two mutexes
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t scan_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to create and join threads
void create_and_run_threads(job* jobs, int num_jobs)
{
    pthread_t print_thread, scan_thread;

    // Create threads
    pthread_create(&print_thread, NULL, print_job_thread, (void*)jobs);
    pthread_create(&scan_thread, NULL, scan_job_thread, (void*)jobs);

    // Join threads
    pthread_join(print_thread, NULL);
    pthread_join(scan_thread, NULL);
}

// Function to handle print jobs using mutex
void* print_job_thread(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        pthread_mutex_lock(&print_mutex);
        // Find and process print jobs
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        if (selected_print_index != -1)
        {
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;
            if (jobs[selected_print_index].page <= 0)
            {
                num_jobs--;
            }
        }
        pthread_mutex_unlock(&print_mutex);
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}

// Function to handle scan jobs using mutex
void* scan_job_thread(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        pthread_mutex_lock(&scan_mutex);
        // Find and process scan jobs
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);
        if (selected_scan_index != -1)
        {
            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;
            if (jobs[selected_scan_index].page <= 0)
            {
                num_jobs--;
            }
        }
        pthread_mutex_unlock(&scan_mutex);
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}
