#include "vector.h"
#include <string.h>

#define CHECK(p)                                                               \
    if (!p) {                                                                  \
        raise_error(SIMUTIL_ALLOCATE_ERROR, "NULL allocation for vector!\n");  \
        exit(EXIT_FAILURE);                                                    \
    }

void* __init_vector(size_t size, size_t n_elem) {
    void* vec_start = calloc(1, size);
    CHECK(vec_start);
    *(((size_t*)vec_start) + 0) = n_elem;
    char* out = (char*)vec_start + VECTOR_SIZE_BYTE;
    return (void*)out;
}

/* void save_vector(vector vec, const char* filename) {
    FILE* file = fopen(filename, "wb");
    CHECK(file);
    const unsigned int size = LENGTH(vec);
    fwrite(&size, sizeof(size), 1, file);
    fwrite(vec, sizeof(double), size + 1, file);
    fclose(file);
} */

/* vector read_vector(const char* filename) {
    FILE* file = fopen(filename, "rb");
    CHECK(file);
    unsigned int size;
    if (!fread(&size, sizeof(size), 1, file)) {
        raise_error(SIMUTIL_ALLOCATE_ERROR, "Problem reading the 'size'.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    void* vec_mem = calloc(1, (size + 1) * sizeof(double) + VECTOR_SIZE_BYTE);
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
} */

void __append_element(void** vec_mem, void* elem, size_t elem_size) {
    CHECK(*vec_mem);
    const int new_length = LENGTH(*vec_mem) + 1;
    void* vec_start = (void*)((char*)*vec_mem - VECTOR_SIZE_BYTE);
    void* vec_start_new = realloc(vec_start, new_length * elem_size +
                                                 VECTOR_SIZE_BYTE + elem_size);
    CHECK(vec_start_new);
    *(((size_t*)vec_start_new) + 0) = new_length;
    memcpy((void*)((char*)vec_start_new + new_length * elem_size +
                   VECTOR_SIZE_BYTE),
           elem, elem_size);
    char* out = (char*)vec_start_new;
    *(vec_mem) = (void*)(out + VECTOR_SIZE_BYTE);
}

#define CAST(type) type value = *((type*)element)

#define PRINT_FUNC(name, type, fmt)                                            \
    static void print_##name(FILE* fp, void* element) {                        \
        CAST(type);                                                            \
        fprintf(fp, fmt, value);                                               \
    }

PRINT_FUNC(f32, float, "%.3f")
PRINT_FUNC(f64, double, "%.3f")
PRINT_FUNC(f128, long double, "%.3Lf")

void __print_dvector(FILE* fp, void* vec_mem, size_t elem_size) {
    const int size = (int)LENGTH(vec_mem);
    void (*print_function)(FILE*, void*) = NULL;
    if (elem_size == 4) {
        print_function = print_f32;
    } else if (elem_size == 8) {
        print_function = print_f64;
    } else if (elem_size == 16) {
        print_function = print_f128;
    }
    if (fp == stdout || fp == stderr)
        fprintf(fp, "[");
    for (int i = 1; i <= size; i++) {
        print_function(fp, (char*)vec_mem + i * elem_size);
        if (i != size)
            fprintf(fp, ", ");
    }
    if (fp == stdout || fp == stderr)
        fprintf(fp, "]\n");
}

PRINT_FUNC(i8, char, "%c")
PRINT_FUNC(i16, short, "%hi")
PRINT_FUNC(i32, int, "%d")
PRINT_FUNC(i64, long, "%ld")

void __print_ivector(FILE* fp, void* vec_mem, size_t elem_size) {
    const int size = (int)LENGTH(vec_mem);
    void (*print_function)(FILE*, void*) = NULL;
    if (elem_size == 1) {
        print_function = print_i8;
    } else if (elem_size == 2) {
        print_function = print_i16;
    } else if (elem_size == 4) {
        print_function = print_i32;
    } else if (elem_size == 8) {
        print_function = print_i64;
    }
    if (fp == stdout || fp == stderr)
        fprintf(fp, "[");
    for (int i = 1; i <= size; i++) {
        print_function(fp, (char*)vec_mem + i * elem_size);
        if (i != size)
            fprintf(fp, ", ");
    }
    if (fp == stdout || fp == stderr)
        fprintf(fp, "]\n");
}
