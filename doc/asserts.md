# EzTest documentation - asserts
Each assert is a macro prefixed with ```ASSERT_```. Some of the macros also have a short-hand version. The short-hand 
version has the same underlying functionality.

---

### Assert NULL
Tests whether the provided pointer is ```NULL```. This test uses ```NULL``` as defined by ```stddef.h```.

##### Declaration
```C
ASSERT_IS_NULL(const void *value);
```
##### Parameters
<i>value</i> The pointer to test for NULL. 

---

### Assert not NULL
Tests whether the provided pointer is non-```NULL```. This test uses ```NULL``` as defined by ```stddef.h```.

##### Declaration
```C
ASSERT_IS_NOT_NULL(const void *value);
```
##### Parameters
<i>value</i> The pointer to test for NULL. 

---

### Assert true
Tests whether the passed condition is ```true```. This test uses the boolean definitions in ```stdbool.h```.

##### Declaration
```C
ASSERT_IS_TRUE(const bool condition);
```
##### Parameters
<i>condition</i> The boolean condition to test. 

---

### Assert false
Tests whether the passed condition is ```false```. This test uses the boolean definitions in ```stdbool.h```.

##### Declaration
```C
ASSERT_IS_FALSE(const bool condition);
```
##### Parameters
<i>condition</i> The boolean condition to test. 

---

### Assert same
Tests whether the two pointers refer to the same memory location.

##### Declaration
```C
ASSERT_ARE_SAME(const void *expected, const void *actual);
```
##### Parameters
<i>expected</i> This is the value the tests expects.
<i>actual</i> This is the value produced by the code under test.

---

### Assert not same
Tests whether the two pointers refer to different memory locations.

##### Declaration
```C
ASSERT_ARE_NOT_SAME(const void *unexpected, const void *actual);
```
##### Parameters
<i>unexpected</i> This is the value that should not occur.
<i>actual</i> This is the value produced by the code under test.

---
