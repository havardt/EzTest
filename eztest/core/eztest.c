/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdarg.h>
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

//region printers

/** Prints an overall report of the test results. */
static void print_report(void)
{
    printf("\n-----------------------------------\n"
           "|  " COLOR_GREEN  "PASSED"  COLOR_NONE "  |  "
           COLOR_YELLOW "SKIPPED" COLOR_NONE "  |  "
           COLOR_RED    "FAILED"  COLOR_NONE "  |\n"
           "-----------------------------------\n"
           "| " COLOR_GREEN  " %-7d" COLOR_NONE " | "
           COLOR_YELLOW " %-8d" COLOR_NONE " | "
           COLOR_RED    " %-7d" COLOR_NONE " |\n"
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

/**
 * Prints the n first bytes at the memory location pointed to by the given pointer in hex.
 *
 * @param ptr Pointer to the memory location of which to start.
 * @param n   The amount of bytes to print.
 */
static void print_bytes(const void *ptr, size_t n)
{
    const unsigned char *bytes = (const unsigned char *)ptr;
    for (; n > 0; --n, ++bytes)
    {
        printf("%02X ", *bytes);
    }
}

/**
 * Registers the current test as a failure and prints the given failure message.
 *
 * @param msg The failure message to print.
 * @param ... Message arguments.
 */
static void register_fail(const char *msg, ...)
{
    va_list va;
    result = fail;

    printf("[%s : %s] " COLOR_YELLOW, current->test_suite, current->test_name);
    va_start(va, msg);
    vprintf(msg, va);
    va_end(va);
    printf("\n" COLOR_NONE);
}

/**
 * Registers the current test as a failure and prints the given failure message.
 *
 * @param msg The failure message to print.
 * @param ... Message arguments.
 */
static void register_fail_w(const wchar_t *msg, ...)
{
    va_list va;
    result = fail;

    printf("[%s : %s] " COLOR_YELLOW, current->test_suite, current->test_name);
    va_start(va, msg);
    vwprintf(msg, va);
    va_end(va);
    printf("\n" COLOR_NONE);
}

//endregion printers

//region asserts

void _assert_is_null(const void *value, char *file, int line)
{
    if (value != NULL)
    {
        register_fail("Assert is null failed: value is not null.");
    }
}

void _assert_is_not_null(const void *value, char *file, int line)
{
    if (value == NULL)
    {
        register_fail("Assert is not null failed: value is null.");
    }
}

void _assert_is_true(bool condition, char *file, int line)
{
    if(condition != true)
    {
        register_fail("Assert is true failed.");
    }
}

void _assert_is_false(bool condition, char *file, int line)
{
    if(condition != false)
    {
        register_fail("Assert is false failed.");
    }
}

void _assert_are_same(const void *expected, const void *actual, char *file, int line)
{
    if(expected != actual)
    {
        register_fail("Assert are same failed: different memory location.");
    }
}

void _assert_are_not_same(const void *unexpected, const void *actual, char *file, int line)
{
    if(unexpected == actual)
    {
        register_fail("Assert are not same failed: same memory location.");
    }
}

#ifdef NAN

void _assert_is_nan(float value, char *file, int line)
{
    if(!isnan(value))
    {
        register_fail("Assert is NaN failed.");
    }
}

#endif

void _assert_equal_mem(const void *expected, const void *actual, size_t size)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && memcmp(expected, actual, size) != 0))
    {
        result = fail;
        printf("[%s : %s]" COLOR_YELLOW " Assert are equal failed: expected ",
               current->test_suite, current->test_name);
        print_bytes(expected, size);
        printf("but got ");
        print_bytes(actual, size);
        printf("\n" COLOR_NONE);
    }
}

void _assert_not_equal_mem(const void *unexpected, const void *actual, size_t size)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && memcmp(unexpected, actual, size) == 0))
    {
        register_fail("Assert not equal failed.");
    }
}

void _assert_are_equal_ch(char expected, char actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_sch(signed char expected, signed char actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_uch(unsigned char expected, unsigned char actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_int(intmax_t expected, intmax_t actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%ld', but got '%ld'.", expected, actual);
    }
}

void _assert_are_equal_uint(uintmax_t expected, uintmax_t actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%ld', but got '%ld'.", expected, actual);
    }
}

void _assert_are_equal_dbl(long double expected, long double actual)
{
    if(expected != actual)
    {
        register_fail("Assert are equal failed: expected '%Lf', but got '%Lf'.", expected, actual);
    }
}
void _assert_are_equal_str(const char *expected, const char *actual)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && strcmp(expected, actual) != 0))
    {
        register_fail("Assert are equal failed: expected '%s', but got '%s'.", expected, actual);
    }
}

void _assert_are_equal_wstr(const wchar_t *expected, const wchar_t *actual)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && wcscmp(expected, actual) != 0))
    {
            register_fail_w(L"Assert are equal failed: expected '%ls', but got '%ls'.", expected, actual);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_are_equal()
{
    register_fail("Assert are equal failed: unsupported data type.");
}

void _assert_are_not_equal_ch(char unexpected, char actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_sch(signed char unexpected, signed char actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_uch(unsigned char unexpected, unsigned char actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_int(intmax_t unexpected, intmax_t actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%ld) and actual(%ld) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_uint(uintmax_t unexpected, uintmax_t actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%ld) and actual(%ld) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_dbl(long double unexpected, long double actual)
{
    if(unexpected == actual)
    {
        register_fail("Assert not equal failed: unexpected(%Lf) and actual(%Lf) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_str(const char *unexpected, const char *actual)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && strcmp(unexpected, actual) == 0))
    {
        register_fail("Assert not equal failed: unexpected(%s) and actual(%s) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_wstr(const wchar_t *unexpected, const wchar_t *actual)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && wcscmp(unexpected, actual) == 0))
    {
        register_fail_w(L"Assert not equal failed: unexpected(%ls) and actual(%ls) are equal.", unexpected, actual);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_are_not_equal()
{
    register_fail("Assert not equal failed: unsupported data type.");
}

//endregion asserts

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
