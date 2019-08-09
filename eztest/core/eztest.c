/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"
#include "eztest.h"
#include "../common/color.h"

static int pass_count = 0;
static int fail_count = 0;
static int skip_count = 0;

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
    printf("-----------------------------------\n"
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

static void print_success(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_GREEN " PASSED \n" COLOR_NONE, test->test_suite, test->test_name);
}

static void print_skipped(const struct unit_test *test)
{
    printf("[%s : %s]" COLOR_YELLOW  " SKIPPED \n" COLOR_NONE, test->test_suite, test->test_name);
}

/** See eztest.h */
__attribute__((no_sanitize_address)) int eztest_run(struct options *opts)
{
    struct unit_test *test = &_GET_STRUCT_NAME(_base_suite, _base_test);
    int test_count = discover(&test);

    for (int i = 0; i < test_count; i++, test++)
    {
        // TODO: run test :)
    }

    print_report();

    return EXIT_SUCCESS;
}

