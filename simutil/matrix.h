#ifndef SIMUTIL_MATRIX_H
#define SIMUTIL_MATRIX_H

#include <omp.h>

#include "error.h"
#include "simutil_includes.h"

typedef double** matrix;

#define MATRIX_SIZE_BYTE (size_t)(sizeof(unsigned int) * 2)
#define MATRIX_ROW_OFFSET (size_t)(sizeof(double*))

/****************************************************************************/
/*                                                                          */
/*                        Basic Functions and Macros                        */
/*                                                                          */
/****************************************************************************/

/**
 * @brief Macro to access the size byte of the matrix
 *
 */
//  (*( (unsigned int*)(((char*)(vec) - VECTOR_SIZE_BYTE))+0 ))
#define COLS(mat)                                                              \
    ((int)(*((unsigned int*)(((char*)(mat)-MATRIX_SIZE_BYTE +                  \
                              sizeof(unsigned int) * 0)))))

#define ROWS(mat)                                                              \
    ((int)(*((unsigned int*)(((char*)(mat)-MATRIX_SIZE_BYTE +                  \
                              sizeof(unsigned int) * 1)))))

/**
 * @brief Macro to create a new matrix based on an existing stack-allocated
 * matrix. Assumes that there is already an existing pointer to the matrix
 * 'targ' that is the same size as the static matrix.
 *
 */
#define FROM_MATRIX(from, _targ, _col, _row)                                   \
    do {                                                                       \
        const int row = (const int)(_row);                                     \
        const int col = (const int)(_col);                                     \
        matrix targ = (_targ);                                                 \
        if (ROWS(targ) != row || COLS(targ) != col)                            \
            raise_error(                                                       \
                SIMUTIL_DIMENSION_ERROR,                                       \
                "Unmatching matrix dimensions for matrix creation!\n");        \
        for (int i = 0; i < row; i++) {                                        \
            for (int j = 0; j < col; j++) {                                    \
                targ[j + 1][i + 1] = (from)[i][j];                             \
            }                                                                  \
        }                                                                      \
    } while (0)

/**
 * @brief Function to create a new matrix with a given size
 *
 * @param size The size of the new matrix
 * @return matrix Double pointer to a new matrix
 */
matrix new_matrix(unsigned int ncols, unsigned int nrows);
// matrix new_matrix(unsigned int ncols, unsigned int nrows);

/**
 * @brief Function to properly free the memory allocated to the matrix
 *
 * @param mat Vector to free
 */
void free_matrix(matrix mat);

/**
 * @brief Function to save a matrix into a file
 *
 * @param mat
 * @param filename
 */
void save_matrix(matrix mat, const char* filename);

/**
 * @brief Function to read a saved matrix from a given filename
 *
 * @param filename
 * @return
 */
matrix read_matrix(const char* filename);

/**
 * @brief Function to print a matrix
 *
 * @param mat Matrix to print
 */
void print_matrix(matrix mat);

/**
 * @brief Function to print a matrix to a file pointer
 *
 * @param mat Matrix to print
 */
void fprint_matrix(FILE* fp, matrix mat);

/****************************************************************************/
/*                                                                          */
/*                            Macro Definitions                             */
/*                                                                          */
/****************************************************************************/

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

#define PARALLEL_FOR _Pragma("omp parallel for schedule(static,256)")

/**
 * @brief Macro to set two matrices to be equal.
 *
 * @param targ Target matrix to which the values will be assigned to
 * @param from Source matrix from which the values will be copied from
 */
#define ELEM_SET_EQUAL(_targ, _from)                                           \
    do {                                                                       \
        matrix targ = (_targ);                                                 \
        matrix from = (_from);                                                 \
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
        matrix targ = (_targ);                                                 \
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
        matrix targ = (_targ);                                                 \
        matrix from = (_from);                                                 \
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
        matrix targ = (_targ);                                                 \
        matrix lhs = (_lhs);                                                   \
        matrix rhs = (_rhs);                                                   \
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
        matrix targ = (_targ);                                                 \
        matrix from = (_from);                                                 \
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
        matrix targ = (_targ);                                                 \
        matrix from = (_from);                                                 \
        matrix like = (_like);                                                 \
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
        matrix targ = (_targ);                                                 \
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
        matrix targ = (_targ);                                                 \
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
        matrix targ = (_targ);                                                 \
        matrix like = (_like);                                                 \
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
