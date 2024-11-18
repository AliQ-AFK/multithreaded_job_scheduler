#ifndef JOB_H
#define JOB_H
#define MAX_TIME 100
//job  struct serves as a container (process)
//the struct it self will be used in syncronization NO THREADS!
typedef struct 
{
    int    user_id;
    char   job_type[6];//print, scan + null terminator
    int    page;//number of pages
    unsigned int    arrival_time;
    int    start_time;
    int    completion_time;
} job;
job *initialize_job(int* num_users, int* num_jobs);
void schedule_jobs(job* jobs, int num_jobs);
void schedule_jobs_unsync(job* jobs, int num_jobs);
void schedule_jobs_mutex(job* jobs, int num_jobs);
void schedule_jobs_semaphore(job* jobs, int num_jobs);
#endif