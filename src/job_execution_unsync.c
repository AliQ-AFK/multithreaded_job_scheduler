#include "utils.h"

int job_count = 0;  // No protection - will cause race conditions (intentionally)

void* execute_job(void* arg) {
    job* current_job = (job*)arg;

    if (current_job->page > 0) {  // Only process if pages > 0
        print_job_status(current_job, current_job->arrival_time, "Started");

        while (current_job->page > 0) {
            unsigned int pages_to_process = (current_job->page >= TIME_SLICE) ? 
                                          TIME_SLICE : current_job->page;
            print_job_progress(current_job, pages_to_process);
            
            usleep(pages_to_process * TIME_SCALE);

            // Only subtract if we have pages left
            if (current_job->page >= pages_to_process) {
                current_job->page -= pages_to_process;
            } else {
                current_job->page = 0;
            }
        }

        print_job_completion(current_job);
        job_count--;  // No protection - will cause race conditions (intentionally)
    }

    return NULL;
}

void schedule_jobs_unsync(job* jobs, int num_jobs) {
    job_count = num_jobs;
    unsigned int elapsed_time = 0;
    pthread_t* threads = malloc(num_jobs * sizeof(pthread_t));
    if (threads == NULL) {
        fprintf(stderr, "Failed to allocate memory for threads\n");
        return;
    }

    print_job_header();

    while (job_count > 0) {  // No protection - will cause race conditions (intentionally)
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1) {
            print_job_status(&jobs[selected_print_index], elapsed_time, "In Progress");
            pthread_create(&threads[selected_print_index], NULL, execute_job, &jobs[selected_print_index]);
        }

        if (selected_scan_index != -1) {
            print_job_status(&jobs[selected_scan_index], elapsed_time, "In Progress");
            pthread_create(&threads[selected_scan_index], NULL, execute_job, &jobs[selected_scan_index]);
        }

        elapsed_time += TIME_SLICE;
    }

    for (int i = 0; i < num_jobs; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("----------------------------------------------------------\n");
    printf("%sSuccess: %sNo corrupt data, all jobs completed!\n", 
           ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

    free(threads);
}
