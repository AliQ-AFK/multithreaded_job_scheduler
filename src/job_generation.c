#include "utils.h"

job* initialize_job(int* num_users, int* num_jobs)
{
    #ifdef TESTING
    // Skip input during testing - use provided values
    printf("TESTING MODE: Using predefined values\n");
    #else
    // Only ask for input in non-testing mode
    printf("Enter a positive number of users: ");
    while (scanf("%d", num_users) != 1 || *num_users <= 0)
    {
        printf("Invalid input. Enter a positive number: ");
        while(getchar() != '\n');
    }

    printf("Enter a positive number of jobs per user: ");
    while (scanf("%d", num_jobs) != 1 || *num_jobs <= 0)
    {
        printf("Invalid input. Enter a positive number: ");
        while(getchar() != '\n');
    }
    #endif

    // Input validation
    if (*num_users <= 0 || *num_jobs <= 0)
    {
        return NULL;
    }

    int total_jobs = (*num_users) * (*num_jobs);
    job* jobs = (job*)malloc(total_jobs * sizeof(job));
    if (!jobs) return NULL;

    srand(time(NULL));

    for (int user = 0; user < *num_users; user++)
    {
        for (int i = 0; i < *num_jobs; i++)
        {
            int job_index = (user * (*num_jobs)) + i;
            jobs[job_index].user_id = user + 1;
            strcpy(jobs[job_index].job_type, (rand() % 2) ? "print" : "scan");
            jobs[job_index].page = rand() % 50 + 1;
            jobs[job_index].arrival_time = rand() % MAX_TIME + 1;
        }
    }

    *num_jobs = total_jobs;
    return jobs;
}