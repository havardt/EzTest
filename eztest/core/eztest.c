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
#include <float.h>
#include <assert.h>
#include <time.h>
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

/** Application options */
static struct options *options = NULL;

/** The result of the current/ latest test. */
static enum test_result result = undefined;

/** Create base/ reference test. */
TEST(_base_suite, _base_test){}

//region printers

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
    if(path == NULL)
    {
        return "";
    }
    return ++path;
}

void print_file_marker(char *file, const int line)
{
    printf(" (%s:%d)\n" COLOR_NONE, extract_file_name(file), line);
}

/**
 * Get the requested color if and only if the application options allow it.
 *
 * @param color The color to request (not NULL).
 * @return If 'no-color' has been set in the application options then
 *         @code COLOR_NONE @endcode is returned; otherwise the requested
 *         color is returned.
 */
static const char *color(const char *color)
{
    assert(color != NULL);

    if(options->no_color)
    {
        return COLOR_NONE;
    }
    return color;
}

/** Prints an overall report of the test results. */
static void print_report(void)
{
    printf("-----------------------------------\n"
           "|  "
           "%sPASSED"  COLOR_NONE "  |  "
           "%sSKIPPED" COLOR_NONE "  |  "
           "%sFAILED"  COLOR_NONE "  |\n"
           "-----------------------------------\n"
           "| "
           " %s%-7d" COLOR_NONE " | "
           " %s%-8d" COLOR_NONE " | "
           " %s%-7d" COLOR_NONE " |\n"
           "-----------------------------------\n\n",
           color(COLOR_GREEN) ,
           color(COLOR_YELLOW),
           color(COLOR_RED)   ,
           color(COLOR_GREEN) , pass_count,
           color(COLOR_YELLOW), skip_count,
           color(COLOR_RED)   , fail_count);
}

/**
 * Prints the test result output.
 *
 * @param test The unit test to print result for.
 * @param time The test execution time in ms.
 * @param resstr The string representing the result (Eg. PASS, FAIL etc)
 * @param c The requested color of the output result string.
 */
static void print_result(const struct unit_test *test,
                         const unsigned int time,
                         const char *restrict resstr,
                         const char *restrict c)
{
    printf("[%s : %s]%s %s " COLOR_NONE, test->test_suite, test->test_name, color(c), resstr);
    if(options->timer)
    {
        printf("(%dms)\n\n", time);
    }
    else
    {
        printf("\n\n");
    }
    fflush(stdout);
}

static void print_failed(const struct unit_test *test, const unsigned int time)
{
    print_result(test, time, "FAILED", COLOR_RED);
}

static void print_passed(const struct unit_test *test, const unsigned int time)
{
    print_result(test, time, "PASSED", COLOR_GREEN);
}

static void print_skipped(const struct unit_test *test, const unsigned int time)
{
    print_result(test, time, "SKIPPED", COLOR_YELLOW);
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
static void register_fail(char *file, const int line, const char *msg, ...)
{
    va_list va;
    result = fail;

    printf("[%s : %s] %s", current->test_suite, current->test_name, color(COLOR_YELLOW));
    va_start(va, msg);
    vprintf(msg, va);
    va_end(va);
    print_file_marker(file, line);
}

//endregion printers

//region asserts

void _assert_is_null(const void *value, char *file, const int line)
{
    if (value != NULL)
    {
        register_fail(file, line, "Assert is null failed: value is not null.");
    }
}

void _assert_is_not_null(const void *value, char *file, const int line)
{
    if (value == NULL)
    {
        register_fail(file, line, "Assert is not null failed: value is null.");
    }
}

void _assert_is_true(const bool condition, char *file, const int line)
{
    if(condition != true)
    {
        register_fail(file, line, "Assert is true failed.");
    }
}

void _assert_is_false(const bool condition, char *file, const int line)
{
    if(condition != false)
    {
        register_fail(file, line, "Assert is false failed.");
    }
}

void _assert_are_same(const void *expected, const void *actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are same failed: different memory location.");
    }
}

void _assert_are_not_same(const void *unexpected, const void *actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert are not same failed: same memory location.");
    }
}

#ifdef NAN

void _assert_is_nan(const float value, char *file, const int line)
{
    if(!isnan(value))
    {
        register_fail(file, line, "Assert is NaN failed.");
    }
}

#endif

void _assert_equal_mem(const void *expected, const void *actual, size_t size, char *file, const int line)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && memcmp(expected, actual, size) != 0))
    {
        result = fail;
        printf("[%s : %s]%s Assert are equal failed: expected ",
               current->test_suite, current->test_name, color(COLOR_YELLOW));
        print_bytes(expected, size);
        printf("but got ");
        print_bytes(actual, size);
        print_file_marker(file, line);
    }
}

void _assert_not_equal_mem(const void *unexpected, const void *actual, size_t size, char *file, const int line)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && memcmp(unexpected, actual, size) == 0))
    {
        register_fail(file, line, "Assert not equal failed.");
    }
}

void _assert_are_equal_ch(const char expected, const char actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_sch(const signed char expected, const signed char actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_uch(const unsigned char expected, const unsigned char actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are equal failed: expected '%c', but got '%c'.", expected, actual);
    }
}

void _assert_are_equal_int(const intmax_t expected, const intmax_t actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are equal failed: expected '%ld', but got '%ld'.", expected, actual);
    }
}

void _assert_are_equal_uint(const uintmax_t expected, const uintmax_t actual, char *file, const int line)
{
    if(expected != actual)
    {
        register_fail(file, line, "Assert are equal failed: expected '%ld', but got '%ld'.", expected, actual);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param expected The first and expected value.
 * @param actual   The actual value generated by the code under test.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use assert_are_equal_precision()
 *          and provide the application specific epsilon.
 */
void _assert_are_equal_dbl(const long double expected, const long double actual, char *file, const int line)
{
    if(fabsl(expected - actual) > LDBL_EPSILON)
    {
        register_fail(file, line, "Assert are equal failed: expected '%0.8Lf', but got '%0.8Lf'.", expected, actual);
    }
}
void _assert_are_equal_str(const char *expected, const char *actual, char *file, const int line)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && strcmp(expected, actual) != 0))
    {
        register_fail(file, line, "Assert are equal failed: expected '%s', but got '%s'.", expected, actual);
    }
}

void _assert_are_equal_wstr(const wchar_t *expected, const wchar_t *actual, char *file, const int line)
{
    if((expected == NULL && actual != NULL) ||
       (expected != NULL && actual == NULL) ||
       (expected != NULL && wcscmp(expected, actual) != 0))
    {
        register_fail(file, line, "Assert are equal failed: expected '%ls', but got '%ls'.", expected, actual);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_are_equal(const void *expected, const void *actual, char *file, const int line)
{
    register_fail(file, line, "Assert are equal failed: unsupported data type.");
}

void _assert_are_not_equal_ch(const char unexpected, const char actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_sch(const signed char unexpected, const signed char actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_uch(const unsigned char unexpected, const unsigned char actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%c) and actual(%c) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_int(const intmax_t unexpected, const intmax_t actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%ld) and actual(%ld) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_uint(const uintmax_t unexpected, const uintmax_t actual, char *file, const int line)
{
    if(unexpected == actual)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%ld) and actual(%ld) are equal.", unexpected, actual);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param unexpected The first and unexpected value.
 * @param actual     The actual value generated by the code under test.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use assert_are_equal_precision()
 *          and provide the application specific epsilon.
 */
void _assert_are_not_equal_dbl(const long double unexpected, const long double actual, char *file, const int line)
{
    if(fabsl(unexpected - actual) <= LDBL_EPSILON)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%0.8Lf) and actual(%0.8Lf) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_str(const char *unexpected, const char *actual, char *file, const int line)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && strcmp(unexpected, actual) == 0))
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%s) and actual(%s) are equal.", unexpected, actual);
    }
}

void _assert_are_not_equal_wstr(const wchar_t *unexpected, const wchar_t *actual, char *file, const int line)
{
    if((unexpected == NULL && actual == NULL) ||
       (unexpected != NULL && actual != NULL && wcscmp(unexpected, actual) == 0))
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%ls) and actual(%ls) are equal.", unexpected, actual);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_are_not_equal(const void *expected, const void *actual, char *file, const int line)
{
    register_fail(file, line, "Assert not equal failed: unsupported data type.");
}

void _assert_are_equal_precision(const long double  expected, 
                                 const long double  actual, 
                                 const long double  epsilon, 
                                 char              *file, 
                                 const int          line)
{
    if(fabsl(expected - actual) > epsilon)
    {
        register_fail(file, line, "Assert are equal failed: expected '%0.10Lf', but got '%0.10Lf'.", expected, actual);
    }
}

void _assert_are_not_equal_precision(const long double  unexpected, 
                                     const long double  actual, 
                                     const long double  epsilon, 
                                     char              *file, 
                                     const int          line)
{
    if(fabsl(unexpected - actual) <= epsilon)
    {
        register_fail(file, line, "Assert not equal failed: unexpected(%0.10Lf) and actual(%0.10Lf) are equal.", unexpected, actual);
    }
}

void _assert_greater_ch(const char greater, const char lesser, char *file, const int line)
{
    if(greater <= lesser)
    {
        register_fail(file, line, "Assert greater failed: '%c' is not greater than '%c'.", greater, lesser);
    }
}

void _assert_greater_sch(const signed char greater, const signed char lesser, char *file, const int line)
{
    if(greater <= lesser)
    {
        register_fail(file, line, "Assert greater failed: '%c' is not greater than '%c'.", greater, lesser);
    }
}

void _assert_greater_uch(const unsigned char greater, const unsigned char lesser, char *file, const int line)
{
    if(greater <= lesser)
    {
        register_fail(file, line, "Assert greater failed: '%c' is not greater than '%c'.", greater, lesser);
    }
}

void _assert_greater_int(const intmax_t greater, const intmax_t lesser, char *file, const int line)
{
    if(greater <= lesser)
    {
        register_fail(file, line, "Assert greater failed: '%ld' is not greater than '%ld'.", greater, lesser);
    }
}

void _assert_greater_uint(const uintmax_t greater, const uintmax_t lesser, char *file, const int line)
{
    if(greater <= lesser)
    {
        register_fail(file, line, "Assert greater failed: '%ld' is not greater than '%ld'.", greater, lesser);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param greater The value that is expected to be greater.
 * @param lesser  The value that is expected to be lesser.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use assert_greater_precision()
 *          and provide the application specific epsilon.
 */
void _assert_greater_dbl(const long double greater, const long double lesser, char *file, const int line)
{
    if(fabsl(greater - lesser) <= LDBL_EPSILON || greater < lesser)
    {
        register_fail(file, line, "Assert greater failed: '%0.8Lf' is not greater than '%0.8Lf'.", greater, lesser);
    }
}
void _assert_greater_str(const char *greater, const char *lesser, char *file, const int line)
{
    if((greater == NULL && lesser != NULL) ||
       (greater == NULL && lesser == NULL) ||
       (greater != NULL && lesser != NULL && strcmp(greater, lesser) <= 0))
    {
        register_fail(file, line, "Assert greater failed: '%s' is not greater than '%s'.", greater, lesser);
    }
}

void _assert_greater_wstr(const wchar_t *greater, const wchar_t *lesser, char *file, const int line)
{
    if((greater == NULL && lesser != NULL) ||
       (greater == NULL && lesser == NULL) ||
       (greater != NULL && lesser != NULL && wcscmp(greater, lesser) <= 0))
    {
        register_fail(file, line, "Assert greater failed: '%ls' is not greater than '%ls'.", greater, lesser);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_greater(const void *greater, const void *lesser, char *file, const int line)
{
    register_fail(file, line, "Assert greater failed: unsupported data type.");
}

void _assert_greater_precision(const long double   greater,
                               const long double   lesser,
                               const long double   epsilon,
                               char              * file,
                               const int           line)
{
    if(fabsl(greater - lesser) <= epsilon || greater < lesser)
    {
        register_fail(file, line, "Assert greater failed: '%0.8Lf' is not greater than '%0.8Lf'.", greater, lesser);
    }
}

void _assert_greater_equal_ch(const char ge, const char le, char *file, const int line)
{
    if(ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%c' is lesser than '%c'.", ge, le);
    }
}

void _assert_greater_equal_sch (const signed char ge, const signed char le, char *file, const int line)
{
    if(ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%c' is lesser than '%c'.", ge, le);
    }
}

void _assert_greater_equal_uch (const unsigned char ge, const unsigned char le, char *file, const int line)
{
    if(ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%c' is lesser than '%c'.", ge, le);
    }
}

void _assert_greater_equal_int (const intmax_t ge, const intmax_t le, char *file, const int line)
{
    if(ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%ld' is lesser than '%ld'.", ge, le);
    }
}

void _assert_greater_equal_uint(const uintmax_t ge, const uintmax_t le, char *file, const int line)
{
    if(ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%ld' is lesser than '%ld'.", ge, le);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param ge The value that is expected to be greater or equal to the second value.
 * @param le  The value that is expected to be less than or equal to the first value.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use _assert_greater_equal_precision()
 *          and provide the application specific epsilon.
 */
void _assert_greater_equal_dbl(const long double ge, const long double le, char *file, const int line)
{
    if(fabsl(ge - le) > LDBL_EPSILON && ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%0.8Lf' is lesser than '%0.8Lf'.", ge, le);
    }
}

void _assert_greater_equal_str(const char *ge, const char *le, char *file, const int line)
{
    if((ge == NULL && le != NULL) ||
       (ge != NULL && le != NULL && strcmp(ge, le) < 0))
    {
        register_fail(file, line, "Assert greater or equal failed: '%s' is lesser than '%s'.", ge, le);
    }
}

void _assert_greater_equal_wstr(const wchar_t *ge, const wchar_t *le, char *file, const int line)
{
    if((ge == NULL && le != NULL) ||
       (ge != NULL && le != NULL && wcscmp(ge, le) < 0))
    {
        register_fail(file, line, "Assert greater or equal failed: '%ls' is lesser than '%ls'.", ge, le);
    }
}
void _assert_greater_equal(const void *ge, const void *le, char *file, const int line)
{
    register_fail(file, line, "Assert greater or equal failed: unsupported data type.");
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
static void register_result(const unsigned int time)
{
    if(result == fail)
    {
        fail_count++;
        print_failed(current, time);
    }
    else if(result == skip)
    {
        skip_count++;
        print_skipped(current, time);
    }
    else
    {
        pass_count++;
        print_passed(current, time);
    }
}

/**
 * Executes the passed test, running the setup and teardown functions
 * if they are not @code NULL @endcode
 *
 * @param test The test to run.
 * @return The execution time in milliseconds.
 */
static unsigned int execute(const struct unit_test *test)
{
    clock_t t;
    t = clock();
    if(test->setup_fn != NULL)
    {
        test->setup_fn();
    }
    test->run_fn();
    if(test->teardown_fn != NULL)
    {
        test->teardown_fn();
    }
    t = clock() - t;
    return ((unsigned int)(((double)t) / CLOCKS_PER_SEC) * 1000);
}

/** See eztest.h */
int eztest_run(struct options *opts)
{
    assert(opts != NULL);

    options = opts;
    current = &_GET_STRUCT_NAME(_base_suite, _base_test);
    int count = discover(&current);
    unsigned int t;

    for (int i = 0; i < count; i++, current++)
    {
        if(should_skip(current))
        {
            result = skip;
            t = 0;
        }
        else
        {
            result = undefined; // Reset result before running new test.
            t = execute(current);
        }
        register_result(t);
    }
    print_report();

    return RESULT_OK;
}

//endregion runner
