#include "utils.h"
#include "test_utils.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <sys/stat.h>

void test_find_next_job(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats)
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    // Test case 1: Find print job
    {
        job jobs[] =
        {
            {.user_id = 1, .job_type = "print", .page = 5, .arrival_time = 0},
            {.user_id = 2, .job_type = "scan", .page = 3, .arrival_time = 1},
            {.user_id = 3, .job_type = "print", .page = 4, .arrival_time = 2}
        };
        int num_jobs = 3;
        int elapsed_time = 1;

        int job_index = find_next_job(jobs, &num_jobs, "print", elapsed_time);
        CU_ASSERT_EQUAL(job_index, 0);  // Should find first print job
    }

    // Test case 2: Find scan job
    {
        job jobs[] =
        {
            {.user_id = 1, .job_type = "print", .page = 5, .arrival_time = 0},
            {.user_id = 2, .job_type = "scan", .page = 3, .arrival_time = 1},
            {.user_id = 3, .job_type = "print", .page = 4, .arrival_time = 2}
        };
        int num_jobs = 3;
        int elapsed_time = 2;

        int job_index = find_next_job(jobs, &num_jobs, "scan", elapsed_time);
        CU_ASSERT_EQUAL(job_index, 1);  // Should find scan job
    }

    // Test case 3: No jobs ready (arrival time)
    {
        job jobs[] =
        {
            {.user_id = 1, .job_type = "print", .page = 5, .arrival_time = 10},
            {.user_id = 2, .job_type = "scan", .page = 3, .arrival_time = 11}
        };
        int num_jobs = 2;
        int elapsed_time = 5;

        int job_index = find_next_job(jobs, &num_jobs, "print", elapsed_time);
        CU_ASSERT_EQUAL(job_index, -1);  // Should find no jobs
    }

    // Test case 4: No matching job type
    {
        job jobs[] =
        {
            {.user_id = 1, .job_type = "print", .page = 5, .arrival_time = 0},
            {.user_id = 2, .job_type = "print", .page = 3, .arrival_time = 1}
        };
        int num_jobs = 2;
        int elapsed_time = 5;

        int job_index = find_next_job(jobs, &num_jobs, "scan", elapsed_time);
        CU_ASSERT_EQUAL(job_index, -1);  // Should find no scan jobs
    }

    // Test case 5: Empty job list
    {
        job jobs[1];  // Empty array
        int num_jobs = 0;
        int elapsed_time = 5;

        int job_index = find_next_job(jobs, &num_jobs, "print", elapsed_time);
        CU_ASSERT_EQUAL(job_index, -1);  // Should find no jobs
    }

    log_test_result(log_file, stats, "test_find_next_job", 
        CU_get_number_of_failures() == 0);
}

int main()
{
    FILE* log_file = open_log_file("test_scheduler.log", "a");
    if (!log_file)
    {
        return 1;
    }

    // Initialize test stats
    TestStats stats = {0};
    clock_t start_time = clock();

    // Initialize CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        fclose(log_file);
        return CU_get_error();
    }

    // Add suite to registry
    CU_pSuite suite = CU_add_suite("Scheduler Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        fclose(log_file);
        return CU_get_error();
    }

    // Log test header
    log_header(log_file, "Job Scheduler Tests");

    // Add tests to suite
    if (NULL == CU_add_test(suite, "test_find_next_job", 
                           (CU_TestFunc)test_find_next_job))
    {
        CU_cleanup_registry();
        fclose(log_file);
        return CU_get_error();
    }

    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Log test summary
    log_test_summary(log_file, &stats, start_time);

    // Cleanup
    CU_cleanup_registry();
    fclose(log_file);

    return CU_get_error();
}
