#include "../include/job.h"
#include "../include/test.h"

// Test: Generate jobs with valid attributes
void test_initialize_job_valid_attributes()
{
    int num_users = 3;
    int num_jobs = 2;

    job *jobs = initialize_job(&num_users, &num_jobs);

    int passed = (jobs != NULL);

    CU_ASSERT(jobs != NULL);

    log_test_result("test_initialize_job_valid_attributes", passed);
    if (jobs)
        free(jobs);
}

// Main function to run tests
int main()
{
    log_file = fopen("test_job_generation.log", "w");
    if (!log_file)
    {
        printf(COLOR_RED "Failed to open log file.\n" COLOR_RESET);
        return 1;
    }

    log_header("Job Generation Tests");

    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Job Generation Tests", NULL, NULL);

    CU_add_test(suite, "test_initialize_job_valid_attributes", test_initialize_job_valid_attributes);

    clock_t start_time = clock();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    log_test_summary(start_time);

    CU_cleanup_registry();
    fclose(log_file);

    return 0;
}
