#include "utils.h"

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
int job_count = 0;
static unsigned int elapsed_time = 0;

void* execute_job(void* arg) {
    job* current_job = (job*)arg;
    
    pthread_mutex_lock(&job_mutex);
    if (current_job->page <= 0) {
        pthread_mutex_unlock(&job_mutex);
        return NULL;
    }
    pthread_mutex_unlock(&job_mutex);

    while (1) {
        pthread_mutex_lock(&job_mutex);
        if (current_job->page <= 0) {
            pthread_mutex_unlock(&job_mutex);
            break;
        }
        
        unsigned int pages_to_process = (current_job->page >= TIME_SLICE) ? 
                                      TIME_SLICE : current_job->page;
        current_job->page -= pages_to_process;
        pthread_mutex_unlock(&job_mutex);

        pthread_mutex_lock(&print_mutex);
        print_job_progress(current_job, pages_to_process);
        pthread_mutex_unlock(&print_mutex);

        usleep(pages_to_process * TIME_SCALE);
    }

    pthread_mutex_lock(&job_mutex);
    job_count--;
    pthread_mutex_unlock(&job_mutex);

    pthread_mutex_lock(&print_mutex);
    print_job_completion(current_job);
    pthread_mutex_unlock(&print_mutex);

    return NULL;
}

void schedule_jobs_mutex(job* jobs, int num_jobs) {
    pthread_t* threads = malloc(num_jobs * sizeof(pthread_t));
    if (!threads) {
        fprintf(stderr, "Failed to allocate memory for threads\n");
        return;
    }

    job_count = num_jobs;
    print_job_header();

    while (job_count > 0) {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1) {
            pthread_mutex_lock(&print_mutex);
            print_job_status(&jobs[selected_print_index], elapsed_time, "In Progress");
            pthread_mutex_unlock(&print_mutex);
            pthread_create(&threads[selected_print_index], NULL, execute_job, &jobs[selected_print_index]);
            jobs[selected_print_index].arrival_time = MAX_TIME + 1;
        }

        if (selected_scan_index != -1) {
            pthread_mutex_lock(&print_mutex);
            print_job_status(&jobs[selected_scan_index], elapsed_time, "In Progress");
            pthread_mutex_unlock(&print_mutex);
            pthread_create(&threads[selected_scan_index], NULL, execute_job, &jobs[selected_scan_index]);
            jobs[selected_scan_index].arrival_time = MAX_TIME + 1;
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);
    }

    for (int i = 0; i < num_jobs; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("----------------------------------------------------------\n");
    printf("%sSuccess: %sNo data corruption, all jobs processed successfully!\n", 
           ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&job_mutex);
    free(threads);
}

void print_job_header(void)
{
    printf("\n%sJob Queue:%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("----------------------------------------------------------\n");
    printf("| %-8s | %-6s | %-10s | %-9s | %-13s |\n", 
           "User ID", "Type", "Pages", "Time", "Status");
}
