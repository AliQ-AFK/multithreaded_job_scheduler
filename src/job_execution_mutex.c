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
        usleep(TIME_SLICE * TIME_SCALE);
    }

    printf("----------------------------------------------------------\n");
    printf("%sSuccess: %sAll jobs completed!\n", 
           ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
}

void print_job_header(void)
{
    printf("\n%sJob Queue:%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("----------------------------------------------------------\n");
    printf("| %-8s | %-6s | %-10s | %-9s | %-13s |\n", 
           "User ID", "Type", "Pages", "Time (s)", "Status");
    printf("----------------------------------------------------------\n");
}

void print_job_status(job* current_job, int elapsed_time, const char* status)
{
    char pages_str[16];
    char time_str[16];
    
    snprintf(pages_str, sizeof(pages_str), "%d pages", current_job->page);
    snprintf(time_str, sizeof(time_str), "%.1fs", elapsed_time * (TIME_SCALE / 1000000.0));

    printf("| %-8d | %-6s | %-10s | %-9s | %s%-13s%s |\n",
           current_job->user_id,
           current_job->job_type,
           pages_str,
           time_str,
           ANSI_COLOR_RED,
           status,
           ANSI_COLOR_RESET);
}

void print_job_completion(job* current_job)
{
    printf("%sCompleted: User %d's %s job%s\n",
           ANSI_COLOR_GREEN,
           current_job->user_id,
           current_job->job_type,
           ANSI_COLOR_RESET);
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
