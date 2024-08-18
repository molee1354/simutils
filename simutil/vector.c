#include "vector.h"
#include "error.h"
#include <string.h>

#define __VECTOR_NULLCHECK(p)                                                  \
    do {                                                                       \
        if (!p) {                                                              \
            raise_error(SIMUTIL_ALLOCATE_ERROR,                                \
                        "Pointer passed in is NULL!\n");                       \
            p = NULL;                                                          \
            return 1;                                                          \
        }                                                                      \
    } while (0)

int __append_element(void** vec_mem, void* elem, size_t elem_size) {
    __VECTOR_NULLCHECK(*vec_mem);
    __VECTOR_NULLCHECK(vec_mem);
    const int new_length = LENGTH(*vec_mem) + 1;
    void* vec_start = (void*)((char*)*vec_mem - VECTOR_SIZE_BYTE);
    void* vec_start_new = realloc(vec_start, new_length * elem_size +
                                                 VECTOR_SIZE_BYTE + elem_size);
    __VECTOR_NULLCHECK(vec_start_new);
    *(((size_t*)vec_start_new) + 0) = new_length;
    memcpy((void*)((char*)vec_start_new + new_length * elem_size +
                   VECTOR_SIZE_BYTE),
           elem, elem_size);
    char* out = (char*)vec_start_new;
    *(vec_mem) = (void*)(out + VECTOR_SIZE_BYTE);
    return 0;
}

int __resize_vector(void** vec_mem, size_t new_length, size_t elem_size) {
    __VECTOR_NULLCHECK(*vec_mem);
    __VECTOR_NULLCHECK(vec_mem);
    void* vec_start = (void*)((char*)*vec_mem - VECTOR_SIZE_BYTE);
    void* vec_start_new = realloc(vec_start, new_length * elem_size +
                                                 VECTOR_SIZE_BYTE + elem_size);
    __VECTOR_NULLCHECK(vec_start_new);
    *(((size_t*)vec_start_new) + 0) = new_length;
    char* out = (char*)vec_start_new;
    *(vec_mem) = (void*)(out + VECTOR_SIZE_BYTE);
    return 0;
}
