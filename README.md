# `simutils`

A minimal set of C headers and functions to reduce the memory-management
overhead involved in creating data structures used in scientific and
mathematical modeling.

The functionality was inspired by [*Numerical Recipes in C* 2nd Edition](https://numerical.recipes/),
and [cvec](https://github.com/ephf/libcvec).

Below is a simple 2D simulation created using `simutils`.

![simutil_demo](./demos/MAC_visualization.gif)

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
int my_len = LENGTH(vec1)

// Accessing the number of rows/columns in a matrix
int rows = ROWS(mat1);
int cols = COLS(mat1);
```

### 1-Indexed Data Structures

A lot of programming languages designed with scientific/mathematical
applications in mind have 1-indexed vectors and matrices. `simutils` follows
this convention.

```C
vec1[1] = 1; // setting the first element of a vector to '1'
mat1[1][1] = 1; // setting the first element of a matrix to '1'
```

The final index of a `vector` or `matrix` therefore is *the number of elements
in that dimension*.

```C
vec1[LENGTH(vec1)] = -1; // setting the final element of a vector to '-1'
mat1[COLS(mat1)][ROWS(mat1)] = -1; // setting the final element of a matrix to '-1'
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

A bit more on how these features are used can be found in the [documents](/docs/usage.md), and some code examples that show how `simutils` can be used are in the `examples/` directory.

## Installing

Clone this repository with the following command:

```shell
git clone https://github.com/molee1354/simutils.git
```

Navigate into the new `simutils` directory

```shell
cd simutils
```

Compile the code

```shell
make
```

Install the library by copying the compiled shared-object `libsimutils.so` into `/usr/lib/`, and the header files to `/usr/include/`. This step will require elevated privileges as it runs `sudo` commands.

```shell
make install
```

You can also specify the path to the `simutils` directory using the `-I` flag if
you wish to use `simutils` without having to add them globally.

```shell
... -I/path/to/simutils
```

## Usage

To use the data structures and math modules provided by `simutils`, simply include the `simutil/*.h` with the specified header file in your program's include directive.

Once `simutils` is properly installed, make sure to add the `-lsimutils` flag in the linking stage of your code.

For more specific instructions on how to use the different `simutils` modules, refer to the [documentations](docs/usage.md).

