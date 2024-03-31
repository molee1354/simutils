#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

typedef double* vector;


/**
 * @brief Macro to access the size byte of the vector
 *
 */
#define LENGTH(vec) \
    (unsigned int)(*( (vector)vec-1 ))

/**
 * @brief Function to create a new vector with a given size
 *
 * @param size The size of the new vector
 * @return vector Double pointer to a new vector
 */
vector make_vector(unsigned int size);

/**
 * @brief Function to properly free the memory allocated to the vector
 *
 * @param vec Vector to free
 */
void free_vector(vector vec);


/**
 * @brief Function to print a vector
 *
 * @param vec Vector to print
 */
void print_vector(vector vec);

/**
 * @brief Function to get the length of the vector
 *
 * @param vec 
 */
unsigned int get_length(vector vec);

/**
 * @brief Function to add an element to a vector
 *
 * @param vec 
 * @param elem 
 */
void grow_vector(vector* vec, double elem);

#endif
