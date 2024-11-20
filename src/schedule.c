#include "utils.h"

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
            for (int i = 0; i < num_jobs; i++)
            {
                if (jobs[i].page > 0 && jobs[i].arrival_time > elapsed_time)
                {
                    if (jobs[i].arrival_time < next_arrival_time)
                    {
                        next_arrival_time = jobs[i].arrival_time;
                    }
                }
            }
            if (next_arrival_time != MAX_TIME)
            {
                printf("Skipping time forward from %u to %u\n", elapsed_time, next_arrival_time);
                elapsed_time = next_arrival_time;
            }
            else
            {
                // No more jobs to process, break out of the loop
                break;
            }
            continue;
        }

        // Instead of executing the job here, prepare it for execution
        if (selected_print_index != -1)
        {
            // Mark job for execution
            printf("Ready to execute print job for User %d\n", jobs[selected_print_index].user_id);
            fflush(stdout);
        }

        if (selected_scan_index != -1)
        {
            // Mark job for execution
            printf("Ready to execute scan job for User %d\n", jobs[selected_scan_index].user_id);
            fflush(stdout);
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate time passing for scheduling
    }

    printf("Scheduling ends...\n");
    fflush(stdout);
}
