#include "vector.h"
#include "error.h"
#include <string.h>

#define CHECK(p)                                                               \
    do {                                                                       \
        if (!p) {                                                              \
            raise_error(SIMUTIL_ALLOCATE_ERROR,                                \
                        "NULL allocation for vector!\n");                      \
        }                                                                      \
    } while (0)

void* __init_vector(size_t size, size_t n_elem) {
    void* vec_start = calloc(1, size);
    CHECK(vec_start);
    *(((size_t*)vec_start) + 0) = n_elem;
    char* out = (char*)vec_start + VECTOR_SIZE_BYTE;
    CHECK(out);
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

void __resize_vector(void** vec_mem, size_t new_length, size_t elem_size) {
    CHECK(*vec_mem);
    // const size_t old_length = LENGTH(*vec_mem);
    void* vec_start = (void*)((char*)*vec_mem - VECTOR_SIZE_BYTE);
    void* vec_start_new = realloc(vec_start, new_length * elem_size +
                                                 VECTOR_SIZE_BYTE + elem_size);
    CHECK(vec_start_new);
    *(((size_t*)vec_start_new) + 0) = new_length;
    /* for (int i = 1; i != new_length;) {

    } */
    /* memcpy((void*)((char*)vec_start_new + new_length * elem_size +
                   VECTOR_SIZE_BYTE),
           elem, elem_size); */
    char* out = (char*)vec_start_new;
    *(vec_mem) = (void*)(out + VECTOR_SIZE_BYTE);
}

#define PRINT_FUNC(name, type, fmt)                                            \
    void __print##name##_v(FILE* fp, type vec) {                               \
        const int length = LENGTH(vec);                                        \
        if (fp == stdout || fp == stderr)                                      \
            fprintf(fp, "[");                                                  \
        for (int i = 1; i <= length; i++) {                                    \
            if (i != length) {                                                 \
                fprintf(fp, fmt, vec[i]);                                      \
                fprintf(fp, ", ");                                             \
            } else                                                             \
                fprintf(fp, fmt, vec[i]);                                      \
        }                                                                      \
        if (fp == stdout || fp == stderr)                                      \
            fprintf(fp, "]\n");                                                \
        else                                                                   \
            fprintf(fp, "\n");                                                 \
    }

// printing floating-point numbers
PRINT_FUNC(_float, vector(float), "%6.3f")
PRINT_FUNC(_double, vector(double), "%6.3f")
PRINT_FUNC(_long_double, vector(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, vector(char), "%c")
PRINT_FUNC(_uchar, vector(unsigned char), "%3d")
PRINT_FUNC(_short, vector(short), "%3hd")
PRINT_FUNC(_ushort, vector(unsigned short), "%3hd")
PRINT_FUNC(_int, vector(int), "%3d")
PRINT_FUNC(_uint, vector(unsigned int), "%3u")
PRINT_FUNC(_long, vector(long), "%3ld")
PRINT_FUNC(_ulong, vector(unsigned long), "%3lu")
