#include "utils.h"
#include <pthread.h>

// Function to create and join threads without any synchronization
void create_and_run_threads_unsynced(job* jobs, int num_jobs)
{
    pthread_t print_thread, scan_thread;

    // Create threads
    pthread_create(&print_thread, NULL, print_job_thread_unsynced, (void*)jobs);
    pthread_create(&scan_thread, NULL, scan_job_thread_unsynced, (void*)jobs);

    // Join threads
    pthread_join(print_thread, NULL);
    pthread_join(scan_thread, NULL);
}

// Function to handle print jobs without any synchronization
void* print_job_thread_unsynced(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        // Find and process print jobs without any locking
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

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}

// Function to handle scan jobs without any synchronization
void* scan_job_thread_unsynced(void* arg)
{
    job* jobs = (job*)arg;
    unsigned int elapsed_time = 0;
    int num_jobs = get_total_jobs();  // Adjust this to get the total number of jobs

    while (num_jobs > 0)
    {
        // Find and process scan jobs without any locking
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

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }
    return NULL;
}
