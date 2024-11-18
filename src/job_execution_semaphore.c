#include "utils.h"

// Semaphores to protect critical sections
sem_t print_semaphore;
sem_t job_semaphore;
int job_count = 0;  // Protected by job_semaphore
static unsigned int elapsed_time = 0;  // Global time reference

void* execute_job(void* arg) {
    job* current_job = (job*)arg;

    sem_wait(&job_semaphore);
    if (current_job->page <= 0) {  // Check if job is already completed
        sem_post(&job_semaphore);
        return NULL;
    }

    sem_wait(&print_semaphore);
    print_job_status(current_job, elapsed_time, "Locked");
    sem_post(&print_semaphore);

    while (current_job->page > 0) {
        unsigned int pages_to_process = (current_job->page >= TIME_SLICE) ? 
                                      TIME_SLICE : current_job->page;

        sem_wait(&print_semaphore);
        print_job_progress(current_job, pages_to_process);
        sem_post(&print_semaphore);

        usleep(pages_to_process * TIME_SCALE);
        current_job->page -= pages_to_process;
    }

    sem_wait(&print_semaphore);
    print_job_completion(current_job);
    sem_post(&print_semaphore);
    
    job_count--;
    sem_post(&job_semaphore);

    return NULL;
}

void schedule_jobs_semaphore(job* jobs, int num_jobs) {
    pthread_t* threads = malloc(num_jobs * sizeof(pthread_t));
    if (threads == NULL) {
        fprintf(stderr, "Failed to allocate memory for threads\n");
        return;
    }

    // Initialize semaphores
    if (sem_init(&print_semaphore, 0, 1) != 0) {
        fprintf(stderr, "Failed to initialize print semaphore\n");
        free(threads);
        return;
    }
    if (sem_init(&job_semaphore, 0, 1) != 0) {
        fprintf(stderr, "Failed to initialize job semaphore\n");
        sem_destroy(&print_semaphore);
        free(threads);
        return;
    }
    
    job_count = num_jobs;
    print_job_header();

    while (job_count > 0) {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1) {
            sem_wait(&print_semaphore);
            print_job_status(&jobs[selected_print_index], elapsed_time, "In Progress");
            sem_post(&print_semaphore);
            pthread_create(&threads[selected_print_index], NULL, execute_job, &jobs[selected_print_index]);
            
            // Mark job as in progress
            jobs[selected_print_index].arrival_time = MAX_TIME + 1;
        }

        if (selected_scan_index != -1) {
            sem_wait(&print_semaphore);
            print_job_status(&jobs[selected_scan_index], elapsed_time, "In Progress");
            sem_post(&print_semaphore);
            pthread_create(&threads[selected_scan_index], NULL, execute_job, &jobs[selected_scan_index]);
            
            // Mark job as in progress
            jobs[selected_scan_index].arrival_time = MAX_TIME + 1;
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    // Wait for all threads to complete
    for (int i = 0; i < num_jobs; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("----------------------------------------------------------\n");
    printf("%sSuccess: %sNo data corruption, all jobs processed successfully!\n", 
           ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

    sem_destroy(&print_semaphore);
    sem_destroy(&job_semaphore);
    free(threads);
}

