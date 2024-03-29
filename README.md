<h1 align="center">EzTest</h1>

<div align="center">

[![Release](https://img.shields.io/github/v/release/havardt/eztest?label=Release)](https://github.com/havardt/EzTest/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)    

</div>

<p align="center"><i>An easy to use unit testing framework written in, and created for, the C language.</i></p>

<div align="center">
<img src=".github/assets/example_output.png"/>
</div>



## Table of contents
* [Introduction](#introduction)
* [Getting started](#getting-started)
* [Asserts](#asserts)
* [Runner](#runner)
* [How to contribute](#contribute)
* [License info](#license)

## Introduction 


###### A simple test

```C
TEST(Math, AddingTwoOnTwoShouldEqualFour)
{
    const int expected = 4;
    const int actual = math_add(2, 2);
    
    ASSERT_ARE_EQUAL(expected, actual); // or ASSERT_EQ(expected, actual); using the shorthand.
}
```
The [asserts](#asserts) are created using the power of the C11 macro ```_Generic```. As seen in the above example, this means that the user doesn't have to provide any prefix or suffix to represent the data type. 

###### A full test (Setup and Teardown)
EzTest also provides the option of setup and teardown functions. The setup function is a function that runs before every test within a test suite. The teardown function has similar behaviour, but runs after the test. A test utilizing setup and teardown functions is defined by using the ```TEST_FULL(suite, name)``` macro. A setup and teardown function *must* also be defined for the test suite when using the full test macro. This is done with the ```SETUP(suite)``` and ```TEARDOWN(suite)``` macros.

```C

SETUP(Math)
{
    // Code to run before every test in the Math suite.
}

TEST_FULL(Math, AddingTwoOnTwoShouldEqualFour)
{
    // Test code placed here.
}

TEARDOWN(Math)
{
    // This code runs after every test in the Math suite.
}

```

See the next section for information on how to get started with EzTest.

## Getting started
EzTest was created to make unit testing quick and easy in C. The fastest way to get started with EzTest is by using the [template](https://github.com/havardt/EzTest-Template). The template provides a default project layout for C projects using EzTest. After getting the template or creating your own project structure, you are ready to start.

#### 1. Write tests using the test macros    
Before having access to the test macros you need to include the eztest header file: ```#include "eztest.h"```.
There are two macros for creating tests: ```TEST(suite, test)``` and ```TEST_FULL(suite, test)```.
The first is for simple unit tests while the second is for tests with a setup and teardown function. Both of these macros take the same arguments.

###### Argument 1: Suite
This is the name of the test suite that the test will be associated with. The name must follow the rules for variable names in C and must be unique through out your tests. The suite name can be used to group multiple tests providing a better overview for the tester.

###### Argument 2: Test
This is the name of the test. The name must follow the rules for variable names in C and must be unique within the test suite.

Example usage of both ```TEST(suite, test)``` and ```TEST_FULL(suite, test)``` can be found in the [usage section](#usage).     


#### 2. Build/ Compile

###### Option I: CMake/Template
The perhaps simplest way to build your tests is to use CMake. If you are using CMake and/or the template, then follow the build instructions given [here](https://github.com/havardt/EzTest-Template#hammer_and_wrench-build).

###### Option II: Manual build
To build the EzTest runner, compile using a C11 compatible C compiler by providing the ```runner.c``` source file along with your test files.       
Example compile: ```$ gcc -o ezrunner runner.c <test-files>```     


#### 3. Run
Run the executable to run your tests. The test runner can take multiple optional arguments to customize your test experience. Learn more about the test runner [here](#runner).

## Asserts

| Macro | Shorthand | Description | Details |
| --- | --- | --- | --- |
| ``` ASSERT_IS_NULL ``` | | Tests whether the provided pointer is null. | [Documentation](doc/asserts.md#Assert-NULL) |
| ``` ASSERT_IS_NOT_NULL ``` | | Tests whether the provided pointer is non-null. | [Documentation](doc/asserts.md#Assert-not-NULL) |
| ``` ASSERT_IS_TRUE ``` | | Tests whether the condition is true. | [Documentation](doc/asserts.md#Assert-true) |
| ``` ASSERT_IS_FALSE ``` | | Tests whether the condition is false. | [Documentation](doc/asserts.md#Assert-false) |
| ``` ASSERT_ARE_SAME ``` | | Tests whether the two pointers refer to the same memory location. | [Documentation](doc/asserts.md#Assert-same) |
| ``` ASSERT_ARE_NOT_SAME ``` | | Tests whether the two pointers refer to different memory locations. | [Documentation](doc/asserts.md#Assert-not-same) |
| ``` ASSERT_IS_NAN ``` | | Tests whether the provided float is NaN. | [Documentation](doc/asserts.md#Assert-NaN) |
| ``` ASSERT_ARE_EQUAL ``` | ``` ASSERT_EQ ``` | Tests whether the two values are equal. | [Documentation](doc/asserts.md#Assert-equal) |
| ``` ASSERT_ARE_EQUAL_PRECISION ``` | ```ASSERT_EQ_PRECISION```| Tests whether two floating point numbers are equal using a user provided epsilon. | [Documentation](doc/asserts.md#Assert-equal-precision) |
| ``` ASSERT_ARE_EQUAL_MEM ``` | ``` ASSERT_EQ_MEM ``` | Tests whether the two values are equal by comparing each byte at the given memory locations. | [Documentation](doc/asserts.md#Assert-equal-memory) |
| ``` ASSERT_ARE_EQUAL_CMP ``` | ``` ASSERT_EQ_CMP ``` | Tests whether the two values are equal by using the passed comparator function. | [Documentation](doc/asserts.md#Assert-equal-cmp) |
| ``` ASSERT_ARE_NOT_EQUAL ``` |``` ASSERT_NE ``` | Tests whether the two values are different. | [Documentation](doc/asserts.md#Assert-not-equal) |
| ``` ASSERT_ARE_NOT_EQUAL_PRECISION ``` | ```ASSERT_NE_PRECISION``` | Tests whether two floating point numbers are different using a user provided epsilon. | [Documentation](doc/asserts.md#Assert-not-equal-precision) |
| ``` ASSERT_ARE_NOT_EQUAL_MEM ``` | ```ASSERT_NE_MEM``` | Tests whether the two values are different by comparing each byte at the given memory locations. | [Documentation](doc/asserts.md#Assert-not-equal-mem) |
| ``` ASSERT_ARE_NOT_EQUAL_CMP ``` | ``` ASSERT_NE_CMP ``` | Tests whether the two values are different by using the passed comparator function. | [Documentation](doc/asserts.md#Assert-not-equal-cmp) |
| ``` ASSERT_GREATER ``` | ``` ASSERT_GT ``` | Tests whether the first value is greater than the second value. |
| ``` ASSERT_GREATER_PRECISION ``` | ``` ASSERT_GT_PRECISION ``` | Tests whether the first floating point value is greater than the second floating point value using a user provided epsilon. |
| ``` ASSERT_GREATER_MEM ``` | ```ASSERT_GT_MEM``` | Tests whether the first n bytes at the first memory location is greater than the n first bytes at the second memory location. |
| ``` ASSERT_GREATER_CMP ``` | ``` ASSERT_GT_CMP ``` | Tests whether the first value is greater than the second value by using the passed comparator function. |
| ``` ASSERT_GREATER_EQUAL ``` | ```ASSERT_GE``` | Tests whether the first value is greater than or equal to the second value. |
| ``` ASSERT_GREATER_EQUAL_PRECISION ``` | ```ASSERT_GE_PRECISION``` | Tests whether the first floating point value is greater than or equal to the second floating point value using a user provided epsilon. |
| ``` ASSERT_GREATER_EQUAL_MEM ``` | ```ASSERT_GE_MEM``` | Tests whether the first n bytes at the first memory location is greater than or equal to the n first bytes at the second memory location. |
| ``` ASSERT_GREATER_EQUAL_CMP ``` | ``` ASSERT_GE_CMP ``` | Tests whether the first value is greater than or equal to the second value by using the passed comparator function. |
| ``` ASSERT_LESS ``` | ``` ASSERT_LT ``` | Tests whether the first value is lesser than the second value. |
| ``` ASSERT_LESS_PRECISION ``` | ``` ASSERT_LT_PRECISION ``` | Tests whether the first floating point value is lesser than the second floating point value using a user provided epsilon. |
| ``` ASSERT_LESS_MEM ``` | ```ASSERT_LT_MEM``` | Tests whether the first n bytes at the first memory location is lesser than the n first bytes at the second memory location. |
| ``` ASSERT_LESS_CMP ``` | ``` ASSERT_LT_CMP ``` | Tests whether the first value is lesser than the second value by using the passed comparator function. |
| ``` ASSERT_LESS_EQUAL ``` | ``` ASSERT_LE ``` | Tests whether the first value is lesser than or equal to the second value. |
| ``` ASSERT_LESS_EQUAL_PRECISION ``` | ``` ASSERT_LE_PRECISION ``` | Tests whether the first floating point value is lesser than or equal to the second floating point value using a user provided epsilon. |
| ``` ASSERT_LESS_EQUAL_MEM ``` | ```ASSERT_LE_MEM``` | Tests whether the first n bytes at the first memory location is lesser than or equal to the n first bytes at the second memory location. |
| ``` ASSERT_LESS_EQUAL_CMP ``` | ``` ASSERT_LE_CMP ``` | Tests whether the first value is lesser than or equal to the second value by using the passed comparator function. |

## Runner
The runner is the program that executes the tests.

#### Exit code
The exit code is EXIT_SUCCESS (0) if all tests passed and EXIT_FAILURE (non-zero) if one or more tests failed.

#### Options
| Short | Long&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | Description |    
| --- | --- | --- |
| -v | --version | Prints the version number. |       
| -h | --help | Prints help/ usage information. |           
| -c | --no-color | Don't use any color when printing. |  
| -t | --timer | Display execution time for each test. |
| -q | --quiet | No output. |
| -s | --skip | Skips all tests in the passed list of test suits. The argument for this option should be a comma separated list of case-sensitive test suit names that you want to skip. |
| -f | --SIGSEGV | Segmentation fault is handled like other test failures. |


## Contribute
We welcome all contributions, please see the [contribution guidelines](.github/CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) for details.
