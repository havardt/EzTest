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
