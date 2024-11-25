#include "utils.h"
#include "execution.h"

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