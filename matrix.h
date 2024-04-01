#ifndef SIMUTIL_MATRIX_H
#define SIMUTIL_MATRIX_H

typedef double** matrix;

#define SIZE_BYTE (size_t)(sizeof(double)*2)

/**
 * @brief Macro to access the size byte of the matrix
 *
 */
#define ROWS(mat) \
    *( ((unsigned int*)((char*)mat - SIZE_BYTE))+0 )

#define COLS(mat) \
    *( ((unsigned int*)((char*)mat - SIZE_BYTE))+1 )

/**
 * @brief Function to create a new matrix with a given size
 *
 * @param size The size of the new matrix
 * @return matrix Double pointer to a new matrix
 */
matrix make_matrix(unsigned int nrows, unsigned int ncols);

/**
 * @brief Function to properly free the memory allocated to the matrix
 *
 * @param mat Vector to free
 */
void free_matrix(matrix mat);


/**
 * @brief Function to print a matrix
 *
 * @param mat Vector to print
 */
void print_matrix(matrix mat);

/**
 * @brief Function to add an element to a matrix
 *
 * @param mat 
 * @param elem 
 */
void grow_matrix(matrix* mat, double elem);

#endif
