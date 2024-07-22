#include "matrix3.h"

void* __init_matrix3(size_t size, size_t elem_size, size_t ncols, size_t nrows,
                     size_t ndeps) {
    // Allocate memory
    // void* mat_start = calloc(1, size);
    void* mat_start = aligned_alloc(elem_size, size);
    CHECK(mat_start);

    // Store dimensions at the beginning of the allocated memory
    size_t* meta_data = (size_t*)mat_start;
    meta_data[0] = ncols;
    meta_data[1] = nrows;
    meta_data[2] = ndeps;

    // Pointer to the array of pointers to rows
    char*** out = (char***)((char*)mat_start + MATRIX3_SIZE_BYTE);
    CHECK(out);

    // Pointer to the array of pointers to depth
    char** row_pointers = (char**)(out + (ncols + 1));
    CHECK(row_pointers);

    // Pointer to the actual data start
    char* data_start = (char*)(row_pointers + ((ncols + 1) * (nrows + 1)));
    CHECK(data_start);

    // Initialize the array of pointers
    for (size_t i = 1; i <= ncols; i++) {
        out[i] = row_pointers + i * nrows;
        for (size_t j = 1; j <= nrows; j++) {
            out[i][j] =
                data_start +
                (i * (nrows + 1) * (ndeps + 1) + j * (ndeps + 1)) * elem_size;
            CHECK(out[i][j]);
        }
    }

    return (void*)out;
}

#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_m3(FILE* fp, type mat3) {                             \
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
