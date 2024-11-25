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

// Job execution function (for mutex)

void* mutex_job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;
    pthread_mutex_t* print_mutex = args->print_mutex;
    pthread_mutex_t* scan_mutex = args->scan_mutex;

    FILE* log_file = fopen("mutex_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;  // Initialize elapsed_time to 0
    pthread_t thread_id = pthread_self();  // Get the current thread ID

    char timestamp[20];  // Declare timestamp buffer once at the start of the function

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));  // Get the current time

        // Lock and process print jobs
        pthread_mutex_lock(print_mutex);
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id);
        pthread_mutex_unlock(print_mutex);

        // Lock and process scan jobs
        pthread_mutex_lock(scan_mutex);
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id);
        pthread_mutex_unlock(scan_mutex);

        usleep(TIME_SLICE * TIME_SCALE);

        if (*num_jobs <= 0) {
            break;  // Exit if no more jobs
        }

        // Add a small sleep to prevent busy-waiting
        usleep(1000);
    }

    fprintf(log_file, "\033[32m[%s] [THREAD %ld] [SUCCESS] Mutex Job Execution Complete!\033[0m\n", timestamp, thread_id);
    fclose(log_file);
    return NULL;
}


// Job execution function (for semaphore)

void* semaphore_job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;
    sem_t* print_semaphore = args->print_semaphore;
    sem_t* scan_semaphore = args->scan_semaphore;

    FILE* log_file = fopen("semaphore_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;  // Initialize elapsed_time to 0
    pthread_t thread_id = pthread_self();  // Get the current thread ID

    char timestamp[20];  // Declare timestamp buffer once at the start of the function

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));  // Get the current time

        // Wait for semaphore for print jobs
        sem_wait(print_semaphore);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Semaphore Wait Completed. Processing Print Jobs...\n", timestamp, thread_id);
        fflush(log_file);

        // Process print jobs
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id);

        sem_post(print_semaphore);  // Post semaphore for print jobs
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Semaphore Posted. Time Slice Completed.\n", timestamp, thread_id);
        fflush(log_file);

        // Wait for semaphore for scan jobs
        sem_wait(scan_semaphore);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Semaphore Wait Completed. Processing Scan Jobs...\n", timestamp, thread_id);
        fflush(log_file);

        // Process scan jobs
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id);

        sem_post(scan_semaphore);  // Post semaphore for scan jobs
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Semaphore Posted. Time Slice Completed.\n", timestamp, thread_id);
        fflush(log_file);

        // Continue with the time slice
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);

        // Add a small sleep to prevent busy-waiting
        usleep(1000);
    }

    fprintf(log_file, "\033[32m[%s] [THREAD %ld] [SUCCESS] Semaphore Job Execution Complete!\033[0m\n", timestamp, thread_id);  // Green Success Message
    fflush(log_file);
    fclose(log_file);
    return NULL;
}



// Job execution function (for unsynchronized)
void* unsynced_job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;

    FILE* log_file = fopen("unsynced_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;  // Initialize elapsed_time to 0
    pthread_t thread_id = pthread_self();  // Get the current thread ID

    char timestamp[20];  // Declare timestamp buffer once at the start of the function

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));  // Get the current time

        // Process print jobs without synchronization
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id);

        // Process scan jobs without synchronization
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id);

        elapsed_time += TIME_SLICE;  // Increment elapsed time
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate the passage of time

        // Add a small sleep to prevent busy-waiting
        usleep(1000);
    }

    fprintf(log_file, "\033[32m[%s] [THREAD %ld] [SUCCESS] Unsynced Job Execution Complete!\033[0m\n", timestamp, thread_id);  // Green Success Message
    fflush(log_file);
    fclose(log_file);
    return NULL;
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