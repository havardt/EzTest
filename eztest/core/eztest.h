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

