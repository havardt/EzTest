# EzTest
[![Release](https://img.shields.io/github/v/release/havardt/eztest?label=Release)](https://github.com/havardt/EzTest/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) 

An easy to use unit testing framework written in, and created for, the C language.

## :scroll: Table of contents :scroll:
* [Why use EzTest?](#Why-use-EzTest)
* [Usage](#usage)
* [Getting started](#getting-started)
* [Asserts](#asserts)
* [Runner](#runner)
* [How to contribute](#contribute)
* [License info](#license)

## Why use EzTest?

:heavy_check_mark: Easy to use: single header and runner file.    
:heavy_check_mark: No writing main functions or add-test functions.    
:heavy_check_mark: Skip tests without altering test code.    
:heavy_check_mark: No recompilation to change runner settings/ options.    
:heavy_check_mark: Wide list of asserts.    
:heavy_check_mark: No type prefix or suffix.    

## Usage
Here is an example of a simple unit test written using EzTest:

```C
TEST(MathTests, AddingTwoOnTwoShouldEqualFour)
{
    const int expected = 4;
    const int actual = math_add(2, 2);
    
    ASSERT_ARE_EQUAL(expected, actual); // or ASSERT_EQ(expected, actual); using the shorthand.
}
```
The [asserts](#asserts) are created using the power of the C11 macro ```_Generic```. The user doesn't have to provide some prefix or suffix to represent the data type. 

EzTest also provides the option of setup and teardown functions that run before/ after each test. Using the ```TEST_FULL(suite, test)```, ```SETUP(suite)``` and ```TEARDOWN(suite)``` macros the developer can create a more complex test suite. When using the ```TEST_FULL(suite, name)``` macro the developer *must* implement the ```SETUP()``` and ```TEARDOWN()``` macros.

```C

SETUP(MathTests)
{
    // Code to run before every test in the MathTests suite.
}

TEST_FULL(MathTests, AddingTwoOnTwoShouldEqualFour)
{
    // Test code placed here.
}

TEARDOWN(MathTests)
{
    // This code runs after every test in the MathTests suite.
}

```

See the next section for information on how to get started with EzTest.

## Getting started
EzTest was created to make unit testing quick and easy in C. Therefore there are only three steps to test your code:

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

###### Option 1
To build the EzTest runner, compile using a C11 compatible C compiler by providing the ```runner.c``` source file along with your test files.    
Example compile: ```$ gcc -o ezrunner runner.c <test-files>```     

###### Option 2
Another option is to use the provided [CMake files and BASH build script](build). Before running the build script it is important that you add your test files to the CMakeLists.txt file. The build script leaves a single executable 'eztest', this is the test runner.

#### 3. Run
Run the executable to run your tests. The test runner can take multiple optional arguments to customize your test experience. Learn more about the test runner [here](#runner).

## Asserts

| Macro | Shorthand | Description | Details |
| --- | --- | --- | --- |
| ``` ASSERT_IS_NULL ``` | | Tests whether the provided pointer is null. | [Documentation](doc/asserts.md#Assert-NULL) |
| ``` ASSERT_IS_NOT_NULL ``` | | Tests whether the provided pointer is non-null. | [Documentation](doc/asserts.md#Assert-not-NULL) |
| ``` ASSERT_IS_TRUE ``` | | Tests whether the condition is true. |
| ``` ASSERT_IS_FALSE ``` | | Tests whether the condition is false. |
| ``` ASSERT_ARE_SAME ``` | | Tests whether the two pointers refer to the same memory location. |
| ``` ASSERT_ARE_NOT_SAME ``` | | Tests whether the two pointers refer to different memory locations. |
| ``` ASSERT_IS_NAN ``` | | Tests whether the provided float is NaN. |
| ``` ASSERT_ARE_EQUAL ``` | ``` ASSERT_EQ ``` | Tests whether the two values are equal. |
| ``` ASSERT_ARE_EQUAL_PRECISION ``` | ```ASSERT_EQ_PRECISION```| Tests whether two floating point numbers are equal using a user provided epsilon. |
| ``` ASSERT_ARE_EQUAL_MEM ``` | ``` ASSERT_EQ_MEM ``` | Tests whether the two values are equal by comparing each byte at the given memory locations. |
| ``` ASSERT_ARE_EQUAL_CMP ``` | ``` ASSERT_EQ_CMP ``` | Tests whether the two values are equal by using the passed comparator function. |
| ``` ASSERT_ARE_NOT_EQUAL ``` |``` ASSERT_NE ``` | Tests whether the two values are different. |
| ``` ASSERT_ARE_NOT_EQUAL_PRECISION ``` | ```ASSERT_NE_PRECISION``` | Tests whether two floating point numbers are different using a user provided epsilon. |
| ``` ASSERT_ARE_NOT_EQUAL_MEM ``` | ```ASSERT_NE_MEM``` | Tests whether the two values are different by comparing each byte at the given memory locations. |
| ``` ASSERT_ARE_NOT_EQUAL_CMP ``` | ``` ASSERT_NE_CMP ``` | Tests whether the two values are different by using the passed comparator function. |
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

| Short | Long | Description |    
| --- | --- | --- |
| -v | --version | Prints the version number. |       
| -h | --help | Prints help/ usage information. |           
| -c | --no-color | Don't use any color when printing. |  
| -t | --timer | Display execution time for each test. |
| -q | --quiet | No output. |
| -s | --skip | Skips all tests in the passed list of test suits. The argument for this option should be a comma separated list of case-sensitive test suit names that you want to skip. |


## Contribute
We welcome all contributions, please see the [contribution guidelines](.github/CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) for details.
