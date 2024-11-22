#include "utils.h"
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

}