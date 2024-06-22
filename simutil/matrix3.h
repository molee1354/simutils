#ifndef SIMUTIL_MATRIX3_H
#define SIMUTIL_MATRIX3_H

#include "error.h"
#include "simutil_includes.h"

#define matrix3(T) T***

#define MATRIX3_SIZE_BYTE (size_t)(sizeof(size_t) * 3)

// col
#define DIM1(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 0)))))

// row
#define DIM2(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 1)))))

// dep
#define DIM3(ten)                                                              \
    ((int)(*(                                                                  \
        (size_t*)(((char*)(ten) - MATRIX3_SIZE_BYTE + sizeof(size_t) * 2)))))

void* __init_matrix3(size_t size, size_t elem_size, size_t ncols, size_t nrows,
                     size_t ndeps);

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

#endif
