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
 * Extracts the file name from the given path.
 * Note that the file name includes the file extension.
 *
 * @param path The path of which to extract the file name.
 * @return A char pointer to the first char in the file name.
 */
const char *extract_file_name(char *path)
{
    path = strrchr(path, '/');
    return ++path;
}

//region asserts

void _assert_is_null(const void *value, char *file, int line)
{
    if (value != NULL)
    {
        result = fail;
        printf("[%s : %s]" COLOR_YELLOW " Assert is null failed: value is not null.\n" COLOR_NONE,
                current->test_suite, current->test_name);
    }
}

void _assert_is_not_null(const void *value, char *file, int line)
{
    if (value == NULL)
    {
        result = fail;
        printf("[%s : %s]" COLOR_YELLOW " Assert is not null failed: value is null.\n" COLOR_NONE,
               current->test_suite, current->test_name);
    }
}

void _assert_is_true(bool condition, char *file, int line)
{
    if(condition != true)
    {
        result = fail;
        printf("[%s : %s]" COLOR_YELLOW " Assert is true failed.\n" COLOR_NONE,
               current->test_suite, current->test_name);
    }
}

void _assert_is_false(bool condition, char *file, int line)
{
    if(condition != false)
    {
        result = fail;
        printf("[%s : %s]" COLOR_YELLOW " Assert is false failed.\n" COLOR_NONE,
               current->test_suite, current->test_name);
    }
}

//endregion asserts


//region printers

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
           pass_count, skip_count, fail_count);
}

static void print_failed(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_RED  " FAILED \n\n" COLOR_NONE, test->test_suite, test->test_name);
}

static void print_passed(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_GREEN " PASSED \n\n" COLOR_NONE, test->test_suite, test->test_name);
}

static void print_skipped(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_YELLOW  " SKIPPED \n\n" COLOR_NONE, test->test_suite, test->test_name);
}

//endregion printers


//region runner

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

//endregion runner
