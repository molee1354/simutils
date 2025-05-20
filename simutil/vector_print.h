#ifndef SIMUTIL_VECTOR_PRINT_H
#define SIMUTIL_VECTOR_PRINT_H

#ifndef SIMUTIL_VECTOR_BASE_H
#include "vector.h"
#endif

// macro to generate printing functions
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_v(FILE* fp, type vec) {                 \
        const int length = LENGTH(vec) + VECTOR_START_IDX - 1;                 \
        if (fp == stdout || fp == stderr)                                      \
            fprintf(fp, "[");                                                  \
        for (int i = VECTOR_START_IDX; i <= length; i++) {                     \
            if (i != length) {                                                 \
                fprintf(fp, fmt, vec[i]);                                      \
                fprintf(fp, ", ");                                             \
            } else                                                             \
                fprintf(fp, fmt, vec[i]);                                      \
        }                                                                      \
        if (fp == stdout || fp == stderr)                                      \
            fprintf(fp, "]\n");                                                \
        else                                                                   \
            fprintf(fp, "\n");                                                 \
    }

// printing floating-point numbers
PRINT_FUNC(_float, vector(float), "%6.3f")
PRINT_FUNC(_double, vector(double), "%6.3f")
PRINT_FUNC(_long_double, vector(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, vector(char), "%c")
PRINT_FUNC(_uchar, vector(unsigned char), "%3d")
PRINT_FUNC(_short, vector(short), "%3hd")
PRINT_FUNC(_ushort, vector(unsigned short), "%3hd")
PRINT_FUNC(_int, vector(int), "%3d")
PRINT_FUNC(_uint, vector(unsigned int), "%3u")
PRINT_FUNC(_long, vector(long), "%3ld")
PRINT_FUNC(_ulong, vector(unsigned long), "%3lu")

#define print_vector(vec)                                                      \
    _Generic((vec),                                                            \
        vector(char): __print_char_v,                                          \
        vector(unsigned char): __print_uchar_v,                                \
        vector(short): __print_short_v,                                        \
        vector(unsigned short): __print_ushort_v,                              \
        vector(int): __print_int_v,                                            \
        vector(unsigned int): __print_uint_v,                                  \
        vector(long): __print_long_v,                                          \
        vector(unsigned long): __print_ulong_v,                                \
        vector(float): __print_float_v,                                        \
        vector(double): __print_double_v,                                      \
        vector(long double): __print_long_double_v)(stdout, vec)

#define fprint_vector(fp, vec)                                                 \
    _Generic((vec),                                                            \
        vector(char): __print_char_v,                                          \
        vector(unsigned char): __print_uchar_v,                                \
        vector(short): __print_short_v,                                        \
        vector(unsigned short): __print_ushort_v,                              \
        vector(int): __print_int_v,                                            \
        vector(unsigned int): __print_uint_v,                                  \
        vector(long): __print_long_v,                                          \
        vector(unsigned long): __print_ulong_v,                                \
        vector(float): __print_float_v,                                        \
        vector(double): __print_double_v,                                      \
        vector(long double): __print_long_double_v)(fp, vec)

#undef PRINT_FUNC

#endif
