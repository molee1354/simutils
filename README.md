# `simutils`

C header files and definitions for common data structures and math modules.

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

