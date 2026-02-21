#ifndef SIMUTIL_MATRIX_H
#define SIMUTIL_MATRIX_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#ifdef SIMUTIL_MATRIX_COL_MAJOR
#define __SET_COL_MAJOR
#endif

#ifdef SIMUTIL_MATRIX_START_IDX_1
#define __MATRIX_START_IDX 1
#else
#define __MATRIX_START_IDX 0
#endif

/* Type alias for matrix */
#define matrix(T) T **

/* Metadata memory size */
#define __MATRIX_SIZE_BYTE (size_t)(sizeof(size_t) * 2)

/******************************************************************************/
/*                                                                            */
/*                          Basic Functions and Macros                        */
/*                                                                            */
/******************************************************************************/

/**
 * @brief Macros to access the size byte of the matrix
 *
 */
#define __MATRIX_COLS(__mptr)                                                  \
    (*((size_t *)(((char *)(__mptr) - __MATRIX_SIZE_BYTE +                     \
                   sizeof(size_t) * 0))))

#define __MATRIX_ROWS(__mptr)                                                  \
    (*((size_t *)(((char *)(__mptr) - __MATRIX_SIZE_BYTE +                     \
                   sizeof(size_t) * 1))))

/******************************************************************************/
/*                                                                            */
/*                             Internal Functions                             */
/*                                                                            */
/******************************************************************************/

// #define SIMUTIL_MATRIX_IMPLEMENTATION
// #define __SET_COL_MAJOR

/**
 * @brief Function to initialize the memory needed for a new matrix.
 *
 * @param size The size of the total memory block used by the matrix
 * @param elem_size The size of a single element in the matrix
 * @param ncols The number of columns in the matrix
 * @param nrows The number of rows in the matrix
 */
void *__init_matrix(size_t size, size_t elem_size, size_t ncol, size_t nrow);
#ifdef SIMUTIL_MATRIX_IMPLEMENTATION
void *__init_matrix(size_t size, size_t elem_size, size_t ncol, size_t nrow) {
    void *mat_start = calloc(1, size);
    if (!mat_start)
        return NULL;
    *((size_t *)mat_start + 0) = ncol;
    *((size_t *)mat_start + 1) = nrow;
#ifndef __SET_COL_MAJOR
    const size_t nrows = nrow - __MATRIX_START_IDX + 1;
    const size_t ncols = ncol - __MATRIX_START_IDX + 1;
#endif
    char **out = (char **)((char *)mat_start + __MATRIX_SIZE_BYTE);
    if (!out)
        return NULL;
#ifdef __SET_COL_MAJOR
    char *data_start = (char *)(out + (ncol + __MATRIX_START_IDX));
    if (!data_start)
        return NULL;
    for (size_t i = __MATRIX_START_IDX; i <= ncol + __MATRIX_START_IDX - 1;
         i++) {
        out[i] = data_start + i * (nrow + __MATRIX_START_IDX) * elem_size;
        if (!out[i])
            NULL;
    }
#else
    out[__MATRIX_START_IDX] = (char *)calloc(
        1, (size_t)(((nrows * ncols + 0) + __MATRIX_SIZE_BYTE) * elem_size));
    if (!out)
        return NULL;
    for (size_t i = 1 + __MATRIX_START_IDX; i <= nrow+__MATRIX_START_IDX-1; i++) {
        out[i] = out[i - 1] + (ncols * elem_size);
        if (!out[i])
            return NULL;
    }
#endif
    return (void *)out;
}
#endif

/******************************************************************************/
/*                                                                            */
/*                                User-end Macros                             */
/*                                                                            */
/******************************************************************************/

// User-end functions for matrix size
#define ncol_matrix(__mptr) __MATRIX_COLS(__mptr)
#define nrow_matrix(__mptr) __MATRIX_ROWS(__mptr)

/**
 * @brief Macro to create a new matrix of type T
 *
 * @param T Type of matrix element
 * @param ncols Number of columns
 * @param nrows Number of rows
 */
#ifdef __SET_COL_MAJOR
#define new_matrix(T, ncols, nrows)                                            \
    ((matrix(T))__init_matrix(((ncols + __MATRIX_START_IDX) * sizeof(T *) +    \
                               (ncols + __MATRIX_START_IDX) *                  \
                                   (nrows + __MATRIX_START_IDX) * sizeof(T) +  \
                               __MATRIX_SIZE_BYTE),                            \
                              sizeof(T), ncols, nrows))
#else
#define new_matrix(T, ncols, nrows)                                            \
    ((matrix(T))__init_matrix((size_t)(nrows + 1) * sizeof(void *) +           \
                                  __MATRIX_SIZE_BYTE,                          \
                              sizeof(T), ncols, nrows))
#endif

/**
 * @brief Macro to properly free the memory allocated to the matrix
 *
 * @param mat Matrix to free
 */
#ifdef __SET_COL_MAJOR
#define free_matrix(__mptr)                                                    \
    do {                                                                       \
        void *mat_start = (void *)((char *)(__mptr) - __MATRIX_SIZE_BYTE);     \
        free(mat_start);                                                       \
        mat_start = NULL;                                                      \
    } while (0)
#else
#define free_matrix(__mptr)                                                    \
    do {                                                                       \
        free((char *)__mptr[__MATRIX_START_IDX]);                              \
        __mptr[__MATRIX_START_IDX] = NULL;                                     \
        void *mat_start = (void *)((char *)__mptr - __MATRIX_SIZE_BYTE);       \
        free(mat_start);                                                       \
        mat_start = NULL;                                                      \
    } while (0)
#endif

#define SIMUTIL_MATRIX_PRINT
#ifdef SIMUTIL_MATRIX_PRINT
/******************************************************************************/
/*                                                                            */
/*                              Print Functions                               */
/*                                                                            */
/******************************************************************************/

#ifdef __SET_COL_MAJOR
#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_m(FILE *fp, type mat) {                               \
        const int nrow = nrow_matrix(mat) + __MATRIX_START_IDX - 1;            \
        const int ncol = ncol_matrix(mat) + __MATRIX_START_IDX - 1;            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = __MATRIX_START_IDX; j <= nrow; j++) {                         \
            fprintf(fp, "[");                                                  \
            for (i = __MATRIX_START_IDX; i <= ncol; i++) {                     \
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
#else
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_m(FILE *fp, type mat) {                 \
        const int nrow = nrow_matrix(mat) + __MATRIX_START_IDX - 1;            \
        const int ncol = ncol_matrix(mat) + __MATRIX_START_IDX - 1;            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = __MATRIX_START_IDX; j <= nrow; j++) {                         \
            fprintf(fp, "[");                                                  \
            for (i = __MATRIX_START_IDX; i <= ncol; i++) {                     \
                if (i != ncol) {                                               \
                    fprintf(fp, fmt, mat[j][i]);                               \
                    fprintf(fp, ", ");                                         \
                } else                                                         \
                    fprintf(fp, fmt, mat[j][i]);                               \
            }                                                                  \
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "]\n");                                                    \
    }
#endif

// printing floating-point numbers
PRINT_FUNC(_float, matrix(float), "%6.3f")
PRINT_FUNC(_double, matrix(double), "%6.3f")
PRINT_FUNC(_long_double, matrix(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, matrix(char), "%c")
PRINT_FUNC(_uchar, matrix(unsigned char), "%3d")
PRINT_FUNC(_short, matrix(short), "%3hd")
PRINT_FUNC(_ushort, matrix(unsigned short), "%3hd")
PRINT_FUNC(_int, matrix(int), "%3d")
PRINT_FUNC(_uint, matrix(unsigned int), "%3u")
PRINT_FUNC(_long, matrix(long), "%3ld")
PRINT_FUNC(_ulong, matrix(unsigned long), "%3lu")

/* Function-like macros for printing numerical matrices */
#define print_matrix(mat)                                                      \
    _Generic((mat),                                                            \
        matrix(char): __print_char_m,                                          \
        matrix(unsigned char): __print_uchar_m,                                \
        matrix(short): __print_short_m,                                        \
        matrix(unsigned short): __print_ushort_m,                              \
        matrix(int): __print_int_m,                                            \
        matrix(unsigned int): __print_uint_m,                                  \
        matrix(long): __print_long_m,                                          \
        matrix(unsigned long): __print_ulong_m,                                \
        matrix(float): __print_float_m,                                        \
        matrix(double): __print_double_m,                                      \
        matrix(long double): __print_long_double_m)(stdout, mat)

#define fprint_matrix(fp, mat)                                                 \
    _Generic((mat),                                                            \
        matrix(char): __print_char_m,                                          \
        matrix(unsigned char): __print_uchar_m,                                \
        matrix(short): __print_short_m,                                        \
        matrix(unsigned short): __print_ushort_m,                              \
        matrix(int): __print_int_m,                                            \
        matrix(unsigned int): __print_uint_m,                                  \
        matrix(long): __print_long_m,                                          \
        matrix(unsigned long): __print_ulong_m,                                \
        matrix(float): __print_float_m,                                        \
        matrix(double): __print_double_m,                                      \
        matrix(long double): __print_long_double_m)(fp, mat)

#endif

#endif
