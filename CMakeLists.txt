cmake_minimum_required (VERSION 2.8)

project (eztest)

set (CMAKE_C_STANDARD 11)

set (eztest_VERSION_MAJOR 3)
set (eztest_VERSION_MINOR 0)
set (eztest_VERSION_PATCH 0)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "-Wall")
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()

add_executable(eztest eztest/runner.c eztest/eztest.h)
