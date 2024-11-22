#include "utils.h"
#include "execution.h"

int main()
{
    int num_users;
    int num_jobs;

    // Initialize jobs dynamically
    job* jobs = initialize_job(&num_users, &num_jobs);
    if (jobs == NULL) // Error handling
    {
        fprintf(stderr, "Error: Job initialization failed.\n");
        return 1;
    }

    printf("[INFO] Initialized %d jobs for %d users.\n", num_jobs, num_users);

    // Execute only unsynced jobs for debugging
    execute_unsynced_jobs(jobs, &num_jobs);

    // Free allocated memory for jobs
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