#ifndef SIMUTIL_MATRIX_PRINT_H
#define SIMUTIL_MATRIX_PRINT_H

#ifndef SIMUTIL_MATRIX_BASE_H
#include "matrix.h"
#endif

/****************************************************************************/
/*                                                                          */
/*                            Print Definitions                             */
/*                                                                          */
/****************************************************************************/

#ifdef __SET_COL_MAJOR
#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_m(FILE *fp, type mat) {                               \
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
#else
#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_m(FILE *fp, type mat) {                               \
        const int nrow = ROWS(mat);                                            \
        const int ncol = COLS(mat);                                            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = 1; j <= nrow; j++) {                                          \
            fprintf(fp, "[");                                                  \
            for (i = 1; i <= ncol; i++) {                                      \
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
