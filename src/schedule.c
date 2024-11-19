#include "utils.h"

void schedule_jobs(job* jobs, int num_jobs)
{
    unsigned int elapsed_time = 0;
    print_job_header();  // You can decide whether to keep this or replace it later

    while (num_jobs > 0)
    {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

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
}
