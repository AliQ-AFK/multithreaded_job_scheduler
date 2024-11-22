#include "utils.h"
#include "execution.h"

void* job_execution(void* arg) {
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;  // Pointer to specific num_jobs for this execution

    FILE* log_file = fopen("unsynced_execution.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    fprintf(log_file, "[DEBUG] Starting unsynced job execution. Total jobs: %d\n", *num_jobs);
    fflush(log_file);

    while (*num_jobs > 0) {
        fprintf(log_file, "[DEBUG] Time slice %u begins. Jobs remaining: %d\n", elapsed_time, *num_jobs);
        fflush(log_file);

        int selected_index = find_next_job(jobs, num_jobs, "print", elapsed_time); // Pass pointer
        if (selected_index != -1) {
            int pages_to_process = (jobs[selected_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_index].page;
            jobs[selected_index].page -= pages_to_process;

            fprintf(log_file, "[DEBUG] Processed job for User %d. Pages remaining: %d\n",
                    jobs[selected_index].user_id, jobs[selected_index].page);
            fflush(log_file);

            if (jobs[selected_index].page <= 0) {
                fprintf(log_file, "[DEBUG] Completed job for User %d. Decrementing num_jobs.\n",
                        jobs[selected_index].user_id);
                (*num_jobs)--;
            }
        } else {
            fprintf(log_file, "[DEBUG] No job found for this time slice.\n");
        }

        elapsed_time += TIME_SLICE;
        fprintf(log_file, "[DEBUG] Incremented elapsed time to %u.\n", elapsed_time);
        fflush(log_file);

        usleep(TIME_SLICE * TIME_SCALE);
    }

    fprintf(log_file, "[DEBUG] Unsynced job execution completed. Final job count: %d\n", *num_jobs);
    fflush(log_file);

    fclose(log_file);
    return NULL;
}

void execute_unsynced_jobs(job* jobs, int* num_jobs) {
    job* jobs_unsync = malloc((*num_jobs) * sizeof(job));
    if (!jobs_unsync) {
        perror("Failed to allocate memory for jobs_unsync");
        exit(EXIT_FAILURE);
    }
    memcpy(jobs_unsync, jobs, (*num_jobs) * sizeof(job));

    int num_jobs_unsync = *num_jobs;

    pthread_t threads[2];

    execution_args args_unsync = {jobs_unsync, &num_jobs_unsync, NULL, NULL}; // Pass pointer to num_jobs_unsync

    pthread_create(&threads[0], NULL, job_execution, &args_unsync);
    pthread_create(&threads[1], NULL, job_execution, &args_unsync);

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    free(jobs_unsync);
}



























































































































/*#include "utils.h"
#include "execution.h"

// Job execution function (generic for all execution types)
void* job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;  // Pointer to specific num_jobs for this execution
    pthread_mutex_t* mutex = args->mutex;
    sem_t* semaphore = args->semaphore;

    // Determine the log file name based on the execution type
    char log_filename[50];
    if (mutex != NULL)
        snprintf(log_filename, sizeof(log_filename), "mutex_execution.log");
    else if (semaphore != NULL)
        snprintf(log_filename, sizeof(log_filename), "semaphore_execution.log");
    else
        snprintf(log_filename, sizeof(log_filename), "unsynced_execution.log");

    // Open the log file in append mode
    FILE* log_file = fopen(log_filename, "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    fprintf(log_file, "[DEBUG] Starting job execution. Total jobs: %d\n", *num_jobs);
    fflush(log_file);

    while (*num_jobs > 0)
    {
        fprintf(log_file, "[DEBUG] Time slice %u begins. Jobs remaining: %d\n", elapsed_time, *num_jobs);
        fflush(log_file);

        if (mutex != NULL)
        {
            fprintf(log_file, "[DEBUG] Locking mutex for execution.\n");
            fflush(log_file);
            pthread_mutex_lock(mutex);
        }
        else if (semaphore != NULL)
        {
            fprintf(log_file, "[DEBUG] Waiting on semaphore for execution.\n");
            fflush(log_file);
            sem_wait(semaphore);
        }

        int selected_print_index = find_next_job(jobs, *num_jobs, "print", elapsed_time);
        if (selected_print_index != -1)
        {
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;

            fprintf(log_file, "[DEBUG] Processed print job for User %d. Pages remaining: %d\n",
                    jobs[selected_print_index].user_id, jobs[selected_print_index].page);
            fflush(log_file);

            if (jobs[selected_print_index].page <= 0)
            {
                fprintf(log_file, "[DEBUG] Completed print job for User %d\n", jobs[selected_print_index].user_id);
                fflush(log_file);
                (*num_jobs)--;
            }
        }

        int selected_scan_index = find_next_job(jobs, *num_jobs, "scan", elapsed_time);
        if (selected_scan_index != -1)
        {
            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;

            fprintf(log_file, "[DEBUG] Processed scan job for User %d. Pages remaining: %d\n",
                    jobs[selected_scan_index].user_id, jobs[selected_scan_index].page);
            fflush(log_file);

            if (jobs[selected_scan_index].page <= 0)
            {
                fprintf(log_file, "[DEBUG] Completed scan job for User %d\n", jobs[selected_scan_index].user_id);
                fflush(log_file);
                (*num_jobs)--;
            }
        }

        if (mutex != NULL)
        {
            fprintf(log_file, "[DEBUG] Unlocking mutex after execution.\n");
            fflush(log_file);
            pthread_mutex_unlock(mutex);
        }
        else if (semaphore != NULL)
        {
            fprintf(log_file, "[DEBUG] Posting semaphore after execution.\n");
            fflush(log_file);
            sem_post(semaphore);
        }

        elapsed_time += TIME_SLICE;
        fprintf(log_file, "[DEBUG] Incremented elapsed time to %u.\n", elapsed_time);
        fflush(log_file);

        usleep(TIME_SLICE * TIME_SCALE);
    }

    fprintf(log_file, "[DEBUG] Job execution completed.\n");
    fflush(log_file);

    fclose(log_file); // Close the log file
    return NULL;
}

// Function to manage execution
void execute_all_jobs(job* jobs, int *num_jobs)
{
    // Create three separate copies of the job arrays
    job* jobs_mutex = malloc((*num_jobs) * sizeof(job));
    job* jobs_semaphore = malloc((*num_jobs) * sizeof(job));
    job* jobs_unsync = malloc((*num_jobs) * sizeof(job));

    memcpy(jobs_mutex, jobs, (*num_jobs) * sizeof(job));
    memcpy(jobs_semaphore, jobs, (*num_jobs) * sizeof(job));
    memcpy(jobs_unsync, jobs, (*num_jobs) * sizeof(job));

    // Create separate copies of num_jobs
    int num_jobs_mutex = *num_jobs;
    int num_jobs_semaphore = *num_jobs;
    int num_jobs_unsync = *num_jobs;

    // Create mutexes and semaphores
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    // Thread identifiers
    pthread_t threads[6];

    // Execution arguments
    execution_args args_mutex = {jobs_mutex, &num_jobs_mutex, &mutex, NULL};
    execution_args args_semaphore = {jobs_semaphore, &num_jobs_semaphore, NULL, &semaphore};
    execution_args args_unsync = {jobs_unsync, &num_jobs_unsync, NULL, NULL};

    // Create threads
    pthread_create(&threads[0], NULL, job_execution, &args_unsync);
    pthread_create(&threads[1], NULL, job_execution, &args_unsync);
    pthread_create(&threads[2], NULL, job_execution, &args_mutex);
    pthread_create(&threads[3], NULL, job_execution, &args_mutex);
    pthread_create(&threads[4], NULL, job_execution, &args_semaphore);
    pthread_create(&threads[5], NULL, job_execution, &args_semaphore);

    // Wait for all threads
    for (int i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Clean up synchronization objects
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

    // Free memory
    free(jobs_mutex);
    free(jobs_semaphore);
    free(jobs_unsync);
}*/
