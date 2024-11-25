#include "utils.h"
#include "execution.h"
#include <time.h>  // For timestamp

// Function to get current time for logging in the format: YYYY-MM-DD HH:MM:SS
void get_current_time(char* buffer, size_t size)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);  // Get the current time
    timeinfo = localtime(&rawtime);  // Convert it to local time

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);  // Format time into string
}

// Generalized job execution function (handles both print and scan jobs)
void process_job(job* jobs, int* num_jobs, const char* job_type, unsigned int* elapsed_time, FILE* log_file, pthread_t thread_id)
{
    // Find the next job based on job type and elapsed time
    int selected_job_index = find_next_job(jobs, num_jobs, job_type, *elapsed_time);
    if (selected_job_index != -1)
    {
        // Process the job for 2 pages (time slice)
        jobs[selected_job_index].page -= 2;  // Access job directly using the pointer

        // Declare timestamp buffer once at the start of the function
        char timestamp[20];
        get_current_time(timestamp, sizeof(timestamp));  // Call the function to get current time

        // Log the current processing of the job
        fprintf(log_file, "[%s] [THREAD %ld] [DEBUG] %-10s Job for User %-3d | Pages Remaining: %-3d | Elapsed Time: %-5u\n",
                timestamp, thread_id, job_type, jobs[selected_job_index].user_id, jobs[selected_job_index].page, *elapsed_time);
        fflush(log_file);

        // If job is completed (page <= 0)
        if (jobs[selected_job_index].page <= 0)
        {
            // Job is completed, log and remove it
            fprintf(log_file, "[%s] [THREAD %ld] [DEBUG] Completed %-10s Job for User %-3d | Job Removed\n", 
                    timestamp, thread_id, job_type, jobs[selected_job_index].user_id);
            fflush(log_file);
            (*num_jobs)--;  // Decrement job count
           
        }
        else
        {
            // Increment elapsed time for the time slice (after each job processing)
            *elapsed_time += TIME_SLICE;

            // Update the arrival time AFTER the elapsed time is incremented
            jobs[selected_job_index].arrival_time = *elapsed_time;

            // Log that we have updated the arrival time
            fprintf(log_file, "[%s] [THREAD %ld] [INFO] %-10s Job for User %-3d | Arrival Time Updated to %-5u\n", 
                    timestamp, thread_id, job_type, jobs[selected_job_index].user_id, jobs[selected_job_index].arrival_time);
            fflush(log_file);
        }
    }
    else
    {
        char timestamp[20];
        get_current_time(timestamp, sizeof(timestamp));
        fprintf(log_file, "[%s] [THREAD %ld] [DEBUG] No %-10s Job Found at Time %-5u.\n", 
                timestamp, thread_id, job_type, *elapsed_time);
        fflush(log_file);
        *elapsed_time += 1; // Fix: Increment elapsed_time when no job is found
    }
}

void execute_all_jobs(job* jobs, int* num_jobs)
{
    job* jobs_mutex = malloc(*num_jobs * sizeof(job));
    job* jobs_semaphore = malloc(*num_jobs * sizeof(job));
    job* jobs_unsync = malloc(*num_jobs * sizeof(job));

    memcpy(jobs_mutex, jobs, *num_jobs * sizeof(job));
    memcpy(jobs_semaphore, jobs, *num_jobs * sizeof(job));
    memcpy(jobs_unsync, jobs, *num_jobs * sizeof(job));

    int num_jobs_mutex = *num_jobs;
    int num_jobs_semaphore = *num_jobs;
    int num_jobs_unsync = *num_jobs;

    // Synchronization objects
    pthread_mutex_t print_mutex, scan_mutex;
    pthread_mutex_init(&print_mutex, NULL);
    pthread_mutex_init(&scan_mutex, NULL);

    sem_t print_semaphore, scan_semaphore;
    sem_init(&print_semaphore, 0, 1);  // Initialize the semaphore for print
    sem_init(&scan_semaphore, 0, 1);   // Initialize the semaphore for scan

    pthread_t threads[6];

    // Passing semaphore to threads via execution_args
    execution_args args_mutex = {jobs_mutex, &num_jobs_mutex, &print_mutex, &scan_mutex, NULL, NULL};
    execution_args args_semaphore = {jobs_semaphore, &num_jobs_semaphore, NULL, NULL, &print_semaphore, &scan_semaphore};
    execution_args args_unsync = {jobs_unsync, &num_jobs_unsync, NULL, NULL, NULL, NULL};

    // Create threads for each execution type
    pthread_create(&threads[0], NULL, unsynced_job_execution, &args_unsync);
    pthread_create(&threads[1], NULL, unsynced_job_execution, &args_unsync);
    pthread_create(&threads[2], NULL, mutex_job_execution, &args_mutex);
    pthread_create(&threads[3], NULL, mutex_job_execution, &args_mutex);
    pthread_create(&threads[4], NULL, semaphore_job_execution, &args_semaphore); // Pass semaphore here
    pthread_create(&threads[5], NULL, semaphore_job_execution, &args_semaphore); // Pass semaphore here

    // Wait for all threads to finish
    for (int i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Clean up synchronization objects
    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&scan_mutex);
    sem_destroy(&print_semaphore);
    sem_destroy(&scan_semaphore);

    // Free memory
    free(jobs_mutex);
    free(jobs_semaphore);
    free(jobs_unsync);
}