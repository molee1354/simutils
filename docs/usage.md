# Basics

Basic synopsis for `simutils` modules.

## `simutil/vector.h`

### The `vector(T)` Data Structure

The `vector(T)` is simply a macro for a single pointer of type `T`.

```C
#define vector(T) T*
```

The advantage of using a `vector(T)` instead of a single pointer `T*` is that a `vector(T)` carries along with it the size information (like how you could with a `struct`), while still preserving the ability to be directly indexed with square brackets. The `vector(T)` is also *1-indexed*, which makes the expression of mathematical models and operations very intuitive.

### Usage

#### Including the Header File

To use the `vector` module, simply include the `simutil/vector.h` header in the include directive of your program.

```c
#include "simutil/vector.h"
```

#### The `vector` Constructor and Destructor

Memory for the `vector` is allocated on the heap, so every `vector` constructor needs to be paired with a complementary destructor.

##### Constructor

Vectors can be created using a macro call that defines its type and size.

```C
// creating a new vector with 3 'double' elements
vector(double) vec = new_vector(double, 3);

vec[1] = 1.0; // setting "first" element to 1.0
vec[3] = 1.0; // setting "last" element to 1.0
```

The `vector` constructor returns a new vector with specified size and type that is zero-initialized. The elements can be accessed and modified using square brackets.

##### Destructor

Freeing the memory allocated for the vector is also a simple macro call. The internal details are all managed in the macro.

```C
// freeing the memory allocated for a vector
free_vector(vec);
```

Other `vector` functions are listed in the [vector modules](./modules/vector.md) document.

## The `matrix(T)` Data Structure

The `matrix(T)`, like the `vector(T)`, is simply a macro a double pointer of type `T`.

```C
#define matrix(T) T**
```

Similar to `vector`, the advantage of using a `matrix(T)` instead of a `T**` is that the `matrix` carries along with it the size information, while still preserving the ability to be directly indexed with double square brackets.

The `matrix` in `simutils` is both *column-major* and *1-indexed*, similar to matrix implentations in programming languages like Matlab, Fortran, and Julia.

### Usage

#### Including the Header File

To use the `matrix` module, simply include the `simutil/matrix.h` header in the include directive of your program.

```c
#include "simutil/matrix.h"
```

#### The `matrix` Constructor and Destructor

Memory for the `matrix` is allocated on the heap, so every `matrix` "constructor" needs to be paired with a complementary destructor.

##### Constructor

```C
// creating a new matrix with integer elements with 3 columns and 4 rows
matrix(int) mat = new_matrix(int, 3, 4);

mat[1][1] = 1; // setting "first" element to 1
mat[3][4] = 1; // setting "last" element to 1
```

The `matrix` constructor is a macro that returns a new matrix with specified type and size that is zero-initialized. The elements can be modified and accessed using square brackets.

##### Destructor

```C
// freeing the memory allocated for a vector
free_matrix(mat);
```

Other `matrix` functions are listed in the [matrix modules](./modules/matrix.md) document.


## The `matrix3` Data Structure

The `matrix3(T)` is a macro for a triple pointer of type `T`, which provides an easy way to represent a 3-D matrix.

```C
#define matrix3(T) T***
```

Similar to `vector` and `matrix`, the `matrix3` carries along with it the size information, while still preserving the ability to be directly indexed with double square brackets.

The `matrix3` in `simutils` are *column-major* and *1-indexed*, similar to indexing schemes in programming languages like Matlab, Fortran, and Julia.

### Usage

#### Including the Header File

To use the data structures and math operations provided by the `matrix3` module, simply include the `simutil/matrix3.h` header in the include directive of your program.

```c
#include "simutil/matrix3.h"
```

#### The `matrix3` Constructor and Destructor

Memory for the `matrix3` is allocated on the heap, so every `matrix3` constructor needs to be paired with a complementary destructor.

##### Constructor

```C
// creating a new 3-D matrix of type 'float' with 3 columns, 4 rows, 5 "pages"
matrix3(float) mat = new_matrix3(float, 3,4,5);

ten[1][1][1] = 1.0; // setting "first" element to 1.0
ten[3][4][5] = 1.0; // setting "last" element to 1.0
```

The `matrix3` constructor returns a new 3-D matrix with specified size that is zero-initialized. The elements can be modified and accessed using square brackets.

##### Destructor

```C
// freeing the memory allocated for a vector
free_tensor(mat);
```

Other `matrix3` functions are listed in the [3-D matrix modules](./modules/matrix3.md) document.
