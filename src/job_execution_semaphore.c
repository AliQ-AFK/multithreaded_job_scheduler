#include "utils.h"
#include <pthread.h>
#include <semaphore.h>

// Declare and initialize two semaphores
sem_t print_semaphore;
sem_t scan_semaphore;

// Function to create and join threads
void create_and_run_threads(job* jobs, int num_jobs)
{
    // Initialize semaphores
    sem_init(&print_semaphore, 0, 1);  // Semaphore for print jobs
    sem_init(&scan_semaphore, 0, 1);   // Semaphore for scan jobs

    pthread_t print_thread, scan_thread;

    // Create threads
    pthread_create(&print_thread, NULL, print_job_thread, (void*)jobs);
    pthread_create(&scan_thread, NULL, scan_job_thread, (void*)jobs);

    // Join threads
    pthread_join(print_thread, NULL);
    pthread_join(scan_thread, NULL);

    // Destroy semaphores
    sem_destroy(&print_semaphore);
    sem_destroy(&scan_semaphore);
}

// Function to handle print jobs using semaphore
void* print_job_thread(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        sem_wait(&print_semaphore);  // Wait on the print semaphore

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

        sem_post(&print_semaphore);  // Signal the print semaphore
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}

// Function to handle scan jobs using semaphore
void* scan_job_thread(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        sem_wait(&scan_semaphore);  // Wait on the scan semaphore

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

        sem_post(&scan_semaphore);  // Signal the scan semaphore
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}
