#include "utils.h"
//#include "execution.h"

int main() {
    int num_users;
    int num_jobs;

    // Initialize jobs
    job* jobs = initialize_job(&num_users, &num_jobs);
    if (jobs == NULL) { // Error handling
        fprintf(stderr, "Error: Job initialization failed.\n");
        return 1;
    }

    // Execute jobs using mutex synchronization
    //execute_mutex_jobs(jobs, &num_jobs);
    schedule_jobs(jobs, &num_jobs);
    // Free allocated resources
    free(jobs);
    return 0;
}




/*#include "utils.h"
#include "execution.h"

int main()
{
int num_users;
int num_jobs;

job* jobs = initialize_job(&num_users, &num_jobs);
if (jobs == NULL) // Error handling
    {
        fprintf(stderr, "Error: Job initialization failed.\n");
        return (1);
    }
execute_all_jobs(jobs, &num_jobs);

free(jobs);
return (0);

}*/