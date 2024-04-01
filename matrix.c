#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "error.h"

#define CHECK(p)\
    if (!p) {\
        raise_error(SIMUTIL_ALLOCATE_ERROR,"NULL allocation for matrix!\n");\
        exit(EXIT_FAILURE);\
    }\

matrix make_matrix(unsigned int nrows, unsigned int ncols) {
    void* vec_mem = calloc(1, nrows*sizeof(double*)+SIZE_BYTE);
    CHECK(vec_mem);
    matrix out = (matrix)( (char*)vec_mem + SIZE_BYTE );
    *( ((unsigned int*)vec_mem)+0 ) = nrows;
    *( ((unsigned int*)vec_mem)+1 ) = ncols;
    for (int i = 0; i < (int)nrows; i++) {
        out[i] = (double*)calloc(ncols, sizeof(double));
        CHECK(out[i]);
    }
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
    void* vec_mem = (void*)( (char*)mat - SIZE_BYTE );
    free(vec_mem);
}
