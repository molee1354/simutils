#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

#ifndef SIMUTIL_VECTOR_BASE_H
#include "vector.h"
#endif

/**
 * @brief Macro to create a new vector based on an existing stack-allocated
 * vector. Assumes that there is already an existing pointer to the vector
 * 'targ' that is the same size as the static vector.
 *
 */
#define FROM_VECTOR(from, _targ, _size)                                        \
    do {                                                                       \
        int size = (int)(_size);                                               \
        __typeof__(_targ) targ = (_targ);                                      \
        if (LENGTH(targ) != size)                                              \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching dimensions for vector creation!\n");       \
        for (int i = 0; i < (int)size; i++) {                                  \
            targ[i + 1] = (from)[i];                                           \
        }                                                                      \
    } while (0)

int __resize_vector(void** vec_mem, size_t new_length, size_t elem_size);

int __append_element(void** vec_mem, void* elem, size_t elem_size);

#define grow_vector(vec, elem)                                                 \
    do {                                                                       \
        if (__append_element((void**)(vec), &(__typeof__(**(vec))){elem},      \
                             sizeof(**(vec))))                                 \
            raise_error(SIMUTIL_NULL_ERROR,                                    \
                        "Received null pointer in 'grow_vector()'\n");         \
    } while (0)

#define resize_vector(vec, resize)                                             \
    do {                                                                       \
        if (__resize_vector((void**)(vec), (resize), sizeof(**(vec))))         \
            raise_error(SIMUTIL_NULL_ERROR,                                    \
                        "Received null pointer in 'resize_vector()'\n");       \
    } while (0)

#endif
