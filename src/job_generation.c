#include "utils.h"

job* initialize_job(int* num_users, int* num_jobs)
{
    printf("Enter the number of users:\n");
    if (scanf("%d", num_users) != 1) return NULL;
    printf("Enter the number of jobs per user:\n");
    if (scanf("%d", num_jobs) != 1) return NULL;

    // Calculate total number of jobs (users * jobs per user)
    int total_jobs = (*num_users) * (*num_jobs);
    job* jobs = (job*)malloc(total_jobs * sizeof(job));
    if (jobs == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Generate jobs for each user
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
    ) 

    // Debugging: Print job details to confirm they are created correctly
    printf("Number of users: %d\n", *num_users);
    printf("Number of jobs: %d\n", *num_jobs);
    for (int i = 0; i < total_jobs; i++)
    {
        printf("Job %d: User ID = %d, Type = %s, Pages = %d, Arrival Time = %u\n",
               i, jobs[i].user_id, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
    }

    // Update total number of jobs
    *num_jobs = total_jobs;

    return jobs;
    
}
