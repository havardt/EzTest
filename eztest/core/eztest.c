/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#include <stdio.h>
#include <string.h>
#include "options.h"
#include "eztest.h"
#include "../common/color.h"
#include "../common/result.h"

enum test_result
{
    undefined,
    pass,
    fail,
    skip
};

static int pass_count = 0;
static int fail_count = 0;
static int skip_count = 0;

/** The current test. */
static struct unit_test *current = NULL;

/** The result of the current/ latest test. */
static enum test_result result = undefined;

/**
 * Discovers the unit test starting at the given address.
 *
 * @param base_test the starting address of which to start the discovery.
 * @return the amount of tests found.
 */
static int discover(struct unit_test **base_test)
{
    int count = 0;
    struct unit_test *tmp = *base_test;

    while(true)
    {
        if(tmp->marker == ANSWER_TO_LIFE)
        {
            if(strcmp(tmp->test_name, _BASE_TEST_NAME) == 0)
            {
                (*base_test)++;
                tmp++;
                continue;
            }
            tmp++;
            count++;
        }
        else
        {
            break;
        }
    }
    printf("Test discovery finished, found %d tests.\n\n", count);
    return count;
}

/** Prints an overall report of the test results. */
static void print_report(void)
{
    printf("\n-----------------------------------\n"
           "|  " COLOR_GREEN  "PASSED"  COLOR_NONE "  |  "
                 COLOR_YELLOW "SKIPPED" COLOR_NONE "  |  "
                 COLOR_RED    "FAILED"  COLOR_NONE "  |\n"
           "-----------------------------------\n"
           "| " COLOR_GREEN  "%-8d" COLOR_NONE " | "
                COLOR_YELLOW "%-9d" COLOR_NONE " | "
                COLOR_RED    "%-8d" COLOR_NONE " |\n"
           "-----------------------------------\n",
           pass_count, fail_count, skip_count);
}

static void print_failed(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_RED  " FAILED \n" COLOR_NONE, test->test_suite, test->test_name);
}

static void print_passed(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_GREEN " PASSED \n" COLOR_NONE, test->test_suite, test->test_name);
}

static void print_skipped(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_YELLOW  " SKIPPED \n" COLOR_NONE, test->test_suite, test->test_name);
}

static bool should_skip(struct unit_test *test)
{
    // TODO check skip criteria.
    return false;
}

/**
 * Registers the current result by increasing the result counter
 * and printing the result message.
 */
static void register_result(void)
{
    if(result == fail)
    {
        fail_count++;
        print_failed(current);
    }
    else if(result == skip)
    {
        skip_count++;
        print_skipped(current);
    }
    else
    {
        pass_count++;
        print_passed(current);
    }
}

/**
 * Executes the passed test, running the setup and teardown functions
 * if they are not @code NULL @endcode
 *
 * @param test The test to run.
 */
static void execute(const struct unit_test *test)
{
    if(test->setup_fn != NULL)
    {
        test->setup_fn();
    }
    test->run_fn();
    if(test->teardown_fn != NULL)
    {
        test->teardown_fn();
    }
}

/** See eztest.h */
int eztest_run(struct options *opts)
{
    current = &_GET_STRUCT_NAME(_base_suite, _base_test);
    int count = discover(&current);

    for (int i = 0; i < count; i++, current++)
    {
        if(should_skip(current))
        {
            result = skip;
        }
        else
        {
            result = undefined; // Reset result before running new test.
            execute(current);
        }
        register_result();
    }
    print_report();

    return RESULT_OK;
}

