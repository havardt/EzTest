# EzTest
[![Release](https://img.shields.io/github/v/release/havardt/eztest?label=Release)](https://github.com/havardt/EzTest/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) 

An easy to use unit testing framework written in, and created for, the C language.

## :scroll: Table of contents :scroll:
* [Usage](#usage)
* [Getting started](#getting-started)
* [Asserts](#asserts)
* [Runner](#runner)
* [How to contribute](#contribute)
* [License info](#license)

## Usage
Here is an example of a simple unit test written using EzTest:

```C
TEST(MathTests, AddingTwoOnTwoShouldEqualFour)
{
    const int expected = 4;
    const int actual = math_add(2, 2);
    
    ASSERT_ARE_EQUAL(expected, actual);
}
```
In this example we can see the power of the C11 macro ```_Generic```. The developer doesn't have to provide some prefix or postfix to represent the data type. The correct function for the passed data type, in this case interger, is chosen automagically leaving the developer to focus on the test itself.

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
Before having access to the test macros you need to include the eztest header file: ```#include "core/eztest.h"```.
There are two macros for creating tests: ```TEST(suite, test)``` and ```TEST_FULL(suite, test)```.
The first is for simple unit tests while the second is for tests with a setup and teardown function. Both of these macros take the same arguments.

###### Argument 1: Suite
This is the name of the test suite that the test will be associated with. The name must follow the rules for variable names in C and must be unique through out your tests. The suite name can be used to group multiple tests providing a better overview for the tester.

###### Argument 2: Test
This is the name of the test. The name must follow the rules for variable names in C and must be unique within the test suite.

Example usage of both ```TEST(suite, test)``` and ```TEST_FULL(suite, test)``` can be found in the [usage section](#usage).     

An example project structure while using EzTest could be as following:    
```
project 
│
└───src
|  
|
└───test
    │   mytest.c
    |   mytest2.c
    │   
    └───eztest
```
Using the example above the developer would need to add ```mytest.c``` and ```mytest2.c``` to the eztest CMakeLists.txt file using relative paths. The test files here would also need to include the eztest header which would look like this: ```#include "eztest/core/eztest.h"```.

#### 2. Build     
EzTest uses CMake as its build system. Therefore it is important that you add your test files to the CMakeLists.txt file before building. To build the program we have provided a build script written in BASH. The build script leaves a single executable 'eztest', this is the test runner. Optionally build manually like any other CMake program.  

#### 3. Run
Run the executable to run your tests. The test runner can take multiple optional arguments to customize your test experience. Learn more about the test runner [here](#runner).

## Asserts
Each assert is a macro prefixed with ```ASSERT_```.

| Macro | Description |
| --- | --- |
| ``` ASSERT_IS_NULL ``` | Tests whether the provided pointer is null. |
| ``` ASSERT_IS_NOT_NULL ``` | Tests whether the provided pointer is non-null. |
| ``` ASSERT_IS_TRUE ``` | Tests whether the condition is true. |
| ``` ASSERT_IS_FALSE ``` | Tests whether the condition is false. |
| ``` ASSERT_ARE_SAME ``` | Tests whether the two pointers refer to the same memory location. |
| ``` ASSERT_ARE_NOT_SAME ``` | Tests whether the two pointers refer to different memory locations. |
| ``` ASSERT_IS_NAN ``` | Tests whether the provided float is NaN. |
| ``` ASSERT_ARE_EQUAL ``` | Tests whether the two values are equal. |
| ``` ASSERT_ARE_NOT_EQUAL ``` | Tests whether the two values are different. |
| ``` ASSERT_ARE_EQUAL_MEM ``` | Tests whether the two values are equal by comparing each byte at the given memory locations. |
| ``` ASSERT_ARE_NOT_EQUAL_MEM ``` | Tests whether the two values are different by comparing each byte at the given memory locations. |
| ``` ASSERT_ARE_EQUAL_PRECISION ``` | Tests whether two floating point numbers are equal using a user provided epsilon. |
| ``` ASSERT_ARE_NOT_EQUAL_PRECISION ``` | Tests whether two floating point numbers are different using a user provided epsilon. |

## Runner
EzTest runs the created tests according to the provided application options.

| Short | Long | Description |    
| --- | --- | --- |
| -v | --version | Prints the version number. |       
| -h | --help | Prints help/ usage information. |           
| -c | --no-color | Don't use any color when printing. |        


## Contribute
We welcome all contributions, please see the [contribution guidelines](.github/CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) for details.
