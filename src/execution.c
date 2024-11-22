#include "execution.h"
#include "utils.h"

// Job execution function (generic for all execution types)
void* job_execution(void* arg)
{
    execution_args* args = (execution_args*)arg;
    job* jobs = args->jobs;
    int num_jobs = args->num_jobs;
    pthread_mutex_t* mutex = args->mutex;
    sem_t* semaphore = args->semaphore;

    unsigned int elapsed_time = 0;

    printf("[DEBUG] Starting job execution. Total jobs: %d\n", num_jobs);
    fflush(stdout);

    while (num_jobs > 0)
    {
        printf("[DEBUG] Time slice %u begins. Jobs remaining: %d\n", elapsed_time, num_jobs);
        fflush(stdout);

        if (mutex != NULL)
        {
            pthread_mutex_lock(mutex);
        }
        else if (semaphore != NULL)
        {
            sem_wait(semaphore);
        }

        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        if (selected_print_index != -1)
        {
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;

            if (jobs[selected_print_index].page <= 0)
            {
                num_jobs--;
            }
        }

        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);
        if (selected_scan_index != -1)
        {
            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;

            if (jobs[selected_scan_index].page <= 0)
            {
                num_jobs--;
            }
        }
        

        if (mutex != NULL)
        {
            pthread_mutex_unlock(mutex);
        }
        else if (semaphore != NULL)
        {
            sem_post(semaphore);
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    printf("[DEBUG] Job execution completed.\n");
    fflush(stdout);

    return NULL;
}

// Function to manage execution
void execute_all_jobs(job* jobs, int *num_jobs)
{
    // Create three separate copies of the job arrays
    job* jobs_mutex = malloc((*num_jobs) * sizeof(job));  // Dereference num_jobs
    job* jobs_semaphore = malloc((*num_jobs) * sizeof(job));
    job* jobs_unsync = malloc((*num_jobs) * sizeof(job));

    memcpy(jobs_mutex, jobs, (*num_jobs) * sizeof(job));  // Dereference num_jobs
    memcpy(jobs_semaphore, jobs, (*num_jobs) * sizeof(job));
    memcpy(jobs_unsync, jobs, (*num_jobs) * sizeof(job));

    // Create mutexes and semaphores
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    // Thread identifiers
    pthread_t threads[6];

    // Execution arguments
    execution_args args_mutex = {jobs_mutex, (*num_jobs) , &mutex, NULL};
    execution_args args_semaphore = {jobs_semaphore, (*num_jobs), NULL, &semaphore};
    execution_args args_unsync = {jobs_unsync, (*num_jobs), NULL, NULL};

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
}
