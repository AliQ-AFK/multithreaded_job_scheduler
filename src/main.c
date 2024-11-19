#include "utils.h"

int main()
{
    int num_users;
    int num_jobs;

    // Initialize jobs dynamically
    job* jobs = initialize_job(&num_users, &num_jobs);
    if (jobs == NULL)
    {
        fprintf(stderr, "Failed to initialize jobs\n");
        return EXIT_FAILURE;
    }

    // Call the function that schedules the jobs with mutex handling
    schedule_jobs(jobs, num_jobs);

    // Free allocated memory
    free(jobs);

    return EXIT_SUCCESS;
}
