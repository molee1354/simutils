#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "error.h"

#define CHECK(p)\
    if (!p) {\
        raise_error(SIMUTIL_ALLOCATE_ERROR,"NULL allocation for vector!\n");\
        exit(EXIT_FAILURE);\
    }\

vector make_vector(unsigned int size) {
    void* vec_mem = calloc(1, size*sizeof(double)+SIZE_BYTE);
    CHECK(vec_mem);
    vector out = (vector)( (char*)vec_mem + SIZE_BYTE );
    *(out-1) = (double)size;
    return out;
}

void free_vector(vector vec) {
    void* vec_mem = (void*)( (char*)vec - SIZE_BYTE );
    free(vec_mem);
}

void grow_vector(vector* vec, double elem) {
    CHECK(vec);
    unsigned int new_size = LENGTH(*vec)+1;
    void* vec_mem = (void*)( (char*)*vec - SIZE_BYTE );
    void* new_vec_mem = realloc(vec_mem, (new_size)*sizeof(double)+SIZE_BYTE);
    CHECK(new_vec_mem);
    vector out = (vector)( (char*)new_vec_mem + SIZE_BYTE );
    *(out-1) = (double)new_size;
    out[new_size-1] = elem;
    *vec = out;
}

unsigned int get_length(vector vec) {
    return LENGTH(vec);
}

void print_vector(vector vec) {
    int size = (int)LENGTH(vec);
    printf("[");
    for (int i = 0; i < size; i++) {
        i != size-1 ? printf("%g, ", vec[i]) : 
                      printf("%g", vec[i]);
    }
    printf("]\n");
} 
