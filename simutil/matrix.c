#include "matrix.h"
#include "error.h"
#include <stdio.h>

#define CHECK(p)                                                               \
    if (!p) {                                                                  \
        raise_error(SIMUTIL_ALLOCATE_ERROR,                                    \
                    "[%s:%d] NULL allocation for matrix!\n", __FILE__,         \
                    __LINE__);                                                 \
        exit(EXIT_FAILURE);                                                    \
    }

void* __init_matrix(size_t size, size_t elem_size, size_t ncols, size_t nrows) {
    void* mat_start = calloc(1, size);
    CHECK(mat_start);
    *((size_t*)mat_start + 0) = ncols;
    *((size_t*)mat_start + 1) = nrows;
    char** out = (char**)((char*)mat_start + MATRIX_SIZE_BYTE);
    char* data_start = (char*)(out + (ncols + 1));
    for (size_t i = 1; i <= ncols; i++)
        out[i] = data_start + i * (nrows + 1) * elem_size;
    return (void*)out;
}

#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_m(FILE* fp, type mat) {                               \
        const int nrow = ROWS(mat);                                            \
        const int ncol = COLS(mat);                                            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = 1; j <= nrow; j++) {                                          \
            fprintf(fp, "[");                                                  \
            for (i = 1; i <= ncol; i++) {                                      \
                if (i != ncol) {                                               \
                    fprintf(fp, fmt, mat[i][j]);                               \
                    fprintf(fp, ", ");                                         \
                } else                                                         \
                    fprintf(fp, fmt, mat[i][j]);                               \
            }                                                                  \
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "]\n");                                                    \
    }

// printing floating-point numbers
PRINT_FUNC(_float, matrix(float), "%6.3f")
PRINT_FUNC(_double, matrix(double), "%6.3f")
PRINT_FUNC(_long_double, matrix(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, matrix(char), "%c")
PRINT_FUNC(_short, matrix(short), "%3hd")
PRINT_FUNC(_int, matrix(int), "%3d")
PRINT_FUNC(_uint, matrix(unsigned int), "%3u")
PRINT_FUNC(_long, matrix(long), "%3ld")
PRINT_FUNC(_ulong, matrix(unsigned long), "%3lu")
