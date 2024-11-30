#include "utils.h"
#include "execution.h"

 "\033[32m"  // Green

void* unsynced_job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;

     FILE* log_file = open_log_file("unsync_execution.log", "a");
    if (!log_file)
    {
        return NULL;
    }

    unsigned int elapsed_time = 0;  // Initialize elapsed_time to 0
    pthread_t thread_id = pthread_self();  // Get the current thread ID

    char timestamp[20];  // Declare timestamp buffer once at the start of the function

    execution_summary local_summary = {0, 0, 0, 0, pthread_self()};
    args->summary = &local_summary;

    while (*num_jobs > 0)
    {
        get_current_time(timestamp, sizeof(timestamp));
        
        fprintf(log_file, "%s[DEBUG] [%s] [THREAD %ld] Processing Jobs Without Synchronization%s\n", 
            DEBUG_COLOR, timestamp, thread_id, COLOR_RESET);

        // Process jobs
        process_job(jobs, num_jobs, "print", &elapsed_time, log_file, thread_id, args->summary);
        process_job(jobs, num_jobs, "scan", &elapsed_time, log_file, thread_id, args->summary);

        fprintf(log_file, "%s[INFO] [%s] [THREAD %ld] Elapsed Time: %dms%s\n", 
            INFO_COLOR, timestamp, thread_id, elapsed_time, COLOR_RESET);
    }

    // Add summary at the end
    fprintf(log_file, "\n%s=== UNSYNC EXECUTION SUMMARY ===%s\n", SUCCESS_COLOR, COLOR_RESET);
    fprintf(log_file, "Thread ID: %ld\n", local_summary.thread_id);
    fprintf(log_file, "Total Jobs Processed: %d\n", local_summary.total_jobs_processed);
    fprintf(log_file, "Print Jobs Completed: %d\n", local_summary.print_jobs_completed);
    fprintf(log_file, "Scan Jobs Completed: %d\n", local_summary.scan_jobs_completed);
    fprintf(log_file, "Total Execution Time: %ums\n", local_summary.total_time);
    fprintf(log_file, "%s=================================%s\n\n", 
        SUCCESS_COLOR, COLOR_RESET);

    fclose(log_file);
    return NULL;
}