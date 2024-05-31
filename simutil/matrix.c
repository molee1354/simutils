#include "matrix.h"
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

matrix new_matrix(unsigned int ncols, unsigned int nrows) {
    void* mat_mem = calloc(1, (ncols + 1) * sizeof(double*) + MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    matrix out;
    INIT_MATRIX(mat_mem, out, ncols, nrows);
    return out;
}

void save_matrix(matrix mat, const char* filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const int nrows = (const int)ROWS(mat);
    const int ncols = (const int)COLS(mat);

    fwrite(&nrows, sizeof(nrows), 1, file);
    fwrite(&ncols, sizeof(ncols), 1, file);
    for (int i = 1; i <= ncols; i++)
        fwrite(mat[i], sizeof(double), nrows + 1, file);
    fclose(file);
}

matrix read_matrix(const char* filename) {
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
}

void print_matrix(matrix mat) {
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
}

void fprint_matrix(FILE* fp, matrix mat) {
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
}

void free_matrix(matrix mat) {
    free((char*)mat[1]);
    mat[1] = NULL;
    void* mat_mem = (void*)((char*)mat - MATRIX_SIZE_BYTE);
    free(mat_mem);
    mat_mem = NULL;
}
