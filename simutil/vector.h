#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

#include "error.h"
#include "simutil_includes.h"

#define vector(T) T*

#define VECTOR_SIZE_BYTE (size_t)(sizeof(size_t) * 1)

/**
 * @brief Macro to access the size byte of the vector
 *
 */
#define LENGTH(vec) ((int)(*((size_t*)(((char*)(vec) - VECTOR_SIZE_BYTE)) + 0)))

/**
 * @brief Macro to create a new vector based on an existing stack-allocated
 * vector. Assumes that there is already an existing pointer to the vector
 * 'targ' that is the same size as the static vector.
 *
 */
/* #define FROM_VECTOR(from, _targ, _size) \
    do {                                                                       \
        int size = (int)(_size);                                               \
        vector targ = (_targ);                                                 \
        if (LENGTH(targ) != size)                                              \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching dimensions for vector creation!\n");       \
        for (int i = 0; i < (int)size; i++) {                                  \
            targ[i + 1] = (from)[i];                                           \
        }                                                                      \
    } while (0) */

void* __init_vector(size_t size, size_t n_elem);

#define new_vector(T, length)                                                  \
    ((vector(T))__init_vector(                                                 \
        sizeof(T) * ((size_t)(length) + 1) + VECTOR_SIZE_BYTE, (length)))

void __append_element(void** vec_mem, void* elem, size_t elem_size);

#define grow_vector(vec, elem)                                                 \
    do {                                                                       \
        __append_element((void**)(vec), &(__typeof__(**(vec))){elem},          \
                         sizeof(**(vec)));                                     \
    } while (0)

#define free_vector(vec)                                                       \
    do {                                                                       \
        void* vec_mem = (void*)((char*)(vec) - VECTOR_SIZE_BYTE);              \
        free(vec_mem);                                                         \
        vec_mem = NULL;                                                        \
    } while (0)

void __print_dvector(FILE* fp, void* vec_mem, size_t elem_size);
void __print_ivector(FILE* fp, void* vec_mem, size_t elem_size);

#define print_vector(vec)                                                      \
    _Generic((vec),                                                            \
        vector(char): __print_ivector,                                         \
        vector(short): __print_ivector,                                        \
        vector(int): __print_ivector,                                          \
        vector(unsigned int): __print_ivector,                                 \
        vector(long): __print_ivector,                                         \
        vector(unsigned long): __print_ivector,                                \
        vector(float): __print_dvector,                                        \
        vector(double): __print_dvector,                                       \
        vector(long double): __print_dvector)(stdout, (void*)vec,              \
                                              sizeof(*(vec)))

#define fprint_vector(fp, vec)                                                 \
    _Generic((vec),                                                            \
        vector(char): __print_ivector,                                         \
        vector(short): __print_ivector,                                        \
        vector(int): __print_ivector,                                          \
        vector(unsigned int): __print_ivector,                                 \
        vector(long): __print_ivector,                                         \
        vector(unsigned long): __print_ivector,                                \
        vector(float): __print_dvector,                                        \
        vector(double): __print_dvector,                                       \
        vector(long double): __print_dvector)((fp), (void*)vec,                \
                                              sizeof(*(vec)))

/**
 * @brief Function to save a vector
 *
 * @param vec
 */
// void save_vector(vector vec, const char* filename);

/**
 * @brief Function to read the saved vector from a file
 *
 * @param filename
 */
// vector read_vector(const char* filename);

/**
 * @brief Function to get the length of the vector
 *
 * @param vec
 */
// unsigned int get_length(vector vec);

#endif
