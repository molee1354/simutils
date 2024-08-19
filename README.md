# `simutils`

A minimal set of C headers and functions to reduce the memory-management overhead involved in scientific programming and mathematical modeling.

## Why `simutils`?

`simutils` aims to fix some of the common issues encountered in using C for scientific programming, with its main strengths in the following areas.

### Implicit Size Information

Whenever one starts using more abstract data structures in C like arrays and strings, they immediately run into issues with knowing size. We want the length of an array or string somehow has to be relayed along with data type itself, which many times adds a lot of unnecessary overhead to the program.

All the data structures provided in `simutils` have macros that access size information. No more messing around with structs, global variables, or extra function parameters.

```C
// Getting the length of a vector is just a macro call!
int vector_size = LENGTH(some_vector);

// Getting the rows/columns of a matrix are also just macro calls.
int num_rows = ROWS(some_matrix);
int num_cols = COLS(some_matrix);
```

This is done by setting apart a specific location in memory that includes information about the size of the data structure. Since this location has a fixed memory offset with respect to the pointer that is being used, it can easily be accessed through macros and/or functions.

### User-defined Element Types

Types in C are usually defined at compile time. This makes it so that abstract data structures (like arrays, matrices, and vectors) have to be defined for *each type* that might be used. `simutils` solved this by using function-like macros that take in type definitions as arguments, so that the type of elements contained in the abstract data structes can be defined at *runtime* rather than at *compile time*.

```C
vector(int) my_vector = new_vector(int, 3); // vector of type 'int'
vector(long double) my_vector2 = new_matrix(long double, 4); // vector of type 'long double'

// matrix of vectors of type 'my_type'
typedef float my_type;
matrix(vector(my_type)) my_matrix = new_matrix(vector(my_type), 3);
```

### 1-Indexed Data Structures

While to some, array-like data structures being 1-indexed might appear to be a bit out-of-style, such a scheme allows for the seamless expression of mathematical notations within programming. The `vector`, and `matrix` data structures proveided in `simutils` are all 1-indexed to fit this purpose.

### Modifiable Row-major, Column-major Behavior

Converting between row-major and column-major matrices might be simple conceptually, but one quickly learns that implementing that "simple" change turns out to be a bit more involved than just switching `i` and `j`.

```C
// The 'i' index corresponds to the 'row' index in a row-major indexing scheme
some_matrix[i][j];
some_tensor[i][j][k];

// The 'i' index corresponds to the 'column' index in a column-major indexing scheme
some_matrix[i][j];
some_tensor[i][j][k];
```

The `matrix` and `matrix3` data structures in `simutils` are column-majored. Memory magic is done in the constructors for both the `matrix` and `tensor` so that the data structures can be both represented and indexed in column-major style.

To solve this problem, the row-major/column-major indexing behavior for `matrix` and `matrix3` data structues provided in `simutils` is ***modifiable***. While the default behavior for matrix indexing is *column-major* (like in FORTRAN, MATLAB, or Julia), a simple preprocessor macro can be defined to use *row-major* indexing (like in C/C++, Java, or Python).

This will simply give you a *column-major* matrix with 3 columns and 4 rows:

```C
#include "simutil/matrix.h"
int main(char** argv, int argc) {
    matrix(int) my_matrix = new_matrix(int, 3, 4);
    return 0;
}
```

And adding the `#define ROW_MAJOR` preprocessor directive will give you a *row-major* matrix with 3 columns and 4 rows. Note that `#define ROW_MAJOR` has to come *before* the `#include "simutil/matrix.h"` statement:

```C
#define ROW_MAJOR // must come before the '#include'
#include "simutil/matrix.h"
int main(char** argv, int argc) {
    matrix(int) my_matrix = new_matrix(int, 3, 4); // 'my_matrix' is now row-majored
    return 0;
}
```

A bit more on how these features are used can be found in the [documents](/docs/usage.md).

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
## Usage

To use the data structures and math modules provided by `simutils`, simply include the `simutil/*.h` with the specified header file in your program's include directive.

Once `simutils` is properly installed, make sure to add the `-lsimutils` flag in the linking stage of your code.

For more specific instructions on how to use the different `simutils` modules, refer to the [documentations](docs/usage.md).

