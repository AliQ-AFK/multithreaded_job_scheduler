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

    unsigned int elapsed_time = 0;
    pthread_t thread_id = pthread_self();
    char timestamp[20];

    execution_summary local_summary = {0, 0, 0, 0, pthread_self()};
    args->summary = &local_summary;

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));
        fprintf(log_file, "%s[DEBUG] [%s] [THREAD %ld] Remaining Jobs: %d%s\n", 
            DEBUG_COLOR, timestamp, thread_id, *num_jobs, COLOR_RESET);
        for (int i = 0; i < *num_jobs; i++)
        {
            fprintf(log_file, "%s  [DEBUG] Job %d: Type=%s, Pages=%d%s\n",
                    DEBUG_COLOR, i, jobs[i].job_type, jobs[i].page, COLOR_RESET);
        }
        
        // Print job processing
        sem_wait(print_semaphore);
        fprintf(log_file, "%s[%s] [THREAD %ld] [INFO] Print Semaphore Acquired. Processing Print Jobs...%s\n", 
            INFO_COLOR, timestamp, thread_id, COLOR_RESET);
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id, args->summary);
        sem_post(print_semaphore);
        fprintf(log_file, "%s[%s] [THREAD %ld] [INFO] Print Semaphore Released.%s\n", 
            INFO_COLOR, timestamp, thread_id, COLOR_RESET);

        // Scan job processing
        sem_wait(scan_semaphore);
        fprintf(log_file, "%s[%s] [THREAD %ld] [INFO] Scan Semaphore Acquired. Processing Scan Jobs...%s\n", 
            INFO_COLOR, timestamp, thread_id, COLOR_RESET);
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id, args->summary);
        sem_post(scan_semaphore);
        fprintf(log_file, "%s[%s] [THREAD %ld] [INFO] Scan Semaphore Released.%s\n", 
            INFO_COLOR, timestamp, thread_id, COLOR_RESET);

        // Elapsed time update
        elapsed_time += TIME_SLICE;
        fprintf(log_file, "%s[%s] [INFO] Elapsed Time: %dms%s\n", 
            INFO_COLOR, timestamp, elapsed_time, COLOR_RESET);

        usleep(TIME_SLICE * TIME_SCALE);
    }

    fprintf(log_file, "\n%s=== SEMAPHORE EXECUTION SUMMARY ===%s\n", SUCCESS_COLOR, COLOR_RESET);
    fprintf(log_file, "Thread ID: %ld\n", local_summary.thread_id);
    fprintf(log_file, "Total Jobs Processed: %d\n", local_summary.total_jobs_processed);
    fprintf(log_file, "Print Jobs Completed: %d\n", local_summary.print_jobs_completed);
    fprintf(log_file, "Scan Jobs Completed: %d\n", local_summary.scan_jobs_completed);
    fprintf(log_file, "Total Execution Time: %ums\n", local_summary.total_time);
    fprintf(log_file, "%s================================%s\n\n", 
        SUCCESS_COLOR, COLOR_RESET);

    fclose(log_file);
    return NULL;
}
