#ifndef SIMUTIL_TENSOR_H
#define SIMUTIL_TENSOR_H

#include "error.h"
#include "simutil_includes.h"

typedef double*** tensor;

#define TENSOR_SIZE_BYTE (size_t)(sizeof(unsigned int) * 3)
#define TENSOR_ROW_OFFSET (size_t)(sizeof(double*))
#define TENSOR_DEPTH_OFFSET (size_t)(sizeof(double**))

/**
 * @brief Macro to access the size byte of the tensor
 *
 */
// col

// (*( (unsigned int*)(((char*)(mat) - MATRIX_SIZE_BYTE + sizeof(unsigned int) *
// 0))))
#define DIM1(ten)                                                              \
    ((int)(*((unsigned int*)(((char*)(ten)-TENSOR_SIZE_BYTE +                  \
                              sizeof(unsigned int) * 0)))))

// row
#define DIM2(ten)                                                              \
    ((int)(*((unsigned int*)(((char*)(ten)-TENSOR_SIZE_BYTE +                  \
                              sizeof(unsigned int) * 1)))))

// dep
#define DIM3(ten)                                                              \
    ((int)(*((unsigned int*)(((char*)(ten)-TENSOR_SIZE_BYTE +                  \
                              sizeof(unsigned int) * 2)))))

/**
 * @brief Function to create a new tensor with a given size
 *
 * @param size The size of the new tensor
 * @return tensor Triple pointer to a new tensor
 */
tensor new_tensor(unsigned int ncols, unsigned int nrows, unsigned int ndeps);

/**
 * @brief Function to properly free the memory allocated to the tensor
 *
 * @param ten Tensor to free
 */
void free_tensor(tensor ten);

/**
 * @brief Function to save a tensor into a file
 *
 * @param ten
 * @param filename
 */
void save_tensor(tensor ten, const char* filename);

/**
 * @brief Function to read a saved tensor from a given filename
 *
 * @param filename
 * @return
 */
tensor read_tensor(const char* filename);

/**
 * @brief Function to print a tensor
 *
 * @param ten Tensor to print
 */
void print_tensor(tensor ten);

/**
 * @brief Function to print a tensor to a file
 *
 * @param fp File pointer
 * @param ten Tensor to print
 */
void fprint_tensor(FILE* fp, tensor ten);

#endif
