#ifndef SIMUTIL_ERROR_H
#define SIMUTIL_ERROR_H

#include <stdlib.h>

typedef enum {
    SIMUTIL_DIMENSION_ERROR,
    SIMUTIL_ALLOCATE_ERROR,
    SIMUTIL_TYPE_ERROR,
    SIMUTIL_NULL_ERROR,
    SIMUTIL_DEFAULT_ERROR
} error_t;

void raise_error(error_t err, const char* format, ...);

#define __SIMUTIL_NULLPTR_CHECK(p)                                             \
    do {                                                                       \
        if (!p) {                                                              \
            raise_error(SIMUTIL_ALLOCATE_ERROR,                                \
                        "NULL allocation for matrix!\n");                      \
            return NULL;                                                       \
        }                                                                      \
    } while (0)

#endif
