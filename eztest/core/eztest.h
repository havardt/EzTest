/**
* 
* @author  havardt <https://github.com/havardt>
*
* @license MIT     <https://opensource.org/licenses/MIT>
*
*/

#ifndef EZTEST_H
#define EZTEST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct unit_test
{
    char *test_name;
    char *test_suite;
    void (*setup_fn)();
    void (*teardown_fn)();
    void (*run_fn)();

    /** Marks each unit test making it easier to find in memory. */
    uint64_t marker;
};

#define ANSWER_TO_LIFE 4242424242424242

#define _BASE_TEST_NAME "_base_test"

#define _GET_RUN_FN_NAME(suite, name) run_##suite##_##name

#define _GET_SETUP_FN_NAME(suite) setup_##suite

#define _GET_TEARDOWN_FN_NAME(suite) teardown_##suite

#define _GET_STRUCT_NAME(suite, name) struct_##suite##_##name

/**
 * Initializes the setup function for the given suite.
 *
 * @param suite The name of the test suite that this setup function belongs.
 * @note Each suite should only have one (1) setup function.
 */
#define SETUP(suite) static void _GET_SETUP_FN_NAME(suite)(void)

/**
 * Initializes the teardown function for the given suite.
 *
 * @param suite The name of the test suite that this teardown function belongs.
 * @note Each suite should only have one (1) teardown function.
 */
#define TEARDOWN(suite) static void _GET_TEARDOWN_FN_NAME(suite)(void)

#define _NEW_UNIT_TEST_STRUCT(suite, name)\
    static struct unit_test _GET_STRUCT_NAME(suite, name) __attribute__ ((used, section(".tests"), aligned(1))) = {\
        .test_name=#name,\
        .test_suite=#suite,\
        .run_fn = _GET_RUN_FN_NAME(suite, name),\
        .setup_fn = NULL,\
        .teardown_fn = NULL,\
        .marker = ANSWER_TO_LIFE\
    }

#define _NEW_FULL_UNIT_TEST_STRUCT(suite, name)\
    static struct unit_test _GET_STRUCT_NAME(suite, name) __attribute__ ((used, section(".tests"), aligned(1))) = {\
        .test_name=#name,\
        .test_suite=#suite,\
        .run_fn = _GET_RUN_FN_NAME(suite, name),\
        .setup_fn = _GET_SETUP_FN_NAME(suite),\
        .teardown_fn = _GET_TEARDOWN_FN_NAME(suite),\
        .marker = ANSWER_TO_LIFE\
    }

/**
 * Initialize a new unit test.
 *
 * @param suite The name of the test suite that this test belongs.
 * @param name  The name of the test.
 * @note        Suite and test-names must follow C naming rules.
 *              Suite and test-name combination must be unique for the project.
 */
#define TEST(suite, name)\
    static void _GET_RUN_FN_NAME(suite, name)(void);\
    _NEW_UNIT_TEST_STRUCT(suite, name);\
    static void _GET_RUN_FN_NAME(suite, name)(void)

/**
 * Initializes a new unit test with setup and teardown functions.
 *
 * @param suite The name of the test suite that this test belongs.
 * @param name The name of the test.
 * @note       Suite and test-names must follow C naming rules.
 *             Suite and test-name combination must be unique for the project.
 */
#define TEST_FULL(suite, name)\
    static void _GET_RUN_FN_NAME(suite, name)(void);\
    _NEW_FULL_UNIT_TEST_STRUCT(suite, name);\
    static void _GET_RUN_FN_NAME(suite, name)(void)

void _assert_is_null(const void *value, char *file, int line);
#define ASSERT_IS_NULL(value) _assert_is_null(value, __FILE__, __LINE__);

void _assert_is_not_null(const void *value, char *file, int line);
#define ASSERT_IS_NOT_NULL(value) _assert_is_not_null(value, __FILE__, __LINE__);

void _assert_is_true(bool condition, char *file, int line);
#define ASSERT_IS_TRUE(condition) _assert_is_true(condition, __FILE__, __LINE__);

void _assert_is_false(bool condition, char *file, int line);
#define ASSERT_IS_FALSE(condition) _assert_is_false(condition, __FILE__, __LINE__);

void _assert_are_same(const void *expected, const void *actual, char *file, int line);
#define ASSERT_ARE_SAME(expected, actual) _assert_are_same(expected, actual, __FILE__, __LINE__);

void _assert_are_not_same(const void *unexpected, const void *actual, char *file, int line);
#define ASSERT_ARE_NOT_SAME(unexpected, actual) _assert_are_not_same(unexpected, actual, __FILE__, __LINE__);

#ifdef NAN

void _assert_is_nan(float value, char *file, int line);
#define ASSERT_IS_NAN(value) _assert_is_nan(value, __FILE__, __LINE__);

#endif

void _assert_are_equal_ch  (char            expected, char            actual);
void _assert_are_equal_sch (signed char     expected, signed char     actual);
void _assert_are_equal_uch (unsigned char   expected, unsigned char   actual);
void _assert_are_equal_int (intmax_t        expected, intmax_t        actual);
void _assert_are_equal_uint(uintmax_t       expected, uintmax_t       actual);
void _assert_are_equal_dbl (long double     expected, long double     actual);
void _assert_are_equal_str (const char    * expected, const char    * actual);
void _assert_are_equal_wstr(const wchar_t * expected, const wchar_t * actual);
void _assert_are_equal     (                                                );
/**
 * Tests whether the two values are equal.
 *
 * @param expected The first value to compare. This is the value the tests expects.
 * @param actual   The second value to compare. This is the value produced by the code under test.
 */
#define ASSERT_ARE_EQUAL(expected, actual) _Generic((expected, actual),\
    signed char        : _assert_are_equal_sch,\
    char               : _assert_are_equal_ch,\
    short              : _assert_are_equal_int,\
    int                : _assert_are_equal_int,\
    long               : _assert_are_equal_int,\
    long long          : _assert_are_equal_int,\
    unsigned char      : _assert_are_equal_uch,\
    unsigned short     : _assert_are_equal_uint,\
    unsigned int       : _assert_are_equal_uint,\
    unsigned long      : _assert_are_equal_uint,\
    unsigned long long : _assert_are_equal_uint,\
    float              : _assert_are_equal_dbl,\
    double             : _assert_are_equal_dbl,\
    long double        : _assert_are_equal_dbl,\
    char *             : _assert_are_equal_str,\
    wchar_t *          : _assert_are_equal_wstr,\
    default            : _assert_are_equal)(expected, actual)

void _assert_are_equal_mem(const void *expected, const void *actual, size_t size);
/**
 * Checks for equality by comparing each byte at the given memory
 * locations.
 *
 * @param expected A pointer to the expected value.
 * @param actual   A pointer to the actual value.
 * @param size     The size of the passed types.
 */
#define ASSERT_ARE_EQUAL_MEM(expected, actual, size) _assert_are_equal_mem(expected, actual, size)

TEST(_base_suite, _base_test){}

#ifdef TEST_RUNNER

/**
 * Starts running tests.
 *
 * @param opts Application options.
 * @return @code EXIT_SUCCESS @endcode if no tests failed; otherwise @code EXIT_FAILURE @endcode.
 */
int eztest_run(struct options *opts);

#endif // TEST_RUNNER

#endif // EZTEST_H

