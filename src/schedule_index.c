#include "utils.h"

int find_next_job(job* jobs, int num_jobs, const char* type, int elapsed_time) {
    printf("Finding next job of type %s at elapsed time %u\n", type, elapsed_time);
    fflush(stdout);

    for (int i = 0; i < num_jobs; i++)
    {
        printf("Checking job %d: Type = %s, Pages = %d, Arrival Time = %u\n",
               i, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
        fflush(stdout);

        if (jobs[i].page > 0 &&
            strcmp(jobs[i].job_type, type) == 0 &&
            jobs[i].arrival_time <= (unsigned int)elapsed_time)
        {
            printf("Selected job %d\n", i);
            fflush(stdout);
            return i;
        }
    }
    printf("No job found\n");
    fflush(stdout);
    return -1;
}

