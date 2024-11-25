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
    int    arrival_time;
    int    start_time;
    int    completion_time;
} job;
job *initialize_job(int* num_users, int* num_jobs);
#endif