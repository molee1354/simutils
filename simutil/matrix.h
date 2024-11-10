#ifndef SIMUTIL_MATRIX_H
#define SIMUTIL_MATRIX_H

#ifndef SIMUTIL_MATRIX_BASE_H
#include "matrix_base.h"
#endif

/****************************************************************************/
/*                                                                          */
/*                            Print Definitions                             */
/*                                                                          */
/****************************************************************************/

#ifdef SIMUTIL_COL_MAJOR
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_m(FILE* fp, type mat) {                 \
        const int nrow = ROWS(mat);                                            \
        const int ncol = COLS(mat);                                            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = 1; j <= nrow; j++) {                                          \
            fprintf(fp, "[");                                                  \
            for (i = 1; i <= ncol; i++) {                                      \
                if (i != ncol) {                                               \
                    fprintf(fp, fmt, mat[i][j]);                               \
                    fprintf(fp, ", ");                                         \
                } else                                                         \
                    fprintf(fp, fmt, mat[i][j]);                               \
            }                                                                  \
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "]\n");                                                    \
    }
#else
#define PRINT_FUNC(name, type, fmt)                                            \
    static inline void __print##name##_m(FILE* fp, type mat) {                 \
        const int nrow = ROWS(mat);                                            \
        const int ncol = COLS(mat);                                            \
        fprintf(fp, "[");                                                      \
        int i, j;                                                              \
        for (j = 1; j <= nrow; j++) {                                          \
            fprintf(fp, "[");                                                  \
            for (i = 1; i <= ncol; i++) {                                      \
                if (i != ncol) {                                               \
                    fprintf(fp, fmt, mat[j][i]);                               \
                    fprintf(fp, ", ");                                         \
                } else                                                         \
                    fprintf(fp, fmt, mat[j][i]);                               \
            }                                                                  \
            (j == nrow) ? fprintf(fp, "]") : fprintf(fp, "]\n ");              \
        }                                                                      \
        fprintf(fp, "]\n");                                                    \
    }
#endif

// printing floating-point numbers
PRINT_FUNC(_float, matrix(float), "%6.3f")
PRINT_FUNC(_double, matrix(double), "%6.3f")
PRINT_FUNC(_long_double, matrix(long double), "%6.3Lf")

// printing integers / char
PRINT_FUNC(_char, matrix(char), "%c")
PRINT_FUNC(_uchar, matrix(unsigned char), "%3d")
PRINT_FUNC(_short, matrix(short), "%3hd")
PRINT_FUNC(_ushort, matrix(unsigned short), "%3hd")
PRINT_FUNC(_int, matrix(int), "%3d")
PRINT_FUNC(_uint, matrix(unsigned int), "%3u")
PRINT_FUNC(_long, matrix(long), "%3ld")
PRINT_FUNC(_ulong, matrix(unsigned long), "%3lu")

/* Function-like macros for printing numerical matrices */
#define print_matrix(mat)                                                      \
    _Generic((mat),                                                            \
        matrix(char): __print_char_m,                                          \
        matrix(unsigned char): __print_uchar_m,                                \
        matrix(short): __print_short_m,                                        \
        matrix(unsigned short): __print_ushort_m,                              \
        matrix(int): __print_int_m,                                            \
        matrix(unsigned int): __print_uint_m,                                  \
        matrix(long): __print_long_m,                                          \
        matrix(unsigned long): __print_ulong_m,                                \
        matrix(float): __print_float_m,                                        \
        matrix(double): __print_double_m,                                      \
        matrix(long double): __print_long_double_m)(stdout, mat)

#define fprint_matrix(fp, mat)                                                 \
    _Generic((mat),                                                            \
        matrix(char): __print_char_m,                                          \
        matrix(unsigned char): __print_uchar_m,                                \
        matrix(short): __print_short_m,                                        \
        matrix(unsigned short): __print_ushort_m,                              \
        matrix(int): __print_int_m,                                            \
        matrix(unsigned int): __print_uint_m,                                  \
        matrix(long): __print_long_m,                                          \
        matrix(unsigned long): __print_ulong_m,                                \
        matrix(float): __print_float_m,                                        \
        matrix(double): __print_double_m,                                      \
        matrix(long double): __print_long_double_m)(fp, mat)

/****************************************************************************/
/*                                                                          */
/*                            Macro Definitions                             */
/*                                                                          */
/****************************************************************************/

#ifdef SIMUTIL_COL_MAJOR
#define FROM_MATRIX(_from, _targ, _ncols, _nrows)                              \
    do {                                                                       \
        int cols = (int)(_nrows);                                              \
        int rows = (int)(_ncols);                                              \
        __typeof__(_targ) targ = (_targ);                                      \
        if (ROWS(targ) != _nrows || COLS(targ) != _ncols)                      \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching dimensions for vector creation!\n");       \
        for (int i = 0; i < (int)rows; i++) {                                  \
            for (int j = 0; j < (int)cols; j++) {                              \
                targ[i + 1][j + 1] = (_from)[j][i];                            \
            }                                                                  \
        }                                                                      \
    } while (0)
#else
#define FROM_MATRIX(_from, _targ, _ncols, _nrows)                              \
    do {                                                                       \
        int cols = (int)(_ncols);                                              \
        int rows = (int)(_nrows);                                              \
        __typeof__(_targ) targ = (_targ);                                      \
        if (ROWS(targ) != _nrows || COLS(targ) != _ncols)                      \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching dimensions for vector creation!\n");       \
        for (int i = 0; i < (int)rows; i++) {                                  \
            for (int j = 0; j < (int)cols; j++) {                              \
                targ[i + 1][j + 1] = (_from)[i][j];                            \
            }                                                                  \
        }                                                                      \
    } while (0)
#endif


/**
 * @brief Macro to check if the two matrices are the same shape. Evaluates to 1
 * if the matrices are equal
 *
 * @param _A Matrix 1
 * @param _B Matrix 2
 */
#define IS_SAME_SHAPE(_A, _B)                                                  \
    ((ROWS((_A)) == ROWS((_B)) && COLS((_A)) == COLS((_B))) ? (1) : (0))

/**
 * @brief Macro to check if the two matrices are not the same shape.
 * Evaluates to 1 if the matrices are not equal
 *
 * @param _A Matrix 1
 * @param _B Matrix 2
 */
#define NOT_SAME_SHAPE(_A, _B)                                                 \
    ((ROWS((_A)) != ROWS((_B)) || COLS((_A)) != COLS((_B))) ? (1) : (0))

#define IS_EQUAL(_A, _B)                                                       \
    (NOT_SAME_SHAPE((_A), (_B)) ? (0) : ({                                     \
        int equal = 1;                                                         \
        for (int i = 1; i < COLS((_A)); i++) {                                 \
            for (int j = 1; j < ROWS((_A)); j++) {                             \
                if ((_A)[i][j] != (_B)[i][j]) {                                \
                    equal = 0;                                                 \
                    break;                                                     \
                }                                                              \
            }                                                                  \
            if (!equal)                                                        \
                break;                                                         \
        }                                                                      \
        equal;                                                                 \
    }))

/**
 * @brief Macro to set two matrices to be equal.
 *
 * @param targ Target matrix to which the values will be assigned to
 * @param from Source matrix from which the values will be copied from
 */
#define ELEM_SET_EQUAL(_targ, _from)                                           \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_from) from = (_from);                                      \
        if (NOT_SAME_SHAPE(targ, from)) {                                      \
            raise_error(                                                       \
                SIMUTIL_DIMENSION_ERROR,                                       \
                "Unmatching matrix dimensions @ matrix ELEM_SET_EQUAL!\n");    \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        const int nrows = (const int)ROWS(targ);                               \
        const int ncols = (const int)COLS(targ);                               \
        for (int i = 1; i <= ncols; i++) {                                     \
            for (int j = 1; j <= nrows; j++) {                                 \
                targ[i][j] = from[i][j];                                       \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to set a matrix to a constant
 *
 * @param targ Target matrix to which the values will be assigned to
 * @param from Source matrix from which the values will be copied from
 */
#define ELEM_SET_CONST(_targ, _constant)                                       \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        double constant = (_constant);                                         \
        const int nrows = (const int)ROWS(targ);                               \
        const int ncols = (const int)COLS(targ);                               \
        for (int i = 1; i <= ncols; i++) {                                     \
            for (int j = 1; j <= nrows; j++) {                                 \
                targ[i][j] = constant;                                         \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do element wise operations '_oper'
 *
 * @param targ Target matrix to which the values will be assigned to
 * @param from Source matrix from which the values for the operand will be used
 * from
 * @param oper The basic operator to apply for every element
 */
#define ELEM_OPER(_targ, _from, _oper)                                         \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_from) from = (_from);                                      \
        if (NOT_SAME_SHAPE(targ, from)) {                                      \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching matrix dimensions @ matrix ELEM_OPER!\n"); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        const int nrows = (const int)ROWS(targ);                               \
        const int ncols = (const int)COLS(targ);                               \
        for (int i = 1; i <= ncols; i++) {                                     \
            for (int j = 1; j <= nrows; j++) {                                 \
                targ[i][j] = targ[i][j] _oper from[i][j];                      \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do element wise operations '_oper'
 *
 * @param targ Target matrix to which the values will be assigned to
 * @param lhs  The left hand side of the operator
 * @param rhs  The right hand side of the operator
 * @param oper The binary operator to apply for every element
 */
#define ELEM_OPER_TARG(_targ, _lhs, _rhs, _oper)                               \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_lhs) lhs = (_lhs);                                         \
        __typeof__(_rhs) rhs = (_rhs);                                         \
        if (NOT_SAME_SHAPE(targ, lhs) || NOT_SAME_SHAPE(targ, rhs) ||          \
            NOT_SAME_SHAPE(lhs, rhs)) {                                        \
            raise_error(                                                       \
                SIMUTIL_DIMENSION_ERROR,                                       \
                "Unmatching matrix dimensions @ matrix ELEM_OPER_TARG!\n");    \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        const int nrows = (const int)ROWS(targ);                               \
        const int ncols = (const int)COLS(targ);                               \
        for (int i = 1; i <= ncols; i++) {                                     \
            for (int j = 1; j <= nrows; j++) {                                 \
                targ[i][j] = lhs[i][j] _oper rhs[i][j];                        \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do element wise math operations on slices of vectors defined
 * by 'left', 'right', 'up', 'down'.  The result will be set to 'targ',
 * new memory will NOT bee allocated, and if slice bounds are valid, the
 * operation WILL ONLY BE DONE IN THE BOUNDS of the targ vector.
 *
 * @param targ Vector of vector* that the operation will be done to
 * @param from Vector of vector* that will be the operation reference
 * @param oper Math operation
 *
 * @param left Left bound for slice (IS included)
 * @param right Right bound for slice (IS included)
 * @param up Top bound for slice (IS included)
 * @param down Bottom bound for slice (IS included)
 *
 */
#define ELEM_OPER_SLICE(_targ, _from, _oper, _l, _r, _u, _d)                   \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_from) from = (_from);                                      \
        int l = (_l);                                                          \
        int r = (_r);                                                          \
        int u = (_u);                                                          \
        int d = (_d);                                                          \
        if (r - l > (int)COLS(targ) || r - l > (int)COLS(from) ||              \
            d - u > (int)ROWS(targ) || d - u > (int)ROWS(from)) {              \
            raise_error(                                                       \
                SIMUTIL_DIMENSION_ERROR,                                       \
                "Unmatching matrix dimensions @ matrix ELEM_OPER_SLICE!\n");   \
            fprintf(stderr, "\t   targ : [%d, %d]\n", COLS(targ), ROWS(targ)); \
            fprintf(stderr, "\t   from : [%d, %d]\n", COLS(from), ROWS(from)); \
            fprintf(stderr, "\tl,r,u,d : %d,%d,%d,%d\n", l, r, u, d);          \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        for (int j = u; j <= d; j++) {                                         \
            for (int i = l; i <= r; i++) {                                     \
                double a = targ[i][j];                                         \
                double b = from[i][j];                                         \
                targ[i][j] = a _oper b;                                        \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do element wise math operations on slices of vectors defined
 * by a reference vector 'like'. 'like' has to be smaller than or equal to
 * 'targ'
 *
 * @param targ Vector of vector* that the operation will be done to
 * @param from Vector of vector* that will be the operation reference
 * @param op Math operation
 * @param like Refernce vector for slice
 *
 */
#define ELEM_OPER_SLICE_LIKE(_targ, _from, _oper, _like)                       \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_from) from = (_from);                                      \
        __typeof__(_like) like = (_like);                                      \
        if (ROWS(like) > ROWS(targ) || COLS(like) > COLS(targ)) {              \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching matrix dimensions @ matrix "               \
                        "ELEM_OPER_SLICE_LIKE!\n");                            \
            fprintf(stderr, "\t   targ : [%d, %d]\n", COLS(targ), ROWS(targ)); \
            fprintf(stderr, "\t   like : [%d, %d]\n", COLS(like), ROWS(like)); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        const int ncols = (const int)COLS(like);                               \
        const int nrows = (const int)ROWS(like);                               \
        for (int j = 1; j <= nrows; j++) {                                     \
            for (int i = 1; i <= ncols; i++) {                                 \
                double a = targ[i][j];                                         \
                double b = from[i][j];                                         \
                targ[i][j] = a _oper b;                                        \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do constant math operations on matrices. The result
 * will be set to 'targ', new memory will NOT be allocated.
 * Zero division checks aren't happening
 *
 * @param targ Vector of vector* that the operation will be done to
 * @param constant Constant that will be used in operation
 * @param op Math operation
 */
#define CONST_OPER(_targ, _constant, _oper)                                    \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        const int ncols = (const int)COLS(targ);                               \
        const int nrows = (const int)ROWS(targ);                               \
        const double constant = (double)(_constant);                           \
        for (int j = 1; j <= nrows; j++) {                                     \
            for (int i = 1; i <= ncols; i++) {                                 \
                double a = targ[i][j];                                         \
                targ[i][j] = a _oper constant;                                 \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do constant math operations on slices of matrices defined
 * by 'left', 'right', 'up', 'down'.  The result will be set to 'targ',
 * new memory will NOT be allocated, and if slice bounds are valid, the
 * operation WILL ONLY BE DONE IN THE BOUNDS of the targ vector.
 *
 * @param targ Matrix of matrix that the operation will be done to
 * @param from Matrix of matrix that will be the operation reference
 * @param oper Math operation
 *
 * @param left Left bound for slice (IS included)
 * @param right Right bound for slice (IS included)
 * @param up Top bound for slice (IS included)
 * @param down Bottom bound for slice (IS included)
 *
 */
#define CONST_OPER_SLICE(_targ, _constant, _oper, _l, _r, _u, _d)              \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        const double constant = (double)(_constant);                           \
        int l = (_l);                                                          \
        int r = (_r);                                                          \
        int u = (_u);                                                          \
        int d = (_d);                                                          \
        if (r - l > (int)COLS(targ) || r - l > (int)COLS(from) ||              \
            d - u > (int)ROWS(targ) || d - u > (int)ROWS(from)) {              \
            raise_error(                                                       \
                SIMUTIL_DIMENSION_ERROR,                                       \
                "Unmatching matrix dimensions @ matrix CONST_OPER_SLICE!\n");  \
            fprintf(stderr, "\t   targ : [%d, %d]\n", COLS(targ), ROWS(targ)); \
            fprintf(stderr, "\tl,r,u,d : %d,%d,%d,%d\n", l, r, u, d);          \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        for (int j = u; j <= d; j++) {                                         \
            for (int i = l; i <= r; i++) {                                     \
                double a = targ[i][j];                                         \
                targ[i][j] = a _oper constant;                                 \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Macro to do constant math operations on slices of matrices defined
 * by a reference matrix 'like'. 'like' has to be smaller than or equal to
 * 'targ'
 *
 * @param targ Vector of vector* that the operation will be done to
 * @param from Vector of vector* that will be the operation reference
 * @param op Math operation
 * @param like Refernce vector for slice
 */
#define CONST_OPER_SLICE_LIKE(_targ, _constant, _oper, _like)                  \
    do {                                                                       \
        __typeof__(_targ) targ = (_targ);                                      \
        __typeof__(_like) like = (_like);                                      \
        const double constant = (double)(_constant);                           \
        if (ROWS(like) > ROWS(targ) || COLS(like) > COLS(targ)) {              \
            raise_error(SIMUTIL_DIMENSION_ERROR,                               \
                        "Unmatching matrix dimensions @ matrix "               \
                        "CONST_OPER_SLICE_LIKE!\n");                           \
            fprintf(stderr, "\t   targ : [%d, %d]\n", COLS(targ), ROWS(targ)); \
            fprintf(stderr, "\t   like : [%d, %d]\n", COLS(like), ROWS(like)); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        const int ncols = (const int)COLS(like);                               \
        const int nrows = (const int)ROWS(like);                               \
        for (int j = 1; j <= nrows; j++) {                                     \
            for (int i = 1; i <= ncols; i++) {                                 \
                double a = targ[i][j];                                         \
                targ[i][j] = a _oper constant;                                 \
            }                                                                  \
        }                                                                      \
    } while (0)

#endif
