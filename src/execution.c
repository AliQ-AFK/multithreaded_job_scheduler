#include "utils.h"
#include "execution.h"

void execute_job(job* jobs, int* num_jobs, const char* job_type, unsigned int elapsed_time, FILE* log_file)
{
    int selected_job_index = find_next_job(jobs, *num_jobs, job_type, elapsed_time);
    if (selected_job_index != -1)
    {
        jobs[selected_job_index].page -= TIME_SLICE;
        
        fprintf(log_file, "[DEBUG] Processed %s job for User %d. Pages remaining: %d\n",
                job_type, jobs[selected_job_index].user_id, jobs[selected_job_index].page);
        fflush(log_file);

        if (jobs[selected_job_index].page <= 0)
        {
            fprintf(log_file, "[DEBUG] Completed %s job for User %d\n", job_type, jobs[selected_job_index].user_id);
            fflush(log_file);
            (*num_jobs)--;  // Decrement job count
        }
    }
}



// Job execution function (for mutex)
void* mutex_job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;
    pthread_mutex_t* mutex = args->mutex;

    FILE* log_file = fopen("mutex_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    while (*num_jobs > 0) {
        pthread_mutex_lock(mutex);  // Lock mutex for synchronization

        // Process jobs...
        int selected_job_index = find_next_job(jobs, *num_jobs, "print", elapsed_time);
        if (selected_job_index != -1)
        {
            jobs[selected_job_index].page -= TIME_SLICE;
            if (jobs[selected_job_index].page <= 0)
            {
                (*num_jobs)--;  // Decrement job count
            }
        }

        pthread_mutex_unlock(mutex);  // Unlock mutex
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    fclose(log_file);
    return NULL;
}

// Job execution function (for semaphore)
void* semaphore_job_execution(void* arg) {
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;
    sem_t* semaphore = args->semaphore;

    FILE* log_file = fopen("semaphore_execution.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    while (*num_jobs > 0) {
        sem_wait(semaphore);  // Wait for semaphore

        // Process jobs...
        int selected_job_index = find_next_job(jobs, *num_jobs, "print", elapsed_time);
        if (selected_job_index != -1) {
            jobs[selected_job_index].page -= TIME_SLICE;
            if (jobs[selected_job_index].page <= 0) {
                (*num_jobs)--;  // Decrement job count
            }
        }

        sem_post(semaphore);  // Post semaphore
        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    fclose(log_file);
    return NULL;
}

// Job execution function (for unsynchronized)
void* unsynced_job_execution(void* arg) {
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;

    FILE* log_file = fopen("unsynced_execution.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    while (*num_jobs > 0) {
        // Process jobs...
        int selected_job_index = find_next_job(jobs, *num_jobs, "print", elapsed_time);
        if (selected_job_index != -1) {
            jobs[selected_job_index].page -= TIME_SLICE;
            if (jobs[selected_job_index].page <= 0) {
                (*num_jobs)--;  // Decrement job count
            }
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    fclose(log_file);
    return NULL;
}

// Function to manage execution
void execute_all_jobs(job* jobs, int* num_jobs) {
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
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    pthread_t threads[6];

    execution_args args_mutex = {jobs_mutex, &num_jobs_mutex, &mutex, NULL};
    execution_args args_semaphore = {jobs_semaphore, &num_jobs_semaphore, NULL, &semaphore};
    execution_args args_unsync = {jobs_unsync, &num_jobs_unsync, NULL, NULL};

    // Create threads for each execution type
    pthread_create(&threads[0], NULL, unsynced_job_execution, &args_unsync);
    pthread_create(&threads[1], NULL, unsynced_job_execution, &args_unsync);
    pthread_create(&threads[2], NULL, mutex_job_execution, &args_mutex);
    pthread_create(&threads[3], NULL, mutex_job_execution, &args_mutex);
    pthread_create(&threads[4], NULL, semaphore_job_execution, &args_semaphore);
    pthread_create(&threads[5], NULL, semaphore_job_execution, &args_semaphore);

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
}



/*#include "utils.h"
#include "execution.h"

void* job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;  // Pointer to specific num_jobs for this execution
    pthread_mutex_t* mutex = args->mutex;

    FILE* log_file = fopen("mutex_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    // Starting the job execution log entry
    fprintf(log_file, "[INFO] Starting mutex job execution. Total jobs: %d\n", *num_jobs);
    fflush(log_file);

    pthread_t thread_id = pthread_self(); // Get the current thread ID

    // Main job processing loop (will only handle 'print' jobs)
    while (*num_jobs > 0) // Continue while there are jobs remaining
    {
        fprintf(log_file, "[INFO] Thread %ld: Time slice %u begins. Jobs remaining: %d\n", thread_id, elapsed_time, *num_jobs);
        fflush(log_file);

        pthread_mutex_lock(mutex);  // Lock the mutex to avoid race conditions
        fprintf(log_file, "[INFO] Thread %ld: Mutex locked.\n", thread_id);
        fflush(log_file);

        int selected_index = find_next_job(jobs, num_jobs, "print", elapsed_time); // Pass pointer to find print jobs
        if (selected_index != -1)
        {

            jobs[selected_index].page -= TIME_SCALE;

            // Log detailed job processing information
            fprintf(log_file, "[INFO] Thread %ld: Job selected for User %d (Job ID: %d). Pages processed: %d. "
                               "Pages remaining: %d\n", 
                               thread_id, jobs[selected_index].user_id, selected_index, 
                               TIME_SCALE, jobs[selected_index].page);
            fflush(log_file);

            // Check if the job is completed
            if (jobs[selected_index].page <= 0)
            {
                fprintf(log_file, "[INFO] Thread %ld: Job for User %d (Job ID: %d) completed. "
                                   "Decrementing num_jobs.\n", 
                                   thread_id, jobs[selected_index].user_id, selected_index);
                (*num_jobs)--; // Decrease number of jobs left
                fprintf(log_file, "[INFO] Thread %ld: Jobs remaining after completion: %d\n", thread_id, *num_jobs);
                fflush(log_file);

                // Force decrement if it's the last job and it's completed
                /*if (*num_jobs == 0 && jobs[selected_index].page == 0)
                {
                    fprintf(log_file, "[INFO] Thread %ld: Last job for User %d (Job ID: %d) completed. Forcing decrement of num_jobs.\n", 
                            thread_id, jobs[selected_index].user_id, selected_index);
                    (*num_jobs)--; // Force decrement to exit the loop
                    fprintf(log_file, "[INFO] Thread %ld: Jobs remaining after forced completion: %d\n", thread_id, *num_jobs);
                    fflush(log_file);
                }
                
            }
        } 
        else 
        {
            fprintf(log_file, "[INFO] Thread %ld: No job found for this time slice (Time: %u).\n", thread_id, elapsed_time);
        }

        pthread_mutex_unlock(mutex);  // Unlock the mutex after processing the job
        fprintf(log_file, "[INFO] Thread %ld: Mutex unlocked.\n", thread_id);
        fflush(log_file);

        // Log the time increment
        elapsed_time += TIME_SLICE;
        fprintf(log_file, "[INFO] Thread %ld: Elapsed time incremented to %u.\n", thread_id, elapsed_time);
        fflush(log_file);

        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing delay
    }

    // Final log entry when all jobs are processed
    fprintf(log_file, "[INFO] Thread %ld: Mutex job execution completed. Final job count: %d\n", thread_id, *num_jobs);
    fflush(log_file);

    fclose(log_file);  // Close the log file when done
    return NULL;
}

void execute_mutex_jobs(job* jobs, int* num_jobs)
{
    pthread_t threads[1];  // Only one thread for print jobs now
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);  // Initialize mutex

    // Log that we're about to start the job execution
    printf("[INFO] Starting mutex-based job execution with 1 thread for print jobs.\n");

    // Prepare execution arguments
    execution_args args_mutex = {jobs, num_jobs, &mutex, NULL}; // Pass pointer to num_jobs

    // Create a thread to execute print jobs only
    printf("[INFO] Creating thread 0\n");
    pthread_create(&threads[0], NULL, job_execution, &args_mutex);
    printf("[INFO] Thread 0 created.\n");

    // Wait for the thread to finish
    printf("[INFO] Waiting for thread 0 to finish.\n");
    pthread_join(threads[0], NULL);
    printf("[INFO] Thread 0 finished.\n");

    pthread_mutex_destroy(&mutex);  // Destroy mutex after job execution
    printf("[INFO] Mutex destroyed.\n");

    printf("[INFO] Mutex-based job execution completed.\n");
}














































#include "utils.h"
#include "execution.h"

void* job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int* num_jobs = args->num_jobs;  // Pointer to specific num_jobs for this execution
    pthread_mutex_t* mutex = args->mutex;

    FILE* log_file = fopen("mutex_execution.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return NULL;
    }

    unsigned int elapsed_time = 0;

    // Starting the job execution log entry
    fprintf(log_file, "[INFO] Starting mutex job execution. Total jobs: %d\n", *num_jobs);
    fflush(log_file);

    pthread_t thread_id = pthread_self(); // Get the current thread ID

    // Main job processing loop
    while (*num_jobs > 1) // Changed condition to while num_jobs > 1 to avoid infinite loop
    {
        fprintf(log_file, "[INFO] Thread %ld: Time slice %u begins. Jobs remaining: %d\n", thread_id, elapsed_time, *num_jobs);
        fflush(log_file);

        pthread_mutex_lock(mutex);  // Lock the mutex to avoid race conditions
        fprintf(log_file, "[INFO] Thread %ld: Mutex locked.\n", thread_id);
        fflush(log_file);

        int selected_index = find_next_job(jobs, num_jobs, "print", elapsed_time); // Pass pointer to find job
        if (selected_index != -1)
        {
            int pages_to_process = (jobs[selected_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_index].page;
            jobs[selected_index].page -= pages_to_process;

            // Log detailed job processing information
            fprintf(log_file, "[INFO] Thread %ld: Job selected for User %d (Job ID: %d). Pages processed: %d. "
                               "Pages remaining: %d\n", 
                               thread_id, jobs[selected_index].user_id, selected_index, 
                               pages_to_process, jobs[selected_index].page);
            fflush(log_file);

            // Check if the job is completed
            if (jobs[selected_index].page <= 0)
            {
                fprintf(log_file, "[INFO] Thread %ld: Job for User %d (Job ID: %d) completed. "
                                   "Decrementing num_jobs.\n", 
                                   thread_id, jobs[selected_index].user_id, selected_index);
                (*num_jobs)--; // Decrease number of jobs left
                fprintf(log_file, "[INFO] Thread %ld: Jobs remaining after completion: %d\n", thread_id, *num_jobs);
                fflush(log_file);

                // Force decrement if it's the last job and it's completed
                if (*num_jobs == 1 && jobs[selected_index].page == 0)
                {
                    fprintf(log_file, "[INFO] Thread %ld: Last job for User %d (Job ID: %d) completed. Forcing decrement of num_jobs.\n", 
                            thread_id, jobs[selected_index].user_id, selected_index);
                    (*num_jobs)--; // Force decrement to exit the loop
                    fprintf(log_file, "[INFO] Thread %ld: Jobs remaining after forced completion: %d\n", thread_id, *num_jobs);
                    fflush(log_file);
                }
            }
        } 
        else 
        {
            fprintf(log_file, "[INFO] Thread %ld: No job found for this time slice (Time: %u).\n", thread_id, elapsed_time);
        }

        pthread_mutex_unlock(mutex);  // Unlock the mutex after processing the job
        fprintf(log_file, "[INFO] Thread %ld: Mutex unlocked.\n", thread_id);
        fflush(log_file);

        // Log the time increment
        elapsed_time += TIME_SLICE;
        fprintf(log_file, "[INFO] Thread %ld: Elapsed time incremented to %u.\n", thread_id, elapsed_time);
        fflush(log_file);

        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing delay
    }

    // Final log entry when all jobs are processed
    fprintf(log_file, "[INFO] Thread %ld: Mutex job execution completed. Final job count: %d\n", thread_id, *num_jobs);
    fflush(log_file);

    fclose(log_file);  // Close the log file when done
    return NULL;
}

void execute_mutex_jobs(job* jobs, int* num_jobs)
{
    pthread_t threads[2];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);  // Initialize mutex

    // Log that we're about to start the job execution
    printf("[INFO] Starting mutex-based job execution with 2 threads.\n");

    // Prepare execution arguments
    execution_args args_mutex = {jobs, num_jobs, &mutex, NULL}; // Pass pointer to num_jobs

    // Create threads to execute jobs
    for (int i = 0; i < 2; i++) {
        printf("[INFO] Creating thread %d\n", i);
        pthread_create(&threads[i], NULL, job_execution, &args_mutex);
        printf("[INFO] Thread %d created.\n", i);
    }

    // Wait for both threads to finish
    for (int i = 0; i < 2; i++) {
        printf("[INFO] Waiting for thread %d to finish.\n", i);
        pthread_join(threads[i], NULL);
        printf("[INFO] Thread %d finished.\n", i);
    }

    pthread_mutex_destroy(&mutex);  // Destroy mutex after job execution
    printf("[INFO] Mutex destroyed.\n");

    printf("[INFO] Mutex-based job execution completed.\n");




#include "utils.h"
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
            jobs[selected_print_index].page -= TIME_SLICE;

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
            jobs[selected_print_index].page -= TIME_SLICE;

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
