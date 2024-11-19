#include "utils.h"

/*void print_job_header(void) {
    printf("\n%sJob Queue:%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("----------------------------------------------------------\n");
    printf("| %-8s | %-6s | %-10s | %-9s | %-13s |\n", 
           "User ID", "Type", "Pages", "Time (s)", "Status");
    printf("----------------------------------------------------------\n");
}

void print_job_status(job* current_job, int elapsed_time, const char* status) {
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

void print_job_progress(job* current_job, int pages_processed) {
    printf("%sProcessing: User %d's %s job - %d pages processed (%d pages remaining)%s\n",
           ANSI_COLOR_BLUE,
           current_job->user_id,
           current_job->job_type,
           pages_processed,
           current_job->page - pages_processed,
           ANSI_COLOR_RESET);
}

void print_job_completion(job* current_job) {
    printf("%sCompleted: User %d's %s job%s\n",
           ANSI_COLOR_GREEN,
           current_job->user_id,
           current_job->job_type,
           ANSI_COLOR_RESET);
}
*/
int find_next_job(job* jobs, int num_jobs, const char* type, int elapsed_time) {
    printf("Finding next job of type %s at elapsed time %u\n", type, elapsed_time);
    fflush(stdout);

    for (int i = 0; i < num_jobs; i++) {
        printf("Checking job %d: Type = %s, Pages = %d, Arrival Time = %u\n",
               i, jobs[i].job_type, jobs[i].page, jobs[i].arrival_time);
        fflush(stdout);

        if (jobs[i].page > 0 &&
            strcmp(jobs[i].job_type, type) == 0 &&
            jobs[i].arrival_time <= (unsigned int)elapsed_time) {
            printf("Selected job %d\n", i);
            fflush(stdout);
            return i;
        }
    }
    printf("No job found\n");
    fflush(stdout);
    return -1;
}

