#include "utils.h"
#include "execution.h"

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