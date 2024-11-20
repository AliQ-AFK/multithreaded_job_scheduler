#include "utils.h"
#include "utils.h"
// find_next_job: Searches for the next job of a specified type that is ready to run.
// It returns the index of the job with the earliest arrival time, or -1 if no job is ready.
//:)
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

// schedule_jobs: Manages the scheduling and processing of jobs using a time-slicing approach.
// It handles job selection, time skipping for idle periods, and updates job status accordingly.

void schedule_jobs(job* jobs, int num_jobs)
{
    unsigned int elapsed_time = 0;
    printf("Scheduling starts...\n");
    fflush(stdout);

    while (num_jobs > 0)
    {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index == -1 && selected_scan_index == -1)
        {
            // No jobs are ready, so skip time forward to the next job's arrival time
            unsigned int next_arrival_time = MAX_TIME;
            for (int i = 0; i < num_jobs; i++) {
                if (jobs[i].page > 0 && jobs[i].arrival_time > elapsed_time) {
                    if (jobs[i].arrival_time < next_arrival_time) {
                        next_arrival_time = jobs[i].arrival_time;
                    }
                }
            }
            if (next_arrival_time != MAX_TIME) {
                printf("Skipping time forward from %u to %u\n", elapsed_time, next_arrival_time);
                elapsed_time = next_arrival_time;
            }
            else {
                // No more jobs to process, break out of the loop
                break;
            }
            continue;
        }

        if (selected_print_index != -1)
        {
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                  TIME_SLICE : jobs[selected_print_index].page;//if page == 1, it picks it to avoid idle:)
            jobs[selected_print_index].page -= pages_to_process;

            if (jobs[selected_print_index].page <= 0)
            {
                printf("Completed print job for User %d\n", jobs[selected_print_index].user_id);
                fflush(stdout);
                num_jobs--;
            }
        }

        if (selected_scan_index != -1)
        {
            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                  TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;

            if (jobs[selected_scan_index].page <= 0)
            {
                printf("Completed scan job for User %d\n", jobs[selected_scan_index].user_id);
                fflush(stdout);
                num_jobs--;
            }
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing time
    }

    printf("Scheduling ends...\n");
    fflush(stdout);
}
