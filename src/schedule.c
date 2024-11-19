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
            continue;  // Skip the rest of the loop and check again
        }

        if (selected_print_index != -1)
        {
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                  TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;

            if (jobs[selected_print_index].page <= 0)
            {
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
                num_jobs--;
            }
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing time
    }

    printf("Scheduling ends...\n");
    fflush(stdout);
}
