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

static inline void* __init_matrix3(size_t size, size_t elem_size, size_t ncols,
                                   size_t nrows, size_t ndeps) {
    void* mat_start = calloc(1, size);
    CHECK(mat_start);
    *((size_t*)mat_start + 0) = ncols;
    *((size_t*)mat_start + 1) = nrows;
    *((size_t*)mat_start + 2) = ndeps;
    char*** out = (char***)((char*)mat_start + MATRIX3_SIZE_BYTE);
    CHECK(out);
#ifndef ROW_MAJOR
    char** row_pointers = (char**)(out + (ncols + 1));
    CHECK(row_pointers);
    char* data_start = (char*)(row_pointers + ((ncols + 1) * (nrows + 1)));
    CHECK(data_start);

    for (size_t i = 1; i <= ncols; i++) {
        out[i] = row_pointers + i * nrows;
        for (size_t j = 1; j <= nrows; j++) {
            out[i][j] =
                data_start +
                (i * (nrows + 1) * (ndeps + 1) + j * (ndeps + 1)) * elem_size;
            CHECK(out[i][j]);
        }
    }
#else
    out[1] = (char**)calloc(1, (size_t)(nrows * ncols + 1) * sizeof(void*));
    CHECK(out[1]);
    out[1][1] = (char*)calloc(1, (nrows * ncols * ndeps + 1) * elem_size);
    CHECK(out[1][1]);
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

#endif
