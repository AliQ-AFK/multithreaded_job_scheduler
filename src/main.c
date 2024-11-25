#include "utils.h"
//#include "execution.h"
#include "job.h"

int main() {
    int num_users;
    int num_jobs;
job* jobs = initialize_job(&num_users, &num_jobs);

  // Now create the three copies of the jobs array using memcpy
    job* mutex_jobs = (job*)malloc(num_jobs * sizeof(job));
    job* unsync_jobs = (job*)malloc(num_jobs * sizeof(job));
    job* semaphore_jobs = (job*)malloc(num_jobs * sizeof(job));

    // Use memcpy to copy the original jobs array to the new arrays
    memcpy(mutex_jobs, jobs, num_jobs * sizeof(job));
    memcpy(unsync_jobs, jobs, num_jobs * sizeof(job));
    memcpy(semaphore_jobs, jobs, num_jobs * sizeof(job));

}