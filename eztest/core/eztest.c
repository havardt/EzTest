/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#include <stdlib.h>
#include <stdio.h>
#include "options.h"
#include "eztest.h"
#include <string.h>

#define _SKIP_TEST(test) test++; continue

/**
 * Counts the amount of tests starting at the memory location provided by start.
 *
 * @param start A pointer to the unit test of which to start counting.
 * @return an @see int representing the amount of tests including the starting test.
 */
static int get_test_count(struct unit_test *start)
{
    int count = 0;
    while(true)
    {
        if(start->marker == ANSWER_TO_LIFE)
        {
            start++;
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

__attribute__((no_sanitize_address)) int eztest_run(struct options *opts)
{
    struct unit_test *test = &_GET_STRUCT_NAME(_base_suite, _base_test);
    int test_count = get_test_count(test);

    for (int i = 0; i < test_count; i++)
    {
        if(strcmp(test->test_name, _BASE_TEST_NAME) == 0)
        {
            _SKIP_TEST(test);
        }
        printf("Test: %s\nSuite: %s\n", test->test_name, test->test_suite);
        test->run_fn();
        test++;
    }

    return EXIT_SUCCESS;
}

