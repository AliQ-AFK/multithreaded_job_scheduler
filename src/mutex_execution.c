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

    unsigned int elapsed_time = 0;
    pthread_t thread_id = pthread_self();
    char timestamp[20];

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));
        fprintf(log_file, "[DEBUG] [%s] [THREAD %ld] Remaining Jobs: %d\n", timestamp, thread_id, *num_jobs);
        for (int i = 0; i < *num_jobs; i++)
        {
            fprintf(log_file, "  [DEBUG] Job %d: Type=%s, Pages=%d\n",
                    i, jobs[i].job_type, jobs[i].page);
        }
        fflush(log_file);

        // Print job processing
        pthread_mutex_lock(print_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Mutex Locked. Processing Print Jobs...\n", timestamp, thread_id);
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id);
        pthread_mutex_unlock(print_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Print Mutex Unlocked.\n", timestamp, thread_id);
        fflush(log_file);

        // Scan job processing
        pthread_mutex_lock(scan_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Mutex Locked. Processing Scan Jobs...\n", timestamp, thread_id);
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id);
        pthread_mutex_unlock(scan_mutex);
        fprintf(log_file, "[%s] [THREAD %ld] [INFO] Scan Mutex Unlocked.\n", timestamp, thread_id);
        fflush(log_file);

        // Elapsed time update
        elapsed_time += TIME_SLICE;
        fprintf(log_file, "[%s] [INFO] Elapsed Time: %dms\n", timestamp, elapsed_time);
        fflush(log_file);

        usleep(TIME_SLICE * TIME_SCALE);
    }

    fprintf(log_file, "\033[32m[%s] [THREAD %ld] [SUCCESS] Mutex Job Execution Complete!\033[0m\n", timestamp, thread_id);
    fflush(log_file);
    fclose(log_file);
    return NULL;
}
