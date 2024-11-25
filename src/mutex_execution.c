#include "utils.h"
#include "execution.h"

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