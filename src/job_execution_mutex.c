#include "utils.h"

void schedule_jobs(job* jobs, int num_jobs)
{
    unsigned int elapsed_time = 0;
    printf("\nJob Queue:\n");
    printf("----------------------------------------------------------\n");
    printf("| User ID | Type  | Pages     | Time (s)  | Status       |\n");
    printf("----------------------------------------------------------\n");

    while (num_jobs > 0)
    {
        int selected_print_index = find_next_job(jobs, num_jobs, "print", elapsed_time);
        int selected_scan_index = find_next_job(jobs, num_jobs, "scan", elapsed_time);

        if (selected_print_index != -1)
        {
            printf("Processing: User %d's %s job at %u seconds\n",
                   jobs[selected_print_index].user_id,
                   jobs[selected_print_index].job_type,
                   elapsed_time);

            int pages_to_process = (jobs[selected_print_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_print_index].page;
            jobs[selected_print_index].page -= pages_to_process;

            if (jobs[selected_print_index].page <= 0)
            {
                printf("Completed: User %d's %s job\n",
                       jobs[selected_print_index].user_id,
                       jobs[selected_print_index].job_type);
                num_jobs--;
            }
        }

        if (selected_scan_index != -1)
        {
            printf("Processing: User %d's %s job at %u seconds\n",
                   jobs[selected_scan_index].user_id,
                   jobs[selected_scan_index].job_type,
                   elapsed_time);

            int pages_to_process = (jobs[selected_scan_index].page >= TIME_SLICE) ? 
                                   TIME_SLICE : jobs[selected_scan_index].page;
            jobs[selected_scan_index].page -= pages_to_process;

            if (jobs[selected_scan_index].page <= 0)
            {
                printf("Completed: User %d's %s job\n",
                       jobs[selected_scan_index].user_id,
                       jobs[selected_scan_index].job_type);
                num_jobs--;
            }
        }

        elapsed_time += TIME_SLICE;
        usleep(TIME_SLICE * TIME_SCALE);  // Simulate processing time
    }

    printf("----------------------------------------------------------\n");
    printf("Success: All jobs completed!\n");
}

int find_next_job(job* jobs, int num_jobs, const char* type, int elapsed_time)
{
    for (int i = 0; i < num_jobs; i++)
    {
        if (jobs[i].page > 0 && 
            strcmp(jobs[i].job_type, type) == 0 && 
            jobs[i].arrival_time <= (unsigned int)elapsed_time)
        {
            return i;
        }
    }
    return -1;
}
