# `matrix` Functions

Documentation for functions provided in the `matrix` module

## Functions

### `matrix new_matrix(unsigned int nrows, unsigned int ncols)`

Creates a new matrix of specified size and returns a pointer to it.

- `nrows`: The number of rows in the matrix.
- `ncols`: The number of columns in the matrix.

### `void save_matrix(matrix mat, const char *filename)`

Saves a matrix to a binary file.

- `mat`: The matrix to be saved.
- `filename`: The name of the file to save the matrix to.

### `matrix read_matrix(const char *filename)`

Reads a matrix from a binary file and returns it.

- `filename`: The name of the file containing the matrix.

### `void print_matrix(matrix mat)`

Prints the elements of a matrix.

- `mat`: The matrix to be printed.

### `void free_matrix(matrix mat)`

Frees the memory allocated for a matrix.

- `mat`: The matrix to be freed.

## Macros

### `ROWS(mat)`

Returns the number of rows in a matrix.

- `mat`: The matrix whose number of rows is to be determined.

### `COLS(mat)`

Returns the number of columns in a matrix.

- `mat`: The matrix whose number of columns is to be determined.

### `FROM_MATRIX(from, targ, row, col)`

Creates a matrix from a given static 2D array (`double[][]`)

- `from`: The static matrix from which the elements will be copied from.
- `targ`: The matrix to which the elements will be copied to.
- `row`: The row index.
- `col`: The column index.

