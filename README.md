# EzTest
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) 

An easy to use unit testing framework written in, and created for, the C language.

## :scroll: Table of contents :scroll:
* [Usage](#Usage)
* [Getting started](#GettingStarted)
* [Asserts](#Asserts)
* [How to contribute](#Contribute)
* [License info](#License)

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

## Asserts
Each assert is a macro prefixed with ```ASSERT_```.

## Contribute
We welcome all contributions, please see the [contribution guidelines](.github/CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) for details.
