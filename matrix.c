#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "error.h"

#define CHECK(p)\
    if (!p) {\
        raise_error(SIMUTIL_ALLOCATE_ERROR,"NULL allocation for matrix!\n");\
        exit(EXIT_FAILURE);\
    }\

#define INIT_MATRIX(mem, out, row, col)\
    do {\
        out = (matrix)( (char*)mem + MATRIX_SIZE_BYTE );\
        *( ((unsigned int*)mem)+0 ) = nrows;\
        *( ((unsigned int*)mem)+1 ) = ncols;\
        for (int i = 0; i < (int)nrows; i++) {\
            out[i] = (double*)calloc(ncols, sizeof(double));\
            CHECK(out[i]);\
        }\
    } while (0)

matrix make_matrix(unsigned int nrows, unsigned int ncols) {
    void* mat_mem = calloc(1, nrows*sizeof(double*)+MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    matrix out;
    INIT_MATRIX(mat_mem, out, nrows, ncols);
    return out;
}

void save_matrix(matrix mat, const char *filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    unsigned int nrows = ROWS(mat);
    unsigned int ncols = COLS(mat);

    fwrite(&nrows, sizeof(nrows), 1, file);
    fwrite(&ncols, sizeof(ncols), 1, file);
    for (int i = 0; i < nrows; i++) {
        fwrite(mat[i], sizeof(double), ncols, file);
    }
    fclose(file);
}

matrix read_matrix(const char *filename) {
    FILE* file = fopen(filename, "rb");
    CHECK(file);
    unsigned int nrows;
    if (!fread(&nrows, sizeof(nrows), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the matrix rows.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    unsigned int ncols;
    if (!fread(&ncols, sizeof(ncols), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the matrix cols.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    matrix out;
    void* mat_mem = calloc(1, nrows*sizeof(double*) + MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    INIT_MATRIX(mat_mem, out, nrows, ncols);
    for (int i = 0; i < nrows; i++) {
        fread(out[i], sizeof(double), ncols, file);
    }
    fclose(file);
    return out;
}

void print_matrix(matrix mat) {
    int nrow = ROWS(mat);
    int ncol = COLS(mat);
    printf("[");
    for (int i = 0; i < nrow; i++) {
        printf("[");
        for (int j = 0; j < ncol; j++) {
            (j == nrow) ? printf("%6.3f", mat[i][j]) :
                          printf("%6.3f, ", mat[i][j]);
        }
        (i == ncol) ? printf("]") :
                      printf("]\n ");
    }
    printf("]\n");
}

void free_matrix(matrix mat) {
    int nrow = ROWS(mat);
    for (int i = 0; i < nrow; i++) {
        free(mat[i]);
    }
    void* vec_mem = (void*)( (char*)mat - MATRIX_SIZE_BYTE );
    free(vec_mem);
}
