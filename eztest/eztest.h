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

/** Represents the application options for EzTest. */
struct options
{
    /** When set to @code true @endcode only default color is used when printing. */
    bool no_color;
    /** When set to @code true @endcode the test execution time is displayed for each test */
    bool timer;
};

struct unit_test
{
    char *test_name;
    char *test_suite;
    void (*setup_fn)();
    void (*teardown_fn)();
    void (*run_fn)();

    /** Marks each unit test: used for test discovery. */
    uint64_t marker;
};

/** Represents the standard ok/ success result value for non-pointer return types. */
#define RESULT_OK 0

/** Represents the standard error/ fail result value for non-pointer return types. */
#define RESULT_ERR -1

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

/**
 * @see ASSERT_ARE_EQUAL(expected, actual);
 *
 * @remarks This is a short-hand for ASSERT_ARE_EQUAL.
 */
#define ASSERT_EQ(expected, actual) ASSERT_ARE_EQUAL(expected, actual)

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

/**
 * @see ASSERT_ARE_NOT_EQUAL(unexpected, actual);
 *
 * @remarks This is a short-hand for ASSERT_ARE_NOT_EQUAL.
 */
#define ASSERT_NE(unexpected, actual) ASSERT_ARE_NOT_EQUAL(unexpected, actual)

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

/**
 * @see ASSERT_EQUAL_MEM(expected, actual, size);
 *
 * @remarks This is a short-hand for ASSERT_EQUAL_MEM.
 */
#define ASSERT_EQ_MEM(expected, actual, size) ASSERT_EQUAL_MEM(expected, actual, size)


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

/**
 * @see ASSERT_NOT_EQUAL_MEM(unexpected, actual, size);
 *
 * @remarks This is a short-hand for ASSERT_NOT_EQUAL_MEM.
 */
#define ASSERT_NE_MEM(unexpected, actual, size) ASSERT_NOT_EQUAL_MEM(unexpected, actual, size)


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

/**
 * @see ASSERT_ARE_EQUAL_PRECISION(expected, actual, epsilon);
 *
 * @remarks This is a short-hand for ASSERT_ARE_EQUAL_PRECISION.
 */
#define ASSERT_EQ_PRECISION(expected, actual, epsilon) ASSERT_ARE_EQUAL_PRECISION(expected, actual, epsilon)

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

/**
 * @see ASSERT_ARE_NOT_EQUAL_PRECISION(unexpected, actual, epsilon);
 *
 * @remarks This is a short-hand for ASSERT_ARE_NOT_EQUAL_PRECISION.
 */
#define ASSERT_NE_PRECISION(unexpected, actual, epsilon) ASSERT_ARE_NOT_EQUAL_PRECISION(unexpected, actual, epsilon)

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
#define ASSERT_GT(greater, lesser) ASSERT_GREATER(greater, lesser)

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

void _assert_greater_equal_precision(long double  ge,
                                     long double  le,
                                     long double  epsilon,
                                     char        *file,
                                     int          line);

/**
 * Tests whether the first value is greater than or equal to the second value.
 *
 * @param ge The first floating point value to compare. This is the value the user
 *           expects to be greater than or equal to the second value.
 *
 * @param le The second floating point value to compare. This is the value the user
 *           expects to be lesser than or equal to the first value.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_GREATER_EQUAL_PRECISION(ge, le, epsilon)\
    _assert_greater_equal_precision(ge, le, epsilon, __FILE__, __LINE__)

/**
* @see ASSERT_GREATER_EQUAL_PRECISION(ge, le, epsilon);
*
* @remarks This is just a short-hand for ASSERT_GREATER_EQUAL_PRECISION.
*/
#define ASSERT_GE_PRECISION(ge, le, epsilon)\
    _assert_greater_equal_precision(ge, le, epsilon, __FILE__, __LINE__)

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

void _assert_less_precision(long double  lesser,
                            long double  greater,
                            long double  epsilon,
                            char        *file,
                            int          line);

/**
 * Tests whether the first value is lesser than the second value.
 *
 * @param lesser  The first floating point value to compare. This is the value the user
 *                expects to be lesser than the second value.
 *
 * @param greater The second floating point value to compare. This is the value the user
 *                expects to be greater than the first value.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_LESS_PRECISION(lesser, greater, epsilon)\
    _assert_less_precision(lesser, greater, epsilon, __FILE__, __LINE__)

/**
* @see ASSERT_LESS_PRECISION(lesser, greater, epsilon);
*
* @remarks This is just a short-hand for ASSERT_LESS_PRECISION.
*/
#define ASSERT_LT_PRECISION(lesser, greater, epsilon)\
    _assert_less_precision(lesser, greater, epsilon, __FILE__, __LINE__)

void _assert_less_equal_ch  (char           le, char           ge, char *file, int line);
void _assert_less_equal_sch (signed char    le, signed char    ge, char *file, int line);
void _assert_less_equal_uch (unsigned char  le, unsigned char  ge, char *file, int line);
void _assert_less_equal_int (intmax_t       le, intmax_t       ge, char *file, int line);
void _assert_less_equal_uint(uintmax_t      le, uintmax_t      ge, char *file, int line);
void _assert_less_equal_dbl (long double    le, long double    ge, char *file, int line);
void _assert_less_equal_str (const char    *le, const char    *ge, char *file, int line);
void _assert_less_equal_wstr(const wchar_t *le, const wchar_t *ge, char *file, int line);
void _assert_less_equal     (const void    *le, const void    *ge, char *file, int line);
/**
 * Tests whether the first value is lesser than or equal to the second value.
 *
 * @param le The first value to compare. This is the value the user
 *           expects to be lesser than or equal to the second value.
 *
 * @param ge The second value to compare. This is the value the user
 *           expects to be greater than or equal to the first value.
 */
#define ASSERT_LESS_EQUAL(le, ge) _Generic((le, ge),\
             char        : _assert_less_equal_ch,   \
    signed   char        : _assert_less_equal_sch,  \
    unsigned char        : _assert_less_equal_uch,  \
                                                    \
             short       : _assert_less_equal_int,  \
    unsigned short       : _assert_less_equal_uint, \
                                                    \
             int         : _assert_less_equal_int,  \
    unsigned int         : _assert_less_equal_uint, \
                                                    \
             long        : _assert_less_equal_int,  \
    unsigned long        : _assert_less_equal_uint, \
                                                    \
             long long   : _assert_less_equal_int,  \
    unsigned long long   : _assert_less_equal_uint, \
                                                    \
             float       : _assert_less_equal_dbl,  \
             double      : _assert_less_equal_dbl,  \
             long double : _assert_less_equal_dbl,  \
                                                    \
             char *      : _assert_less_equal_str,  \
    const    char *      : _assert_less_equal_str,  \
                                                    \
             wchar_t *   : _assert_less_equal_wstr, \
    const    wchar_t *   : _assert_less_equal_wstr, \
                                                    \
    default              : _assert_less_equal)(le, ge, __FILE__, __LINE__)

/**
 * @see ASSERT_LESS_EQUAL(le, ge);
 *
 * @remarks This is just a short-hand for ASSERT_LESS_EQUAL.
 */
#define ASSERT_LE(le, ge) ASSERT_LESS_EQUAL(le, ge)

void _assert_less_equal_precision(long double  le,
                                  long double  ge,
                                  long double  epsilon,
                                  char        *file,
                                  int          line);

/**
 * Tests whether the first value is lesser than or equal to the second value.
 *
 * @param le The first floating point value to compare. This is the value the user
 *           expects to be lesser than or equal to the second value.
 *
 * @param ge The second floating point value to compare. This is the value the user
 *           expects to be greater than or equal to the first value.
 *
 * @param epsilon A floating point representing the precision required when testing
 *                for equality.
 */
#define ASSERT_LESS_EQUAL_PRECISION(le, ge, epsilon)\
    _assert_less_equal_precision(le, ge, epsilon, __FILE__, __LINE__)

/**
* @see ASSERT_LESS_EQUAL_PRECISION(le, ge, epsilon);
*
* @remarks This is just a short-hand for ASSERT_LESS_EQUAL_PRECISION.
*/
#define ASSERT_LE_PRECISION(le, ge, epsilon)\
    _assert_less_equal_precision(le, ge, epsilon, __FILE__, __LINE__)

#ifdef TEST_RUNNER

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdarg.h>
#include <float.h>
#include <assert.h>
#include <time.h>

#define COLOR_RED    "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN  "\033[0;32m"
#define COLOR_NONE   "\033[0m"

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
static const char *extract_file_name(char *path)
{
    path = strrchr(path, '/');
    if(path == NULL)
    {
        return "";
    }
    return ++path;
}

static void print_file_marker(char *file, const int line)
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

void _assert_greater_equal_precision(long double  ge,
                                     long double  le,
                                     long double  epsilon,
                                     char        *file,
                                     int          line)
{
    if(fabsl(ge - le) > epsilon && ge < le)
    {
        register_fail(file, line, "Assert greater or equal failed: '%0.8Lf' is lesser than '%0.8Lf'.", ge, le);
    }
}

void _assert_less_ch(const char lesser, const char greater, char *file, const int line)
{
    if(lesser >= greater)
    {
        register_fail(file, line, "Assert less failed: '%c' is not lesser then '%c'.", lesser, greater);
    }
}

void _assert_less_sch(const signed char lesser, const signed char greater, char *file, const int line)
{
    if(lesser >= greater)
    {
        register_fail(file, line, "Assert less failed: '%c' is not lesser then '%c'.", lesser, greater);
    }
}

void _assert_less_uch(const unsigned char lesser, const unsigned char greater, char *file, const int line)
{
    if(lesser >= greater)
    {
        register_fail(file, line, "Assert less failed: '%c' is not lesser then '%c'.", lesser, greater);
    }
}

void _assert_less_int(const intmax_t lesser, const intmax_t greater, char *file, const int line)
{
    if(lesser >= greater)
    {
        register_fail(file, line, "Assert less failed: '%ld' is not lesser then '%ld'.", lesser, greater);
    }
}

void _assert_less_uint(const uintmax_t lesser, const uintmax_t greater, char *file, const int line)
{
    if(lesser >= greater)
    {
        register_fail(file, line, "Assert less failed: '%ld' is not lesser then '%ld'.", lesser, greater);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param lesser  The value that is expected to be lesser.
 * @param greater The value that is expected to be greater.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use assert_greater_precision()
 *          and provide the application specific epsilon.
 */
void _assert_less_dbl(const long double lesser, const long double greater, char *file, const int line)
{
    if(fabsl(greater - lesser) <= LDBL_EPSILON || lesser > greater)
    {
        register_fail(file, line, "Assert less failed: '%0.8Lf' is not lesser then '%0.8Lf'.", lesser, greater);
    }
}

void _assert_less_str(const char *lesser, const char *greater, char *file, const int line)
{
    if((greater == NULL && lesser != NULL) ||
       (greater == NULL && lesser == NULL) ||
       (greater != NULL && lesser != NULL && strcmp(lesser, greater) >= 0))
    {
        register_fail(file, line, "Assert less failed: '%s' is not lesser then '%s'.", lesser, greater);
    }
}

void _assert_less_wstr(const wchar_t *lesser, const wchar_t *greater, char *file, const int line)
{
    if((greater == NULL && lesser != NULL) ||
       (greater == NULL && lesser == NULL) ||
       (greater != NULL && lesser != NULL && wcscmp(lesser, greater) >= 0))
    {
        register_fail(file, line, "Assert less failed: '%ls' is not lesser then '%ls'.", lesser, greater);
    }
}

/** Triggered when attempting to compare using an unsupported data type. */
void _assert_less(const void *lesser, const void *greater, char *file, const int line)
{
    register_fail(file, line, "Assert less failed: unsupported data type.");
}

void _assert_less_precision(const long double  lesser,
                            const long double  greater,
                            const long double  epsilon,
                            char              *file,
                            const int          line)
{
    if(fabsl(lesser - greater) <= epsilon || lesser > greater)
    {
        register_fail(file, line, "Assert less failed: '%0.8Lf' is not lesser then '%0.8Lf'.", lesser, greater);
    }
}

void _assert_less_equal_ch(const char le, const char ge, char *file, const int line)
{
    if(le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%c' is greater then '%c'.", le, ge);
    }
}

void _assert_less_equal_sch (const signed char le, const signed char ge, char *file, const int line)
{
    if(le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%c' is greater then '%c'.", le, ge);
    }
}

void _assert_less_equal_uch (const unsigned char le, const unsigned char ge, char *file, const int line)
{
    if(le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%c' is greater then '%c'.", le, ge);
    }
}

void _assert_less_equal_int (const intmax_t le, const intmax_t ge, char *file, const int line)
{
    if(le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%ld' is greater then '%ld'.", le, ge);
    }
}

void _assert_less_equal_uint(const uintmax_t le, const uintmax_t ge, char *file, const int line)
{
    if(le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%ld' is greater then '%ld'.", le, ge);
    }
}

/**
 * Tests the equality of two floating point numbers.
 *
 * @param le The value that is expected to be lesser or equal to the second value.
 * @param ge The value that is expected to be greater than or equal to the first value.
 *
 * @remarks It is important to known that this test uses the epsilon macro from float.h
 *          in its equality test. It is therefore often better to use _assert_greater_equal_precision()
 *          and provide the application specific epsilon.
 */
void _assert_less_equal_dbl(const long double le, const long double ge, char *file, const int line)
{
    if(fabsl(ge - le) > LDBL_EPSILON && le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%0.8Lf' is greater then '%0.8Lf'.", le, ge);
    }
}

void _assert_less_equal_str(const char *le, const char *ge, char *file, const int line)
{
    if((ge == NULL && le != NULL) ||
       (ge != NULL && le != NULL && strcmp(le, ge) > 0))
    {
        register_fail(file, line, "Assert less or equal failed: '%s' is greater then '%s'.", le, ge);
    }
}

void _assert_less_equal_wstr(const wchar_t *le, const wchar_t *ge, char *file, const int line)
{
    if((ge == NULL && le != NULL) ||
       (ge != NULL && le != NULL && wcscmp(le, ge) > 0))
    {
        register_fail(file, line, "Assert less or equal failed: '%ls' is greater then '%ls'.", le, ge);
    }
}
void _assert_less_equal(const void *le, const void *ge, char *file, const int line)
{
    register_fail(file, line, "Assert less or equal failed: unsupported data type.");
}

void _assert_less_equal_precision(const long double  le,
                                  const long double  ge,
                                  const long double  epsilon,
                                  char              *file,
                                  const int          line)
{
    if(fabsl(le - ge) > epsilon && le > ge)
    {
        register_fail(file, line, "Assert less or equal failed: '%0.8Lf' is greater then '%0.8Lf'.", le, ge);
    }
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

/**
 * Starts running tests.
 *
 * @param opts Application options (not NULL).
 * @return @code EXIT_SUCCESS @endcode if no tests failed; otherwise @code EXIT_FAILURE @endcode.
 */
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


#endif // TEST_RUNNER

#endif // EZTEST_H
