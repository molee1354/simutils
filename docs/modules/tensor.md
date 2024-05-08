# `tensor` Functions

Documentation for functions provided in the `tensor` module

## Functions

### `tensor new_tensor(unsigned int nrows, unsigned int ncols)`

Creates a new tensor of specified size and returns a pointer to it.

- `nrows`: The number of rows in the tensor.
- `ncols`: The number of columns in the tensor.

### `void save_tensor(tensor ten, const char *filename)`

Saves a tensor to a binary file.

- `ten`: The tensor to be saved.
- `filename`: The name of the file to save the tensor to.

### `tensor read_tensor(const char *filename)`

Reads a tensor from a binary file and returns it.

- `filename`: The name of the file containing the tensor.

### `void print_tensor(tensor ten)`

Prints the elements of a tensor.

- `ten`: The tensor to be printed.

### `void free_tensor(tensor ten)`

Frees the memory allocated for a tensor.

- `ten`: The tensor to be freed.

## Macros

### `DIM1(ten)`

Returns the number of columns in a tensor.

- `ten`: The tensor whose number of rows is to be determined.

### `DIM2(ten)`

Returns the number of rows in a tensor.

### `DIM3(ten)`

Returns the number of "pages" in a tensor.

- `ten`: The tensor whose number of columns is to be determined.

