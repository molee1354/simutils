# `simutils`

C header files and definitions for common data structures and math modules.

## Why `simutils`?

`simutils` aims to fix some of the common issues encountered when doing scientific programming in C, with its main strengths in the following areas.

### Implicit Size Information

Whenever one starts using abstract data structures in C like arrays and strings, they immediately run into issues with size. The length of an array or string somehow has to be relayed along with data type itself, which many times adds a lot of unnecessary overhead to the program.

All the data structures provided in `simutils` come with macros that access size information. No more messing around with structs, global variables, or extra function parameters.

```C
// Getting the length of a vector is just a macro call!
int vector_size = LENGTH(some_vector);
```

### 1-Indexed Data Structures

While to some, array-like data structures being 1-indexed might appear to be a bit out-of-style, such a scheme allows for the seamless expression of mathematical models within a programming language. The `vector`, `matrix`, and `tensor` data structures proveided in `simutils` are all 1-indexed to fit this purpose.

### Column-major Matrices

Converting between row-major and column-major matrices might be simple conceptually, but one quickly learns that implementing that "simple" change turns out to be a bit more involved than just switching `i` and `j`.

The `matrix` and `tensor` data structures in `simutils` are column-majored. Memory magic is done in the constructors for both the `matrix` and `tensor` so that the data structures can be both represented and indexed in column-major style.

```C
// Incrementing j will traverse the *column* for both the data structures.
some_matrix[i][j];
some_tensor[i][j][k];
```

A bit more on how these features are achieved can be found in the [documents](/docs/usage.md).

## Installing

Clone this repository with the following command:

```shell
git clone https://github.com/molee1354/simutils.git
```

Change into the new `simutils` directory

```shell
cd simutils
```

Build the library

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

For specific instructions on how to use the different `simutils` modules, refer to the [documentations](docs/usage.md).


## Inspiration for this 
