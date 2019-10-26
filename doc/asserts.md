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
