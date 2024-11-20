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

    while (num_jobs > 0)
    {
        if (mutex != NULL)
        {
            pthread_mutex_lock(mutex); // Lock the mutex if provided
        }
        else if (semaphore != NULL)
        {
            sem_wait(semaphore); // Wait on the semaphore if provided
        }

        // Find and process jobs
        int selected_job_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        if (selected_job_index != -1)
        {
            int pages_to_process = (jobs[selected_job_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_job_index].page;
            jobs[selected_job_index].page -= pages_to_process;

            if (jobs[selected_job_index].page <= 0)
            {
                printf("Completed job for User %d\n", jobs[selected_job_index].user_id);
                num_jobs--;
            }
        }

        if (mutex != NULL)
        {
            pthread_mutex_unlock(mutex); // Unlock the mutex if provided
        }
        else if (semaphore != NULL)
        {
            sem_post(semaphore); // Signal the semaphore if provided
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    return NULL;
}

void destroy_synchronization_objects(pthread_mutex_t* mutex, sem_t* semaphore)
{
    if (mutex != NULL)
    {
        pthread_mutex_destroy(mutex); // Destroy the mutex if it was initialized
    }

    if (semaphore != NULL)
    {
        sem_destroy(semaphore); // Destroy the semaphore if it was initialized
    }
}
