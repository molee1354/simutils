# `vector` Functions

Documentation for functions provided in the `vector` module.

## Functions

### `vector new_vector(unsigned int size)`

Creates a new vector of specified size and returns a pointer to it.

- `size`: The size of the vector.

### `void add(vector vec1, vector vec2)`

Adds two vectors element-wise. Raises an error if the dimensions of the vectors do not match.

- `vec1`: The first vector.
- `vec2`: The second vector.

### `void save_vector(vector vec, const char* filename)`

Saves a vector to a binary file.

- `vec`: The vector to be saved.
- `filename`: The name of the file to save the vector to.

### `vector read_vector(const char *filename)`

Reads a vector from a binary file and returns it.

- `filename`: The name of the file containing the vector.

### `void free_vector(vector vec)`

Frees the memory allocated for a vector.

- `vec`: The vector to be freed.

### `void grow_vector(vector* vec, double elem)`

Expands a vector by one element and appends the specified element to it.

- `vec`: A pointer to the vector to be expanded.
- `elem`: The element to be appended to the vector.

### `unsigned int get_length(vector vec)`

Returns the length of a vector.

- `vec`: The vector whose length is to be determined.

### `void print_vector(vector vec)`

Prints the elements of a vector.

- `vec`: The vector to be printed.

## Macros

### `LENGTH(vec)`

Macro to get the length of a vector.

- `vec`: Vector to get the size information of.

### `FROM_VECTOR(from, targ, size)`

Macro to set a new vector from a static C vector.

- `from`: Pointer to a static C array of type `double *`
- `targ`: Vector to be set equal to static vector
- `size`: Number of elements in the vector

