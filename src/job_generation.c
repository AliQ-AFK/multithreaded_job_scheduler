#include "utils.h"

job* initialize_job(int* num_users, int* num_jobs) {
    FILE* file = fopen("jobs.dat", "rb");
    if (file) {
        // Read existing jobs from file
        fread(num_users, sizeof(int), 1, file);
        fread(num_jobs, sizeof(int), 1, file);
        
        job* jobs = (job*)malloc(*num_jobs * sizeof(job));
        if (jobs == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return NULL;
        }
        
        fread(jobs, sizeof(job), *num_jobs, file);
        fclose(file);

        // Debugging: Print job details to confirm they are read correctly
        printf("Number of users: %d\n", *num_users);
        printf("Number of jobs: %d\n", *num_jobs);
        for (int i = 0; i < *num_jobs; i++) {
            printf("Job %d: User ID = %d, Type = %s, Pages = %d, Arrival Time = %u\n",
                   i, jobs[i].user_id, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
        }
        return jobs;
    }

    // If no file exists, create new jobs
    printf("Enter the number of users:\n");
    if (scanf("%d", num_users) != 1) return NULL;
    printf("Enter the number of jobs per user:\n");
    if (scanf("%d", num_jobs) != 1) return NULL;

    // Calculate total number of jobs (users * jobs per user)
    int total_jobs = (*num_users) * (*num_jobs);
    job* jobs = (job*)malloc(total_jobs * sizeof(job));
    if (jobs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Generate jobs for each user
    for (int user = 0; user < *num_users; user++) {
        for (int i = 0; i < *num_jobs; i++) {
            int job_index = (user * (*num_jobs)) + i;
            jobs[job_index].user_id = user + 1;
            strcpy(jobs[job_index].job_type, (rand() % 2) ? "print" : "scan");
            jobs[job_index].page = rand() % 50 + 1;
            jobs[job_index].arrival_time = rand() % MAX_TIME + 1;
        }
    }

    // Debugging: Print job details to confirm they are created correctly
    printf("Number of users: %d\n", *num_users);
    printf("Number of jobs: %d\n", *num_jobs);
    for (int i = 0; i < total_jobs; i++) {
        printf("Job %d: User ID = %d, Type = %s, Pages = %d, Arrival Time = %u\n",
               i, jobs[i].user_id, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
    }

    // Update total number of jobs
    *num_jobs = total_jobs;

    // Save jobs to file
    file = fopen("jobs.dat", "wb");
    if (file) {
        fwrite(num_users, sizeof(int), 1, file);
        fwrite(num_jobs, sizeof(int), 1, file);
        fwrite(jobs, sizeof(job), *num_jobs, file);
        fclose(file);
    }

    return jobs;
}
