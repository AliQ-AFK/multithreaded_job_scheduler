#include "utils.h"
#include "execution.h"
#include "job.h"

int main()
{
int num_users, num_jobs;
job* jobs = initialize_job(&num_users, &num_jobs);
execute_all_jobs(jobs, &num_jobs);
free(jobs);
}