#include "execution.h"
#include "utils.h"

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
            printf("[DEBUG] Locking mutex for execution.\n");
            fflush(stdout);
            pthread_mutex_lock(mutex); // Lock the mutex if provided
        }
        else if (semaphore != NULL)
        {
            printf("[DEBUG] Waiting on semaphore for execution.\n");
            fflush(stdout);
            sem_wait(semaphore); // Wait on the semaphore if provided
        }

        // Find and process jobs
        int selected_job_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        if (selected_job_index != -1)
        {
            printf("[DEBUG] Selected job for User %d. Pages before: %d\n",
                   jobs[selected_job_index].user_id, jobs[selected_job_index].page);
            fflush(stdout);

            int pages_to_process = (jobs[selected_job_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_job_index].page;
            jobs[selected_job_index].page -= pages_to_process;

            printf("[DEBUG] Processed %d pages for User %d. Pages remaining: %d\n",
                   pages_to_process, jobs[selected_job_index].user_id, jobs[selected_job_index].page);
            fflush(stdout);

            if (jobs[selected_job_index].page <= 0)
            {
                printf("[DEBUG] Completed job for User %d. Remaining jobs: %d\n", 
                       jobs[selected_job_index].user_id, num_jobs - 1);
                fflush(stdout);
                num_jobs--;
            }
        }
        else
        {
            printf("[DEBUG] No jobs ready at elapsed time %u.\n", elapsed_time);
            fflush(stdout);
        }

        if (mutex != NULL)
        {
            printf("[DEBUG] Unlocking mutex after execution.\n");
            fflush(stdout);
            pthread_mutex_unlock(mutex); // Unlock the mutex if provided
        }
        else if (semaphore != NULL)
        {
            printf("[DEBUG] Posting semaphore after execution.\n");
            fflush(stdout);
            sem_post(semaphore); // Signal the semaphore if provided
        }

        elapsed_time += TIME_SLICE;
        printf("[DEBUG] Incremented elapsed time to %u.\n", elapsed_time);
        fflush(stdout);

        usleep(TIME_SLICE * TIME_SCALE);
    }

    printf("[DEBUG] Job execution completed.\n");
    fflush(stdout);

    return NULL;
}

void destroy_synchronization_objects(pthread_mutex_t* mutex, sem_t* semaphore)
{
    if (mutex != NULL)
    {
        printf("[DEBUG] Destroying mutex.\n");
        fflush(stdout);
        pthread_mutex_destroy(mutex); // Destroy the mutex if it was initialized
    }

    if (semaphore != NULL)
    {
        printf("[DEBUG] Destroying semaphore.\n");
        fflush(stdout);
        sem_destroy(semaphore); // Destroy the semaphore if it was initialized
    }
}
