#ifndef SIMUTIL_MATRIX_BASE_H
#define SIMUTIL_MATRIX_BASE_H

#include "error.h"
#include "simutil_includes.h"

/* Type alias for matrix */
#define matrix(T) T**

/* Metadata memory size */
#define MATRIX_SIZE_BYTE (size_t)(sizeof(size_t) * 2)

/****************************************************************************/
/*                                                                          */
/*                        Basic Functions and Macros                        */
/*                                                                          */
/****************************************************************************/

/**
 * @brief Macros to access the size byte of the matrix
 *
 */
#define COLS(mat)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(mat) - MATRIX_SIZE_BYTE + sizeof(size_t) * 0)))))

#define ROWS(mat)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(mat) - MATRIX_SIZE_BYTE + sizeof(size_t) * 1)))))

/**
 * @brief Function to initialize the memory needed for a new matrix.
 *
 * @param size The size of the total memory block used by the matrix
 * @param elem_size The size of a single element in the matrix
 * @param ncols The number of columns in the matrix
 * @param nrows The number of rows in the matrix
 */
static inline void* __init_matrix(size_t size, size_t elem_size, size_t ncols,
                                  size_t nrows) {
    void* mat_start = calloc(1, size);
    SIMUTIL_NULLPTR_CHECK(mat_start);
    *((size_t*)mat_start + 0) = ncols;
    *((size_t*)mat_start + 1) = nrows;
    char** out = (char**)((char*)mat_start + MATRIX_SIZE_BYTE);
    SIMUTIL_NULLPTR_CHECK(out);
#ifndef ROW_MAJOR
    char* data_start = (char*)(out + (ncols + 1));
    SIMUTIL_NULLPTR_CHECK(data_start);
    for (size_t i = 1; i <= ncols; i++) {
        out[i] = data_start + i * (nrows + 1) * elem_size;
        SIMUTIL_NULLPTR_CHECK(out[i]);
    }
#else
    out[1] = (char*)calloc(
        1, (size_t)(((nrows * ncols) + MATRIX_SIZE_BYTE) * elem_size));
    for (size_t i = 2; i <= nrows; i++)
        out[i] = out[i - 1] + (ncols * elem_size);
#endif
    return (void*)out;
}

/**
 * @brief Macro to create a new matrix of type T
 *
 * @param T Type of matrix element
 * @param ncols Number of columns
 * @param nrows Number of rows
 */
#ifndef ROW_MAJOR
#define new_matrix(T, ncols, nrows)                                            \
    ((matrix(T))__init_matrix(((ncols + 1) * sizeof(T*) +                      \
                               (ncols + 1) * (nrows + 1) * sizeof(T) +         \
                               MATRIX_SIZE_BYTE),                              \
                              sizeof(T), ncols, nrows))
#else
#define new_matrix(T, ncols, nrows)                                            \
    ((matrix(T))__init_matrix((size_t)(nrows + 1) * sizeof(void*) +            \
                                  MATRIX_SIZE_BYTE,                            \
                              sizeof(T), ncols, nrows))
#endif

/**
 * @brief Macro to properly free the memory allocated to the matrix
 *
 * @param mat Matrix to free
 */
#ifndef ROW_MAJOR
#define free_matrix(mat)                                                       \
    do {                                                                       \
        void* mat_start = (void*)((char*)(mat) - MATRIX_SIZE_BYTE);            \
        free(mat_start);                                                       \
        mat_start = NULL;                                                      \
    } while (0)
#else
#define free_matrix(mat)                                                       \
    do {                                                                       \
        free((char*)mat[1]);                                                   \
        mat[1] = NULL;                                                         \
        void* mat_start = (void*)((char*)mat - MATRIX_SIZE_BYTE);              \
        free(mat_start);                                                       \
        mat_start = NULL;                                                      \
    } while (0)
#endif

#endif
