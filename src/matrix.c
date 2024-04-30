#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "error.h"

#define CHECK(p)\
    if (!p) {\
        raise_error(SIMUTIL_ALLOCATE_ERROR,"NULL allocation for matrix!\n");\
        exit(EXIT_FAILURE);\
    }\

/**
 * - starting point of 'out' is offset by 'MATRIX_SIZE_BYTE' to make space to
 *   include the size information
 *   - and then the size information is set using the 'mem' starting point
 * - the corresponding columns are set using the starting point of 'out', so 
 *   addressing out[0] will now be the *starting point* for the first row.
 */
#define INIT_MATRIX(mem, out, row, col)\
    do {\
        *( ((unsigned int*)mem)+0 ) = row;\
        *( ((unsigned int*)mem)+1 ) = col;\
        out = (matrix)( (char*)mem + MATRIX_SIZE_BYTE );\
        out[1] = (double*)calloc(1, (row*col+1)*sizeof(double));\
        CHECK(out[1]);\
        for (int i = 2; i <= (int)row; i++) out[i] = out[i-1] + col;\
    } while (0)

matrix new_matrix(unsigned int nrows, unsigned int ncols) {
    void* mat_mem = calloc(1, (nrows+1)*sizeof(double*) + MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    matrix out;
    INIT_MATRIX(mat_mem, out, nrows, ncols);
    return out;
}

void save_matrix(matrix mat, const char *filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const int nrows = ROWS(mat);
    const int ncols = COLS(mat);

    fwrite(&nrows, sizeof(nrows), 1, file);
    fwrite(&ncols, sizeof(ncols), 1, file);
    for (int i = 1; i <= nrows; i++) {
        fwrite(mat[i], sizeof(double), ncols, file);
    }
    fclose(file);
}

matrix read_matrix(const char *filename) {
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
    void* mat_mem = calloc(1, (nrows+1)*sizeof(double*) + MATRIX_SIZE_BYTE);
    CHECK(mat_mem);
    INIT_MATRIX(mat_mem, out, nrows, ncols);
    for (int i = 1; i <= nrows; i++) {
        if (!fread(out[i], sizeof(double), ncols, file)) {
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
    for (int i = 1; i <= nrow; i++) {
        printf("[");
        for (int j = 1; j <= ncol; j++) {
            (j == ncol) ? printf("%6.3f", mat[i][j]) :
                          printf("%6.3f, ", mat[i][j]);
        }
        (i == nrow) ? printf("]") :
                      printf("]\n ");
    }
    printf("]\n");
}

void free_matrix(matrix mat) {
    free( (char*) mat[1]);
    mat[1] = NULL;
    void* vec_mem = (void*)( (char*)mat - MATRIX_SIZE_BYTE );
    free(vec_mem);
    vec_mem = NULL;
}
