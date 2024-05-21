#include "error.h"
#include <stdarg.h>

static void error_type(error_t err) {
    switch (err) {
    case SIMUTIL_DIMENSION_ERROR: {
        fprintf(stderr, "\n\033[1;31mSIMUTIL DIMENSION ERROR:\033[0m\n");
        break;
    }
    case SIMUTIL_ALLOCATE_ERROR: {
        fprintf(stderr, "\n\033[1;31mSIMUTIL ALLOCATE ERROR:\033[0m\n");
        break;
    }
    default: {
        fprintf(stderr, "\n\033[1;31mSIMUTIL UNDEFINED ERROR:\033[0m\n");
        break;
    }
    }
}

void raise_error(error_t err, const char *format, ...) {
    va_list args;
    va_start(args, format);
    error_type(err);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
}
