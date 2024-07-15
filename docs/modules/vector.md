# `vector(T)` Functions

Documentation for functions provided in the `vector` module.

## Macros

While the vector "functions" listed here are really macros, they will be listed as functions to help with understading.

### `vector(T) new_vector(T, unsigned int size)`

Creates a new vector of specified type and size and returns a pointer of type `T` (`T*`).

- `T`: The type of elements what the vector will hold
- `size`: The size of the vector.

### `void free_vector(vector(T) vec)`

Frees the memory allocated for a vector.

- `vec`: The pointer to the vector to be freed.

### `void grow_vector(vector(T)* vec, T elem)`

Adds an element to the end of a vector.

- `vec`: A **pointer** to the vector to be expanded.
- `elem`: The element of type `T` to be appended to the vector.

### `void print_vector(vector(T) vec)`

Prints the elements of a vector.

- `vec`: The vector to be printed.

### `int LENGTH(vector(T) vec)`

Macro to get the length of a vector.

- `vec`: Vector to get the size information of.

### `void FROM_VECTOR(T* from, vector(T) targ, int size)`

Macro to set a new vector from a static C vector.

- `from`: Pointer to a static C array of type `double *`
- `targ`: Vector to be set equal to static vector
- `size`: Number of elements in the vector

