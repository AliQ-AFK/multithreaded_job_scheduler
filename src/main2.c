#include "utils.h"

int main(void) {
    srand(time(NULL));  // Initialize random seed
    int num_users, num_jobs;
    
    // Initialize jobs
    job* jobs = initialize_job(&num_users, &num_jobs);
    if (jobs == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return EXIT_FAILURE;
    }

    #ifdef UNSYNC
        schedule_jobs_unsync(jobs, num_jobs);
    #elif defined MUTEX
        schedule_jobs_mutex(jobs, num_jobs);
    #elif defined SEMAPHORE
        schedule_jobs_semaphore(jobs, num_jobs);
    #else
        schedule_jobs(jobs, num_jobs);
    #endif

    free(jobs);
    return EXIT_SUCCESS;
}