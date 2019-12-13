cmake_minimum_required (VERSION 3.13)
project (eztest C)

set (CMAKE_C_STANDARD 11)

set (eztest_VERSION_MAJOR 2)
set (eztest_VERSION_MINOR 2)
set (eztest_VERSION_PATCH 0)

set(EXECUTABLE_OUTPUT_PATH ..)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
endif()

add_executable(eztest

    runner.c
    eztest.h

    # Add your test files below using relative paths.
    # Example 1: my_test.c
    # Example 2: ../test/math_tests/addition_test.c

    )
