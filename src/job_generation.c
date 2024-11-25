#include "utils.h"

job* initialize_job(int* num_users, int* num_jobs)
{
    printf("Enter a positive number of users: ");
    while (scanf("%d", num_users) != 1 || *num_users <= 0) {
        printf("Invalid input. Enter a positive number: ");
        while(getchar() != '\n'); // Clear input buffer
    }

    printf("Enter a positive number of jobs per user: ");
    while (scanf("%d", num_jobs) != 1 || *num_jobs <= 0) {
        printf("Invalid input. Enter a positive number: ");
        while(getchar() != '\n'); // Clear input buffer
    }

    int total_jobs = (*num_users) * (*num_jobs);
    job* jobs = (job*)malloc(total_jobs * sizeof(job));
    if (!jobs) return NULL;

    for (int user = 0; user < *num_users; user++) {
        for (int i = 0; i < *num_jobs; i++) {
            int job_index = (user * (*num_jobs)) + i;
            jobs[job_index].user_id = user + 1;
            strcpy(jobs[job_index].job_type, (rand() % 2) ? "print" : "scan");
            jobs[job_index].page = rand() % 10 + 1;
            jobs[job_index].arrival_time = rand() % 20 + 1;
        }
    }

    *num_jobs = total_jobs;
    return jobs;
}