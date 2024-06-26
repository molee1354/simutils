#include "vector.h"

#define CHECK(p)                                                               \
    if (!p) {                                                                  \
        raise_error(SIMUTIL_ALLOCATE_ERROR, "NULL allocation for vector!\n");  \
        exit(EXIT_FAILURE);                                                    \
    }

#define INIT_VECTOR(mem, out, size)                                            \
    do {                                                                       \
        *(((unsigned int *)mem) + 0) = size;                                   \
        out = (vector)((char *)mem + VECTOR_SIZE_BYTE + VECTOR_IDX_BYTE);      \
        out--;                                                                 \
    } while (0)

vector new_vector(unsigned int size) {
    void *vec_mem = calloc(1, (size + 1) * sizeof(double) + VECTOR_SIZE_BYTE);
    CHECK(vec_mem);
    vector out;
    INIT_VECTOR(vec_mem, out, size);
    return out;
}

void add(vector vec1, vector vec2) {
    if (LENGTH(vec1) != LENGTH(vec2)) {
        raise_error(SIMUTIL_DIMENSION_ERROR, "Vector dimension mismatch.");
        exit(EXIT_FAILURE);
    }
    const int len = LENGTH(vec1);
    for (int i = 1; i <= len; i++) {
        vec1[i] += vec2[i];
    }
}

void save_vector(vector vec, const char *filename) {
    FILE *file = fopen(filename, "wb");
    CHECK(file);
    const unsigned int size = LENGTH(vec);
    fwrite(&size, sizeof(size), 1, file);
    fwrite(vec, sizeof(double), size + 1, file);
    fclose(file);
}

vector read_vector(const char *filename) {
    FILE *file = fopen(filename, "rb");
    CHECK(file);
    unsigned int size;
    if (!fread(&size, sizeof(size), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the 'size'.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    void *vec_mem = calloc(1, (size + 1) * sizeof(double) + VECTOR_SIZE_BYTE);
    CHECK(vec_mem);
    vector out;
    INIT_VECTOR(vec_mem, out, size);

    if (!fread(out, sizeof(double), size + 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the 'vec'.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return out;
}

void free_vector(vector vec) {
    void *vec_mem = (void *)((char *)vec - VECTOR_SIZE_BYTE);
    free(vec_mem);
    vec_mem = NULL;
}

void grow_vector(vector *vec, double elem) {
    CHECK(vec);
    const unsigned int new_size = LENGTH(*vec) + 1;
    void *vec_mem = (void *)((char *)*vec - VECTOR_SIZE_BYTE);
    void *new_vec_mem =
        realloc(vec_mem, (new_size) * sizeof(double) + VECTOR_SIZE_BYTE +
                             VECTOR_IDX_BYTE);
    CHECK(new_vec_mem);
    vector out;
    INIT_VECTOR(new_vec_mem, out, new_size);
    out[new_size] = elem;
    *vec = out;
}

unsigned int get_length(vector vec) { return LENGTH(vec); }

void print_vector(vector vec) {
    const int size = (int)LENGTH(vec);
    printf("[");
    for (int i = 1; i <= size; i++) {
        i != size ? printf("%g, ", vec[i]) : printf("%g", vec[i]);
    }
    printf("]\n");
}

void fprint_vector(FILE *fp, vector vec) {
    const int size = (int)LENGTH(vec);
    for (int i = 1; i <= size; i++) {
        i != size ? fprintf(fp, "%.3f, ", vec[i]) : fprintf(fp, "%.3f", vec[i]);
    }
    fprintf(fp, "\n");
}
