# `matrix3(T)` Functions

Documentation for functions provided in the `matrix3` module

## Macros

While the matrix3 "functions" listed here are really macros, they will be listed as functions to help with understading.

### `matrix3(T) new_matrix3(T, unsigned int nrows, unsigned int ncols)`

Creates a new matrix3 of specified type and size and returns a pointer of type `T` (`T**`).

- `T`: The type of elements what the matrix will hold
- `nrows`: The number of rows in the matrix3.
- `ncols`: The number of columns in the matrix3.

### `void print_matrix3(matrix3(T) mat)`

Prints the elements of a matrix3.

- `mat`: The matrix3 to be printed.

### `void free_matrix3(matrix3(T) mat)`

Frees the memory allocated for a matrix3.

- `mat`: The matrix3 to be freed.

### `int DIM1(matrix3(T) mat)`

Returns the number of columns in a matrix3.

- `mat`: The matrix3 whose number of rows is to be determined.

### `int DIM2(matrix3(T) mat)`

Returns the number of rows in a matrix3.

### `int DIM3(matrix3(T) mat)`

Returns the number of "pages" in a matrix3.

- `mat`: The matrix3 whose number of columns is to be determined.

