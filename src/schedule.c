#include "utils.h"

void schedule_jobs(job* jobs, int num_jobs)
{
    unsigned int elapsed_time = 0;
    print_job_header();

    while (num_jobs > 0)
    {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1)
        {
            print_job_status(&jobs[selected_print_index], elapsed_time, "Processing");
            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                  TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;
            
            if (jobs[selected_print_index].page <= 0)
            {
                print_job_completion(&jobs[selected_print_index]);
                num_jobs--;
            }
        }

        if (selected_scan_index != -1)
        {
            print_job_status(&jobs[selected_scan_index], elapsed_time, "Processing");
            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                  TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;
            
            if (jobs[selected_scan_index].page <= 0)
            {
                print_job_completion(&jobs[selected_scan_index]);
                num_jobs--;
            }
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing time
    }

    printf("----------------------------------------------------------\n");
    printf("%sSuccess: %sAll jobs completed!\n", 
           ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
}
    
