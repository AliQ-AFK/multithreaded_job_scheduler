#include "utils.h"
int find_next_job(job* jobs, int* num_jobs, const char* type, int elapsed_time)
{
    printf("[DEBUG] find_next_job called with num_jobs = %d\n", *num_jobs);
    fflush(stdout);

    for (int i = 0; i < *num_jobs; i++)
    {
        printf("[DEBUG] Checking job %d: Type = %s, Pages = %d, Arrival Time = %u\n",
               i, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
        fflush(stdout);

        // Check if the job matches criteria
        if (jobs[i].page > 0 &&                              // Job has remaining pages
            strcmp(jobs[i].job_type, type) == 0 &&           // Matches requested type
            jobs[i].arrival_time <= elapsed_time) 
        { // Job is ready
            printf("[DEBUG] Selected job %d\n", i);
            fflush(stdout);
            return i; // Return the index of the job
        }
    }

    printf("[DEBUG] No job found for type %s at elapsed time %u\n", type, elapsed_time);
    fflush(stdout);
    return -1; // No job is ready
}