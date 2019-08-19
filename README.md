# EzTest
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
TEST(math_tests, adding_two_on_two_should_equal_four)
{
    const int expected = 4;
    const int actual = math_add(2, 2);
    
    ASSERT_ARE_EQUAL(expected, actual);
}
```
In this example we can see the power of the C11 macro ```_Generic```. The developer doesn't have to provide some prefix or postfix to represent the data type. The correct function for the passed data type, in this case interger, is chosen automagically leaving the developer to focus on the test itself.

See the next section for information on how to get started with EzTest.

## Getting started
EzTest was created to make unit testing quick and easy in C. Therefore there are only three steps to test your code:

#### 1. Write tests using the test macros    
There are two macros for creating tests: ```TEST(suite, test)``` and ```TEST_FULL(suite, test)```.
The first is for simple unit tests while the second is for tests with a setup and teardown function. Both of these macros take the same arguments.

###### Argument 1: Suite
This is the name of the test suite that the test will be associated with. The name must follow the rules for variable names in C and must be unique through out your tests. The suite name can be used to group multiple tests providing a better overview for the tester.

###### Argument 2: Test
This is the name of the test. The name must follow the rules for variable names in C and must be unique within the test suite.

Example usage of both ```TEST(suite, test)``` and ```TEST_FULL(suite, test)``` can be found in the [usage section](#usage). 

#### 2. Build     
EzTest uses CMake as its build system. Therefore it is important that you add your test files to the CMakeLists.txt file before building. To build the program we have provided a build script written in BASH. The build script leaves a single executable 'eztest', this is the test runner. Optionally build manually like any other CMake program.  

#### 3. Run
Run the executable to run your tests. The test runner can take multiple optional arguments to customize your test experience. Learn more about the test runner [here](#runner).

## Asserts
Each assert is a macro prefixed with ```ASSERT_```.

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
