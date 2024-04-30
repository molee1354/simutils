#ifndef SIMUTIL_MATRIX_H
#define SIMUTIL_MATRIX_H

#include "error.h"

typedef double** matrix;

#define MATRIX_SIZE_BYTE  (size_t)(sizeof(unsigned int)*2)
#define MATRIX_ROW_OFFSET (size_t)(sizeof(double*))

/**
 * @brief Macro to access the size byte of the matrix
 *
 */
/* #define ROWS(mat) \
    *( ((unsigned int*)((char*)mat - MATRIX_SIZE_BYTE))+0 )

#define COLS(mat) \
    *( ((unsigned int*)((char*)mat - MATRIX_SIZE_BYTE))+1 ) */

#define ROWS(mat) \
    *( (char*)mat - MATRIX_SIZE_BYTE + sizeof(unsigned int)*0 )

#define COLS(mat) \
    *( (char*)mat - MATRIX_SIZE_BYTE + sizeof(unsigned int)*1 )

/**
 * @brief Macro to create a new matrix based on an existing stack-allocated
 * matrix. Assumes that there is already an existing pointer to the matrix
 * 'targ' that is the same size as the static matrix.
 *
 */
#define FROM_MATRIX(from, targ, row, col)\
    do {\
        if (ROWS(targ) != row || COLS(targ) != col)\
            raise_error(SIMUTIL_DIMENSION_ERROR,\
                    "Unmatching matrix dimensions for matrix creation!\n");\
        for (int i = 0; i < (int)row; i++) {\
            for (int j = 0; j < (int)col; j++) {\
                targ[i+1][j+1] = from[i][j];\
            }\
        }\
    } while(0)

/**
 * @brief Function to create a new matrix with a given size
 *
 * @param size The size of the new matrix
 * @return matrix Double pointer to a new matrix
 */
matrix new_matrix(unsigned int nrows, unsigned int ncols);

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
 * @param mat Vector to print
 */
void print_matrix(matrix mat);

#endif
