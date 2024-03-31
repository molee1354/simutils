#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

#define CHECK(p)\
    if (!p) {\
        fprintf(stderr, "Function received a NULL pointer instead of a vector!\n");\
        exit(EXIT_FAILURE);\
    }\

vector make_vector(unsigned int size) {
    size_t size_byte = sizeof(double);
    void* vec_mem = calloc(1, size*sizeof(double)+size_byte);
    CHECK(vec_mem);
    vector out = (vector)( (char*)vec_mem + size_byte );
    *(out-1) = (double)size;
    return out;
}

void free_vector(vector vec) {
    size_t size_byte = sizeof(double);
    void* vec_mem = (void*)( (char*)vec - size_byte );
    free(vec_mem);
}

void grow_vector(vector* vec, double elem) {
    CHECK(vec);
    size_t size_byte = sizeof(double);
    unsigned int new_size = LENGTH(*vec)+1;
    void* vec_mem = (void*)( (char*)*vec - size_byte );
    void* new_vec_mem = realloc(vec_mem, (new_size)*sizeof(double)+size_byte);
    CHECK(new_vec_mem);
    vector out = (vector)( (char*)new_vec_mem + size_byte );
    *(out-1) = (double)new_size;
    out[new_size-1] = elem;
    *vec = out;
}

unsigned int get_length(vector vec) {
    return LENGTH(vec);
}

void print_vector(vector vec) {
    int size = (int)LENGTH(vec);
    printf("{");
    for (int i = 0; i < size; i++) {
        i != size-1 ? printf("%g, ", vec[i]) : 
                      printf("%g", vec[i]);
    }
    printf("}\n");
} 
