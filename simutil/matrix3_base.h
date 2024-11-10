#ifndef SIMUTIL_MATRIX3_BASE_H
#define SIMUTIL_MATRIX3_BASE_H

#include "error.h"
#include "simutil_includes.h"

#define matrix3(T) T***

#define MATRIX3_SIZE_BYTE (size_t)(sizeof(size_t) * 3)

/****************************************************************************/
/*                                                                          */
/*                        Basic Functions and Macros                        */
/*                                                                          */
/****************************************************************************/

// column
#define DIM1(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 0)))))

// row
#define DIM2(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 1)))))

// depth
#define DIM3(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 2)))))

static inline void* __init_matrix3(size_t size, size_t elem_size, size_t ncols,
                                   size_t nrows, size_t ndeps) {
    void* mat_start = calloc(1, size);
    SIMUTIL_NULLPTR_CHECK(mat_start);
    *((size_t*)mat_start + 0) = ncols;
    *((size_t*)mat_start + 1) = nrows;
    *((size_t*)mat_start + 2) = ndeps;
    char*** out = (char***)((char*)mat_start + MATRIX3_SIZE_BYTE);
    SIMUTIL_NULLPTR_CHECK(out);
#ifndef ROW_MAJOR
    char** row_pointers = (char**)(out + (ncols + 1));
    SIMUTIL_NULLPTR_CHECK(row_pointers);
    char* data_start = (char*)(row_pointers + ((ncols + 1) * (nrows + 1)));
    SIMUTIL_NULLPTR_CHECK(data_start);

    for (size_t i = 1; i <= ncols; i++) {
        out[i] = row_pointers + i * nrows;
        for (size_t j = 1; j <= nrows; j++) {
            out[i][j] =
                data_start +
                (i * (nrows + 1) * (ndeps + 1) + j * (ndeps + 1)) * elem_size;
            SIMUTIL_NULLPTR_CHECK(out[i][j]);
        }
    }
#else
    out[1] = (char**)calloc(1, (size_t)(nrows * ncols + 1) * sizeof(void*));
    SIMUTIL_NULLPTR_CHECK(out[1]);
    out[1][1] = (char*)calloc(1, (nrows * ncols * ndeps + 1) * elem_size);
    SIMUTIL_NULLPTR_CHECK(out[1][1]);
    int i, j;
    for (j = 2; j <= (int)ncols; j++)
        out[1][j] = out[1][j - 1] + (ndeps * elem_size);
    for (i = 2; i <= (int)nrows; i++) {
        out[i] = out[i - 1] + ncols;
        out[i][1] = out[i - 1][1] + (ncols * ndeps * elem_size);
        for (j = 2; j <= (int)ncols; j++)
            out[i][j] = out[i][j - 1] + (ndeps * elem_size);
    }
#endif
    return (void*)out;
}

#ifndef ROW_MAJOR
#define new_matrix3(T, ncols, nrows, ndeps)                                    \
    ((matrix3(T))__init_matrix3(                                               \
        ((ncols + 1) * sizeof(T**) + (ncols + 1) * (nrows + 1) * sizeof(T*) +  \
         (ncols + 1) * (nrows + 1) * (ndeps + 1) * sizeof(T) +                 \
         MATRIX3_SIZE_BYTE),                                                   \
        sizeof(T), ncols, nrows, ndeps))

#define free_matrix3(mat)                                                      \
    do {                                                                       \
        void* mat_start = (void*)((char*)(mat) - MATRIX3_SIZE_BYTE);           \
        free(mat_start);                                                       \
        mat_start = NULL;                                                      \
    } while (0)
#else
#define new_matrix3(T, ncols, nrows, ndeps)                                    \
    ((matrix3(T))__init_matrix3(                                               \
        ((nrows + 1) * sizeof(T**) + MATRIX3_SIZE_BYTE), sizeof(T), ncols,     \
        nrows, ndeps))

#define free_matrix3(mat3)                                                     \
    do {                                                                       \
        free((char*)mat3[1][1]);                                               \
        free((char*)mat3[1]);                                               \
        void* mat3_mem = (void*)((char*)mat3 - MATRIX3_SIZE_BYTE);             \
        free(mat3_mem);                                                        \
    } while (0)
#endif


#endif
