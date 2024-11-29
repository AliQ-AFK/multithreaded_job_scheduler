#include "job.h"
#include "test_utils.h"
#include <sys/stat.h>
#include <stdlib.h>

// Test: Generate jobs with valid attributes
void test_initialize_job_valid_attributes(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats)
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    int num_users = 3;
    int num_jobs = 2;
    
    job *jobs = initialize_job(&num_users, &num_jobs);
    
    int passed = (jobs != NULL);
    
    if (jobs)
    {
        free(jobs);
    }

    log_test_result(log_file, stats, "test_initialize_job_valid_attributes", passed);
}

int main()
{
    FILE* log_file = open_log_file("test_job_generation.log", "a");
    if (!log_file)
    {
        return 1;
    }

    // Initialize the TestStats struct
    TestStats stats = {0};

    log_header(log_file, "Job Generation Tests");

    // Initialize the CUnit registry
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        perror("CUnit initialization failed");
        fclose(log_file);
        return 2;
    }

    CU_pSuite suite = CU_add_suite("Job Generation Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        fclose(log_file);
        return 3;
    }

    // Add the test case with the new signature
    CU_add_test(suite, "test_initialize_job_valid_attributes",
                (CU_TestFunc)(void(*)(void))test_initialize_job_valid_attributes);

    clock_t start_time = clock();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    log_test_summary(log_file, &stats, start_time);

    CU_cleanup_registry();
    fclose(log_file);

    return 0;
}