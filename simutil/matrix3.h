#ifndef SIMUTIL_MATRIX3_H
#define SIMUTIL_MATRIX3_H

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

// printing floating-point numbers
void __print_float_m3(FILE* fp, matrix3(float) mat3);
void __print_double_m3(FILE* fp, matrix3(double) mat3);
void __print_long_double_m3(FILE* fp, matrix3(long double) mat3);

// printing integers / chars
void __print_char_m3(FILE* fp, matrix3(char) mat3);
void __print_short_m3(FILE* fp, matrix3(short) mat3);
void __print_int_m3(FILE* fp, matrix3(int) mat3);
void __print_uint_m3(FILE* fp, matrix3(unsigned int) mat3);
void __print_long_m3(FILE* fp, matrix3(long) mat3);
void __print_ulong_m3(FILE* fp, matrix3(unsigned long) mat3);

#define print_matrix3(mat3)                                                    \
    _Generic((mat3),                                                           \
        matrix3(char): __print_char_m3,                                        \
        matrix3(short): __print_short_m3,                                      \
        matrix3(int): __print_int_m3,                                          \
        matrix3(unsigned int): __print_uint_m3,                                \
        matrix3(long): __print_long_m3,                                        \
        matrix3(unsigned long): __print_ulong_m3,                              \
        matrix3(float): __print_float_m3,                                      \
        matrix3(double): __print_double_m3,                                    \
        matrix3(long double): __print_long_double_m3)(stdout, mat3)

#define fprint_matrix3(fp, mat3)                                               \
    _Generic((mat3),                                                           \
        matrix3(char): __print_char_m3,                                        \
        matrix3(int): __print_int_m3,                                          \
        matrix3(short): __print_short_m3,                                      \
        matrix3(unsigned int): __print_uint_m3,                                \
        matrix3(long): __print_long_m3,                                        \
        matrix3(unsigned long): __print_ulong_m3,                              \
        matrix3(float): __print_float_m3,                                      \
        matrix3(double): __print_double_m3,                                    \
        matrix3(long double): __print_long_double_m3)(fp, mat3)

#endif
