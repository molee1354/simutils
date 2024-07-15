# `matrix(T)` Functions

Documentation for functions provided in the `matrix` module

## Macros

While the matrix "functions" listed here are really macros, they will be listed as functions to help with understading.

### `matrix(T) new_matrix(T, unsigned int ncols, unsigned int nrows)`

Creates a new matrix of specified type and size and returns a pointer of type `T` (`T**`).

- `T`: The type of elements what the matrix will hold
- `ncols`: The number of columns in the matrix.
- `nrows`: The number of rows in the matrix.

### `void print_matrix(matrix(T) mat)`

Prints the elements of a matrix.

- `mat`: The matrix to be printed.

### `void free_matrix(matrix(T) mat)`

Frees the memory allocated for a matrix.

- `mat`: The matrix to be freed.

### `int COLS(matrix(T) mat)`

Returns the number of columns in a matrix.

- `mat`: The matrix whose number of columns is to be determined.

### `int ROWS(matrix(T) mat)`

Returns the number of rows in a matrix.

- `mat`: The matrix whose number of rows is to be determined.

### `matrix(T) FROM_MATRIX(T* from, matrix(T) targ, unsigned int col, unsigned int row)`

Creates a matrix from a given static 2D array (`T[][]`)

- `from`: The static matrix from which the elements will be copied from.
- `targ`: The matrix to which the elements will be copied to.
- `row`: The row index.
- `col`: The column index.

