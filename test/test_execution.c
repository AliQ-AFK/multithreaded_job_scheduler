#include "job.h"
#include "execution.h"
#include "test.h"
#include "utils.h"



// Test: Process a single ready job
void test_process_job_with_one_ready_job()
{
    job jobs[1] = {
        { .user_id = 1, .job_type = "print", .page = 5, .arrival_time = 0 }
    };
    int num_jobs = 1;
    unsigned int elapsed_time = 1;

    process_job(jobs, &num_jobs, "print", &elapsed_time, log_file, pthread_self());

    int passed = (num_jobs == 1 && jobs[0].page == 3 && (unsigned int)jobs[0].arrival_time == elapsed_time);

    CU_ASSERT_EQUAL(num_jobs, 1);
    CU_ASSERT_EQUAL(jobs[0].page, 3);
    CU_ASSERT_EQUAL(jobs[0].arrival_time, elapsed_time);

    log_test_result("test_process_job_with_one_ready_job", passed);
}

// Main function to run tests
int main()
{
    log_file = fopen("test_execution.log", "w");
    if (!log_file)
    {
        printf(COLOR_RED "Failed to open log file.\n" COLOR_RESET);
        return 1;
    }

    log_header("Job Execution Tests");

    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Execution Tests", NULL, NULL);

    CU_add_test(suite, "test_process_job_with_one_ready_job", test_process_job_with_one_ready_job);

    clock_t start_time = clock();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    log_test_summary(start_time);

    CU_cleanup_registry();
    fclose(log_file);

    return 0;
}
