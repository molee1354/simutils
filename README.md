# `simutils`

C header files and definitions for common data structures and math modules.

## Usage

To use the data structures and math modules provided by `simutils`, simply include the `simutils.h` header file in your program.

```c
#include "simutils.h"
```

## Vectors and Matrices

`simutils` comes with a two very helpful data structures, `vector` and `matrix`. The advantage of using `vector` and `matrix` instead of single `int*` or double `float**` pointers is that both `vector` and `matrix` carry along their size information (like how you could with a `struct`), while still preserving the ability to be directly indexed (like a array defined from a pointer).

### Using `vector`

Below is an example usage of the `vector` data structure. The `vector` data structure holds elements of type `double`.

```c
#include "simutils.h"

int main(int argc, char** argv) {
    // create a 4-element vector of zeros
    vector vec = make_vector(4);

    // set the first and last elements to 1
    vec[0] = 1.;
    vec[3] = 1.;

    // print the vector to stdout
    print_vector(vec);

    // get the vector's size
    int size = LENGTH(vec);

    // add an element to the vector
    grow_vector(&vec, 1.);

    // free the memory allocated for the vector
    free_vector(vec);
    return 0;
}
```

### Using `matrix`

Below is an example usage of the `matrix` data structure. The `matrix` data structure holds elements of type `double`.

```c
#include "simutils.h"

int main(int argc, char** argv) {
    // create a zero matrix with 4 rows and 3 columns.
    matrix mat = make_matrix(4, 3);

    // set some elements to 1
    mat[0][1] = 1.;
    mat[1][1] = 1.;

    // print the matrix to stdout
    print_matrix(mat);

    // get the matrix's size
    int rows = ROWS(mat);
    int cols = COLS(mat);

    // free the memory allocated for the matrix
    free_matrix(mat);
    return 0;
}
```

