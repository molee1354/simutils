#ifndef SIMUTIL_VECTOR_BASE_H
#define SIMUTIL_VECTOR_BASE_H

#include "error.h"
#include "simutil_includes.h"

#define vector(T) T*

#define VECTOR_SIZE_BYTE (size_t)(sizeof(size_t) * 1)

/****************************************************************************/
/*                                                                          */
/*                        Basic Functions and Macros                        */
/*                                                                          */
/****************************************************************************/

/**
 * @brief Macro to access the size byte of the vector
 *
 */
#define LENGTH(vec) ((int)(*((size_t*)(((char*)(vec) - VECTOR_SIZE_BYTE)) + 0)))

static inline void* __init_vector(size_t size, size_t n_elem) {
    void* vec_start = calloc(1, size);
    __SIMUTIL_NULLPTR_CHECK(vec_start);
    *(((size_t*)vec_start) + 0) = n_elem;
    char* out = (char*)vec_start + VECTOR_SIZE_BYTE;
    __SIMUTIL_NULLPTR_CHECK(out);
    return (void*)out;
}

#define new_vector(T, length)                                                  \
    ((vector(T))__init_vector(                                                 \
        sizeof(T) * ((size_t)(length) + 1) + VECTOR_SIZE_BYTE, (length)))

#define free_vector(vec)                                                       \
    do {                                                                       \
        void* vec_mem = (void*)((char*)(vec) - VECTOR_SIZE_BYTE);              \
        free(vec_mem);                                                         \
        vec_mem = NULL;                                                        \
    } while (0)

#endif
