#ifndef SIMUTIL_ERROR_H
#define SIMUTIL_ERROR_H

#include <stdlib.h>

typedef enum {
    SIMUTIL_DIMENSION_ERROR,
    SIMUTIL_ALLOCATE_ERROR,
    SIMUTIL_TYPE_ERROR,
} error_t;

void raise_error(error_t err, const char* format, ...);

#endif
