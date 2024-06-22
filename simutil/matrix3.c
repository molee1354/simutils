#include "matrix3.h"

#define CHECK(p)                                                               \
    do {                                                                       \
        if (!p) {                                                              \
            raise_error(SIMUTIL_ALLOCATE_ERROR,                                \
                        "NULL allocation for matrix!\n");                      \
        }                                                                      \
    } while (0)

void* __init_matrix3(size_t size, size_t elem_size, size_t ncols, size_t nrows,
                     size_t ndeps) {
    // Allocate memory
    void* mat_start = calloc(1, size);
    CHECK(mat_start);

    // Store dimensions at the beginning of the allocated memory
    size_t* meta_data = (size_t*)mat_start;
    meta_data[0] = ncols;
    meta_data[1] = nrows;
    meta_data[2] = ndeps;

    // Pointer to the array of pointers to rows
    char*** out = (char***)((char*)mat_start + MATRIX3_SIZE_BYTE);
    CHECK(out);

    // Pointer to the array of pointers to depth
    char** row_pointers = (char**)(out + (ncols + 1));
    CHECK(row_pointers);

    // Pointer to the actual data start
    char* data_start = (char*)(row_pointers + ((ncols + 1) * (nrows + 1)));
    CHECK(data_start);

    // Initialize the array of pointers
    for (size_t i = 1; i <= ncols; i++) {
        out[i] = row_pointers + i * nrows;
        for (size_t j = 1; j <= nrows; j++) {
            out[i][j] =
                data_start +
                (i * (nrows + 1) * (ndeps + 1) + j * (ndeps + 1)) * elem_size;
            CHECK(out[i][j]);
        }
    }

    return (void*)out;
}
