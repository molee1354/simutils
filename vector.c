#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "vector.h"
#include "error.h"

#define CHECK(p)\
    if (!p) {\
        raise_error(SIMUTIL_ALLOCATE_ERROR,"NULL allocation for vector!\n");\
        exit(EXIT_FAILURE);\
    }\

#define INIT_VECTOR(mem, out, size) \
    do {\
        out = (vector)( (char*)mem + VECTOR_SIZE_BYTE );\
        *( ((unsigned int*)mem)+0 ) = size;\
    } while(0)

vector new_vector(unsigned int size) {
    void* vec_mem = calloc(1, size*sizeof(double)+VECTOR_SIZE_BYTE);
    CHECK(vec_mem);
    vector out;
    INIT_VECTOR(vec_mem, out, size);
    return out;
}

vector from_vector(double* s_vector, unsigned int size) {
    void* vec_mem = calloc(1, size*sizeof(double)+VECTOR_SIZE_BYTE);
    CHECK(vec_mem);
    vector out;
    INIT_VECTOR(vec_mem, out, size);
    for (int i = 0; i < (int)size; i ++)
        out[i] = s_vector[i];
    return out;
}

void add_simd(vector vec1, vector vec2) {
    if (LENGTH(vec1) != LENGTH(vec2)) {
        raise_error(SIMUTIL_DIMENSION_ERROR, "Vector dimension mismatch.");
        exit(EXIT_FAILURE);
    }
    const int len = LENGTH(vec1);
#pragma omp for simd
    for (int i = 0; i < len; i++) {
        vec1[i] += vec2[i];
    }
}

void add(vector vec1, vector vec2) {
    if (LENGTH(vec1) != LENGTH(vec2)) {
        raise_error(SIMUTIL_DIMENSION_ERROR, "Vector dimension mismatch.");
        exit(EXIT_FAILURE);
    }
    const int len = LENGTH(vec1);
#pragma omp for simd
    for (int i = 0; i < len; i++) {
        vec1[i] += vec2[i];
    }
}

void save_vector(vector vec, const char* filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const unsigned int size = LENGTH(vec);
    fwrite(&size, sizeof(size), 1, file);
    fwrite(vec, sizeof(double), size, file);
    fclose(file);
}

vector read_vector(const char *filename) {
    FILE* file = fopen(filename, "rb");
    CHECK(file);
    unsigned int size;
    if (!fread(&size, sizeof(size), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the 'size'.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    vector out;
    void* vec_mem = calloc(1, size*sizeof(double)+VECTOR_SIZE_BYTE);
    CHECK(vec_mem);\
    INIT_VECTOR(vec_mem, out, size);
    if (!fread(out, sizeof(double), size, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the 'vec'.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return out;
}

void free_vector(vector vec) {
    void* vec_mem = (void*)( (char*)vec - VECTOR_SIZE_BYTE );
    free(vec_mem);
    vec_mem = NULL;
}

void grow_vector(vector* vec, double elem) {
    CHECK(vec);
    const unsigned int new_size = LENGTH(*vec)+1;
    void* vec_mem = (void*)( (char*)*vec - VECTOR_SIZE_BYTE );
    void* new_vec_mem = realloc(vec_mem, (new_size)*sizeof(double)+VECTOR_SIZE_BYTE);
    CHECK(new_vec_mem);
    /* vector out = (vector)( (char*)new_vec_mem + VECTOR_SIZE_BYTE );
    *(out-1) = (double)new_size; */
    vector out;
    INIT_VECTOR(new_vec_mem, out, new_size);
    out[new_size-1] = elem;
    *vec = out;
}

unsigned int get_length(vector vec) {
    return LENGTH(vec);
}

void print_vector(vector vec) {
    const int size = (int)LENGTH(vec);
    printf("[");
    for (int i = 0; i < size; i++) {
        i != size-1 ? printf("%g, ", vec[i]) : 
                      printf("%g", vec[i]);
    }
    printf("]\n");
}
