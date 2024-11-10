#ifndef SIMUTIL_MATRIX3_H
#define SIMUTIL_MATRIX3_H

#ifndef SIMUTIL_MATRIX3_BASE_H
#include "matrix3_base.h"
#endif

#ifdef SIMUTIL_COL_MAJOR
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_m3(FILE* fp, type mat3) {               \
        const int ncol = (const int)DIM1(mat3);                                \
        const int nrow = (const int)DIM2(mat3);                                \
        const int ndep = (const int)DIM3(mat3);                                \
        fprintf(fp, "[\n ");                                                   \
        int i, j, k;                                                           \
        for (k = 1; k <= ndep; k++) {                                          \
            fprintf(fp, "[");                                                  \
            for (j = 1; j <= nrow; j++) {                                      \
                (j == 1) ? fprintf(fp, "[") : fprintf(fp, " [");               \
                for (i = 1; i <= ncol; i++) {                                  \
                    if (i != ncol) {                                           \
                        fprintf(fp, fmt, mat3[i][j][k]);                       \
                        fprintf(fp, ", ");                                     \
                    } else                                                     \
                        fprintf(fp, fmt, mat3[i][j][k]);                       \
                }                                                              \
                (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");          \
            }                                                                  \
            (k == ndep) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "\n]\n ");                                                 \
    }
#else
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_m3(FILE* fp, type mat3) {               \
        const int ncol = (const int)DIM1(mat3);                                \
        const int nrow = (const int)DIM2(mat3);                                \
        const int ndep = (const int)DIM3(mat3);                                \
        fprintf(fp, "[\n ");                                                   \
        int i, j, k;                                                           \
        for (k = 1; k <= ndep; k++) {                                          \
            fprintf(fp, "[");                                                  \
            for (j = 1; j <= nrow; j++) {                                      \
                (j == 1) ? fprintf(fp, "[") : fprintf(fp, " [");               \
                for (i = 1; i <= ncol; i++) {                                  \
                    if (i != ncol) {                                           \
                        fprintf(fp, fmt, mat3[j][i][k]);                       \
                        fprintf(fp, ", ");                                     \
                    } else                                                     \
                        fprintf(fp, fmt, mat3[j][i][k]);                       \
                }                                                              \
                (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");          \
            }                                                                  \
            (k == ndep) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "\n]\n ");                                                 \
    }
#endif

// printing floating-point numbers
PRINT_FUNC(_float, matrix3(float), "%6.3f")
PRINT_FUNC(_double, matrix3(double), "%6.3f")
PRINT_FUNC(_long_double, matrix3(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, matrix3(char), "%c")
PRINT_FUNC(_uchar, matrix3(unsigned char), "%3hd")
PRINT_FUNC(_short, matrix3(short), "%3hd")
PRINT_FUNC(_ushort, matrix3(unsigned short), "%3hd")
PRINT_FUNC(_int, matrix3(int), "%3d")
PRINT_FUNC(_uint, matrix3(unsigned int), "%3u")
PRINT_FUNC(_long, matrix3(long), "%3ld")
PRINT_FUNC(_ulong, matrix3(unsigned long), "%3lu")

#define print_matrix3(mat3)                                                    \
    _Generic((mat3),                                                           \
        matrix3(char): __print_char_m3,                                        \
        matrix3(unsigned char): __print_uchar_m3,                              \
        matrix3(short): __print_short_m3,                                      \
        matrix3(unsigned short): __print_ushort_m3,                            \
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
        matrix3(unsigned char): __print_uchar_m3,                              \
        matrix3(short): __print_short_m3,                                      \
        matrix3(unsigned short): __print_ushort_m3,                            \
        matrix3(int): __print_int_m3,                                          \
        matrix3(unsigned int): __print_uint_m3,                                \
        matrix3(long): __print_long_m3,                                        \
        matrix3(unsigned long): __print_ulong_m3,                              \
        matrix3(float): __print_float_m3,                                      \
        matrix3(double): __print_double_m3,                                    \
        matrix3(long double): __print_long_double_m3)(fp, mat3)

#undef PRINT_FUNC

#endif
