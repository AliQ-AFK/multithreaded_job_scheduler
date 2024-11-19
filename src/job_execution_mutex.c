#include "utils.h"

void mutex(job* jobs, int num_jobs)
{
    unsigned int elapsed_time = 0;
    printf("\nJob Queue:\n");
    printf("----------------------------------------------------------\n");
    printf("| User ID | Type  | Pages     | Time (s)  | Status       |\n");
    printf("----------------------------------------------------------\n");
    fflush(stdout);  // Force flush

    while (num_jobs > 0)
    {
        printf("Checking for jobs at elapsed time: %u\n", elapsed_time);
        fflush(stdout);  // Force flush

        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1)
        {
            printf("Processing: User %d's %s job at %u seconds\n",
                   jobs[selected_print_index].user_id,
                   jobs[selected_print_index].job_type,
                   elapsed_time);
            fflush(stdout);  // Force flush

            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;

            if (jobs[selected_print_index].page <= 0)
            {
                printf("Completed: User %d's %s job\n",
                       jobs[selected_print_index].user_id,
                       jobs[selected_print_index].job_type);
                fflush(stdout);  // Force flush
                num_jobs--;
            }
        }
        else
        {
            printf("No print job selected.\n");
            fflush(stdout);  // Force flush
        }

        if (selected_scan_index != -1)
        {
            printf("Processing: User %d's %s job at %u seconds\n",
                   jobs[selected_scan_index].user_id,
                   jobs[selected_scan_index].job_type,
                   elapsed_time);
            fflush(stdout);  // Force flush

            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;

            if (jobs[selected_scan_index].page <= 0)
            {
                printf("Completed: User %d's %s job\n",
                       jobs[selected_scan_index].user_id,
                       jobs[selected_scan_index].job_type);
                fflush(stdout);  // Force flush
                num_jobs--;
            }
        }
        else
        {
            printf("No scan job selected.\n");
            fflush(stdout);  // Force flush
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing time
    }

    printf("----------------------------------------------------------\n");
    printf("Success: All jobs completed!\n");
    fflush(stdout);  // Force flush
}
