#include "job.h"
#include "execution.h"
#include "test_utils.h"
#include "utils.h"
#include <sys/stat.h>
#include <stdlib.h>

// Test: Process a single ready job
void test_process_job_with_one_ready_job(FILE *log_file, TestStats *stats)
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

    log_test_result(log_file, stats, "test_process_job_with_one_ready_job", passed);
}

int main()
{
    // Ensure the logs directory exists
    mkdir("logs", 0777);

    // Open the log file
    FILE *log_file = fopen("logs/test_execution.log", "w");
    if (!log_file)
    {
        perror("Failed to open log file");
        return 1;
    }

    // Initialize the TestStats struct
    TestStats stats = {0};

    log_header(log_file, "Job Execution Tests");

    // Initialize the CUnit registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        perror("CUnit initialization failed");
        fclose(log_file);
        return 1;
    }

    CU_pSuite suite = CU_add_suite("Execution Tests", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        fclose(log_file);
        return 1;
    }

    // Add the test case with the new signature
    CU_add_test(suite, "test_process_job_with_one_ready_job",
                (CU_TestFunc)(void(*)(void))test_process_job_with_one_ready_job);

    clock_t start_time = clock();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    log_test_summary(log_file, &stats, start_time);

    CU_cleanup_registry();
    fclose(log_file);

    return 0;
}
