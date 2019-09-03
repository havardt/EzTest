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
        .setup_fn = NULL,\
        .teardown_fn = NULL,\
        .run_fn = _GET_RUN_FN_NAME(suite, name),\
        .marker = ANSWER_TO_LIFE\
    }

#define _NEW_FULL_UNIT_TEST_STRUCT(suite, name)\
    static struct unit_test _GET_STRUCT_NAME(suite, name) __attribute__ ((used, section(".tests"), aligned(1))) = {\
        .test_name=#name,\
        .test_suite=#suite,\
        .setup_fn = _GET_SETUP_FN_NAME(suite),\
        .teardown_fn = _GET_TEARDOWN_FN_NAME(suite),\
        .run_fn = _GET_RUN_FN_NAME(suite, name),\
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

void _assert_are_equal_ch  (char            expected, char            actual, char *file, int line);
void _assert_are_equal_sch (signed char     expected, signed char     actual, char *file, int line);
void _assert_are_equal_uch (unsigned char   expected, unsigned char   actual, char *file, int line);
void _assert_are_equal_int (intmax_t        expected, intmax_t        actual, char *file, int line);
void _assert_are_equal_uint(uintmax_t       expected, uintmax_t       actual, char *file, int line);
void _assert_are_equal_dbl (long double     expected, long double     actual, char *file, int line);
void _assert_are_equal_str (const char    * expected, const char    * actual, char *file, int line);
void _assert_are_equal_wstr(const wchar_t * expected, const wchar_t * actual, char *file, int line);
void _assert_are_equal     (const void    * expected, const void    * actual, char *file, int line);
/**
 * Tests whether the two values are equal.
 *
 * @param expected The first value to compare. This is the value the tests expects.
 * @param actual   The second value to compare. This is the value produced by the code under test.
 */
#define ASSERT_ARE_EQUAL(expected, actual) _Generic((expected, actual),\
             char        : _assert_are_equal_ch,   \
    signed   char        : _assert_are_equal_sch,  \
    unsigned char        : _assert_are_equal_uch,  \
                                                   \
             short       : _assert_are_equal_int,  \
    unsigned short       : _assert_are_equal_uint, \
                                                   \
             int         : _assert_are_equal_int,  \
    unsigned int         : _assert_are_equal_uint, \
                                                   \
             long        : _assert_are_equal_int,  \
    unsigned long        : _assert_are_equal_uint, \
                                                   \
             long long   : _assert_are_equal_int,  \
    unsigned long long   : _assert_are_equal_uint, \
                                                   \
             float       : _assert_are_equal_dbl,  \
             double      : _assert_are_equal_dbl,  \
             long double : _assert_are_equal_dbl,  \
                                                   \
             char *      : _assert_are_equal_str,  \
    const    char *      : _assert_are_equal_str,  \
                                                   \
             wchar_t *   : _assert_are_equal_wstr, \
    const    wchar_t *   : _assert_are_equal_wstr, \
                                                   \
    default              : _assert_are_equal)(expected, actual, __FILE__, __LINE__)

void _assert_are_not_equal_ch  (char            unexpected, char            actual, char *file, int line);
void _assert_are_not_equal_sch (signed char     unexpected, signed char     actual, char *file, int line);
void _assert_are_not_equal_uch (unsigned char   unexpected, unsigned char   actual, char *file, int line);
void _assert_are_not_equal_int (intmax_t        unexpected, intmax_t        actual, char *file, int line);
void _assert_are_not_equal_uint(uintmax_t       unexpected, uintmax_t       actual, char *file, int line);
void _assert_are_not_equal_dbl (long double     unexpected, long double     actual, char *file, int line);
void _assert_are_not_equal_str (const char    * unexpected, const char    * actual, char *file, int line);
void _assert_are_not_equal_wstr(const wchar_t * unexpected, const wchar_t * actual, char *file, int line);
void _assert_are_not_equal     (const void    * unexpected, const void    * actual, char *file, int line);
/**
 * Tests for inequality.
 *
 * @param unexpected The first value to compare. This is the value that should not occur.
 * @param actual     The second value to compare. This is the value produced by the code under test.
 * @note The equality function for floating point numbers uses the epsilon macro from float.h
 *       in its equality test. It is therefore often better to use assert_are_equal_precision()
 *       and provide the application specific epsilon.
 */
#define ASSERT_ARE_NOT_EQUAL(unexpected, actual) _Generic((unexpected, actual),\
             char        : _assert_are_not_equal_ch,   \
    signed   char        : _assert_are_not_equal_sch,  \
    unsigned char        : _assert_are_not_equal_uch,  \
                                                       \
             short       : _assert_are_not_equal_int,  \
    unsigned short       : _assert_are_not_equal_uint, \
                                                       \
             int         : _assert_are_not_equal_int,  \
    unsigned int         : _assert_are_not_equal_uint, \
                                                       \
             long        : _assert_are_not_equal_int,  \
    unsigned long        : _assert_are_not_equal_uint, \
                                                       \
             long long   : _assert_are_not_equal_int,  \
    unsigned long long   : _assert_are_not_equal_uint, \
                                                       \
             float       : _assert_are_not_equal_dbl,  \
             double      : _assert_are_not_equal_dbl,  \
             long double : _assert_are_not_equal_dbl,  \
                                                       \
             char *      : _assert_are_not_equal_str,  \
    const    char *      : _assert_are_not_equal_str,  \
                                                       \
             wchar_t *   : _assert_are_not_equal_wstr, \
    const    wchar_t *   : _assert_are_not_equal_wstr, \
                                                       \
    default              : _assert_are_not_equal)(unexpected, actual, __FILE__, __LINE__)

void _assert_equal_mem(const void *expected, const void *actual, size_t size, char *file, int line);
/**
 * Checks for equality by comparing each byte at the given memory locations.
 *
 * @param expected A pointer to the expected value.
 * @param actual   A pointer to the actual value.
 * @param size     The size of the passed types.
 */
#define ASSERT_EQUAL_MEM(expected, actual, size)\
    _assert_equal_mem(expected, actual, size, __FILE__, __LINE__)

void _assert_not_equal_mem(const void *unexpected, const void *actual, size_t size, char *file, int line);
/**
 * Checks for inequality by comparing each byte at the given memory locations.
 *
 * @param unexpected A pointer to the unexpected value.
 * @param actual     A pointer to the actual value.
 * @param size       The size of the passed types.
 */
#define ASSERT_NOT_EQUAL_MEM(unexpected, actual, size)\
    _assert_not_equal_mem(unexpected, actual, size, __FILE__, __LINE__)

void _assert_are_equal_precision(long double expected, long double actual, long double epsilon, char *file, int line);
/**
 * Tests for equality between two floating point numbers.
 *
 * @param expected The first value to compare. This is the value that is expected
 *                 to be generated by the code under test.
 *
 * @param actual The second value to compare. This is the value generated by the
 *               code under test.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_ARE_EQUAL_PRECISION(expected, actual, epsilon)\
    _assert_are_equal_precision(expected, actual, epsilon, __FILE__, __LINE__)

void _assert_are_not_equal_precision(long double   unexpected,
                                     long double   actual,
                                     long double   epsilon,
                                     char        * file,
                                     int           line);
/**
 * Tests for inequality between two floating point numbers.
 *
 * @param expected The first value to compare.
 *
 * @param actual The second value to compare. This is the value generated by the
 *               code under test.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_ARE_NOT_EQUAL_PRECISION(unexpected, actual, epsilon)\
    _assert_are_not_equal_precision(unexpected, actual, epsilon, __FILE__, __LINE__)

void _assert_greater_ch  (char            greater, char            lesser, char *file, int line);
void _assert_greater_sch (signed char     greater, signed char     lesser, char *file, int line);
void _assert_greater_uch (unsigned char   greater, unsigned char   lesser, char *file, int line);
void _assert_greater_int (intmax_t        greater, intmax_t        lesser, char *file, int line);
void _assert_greater_uint(uintmax_t       greater, uintmax_t       lesser, char *file, int line);
void _assert_greater_dbl (long double     greater, long double     lesser, char *file, int line);
void _assert_greater_str (const char    * greater, const char    * lesser, char *file, int line);
void _assert_greater_wstr(const wchar_t * greater, const wchar_t * lesser, char *file, int line);
void _assert_greater     (const void    * greater, const void    * lesser, char *file, int line);
/**
 * Tests whether the first value is greater than the second value.
 *
 * @param greater The first value to compare. This is the value the user
 *                expects to be greater than the second value.
 *
 * @param lesser  The second value to compare. This is the value the user
 *                expects to be lesser than the first value.
 */
#define ASSERT_GREATER(greater, lesser) _Generic((greater, lesser),\
             char        : _assert_greater_ch,   \
    signed   char        : _assert_greater_sch,  \
    unsigned char        : _assert_greater_uch,  \
                                                 \
             short       : _assert_greater_int,  \
    unsigned short       : _assert_greater_uint, \
                                                 \
             int         : _assert_greater_int,  \
    unsigned int         : _assert_greater_uint, \
                                                 \
             long        : _assert_greater_int,  \
    unsigned long        : _assert_greater_uint, \
                                                 \
             long long   : _assert_greater_int,  \
    unsigned long long   : _assert_greater_uint, \
                                                 \
             float       : _assert_greater_dbl,  \
             double      : _assert_greater_dbl,  \
             long double : _assert_greater_dbl,  \
                                                 \
             char *      : _assert_greater_str,  \
    const    char *      : _assert_greater_str,  \
                                                 \
             wchar_t *   : _assert_greater_wstr, \
    const    wchar_t *   : _assert_greater_wstr, \
                                                 \
    default              : _assert_greater)(greater, lesser, __FILE__, __LINE__)

/**
 * @see ASSERT_GREATER(greater, less);
 *
 * @remarks This is just a short-hand for ASSERT_GREATER.
 */
#define ASSERT_GT(greater, lesser) _Generic((greater, lesser),\
             char        : _assert_greater_ch,   \
    signed   char        : _assert_greater_sch,  \
    unsigned char        : _assert_greater_uch,  \
                                                 \
             short       : _assert_greater_int,  \
    unsigned short       : _assert_greater_uint, \
                                                 \
             int         : _assert_greater_int,  \
    unsigned int         : _assert_greater_uint, \
                                                 \
             long        : _assert_greater_int,  \
    unsigned long        : _assert_greater_uint, \
                                                 \
             long long   : _assert_greater_int,  \
    unsigned long long   : _assert_greater_uint, \
                                                 \
             float       : _assert_greater_dbl,  \
             double      : _assert_greater_dbl,  \
             long double : _assert_greater_dbl,  \
                                                 \
             char *      : _assert_greater_str,  \
    const    char *      : _assert_greater_str,  \
                                                 \
             wchar_t *   : _assert_greater_wstr, \
    const    wchar_t *   : _assert_greater_wstr, \
                                                 \
    default              : _assert_greater)(greater, lesser, __FILE__, __LINE__)

void _assert_greater_precision(long double   greater,
                               long double   lesser,
                               long double   epsilon,
                               char        * file,
                               int           line);
/**
 * Tests whether the first value is greater than the second value.
 *
 * @param greater The first floating point value to compare. This is the value the user
 *                expects to be greater than the second value.
 *
 * @param lesser  The second floating point value to compare. This is the value the user
 *                expects to be lesser than the first value.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_GREATER_PRECISION(greater, less, epsilon)\
    _assert_greater_precision(greater, less, epsilon, __FILE__, __LINE__)

/**
* @see ASSERT_GREATER_PRECISION(greater, less, epsilon);
*
* @remarks This is just a short-hand for ASSERT_GREATER_PRECISION.
*/
#define ASSERT_GT_PRECISION(greater, less, epsilon)\
    _assert_greater_precision(greater, less, epsilon, __FILE__, __LINE__)

void _assert_greater_equal_ch  (char            ge, char            le, char *file, int line);
void _assert_greater_equal_sch (signed char     ge, signed char     le, char *file, int line);
void _assert_greater_equal_uch (unsigned char   ge, unsigned char   le, char *file, int line);
void _assert_greater_equal_int (intmax_t        ge, intmax_t        le, char *file, int line);
void _assert_greater_equal_uint(uintmax_t       ge, uintmax_t       le, char *file, int line);
void _assert_greater_equal_dbl (long double     ge, long double     le, char *file, int line);
void _assert_greater_equal_str (const char    * ge, const char    * le, char *file, int line);
void _assert_greater_equal_wstr(const wchar_t * ge, const wchar_t * le, char *file, int line);
void _assert_greater_equal     (const void    * ge, const void    * le, char *file, int line);
/**
 * Tests whether the first value is greater than or equal to the second value.
 *
 * @param greater The first value to compare. This is the value the user
 *                expects to be greater than or equal to the second value.
 *
 * @param lesser  The second value to compare. This is the value the user
 *                expects to be lesser than or equal to the first value.
 */
#define ASSERT_GREATER_EQUAL(ge, le) _Generic((ge, le),\
             char        : _assert_greater_equal_ch,   \
    signed   char        : _assert_greater_equal_sch,  \
    unsigned char        : _assert_greater_equal_uch,  \
                                                       \
             short       : _assert_greater_equal_int,  \
    unsigned short       : _assert_greater_equal_uint, \
                                                       \
             int         : _assert_greater_equal_int,  \
    unsigned int         : _assert_greater_equal_uint, \
                                                       \
             long        : _assert_greater_equal_int,  \
    unsigned long        : _assert_greater_equal_uint, \
                                                       \
             long long   : _assert_greater_equal_int,  \
    unsigned long long   : _assert_greater_equal_uint, \
                                                       \
             float       : _assert_greater_equal_dbl,  \
             double      : _assert_greater_equal_dbl,  \
             long double : _assert_greater_equal_dbl,  \
                                                       \
             char *      : _assert_greater_equal_str,  \
    const    char *      : _assert_greater_equal_str,  \
                                                       \
             wchar_t *   : _assert_greater_equal_wstr, \
    const    wchar_t *   : _assert_greater_equal_wstr, \
                                                       \
    default              : _assert_greater_equal)(ge, le, __FILE__, __LINE__)

/**
* @see ASSERT_GREATER_EQUAL(ge, le);
*
* @remarks This is just a short-hand for ASSERT_GREATER_EQUAL.
*/
#define ASSERT_GE(ge, le) ASSERT_GREATER_EQUAL(ge, le)

void _assert_less_ch  (char           lesser, char           greater, char *file, int line);
void _assert_less_sch (signed char    lesser, signed char    greater, char *file, int line);
void _assert_less_uch (unsigned char  lesser, unsigned char  greater, char *file, int line);
void _assert_less_int (intmax_t       lesser, intmax_t       greater, char *file, int line);
void _assert_less_uint(uintmax_t      lesser, uintmax_t      greater, char *file, int line);
void _assert_less_dbl (long double    lesser, long double    greater, char *file, int line);
void _assert_less_str (const char    *lesser, const char    *greater, char *file, int line);
void _assert_less_wstr(const wchar_t *lesser, const wchar_t *greater, char *file, int line);
void _assert_less     (const void    *lesser, const void    *greater, char *file, int line);
/**
 * Tests whether the first value is lesser than the second value.
 *
 * @param lesser The first value to compare. This is the value the user
 *                expects to be lesser than the second value.
 *
 * @param greater The second value to compare. This is the value the user
 *                expects to be greater than the first value.
 */
#define ASSERT_LESS(lesser, greater) _Generic((lesser, greater),\
             char        : _assert_less_ch,   \
    signed   char        : _assert_less_sch,  \
    unsigned char        : _assert_less_uch,  \
                                              \
             short       : _assert_less_int,  \
    unsigned short       : _assert_less_uint, \
                                              \
             int         : _assert_less_int,  \
    unsigned int         : _assert_less_uint, \
                                              \
             long        : _assert_less_int,  \
    unsigned long        : _assert_less_uint, \
                                              \
             long long   : _assert_less_int,  \
    unsigned long long   : _assert_less_uint, \
                                              \
             float       : _assert_less_dbl,  \
             double      : _assert_less_dbl,  \
             long double : _assert_less_dbl,  \
                                              \
             char *      : _assert_less_str,  \
    const    char *      : _assert_less_str,  \
                                              \
             wchar_t *   : _assert_less_wstr, \
    const    wchar_t *   : _assert_less_wstr, \
                                              \
    default              : _assert_less)(lesser, greater, __FILE__, __LINE__)

/**
 * @see ASSERT_LESS(lesser, greater);
 *
 * @remarks This is just a short-hand for ASSERT_LESS.
 */
#define ASSERT_LT(lesser, greater) ASSERT_LESS(lesser, greater)

#ifdef TEST_RUNNER

/**
 * Starts running tests.
 *
 * @param opts Application options (not NULL).
 * @return @code EXIT_SUCCESS @endcode if no tests failed; otherwise @code EXIT_FAILURE @endcode.
 */
int eztest_run(struct options *opts);

#endif // TEST_RUNNER

#endif // EZTEST_H

