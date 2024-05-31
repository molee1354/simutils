#include "tensor.h"

#define CHECK(p)                                                               \
    if (!p) {                                                                  \
        raise_error(SIMUTIL_ALLOCATE_ERROR, "NULL allocation for matrix!\n");  \
        exit(EXIT_FAILURE);                                                    \
    }

#define INIT_TENSOR(mem, out, row, col, dep)                                   \
    do {                                                                       \
        *(((unsigned int*)mem) + 0) = row;                                     \
        *(((unsigned int*)mem) + 1) = col;                                     \
        *(((unsigned int*)mem) + 2) = dep;                                     \
        out = (tensor)((char*)mem + TENSOR_SIZE_BYTE);                         \
        out[1] = (double**)calloc(1, (row * col + 1) * sizeof(double*));       \
        CHECK(out[1]);                                                         \
        out[1][1] =                                                            \
            (double*)calloc(1, (row * col * dep + 1) * sizeof(double));        \
        CHECK(out[1][1]);                                                      \
        int i, j;                                                              \
        for (j = 2; j <= (int)col; j++)                                        \
            out[1][j] = out[1][j - 1] + dep;                                   \
        for (i = 2; i <= (int)row; i++) {                                      \
            out[i] = out[i - 1] + col;                                         \
            out[i][1] = out[i - 1][1] + col * dep;                             \
            for (j = 2; j <= (int)col; j++)                                    \
                out[i][j] = out[i][j - 1] + dep;                               \
        }                                                                      \
    } while (0)

tensor new_tensor(unsigned int ncols, unsigned int nrows, unsigned int ndeps) {
    void* ten_mem =
        calloc(1, (size_t)((ncols + 1) * sizeof(double**)) + TENSOR_SIZE_BYTE);
    CHECK(ten_mem);
    tensor out;
    INIT_TENSOR(ten_mem, out, ncols, nrows, ndeps);
    return out;
}

void save_tensor(tensor ten, const char* filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const int ncols = (const int)DIM1(ten);
    const int nrows = (const int)DIM2(ten);
    const int ndeps = (const int)DIM3(ten);

    fwrite(&nrows, sizeof(nrows), 1, file);
    fwrite(&ncols, sizeof(ncols), 1, file);
    fwrite(&ndeps, sizeof(ndeps), 1, file);
    for (int i = 1; i <= ncols; i++) {
        for (int j = 1; j <= nrows; j++) {
            fwrite(ten[i][j], sizeof(double), ndeps + 1, file);
        }
    }
    fclose(file);
}

tensor read_tensor(const char* filename) {
    FILE* file = fopen(filename, "rb");
    CHECK(file);
    int nrows;
    if (!fread(&nrows, sizeof(nrows), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the tensor rows.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    int ncols;
    if (!fread(&ncols, sizeof(ncols), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the tensor cols.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    int ndeps;
    if (!fread(&ndeps, sizeof(ndeps), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the tensor deps.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    tensor out;
    void* ten_mem = calloc(1, (ncols + 1) * sizeof(double*) + TENSOR_SIZE_BYTE);
    CHECK(ten_mem);
    INIT_TENSOR(ten_mem, out, ncols, nrows, ndeps);
    for (int i = 1; i <= ncols; i++) {
        for (int j = 1; j <= nrows; j++) {
            if (!fread(out[i][j], sizeof(double), ndeps + 1, file)) {
                raise_error(SIMUTIL_ALLOCATE_ERROR,
                            "Problem reading tensor[%d][%d] vector in tensor.",
                            i, j);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    return out;
}

void print_tensor(tensor ten) {
    const int ncol = (const int)DIM1(ten); // nx
    const int nrow = (const int)DIM2(ten); // ny
    const int ndep = (const int)DIM3(ten); // nz
    printf("[\n ");
    int i, j, k;
    for (k = 1; k <= ndep; k++) {
        printf("[");
        for (j = 1; j <= nrow; j++) {
            (j == 1) ? printf("[") : printf(" [");
            for (i = 1; i <= ncol; i++) {
                (i == ncol) ? printf("%5.2f", ten[i][j][k])
                            : printf("%5.2f, ", ten[i][j][k]);
            }
            (j == nrow) ? printf("]") : printf("]\n ");
        }
        (k == ndep) ? printf("]") : printf("]\n ");
    }
    printf("\n]\n ");
}

void fprint_tensor(FILE* fp, tensor ten) {
    const int ncol = (const int)DIM1(ten); // nx
    const int nrow = (const int)DIM2(ten); // ny
    const int ndep = (const int)DIM3(ten); // nz
    int i, j, k;
    for (k = 1; k <= ndep; k++) {
        for (j = 1; j <= nrow; j++) {
            for (i = 1; i <= ncol; i++) {
                (i == ncol) ? fprintf(fp, "%.3f", ten[i][j][k])
                            : fprintf(fp, "%.3f, ", ten[i][j][k]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "###\n");
    }
}

void free_tensor(tensor ten) {
    free((char*)(ten[1][1]));
    ten[1][1] = NULL;

    free((char*)(ten[1]));
    ten[1] = NULL;

    void* ten_mem = (void*)((char*)ten - TENSOR_SIZE_BYTE);
    free(ten_mem);
}
