# Basics

Basic synopsis for `simutils` modules.

## `simutil/vector.h`

### The `vector` Data Structure

The `vector` is simply a type alias for a single `double` pointer (`double*`).

```C
typedef double* vector;
```

The advantage of using a `vector` instead of a `double*` is that a `vector` carries along with it the size information (like how you could with a `struct`), while still preserving the ability to be directly indexed with square brackets. The `vector` is also *1-indexed*, which makes the expression of mathematical models and operations very intuitive.

### Usage

#### Including the Header File

To use the data structures and math operations provided by the `vector` module, simply include the `simutil/vector.h` header in the include directive of your program.

```c
#include "simutil/vector.h"
```

#### The `vector` Constructor and Destructor

Memory for the `vector` is allocated on the heap, so every `vector` "constructor" needs to be paired with a complementary destructor.

##### Constructor

```C
// creating a new vector with 3 elements
vector vec = new_vector(3);

vec[1] = 1.0; // setting "first" element to 1.0
vec[3] = 1.0; // setting "last" element to 1.0
```

The `vector` constructor returns a new vector with specified size that is zero-initialized. The elements can be modified using square brackets.

##### Destructor

```C
// freeing the memory allocated for a vector
free_vector(vec);
```

Other `vector` functions are listed in the [vector modules](./modules/vector.md) document.

## The `matrix` Data Structure

The `matrix` is simply a type alias for a double `double` pointer (`double**`).

```C
typedef double** matrix;
```

Similar to the `vector`, the advantage of using a `matrix` instead of a `double**` is that a `matrix` carries along with it the size information, while still preserving the ability to be directly indexed with double square brackets.

The `matrix` in `simutils` are *column-major* and *1-indexed*, similar to matrix implentations in programming languages like Matlab, Fortran, and Julia.

### Usage

#### Including the Header File

To use the data structures and math operations provided by the `matrix` module, simply include the `simutil/matrix.h` header in the include directive of your program.

```c
#include "simutil/matrix.h"
```

#### The `matrix` Constructor and Destructor

Memory for the `matrix` is allocated on the heap, so every `matrix` "constructor" needs to be paired with a complementary destructor.

##### Constructor

```C
// creating a new vector with 3 columns and 4 rows
matrix mat = new_matrix(3,4);

mat[1][1] = 1.0; // setting "first" element to 1.0
mat[3][4] = 1.0; // setting "last" element to 1.0
```

The `matrix` constructor returns a new matrix with specified size that is zero-initialized. The elements can be modified using square brackets.

##### Destructor

```C
// freeing the memory allocated for a vector
free_matrix(mat);
```

Other `matrix` functions are listed in the [matrix modules](./modules/matrix.md) document.


## The `tensor` Data Structure

The `tensor` is simply a type alias for a *triple* `double` pointer (`double***`). While the mathematical definition of a tensor is much more general, in `simutils` it's just a 3D matrix.

```C
typedef double*** tensor;
```

Similar to the `vector` and `matrix`, the `tensor` carries along with it the size information, while still preserving the ability to be directly indexed with double square brackets.

The `tensor` in `simutils` are *column-major* and *1-indexed*, similar to tensor implentations in programming languages like Matlab, Fortran, and Julia.

### Usage

#### Including the Header File

To use the data structures and math operations provided by the `tensor` module, simply include the `simutil/tensor.h` header in the include directive of your program.

```c
#include "simutil/tensor.h"
```

#### The `tensor` Constructor and Destructor

Memory for the `tensor` is allocated on the heap, so every `tensor` "constructor" needs to be paired with a complementary destructor.

##### Constructor

```C
// creating a new vector with 3 columns, 4 rows, 5 "pages"
tensor mat = new_tensor(3,4,5);

ten[1][1][1] = 1.0; // setting "first" element to 1.0
ten[3][4][5] = 1.0; // setting "last" element to 1.0
```

The `tensor` constructor returns a new tensor with specified size that is zero-initialized. The elements can be modified using square brackets.

##### Destructor

```C
// freeing the memory allocated for a vector
free_tensor(mat);
```

Other `tensor` functions are listed in the [tensor modules](./modules/tensor.md) document.
