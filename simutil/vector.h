#ifndef SIMUTIL_VECTOR_BASE_H
#define SIMUTIL_VECTOR_BASE_H

#include "error.h"
#include "simutil_includes.h"

#define vector(T) T*

#define VECTOR_SIZE_BYTE (size_t)(sizeof(size_t) * 1)

// Be default, start index is 0
#define VECTOR_START_IDX 0

#ifdef SIMUTIL_VECTOR_START_IDX_1
#define VECTOR_START_IDX 1
#else
#define VECTOR_START_IDX 0
#endif


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

#define size_vector(vec) LENGTH(vec)
#define length_vector(vec) LENGTH(vec)

void* __init_vector(size_t size, size_t n_elem);

#define new_vector(T, length)                                                  \
    ((vector(T))__init_vector(                                                 \
        sizeof(T) * ((size_t)(length) + VECTOR_START_IDX) + VECTOR_SIZE_BYTE,  \
        (length)))

#define free_vector(vec)                                                       \
    do {                                                                       \
        void* vec_mem = (void*)((char*)(vec) - VECTOR_SIZE_BYTE);              \
        free(vec_mem);                                                         \
        vec_mem = NULL;                                                        \
    } while (0)

#endif
