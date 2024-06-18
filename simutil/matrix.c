#include "matrix.h"
#include "error.h"
#include <stdio.h>

#define CHECK(p)                                                               \
    if (!p) {                                                                  \
        raise_error(SIMUTIL_ALLOCATE_ERROR,                                    \
                    "[%s:%d] NULL allocation for matrix!\n", __FILE__,         \
                    __LINE__);                                                 \
        exit(EXIT_FAILURE);                                                    \
    }

/**
 * - starting point of 'out' is offset by 'MATRIX_SIZE_BYTE' to make space to
 *   include the size information
 *   - and then the size information is set using the 'mem' starting point
 * - the corresponding columns are set using the starting point of 'out', so
 *   addressing out[0] will now be the *starting point* for the first row.
 */
#define INIT_MATRIX(mem, out, col, row)                                        \
    do {                                                                       \
        out = (matrix)((char*)mem + MATRIX_SIZE_BYTE);                         \
        *(((unsigned int*)mem) + 0) = col;                                     \
        *(((unsigned int*)mem) + 1) = row;                                     \
        out[1] = (double*)calloc(1, (row * col + 1) * sizeof(double));         \
        CHECK(out[1]);                                                         \
        for (int i = 2; i <= (int)col; i++)                                    \
            out[i] = out[i - 1] + row;                                         \
    } while (0)

/* matrix new_matrix(unsigned int ncols, unsigned int nrows) {
    void* mat_mem = calloc(1, (ncols + 1) * sizeof(double*) + MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    matrix out;
    INIT_MATRIX(mat_mem, out, ncols, nrows);
    return out;
} */

/* void* __init_matrix(size_t size, size_t elem_size, size_t ncols, size_t
nrows) { void* mat_start = calloc(1, size); CHECK(mat_start);
    *(((size_t*)mat_start) + 0) = ncols;
    *(((size_t*)mat_start) + 1) = nrows;
    printf("elem_size = %zu\n", elem_size);
    char** out = (char**)((char*)mat_start + MATRIX_SIZE_BYTE);
    out[1] = calloc(1, (nrows * ncols + 1) * elem_size);
    CHECK(out[1]);
    for (int i = 2; i <= (int)ncols; i++)
        out[i] = out[i-1] + nrows;
    return (void*)out;
} */

void* __init_matrix(size_t size, size_t elem_size, size_t ncols, size_t nrows) {
    void* mat_start = calloc(1, size);
    CHECK(mat_start);
    *((size_t*)mat_start + 0) = ncols;
    *((size_t*)mat_start + 1) = nrows;
    char** out = (char**)((char*)mat_start + MATRIX_SIZE_BYTE);
    char* data_start = (char*)(out + (ncols + 1));
    for (size_t i = 1; i <= ncols; i++)
        out[i] = data_start + i * (nrows + 1) * elem_size;
    return (void*)out;
}

/* void save_matrix(matrix mat, const char* filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const int nrows = (const int)ROWS(mat);
    const int ncols = (const int)COLS(mat);

    fwrite(&nrows, sizeof(nrows), 1, file);
    fwrite(&ncols, sizeof(ncols), 1, file);
    for (int i = 1; i <= ncols; i++)
        fwrite(mat[i], sizeof(double), nrows + 1, file);
    fclose(file);
} */

/* matrix read_matrix(const char* filename) {
    FILE* file = fopen(filename, "rb");
    CHECK(file);
    int nrows;
    if (!fread(&nrows, sizeof(nrows), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the matrix rows.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    int ncols;
    if (!fread(&ncols, sizeof(ncols), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the matrix cols.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    matrix out;
    void* mat_mem = calloc(1, (ncols + 1) * sizeof(double*) + MATRIX_SIZE_BYTE);
    if (mat_mem == NULL)
        puts("oh no!");
    CHECK(mat_mem);
    INIT_MATRIX(mat_mem, out, ncols, nrows);
    for (int i = 1; i <= ncols; i++) {
        if (!fread(out[i], sizeof(double), nrows + 1, file)) {
            raise_error(SIMUTIL_ALLOCATE_ERROR,
                        "Problem reading %dth row in matrix.", i);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return out;
} */

/* void print_matrix(matrix mat) {
    const int nrow = ROWS(mat);
    const int ncol = COLS(mat);
    printf("[");
    int i, j;
    for (j = 1; j <= nrow; j++) {
        printf("[");
        for (i = 1; i <= ncol; i++) {
            (i == ncol) ? printf("%6.3f", mat[i][j])
                        : printf("%6.3f, ", mat[i][j]);
        }
        (j == nrow) ? printf("]") : printf("]\n ");
    }
    printf("]\n");
} */
#define CAST(type) type value = *((type*)element)
#define PRINT_FUNC(name, type, fmt)                                            \
    static void print_##name(FILE* fp, void* element) {                        \
        CAST(type);                                                            \
        fprintf(fp, fmt, value);                                               \
    }

PRINT_FUNC(f32, float, "%.3f")
PRINT_FUNC(f64, double, "%.3f")
PRINT_FUNC(f128, long double, "%.3Lf")

static void* dref_32(char* mat_mem, int rows, int i, int j) {
    size_t e_size = (size_t)(4);
    /* void* col = (void*)(mat_mem + (i+2)*rows*e_size + e_size*1);
    void* out = (void*)((char*)col + e_size*(j-2) + e_size*(i+1)); */
    void* col = (void*)((char*)mat_mem + (i + 1) * rows * e_size + e_size);
    void* out = (void*)((char*)col + e_size * (j - 1) + e_size * i);
    return out;
}

static void* dref_64(char* mat_mem, int rows, int i, int j) {
    size_t e_size = (size_t)(8);
    void* col = (void*)((char*)mat_mem + (i + 1) * rows * e_size + e_size);
    void* out = (void*)((char*)col + e_size * (j - 1) + e_size * i);
    return out;
}

static void* dref_128(char* mat_mem, int rows, int i, int j) {
    size_t e_size = (size_t)(16);
    /* void* col = (void*)((char*)mat_mem + (i+1)*rows*e_size - e_size);
    void* out = (void*)((char*)col + e_size*(j-1) + e_size*i); */
    void* col = (void*)((char*)mat_mem + (i + 1) * rows * e_size + e_size);
    void* out = (void*)((char*)col + e_size * (j - 1) + e_size * i);
    return out;
}

// working version for double
/* void* col = (void*)(test_mat + (i+1)*rows*e_size + e_size*1);
void* e = (void*)((char*)col + e_size*(j-1) + e_size*i); */

// working version for long double
/* void* col = (void*)(test_mat + (i+1)*rows*e_size - e_size*1);
void* e = (void*)((char*)col + e_size*(j-1) + e_size*i); */

void __print_dmatrix(FILE* fp, void* mat_mem, size_t elem_size) {
    const int nrow = ROWS(mat_mem);
    const int ncol = COLS(mat_mem);
    void (*print_function)(FILE*, void*) = NULL;
    void* (*dref_function)(char*, int, int, int) = NULL;

    if (elem_size == 4) {
        print_function = print_f32;
        dref_function = dref_32;
    } else if (elem_size == 8) {
        print_function = print_f64;
        dref_function = dref_64;
    } else if (elem_size == 16) {
        print_function = print_f128;
        dref_function = dref_128;
    } else {
        raise_error(SIMUTIL_TYPE_ERROR, "Unsupported element type...");
        return;
    }

    char* base = (char*)mat_mem;
    if (fp == stdout || fp == stderr)
        fprintf(fp, "[");
    int i, j;
    for (j = 1; j <= nrow; j++) {
        if (fp == stdout || fp == stderr)
            fprintf(fp, "[");
        else
            fprintf(fp, " ");
        for (i = 1; i <= ncol; i++) {
            void* element = dref_function(base, nrow, i, j);
            print_function(fp, element);
            if (i != ncol)
                fprintf(fp, ", ");
        }
        if (fp == stdout || fp == stderr)
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");
        else
            (j == nrow) ? fprintf(fp, " ") : fprintf(fp, "\n ");
    }

    if (fp == stdout || fp == stderr)
        fprintf(fp, "]\n");
    else
        fprintf(fp, "\n");
}

PRINT_FUNC(i8, char, "%c")
PRINT_FUNC(i16, short, "%hi")
PRINT_FUNC(i32, int, "%d")
PRINT_FUNC(i64, long, "%ld")

void __print_imatrix(FILE* fp, void* mat_mem, size_t elem_size) {
    const int nrow = ROWS(mat_mem);
    const int ncol = COLS(mat_mem);
    void (*print_function)(FILE*, void*) = NULL;

    if (elem_size == 1) {
        print_function = print_i8;
    } else if (elem_size == 2) {
        print_function = print_i16;
    } else if (elem_size == 4) {
        print_function = print_i32;
    } else if (elem_size == 8) {
        print_function = print_i64;
    } else {
        raise_error(SIMUTIL_TYPE_ERROR, "Unsupported element type...");
        return;
    }

    if (fp == stdout || fp == stderr)
        fprintf(fp, "[");

    char* base = (char*)mat_mem;
    int i, j;
    for (j = 1; j <= nrow; j++) {
        if (fp == stdout || fp == stderr)
            fprintf(fp, "[");
        else
            fprintf(fp, " ");
        for (i = 1; i <= ncol; i++) {
            void* element =
                (void*)(base + ((i - 1) * (nrow + 1) + (j - 1)) * elem_size);
            print_function(fp, element);
            if (i != ncol)
                fprintf(fp, ", ");
        }
        if (fp == stdout || fp == stderr)
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");
        else
            (j == nrow) ? fprintf(fp, " ") : fprintf(fp, "\n ");
    }

    if (fp == stdout || fp == stderr)
        fprintf(fp, "]\n");
    else
        fprintf(fp, "\n");
}

/* void fprint_matrix(FILE* fp, matrix mat) {
    const int nrow = ROWS(mat);
    const int ncol = COLS(mat);
    int i, j;
    for (j = 1; j <= nrow; j++) {
        for (i = 1; i <= ncol; i++) {
            (i == ncol) ? fprintf(fp, "%.3f", mat[i][j])
                        : fprintf(fp, "%.3f, ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
} */

/* void free_matrix(matrix mat) {
    free((char*)mat[1]);
    mat[1] = NULL;
    void* mat_mem = (void*)((char*)mat - MATRIX_SIZE_BYTE);
    free(mat_mem);
    mat_mem = NULL;
} */
