#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define vector(T) T *

#define __VECTOR_SIZE_BYTE (size_t)(sizeof(size_t) * 1)

#ifdef SIMUTIL_VECTOR_START_IDX_1
#define __VECTOR_START_IDX 1
#else
#define __VECTOR_START_IDX 0
#endif

/******************************************************************************/
/*                                                                            */
/*                          Basic Functions and Macros                        */
/*                                                                            */
/******************************************************************************/

// Macro to access the size byte of the vector
#define __VECTOR_LENGTH(__vptr)                                                \
    ((int)(*((size_t *)(((char *)(__vptr) - __VECTOR_SIZE_BYTE)) + 0)))

// User-end macro to access vector size
#define size_vector(__vptr) __VECTOR_LENGTH(__vptr)
#define length_vector(__vptr) __VECTOR_LENGTH(__vptr)

/******************************************************************************/
/*                                                                            */
/*                             Internal Functions                             */
/*                                                                            */
/******************************************************************************/

// Internal function to allocate memory and store the size of the vector
void *__init_vector(size_t size, size_t n_elem);
#ifdef SIMUTIL_VECTOR_IMPLEMENTATION
void *__init_vector(size_t size, size_t n_elem) {
    void *vec_start = calloc(1, size);
    if (!vec_start)
        return NULL;
    *(((size_t *)vec_start) + 0) = n_elem;
    char *out = (char *)vec_start + __VECTOR_SIZE_BYTE;
    if (!out)
        return NULL;
    return (void *)out;
}
#endif

// Internal function to resize vector
int __resize_vector(void **vec_mem, size_t new_length, size_t elem_size);

#ifdef SIMUTIL_VECTOR_IMPLEMENTATION
int __resize_vector(void **vec_mem, size_t new_length, size_t elem_size) {
    if (!(*vec_mem))
        return 1;
    if (!vec_mem)
        return 1;
    void *vec_start = (void *)((char *)*vec_mem - __VECTOR_SIZE_BYTE);
    void *vec_start_new =
        realloc(vec_start, new_length * elem_size + __VECTOR_SIZE_BYTE +
                               (elem_size * __VECTOR_START_IDX));
    if (!vec_start_new)
        return 1;
    *(((size_t *)vec_start_new) + 0) = new_length;
    char *out = (char *)vec_start_new;
    *(vec_mem) = (void *)(out + __VECTOR_SIZE_BYTE);
    return 0;
}
#endif

// Internal function to add an element to a vector
int __append_element(void **vec_mem, void *elem, size_t elem_size,
                     const int start_idx);
#ifdef SIMUTIL_VECTOR_IMPLEMENTATION
int __append_element(void **vec_mem, void *elem, size_t elem_size,
                     const int start_idx) {
    if (!(*vec_mem))
        return 1;
    if (!vec_mem)
        return 1;
    const int new_length = __VECTOR_LENGTH(*vec_mem) + 1;
    void *vec_start = (void *)((char *)*vec_mem - __VECTOR_SIZE_BYTE);
    void *vec_start_new =
        realloc(vec_start, new_length * elem_size + __VECTOR_SIZE_BYTE +
                               (elem_size * __VECTOR_START_IDX));
    if (!vec_start_new)
        return 1;
    *(((size_t *)vec_start_new) + 0) = new_length;

    memcpy((void *)((char *)vec_start_new + __VECTOR_SIZE_BYTE +
                    (new_length - 1 + start_idx) * elem_size),
           elem, elem_size);

    char *out = (char *)vec_start_new;
    *(vec_mem) = (void *)(out + __VECTOR_SIZE_BYTE);
    return 0;
}
#endif

/******************************************************************************/
/*                                                                            */
/*                                User-end Macros                             */
/*                                                                            */
/******************************************************************************/

// User-end macro to create a new vector
#define new_vector(T, __vlen)                                                  \
    ((vector(T))__init_vector(                                                 \
        sizeof(T) * ((size_t)(__vlen) + __VECTOR_START_IDX) +                  \
            __VECTOR_SIZE_BYTE,                                                \
        (__vlen)))

// User-end macro to free a vector
#define free_vector(__vptr)                                                    \
    do {                                                                       \
        void *__vptr_mem = (void *)((char *)(__vptr) - __VECTOR_SIZE_BYTE);    \
        free(__vptr_mem);                                                      \
        __vptr_mem = NULL;                                                     \
    } while (0)

// User-end macro to add an element to the end of a vector
#define grow_vector(vec, elem)                                                 \
    do {                                                                       \
        if (__append_element((void **)(vec), &(__typeof__(**(vec))){elem},     \
                             sizeof(**(vec)), (const int)__VECTOR_START_IDX))  \
            fprintf(stderr, "Received null pointer from \'grow_vector()\'\n"); \
    } while (0)

// User-end macro to resize a vector
#define resize_vector(vec, resize)                                             \
    do {                                                                       \
        if (__resize_vector((void **)(vec), (resize), sizeof(**(vec))))        \
            fprintf(stderr,                                                    \
                    "Received null pointer from \'resize_vector()\'\n");       \
    } while (0)

/******************************************************************************/
/*                                                                            */
/*                               Printing Vectors                             */
/*                                                                            */
/******************************************************************************/

#ifdef SIMUTIL_VECTOR_PRINT
// macro to generate printing functions
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_v(FILE *fp, type vec) {                 \
        const size_t length = __VECTOR_LENGTH(vec) + __VECTOR_START_IDX - 1;   \
        if (fp == stdout || fp == stderr)                                      \
            fprintf(fp, "[");                                                  \
        for (size_t i = __VECTOR_START_IDX; i <= length; i++) {                \
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

#undef PRINT_FUNC
#endif

#endif
