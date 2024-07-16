#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

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

void* __init_vector(size_t size, size_t elem_size, size_t n_elem);

void __resize_vector(void** vec_mem, size_t new_length, size_t elem_size);

#define new_vector(T, length)                                                  \
    ((vector(T))__init_vector(sizeof(T) * ((size_t)(length) + 1) +             \
                                  VECTOR_SIZE_BYTE,                            \
                              sizeof(T), (length)))

void __append_element(void** vec_mem, void* elem, size_t elem_size);

#define grow_vector(vec, elem)                                                 \
    do {                                                                       \
        __append_element((void**)(vec), &(__typeof__(**(vec))){elem},          \
                         sizeof(**(vec)));                                     \
    } while (0)

#define resize_vector(vec, resize)                                             \
    do {                                                                       \
        __resize_vector((void**)(vec), (resize), sizeof(**(vec)));             \
    } while (0)

#define free_vector(vec)                                                       \
    do {                                                                       \
        void* vec_mem = (void*)((char*)(vec) - VECTOR_SIZE_BYTE);              \
        free(vec_mem);                                                         \
        vec_mem = NULL;                                                        \
    } while (0)

// printing floating-point numbers
void __print_float_v(FILE* fp, vector(float) vec);
void __print_double_v(FILE* fp, vector(double) vec);
void __print_long_double_v(FILE* fp, vector(long double) vec);

// printing integers / chars
void __print_char_v(FILE* fp, vector(char) vec);
void __print_uchar_v(FILE* fp, vector(unsigned char) vec);
void __print_short_v(FILE* fp, vector(short) vec);
void __print_ushort_v(FILE* fp, vector(unsigned short) vec);
void __print_int_v(FILE* fp, vector(int) vec);
void __print_uint_v(FILE* fp, vector(unsigned int) vec);
void __print_long_v(FILE* fp, vector(long) vec);
void __print_ulong_v(FILE* fp, vector(unsigned long) vec);

#define print_vector(vec)                                                      \
    _Generic((vec),                                                            \
        vector(char): __print_char_v,                                          \
        vector(unsigned char): __print_uchar_v,                                \
        vector(short): __print_short_v,                                        \
        vector(unsigned short): __print_ushort_v,                              \
        vector(int): __print_int_v,                                            \
        vector(unsigned int): __print_uint_v,                                  \
        vector(long): __print_long_v,                                          \
        vector(unsigned long): __print_ulong_v,                                \
        vector(float): __print_float_v,                                        \
        vector(double): __print_double_v,                                      \
        vector(long double): __print_long_double_v)(stdout, vec)

#define fprint_vector(fp, vec)                                                 \
    _Generic((vec),                                                            \
        vector(char): __print_char_v,                                          \
        vector(unsigned char): __print_uchar_v,                                \
        vector(short): __print_short_v,                                        \
        vector(unsigned short): __print_ushort_v,                              \
        vector(int): __print_int_v,                                            \
        vector(unsigned int): __print_uint_v,                                  \
        vector(long): __print_long_v,                                          \
        vector(unsigned long): __print_ulong_v,                                \
        vector(float): __print_float_v,                                        \
        vector(double): __print_double_v,                                      \
        vector(long double): __print_long_double_v)(fp, vec)

#endif
