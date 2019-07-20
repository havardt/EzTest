cmake_minimum_required (VERSION 3.13)
project (eztest C)

set (CMAKE_C_STANDARD 11)

set (eztest_VERSION_MAJOR 0)
set (eztest_VERSION_MINOR 0)
set (eztest_VERSION_PATCH 1)

set(EXECUTABLE_OUTPUT_PATH ..)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

#include_directories("")

add_executable(eztest 
    
    # root
    main.c
    options.h
)  
