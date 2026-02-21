# `simutils`

A minimal set of C headers and functions to reduce the memory-management
overhead involved in creating data structures used in scientific and
mathematical modeling.

The functionality was inspired by [*Numerical Recipes in C* 2nd Edition](https://numerical.recipes/),
and [cvec](https://github.com/ephf/libcvec).

## What is `simutils`?

You can think of `simutils` simply as a set of header files that provide helpful
macros and functions for creating and using vectors and matrices in C.

## Why use `simutils`?

### Creation of Objects with Minimal Overhead

With `simutils`, you can easily create common data structures like vectors and
matrices, and define them with *any element type*.

For `vector` types

```C
// Define size and type in instantiation
vector(int) vec1 = new_vector(int, 5);
vector(double) vec2 = new_vector(double, 1000);

// Define custom types
typedef float MyType;
vector(MyType) vec3 = new_vector(MyType, 10);

// Nested data structures
vector(vector(char)) vec4 = new_vector(vector(char), 3);
```

Similarly for `matrix` types

```C
// A matrix of integers with 4 columns and 3 rows
matrix(int) mat1 = new_matrix(int, 4,3);

// A matrix of doubles with 2 columns and 7 rows
matrix(double) mat2 = new_matrix(double, 2,7);

// Define custom types
typedef float MyType;
matrix(MyType) mat3 = new_matrix(MyType, 10,10);

// Nested data structures
matrix(matrix(char)) mat4 = new_matrix(matrix(char), 3, 8);
```

### Implicit Size Information

Getting the size of data structures like vectors and matrices is usually not
very straightforward in C. `simutils` fixes this by including the size
information with the data structure itself, but not as in `structs`.

```C
// Accessing the number of elements in a vector
int my_len = size_matrix(vec1)

// Accessing the number of rows/columns in a matrix
int rows = nrow_matrix(mat1);
int cols = ncol_matrix(mat1);
```

### Modifiable 1 and 0-indexed Data Structures

One of the key features of `simutils` data structures is that they have fully
modifiable behavior, based on the user's needs. By default the vectors and
matrices are 0-indexed, but this can be modified by defining a preprocessor
macro at the start of the program.

```C
// Default behavior is 0-index
#include "simutil/vector.h"
#include "simutil/matrix.h"

vec1[0] = 1; // setting the first element of a vector to '1'
mat1[0][0] = 1; // setting the first element of a matrix to '1'
```

Using the `SIMUTIL_VECTOR_START_IDX_1`, you can modify the indexing behavior of
vectors (and matrices with its corresponding macro definition).

```C
// Define macro for 1-indexed vectors and matrices
#define SIMUTIL_VECTOR_START_IDX_1
#define SIMUTIL_MATRIX_START_IDX_1
#include "simutil/vector.h"
#include "simutil/matrix.h"

vec1[1] = 1; // setting the first element of a vector to '1'
mat1[1][1] = 1; // setting the first element of a matrix to '1'
```

### Modifiable Row-major / Column-major Matrices
The `matrix` data structure provided by `simutils` gives the user the ability to
choose between *row-major* and *column-major* indexing according to their needs.

By default, `matrix` matrices are row-majored

```C
#include "simutil/matrix.h"
int main(char** argv, int argc) {
    matrix(int) my_matrix = new_matrix(int, 3, 4);
    return 0;
}
```

but adding the `#define SIMUTIL_COL_MAJOR` preprocessor directive will give you
a *column-major* matrix with 3 columns and 4 rows. Note that
`#define SIMUTIL_COL_MAJOR` has to come *before* the
`#include "simutil/matrix.h"` statement:

```C
#define SIMUTIL_COL_MAJOR // MUST come before the '#include "simutil/matrix.h' statement
#include "simutil/matrix.h"
int main(char** argv, int argc) {
    matrix(int) my_matrix = new_matrix(int, 3, 4); // 'my_matrix' is now column-majored
    return 0;
}
```

### Easy-`free`

Freeing the memory allocated for vectors and matrices is just a simple macro
call as well.

```C
// All the memory handling is done within the macro
free_vector(vec1);
free_vector(vec2);
free_vector(vec3);
free_vector(vec4);

free_matrix(mat1);
free_matrix(mat2);
free_matrix(mat3);
free_matrix(mat4);
```

## Usage

Simply copy the header files in the `simutil` directory into the project that
you are working on, and make sure to define the implementation macro before
including the header files.

```C
#define SIMUTIL_VECTOR_IMPLEMENTATION
#include "simutil/vector.h"
```

Also, make sure to add any behavior modifier macros *before* the header files
are included. By default, both vectors and matrices will be *0-indexed*, and
matrices will be *row-major* (like in C/C++).

