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
        fprintf(log_file, "[DEBUG] Remaining jobs: %d\n", *num_jobs);
        fflush(log_file);

        fprintf(log_file, "[DEBUG] Current job status:\n");
        for (int i = 0; i < *num_jobs; i++) {
            fprintf(log_file, "Job %d: Type=%s, Pages=%d\n", 
                    i, jobs[i].job_type, jobs[i].page);
        }
        fflush(log_file);

        get_current_time(timestamp, sizeof(timestamp));  // Get the current time

        // Lock for print jobs
        pthread_mutex_lock(print_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Mutex Locked. Processing Print Jobs...\n", timestamp, thread_id);
        fflush(log_file);

        // Process print jobs
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id);

        // Unlock print mutex
        pthread_mutex_unlock(print_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Mutex Unlocked. Time Slice Completed.\n", timestamp, thread_id);
        fflush(log_file);

        // Lock for scan jobs
        pthread_mutex_lock(scan_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Mutex Locked. Processing Scan Jobs...\n", timestamp, thread_id);
        fflush(log_file);

        // Process scan jobs
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id);

        // Unlock scan mutex
        pthread_mutex_unlock(scan_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Mutex Unlocked. Time Slice Completed.\n", timestamp, thread_id);
        fflush(log_file);

        // Continue with the time slice
        usleep(TIME_SLICE * TIME_SCALE);

        // Add a small sleep to prevent busy-waiting
        usleep(1000);
    }

    fprintf(log_file, "\033[32m[%s] [THREAD %ld] [SUCCESS] Mutex Job Execution Complete!\033[0m\n", timestamp, thread_id);  // Green Success Message
    fflush(log_file);
    fclose(log_file);
    return NULL;
}
