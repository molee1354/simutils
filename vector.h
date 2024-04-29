#ifndef SIMUTIL_VECTOR_H
#define SIMUTIL_VECTOR_H

typedef double* vector;

#define VECTOR_SIZE_BYTE (size_t)(sizeof(unsigned int)*1)

#define VECTOR_IDX_BYTE  (size_t)(sizeof(vector)*1)

/**
 * @brief Macro to access the size byte of the vector
 *
 */
/* #define LENGTH(vec) \
    (unsigned int)(*( (vector)vec-1 )) */
#define LENGTH(vec) \
    *( (unsigned int*)(((char*)vec - VECTOR_SIZE_BYTE))+0 ) 

/**
 * @brief Macro to create a new vector based on an existing stack-allocated
 * vector. Assumes that there is already an existing pointer to the vector
 * 'targ' that is the same size as the static vector.
 *
 */
#define FROM_VECTOR(from, targ, size)\
    do {\
        if (LENGTH(targ) != size)\
            raise_error(SIMUTIL_DIMENSION_ERROR,\
                    "Unmatching dimensions for vector creation!\n");\
        for (int i = 0; i < (int)size; i++) {\
            targ[i+1] = from[i];\
        }\
    } while(0)


void add(vector vec1, vector vec2);
void add_simd(vector vec1, vector vec2);

/**
 * @brief Function to create a new vector with a given size
 *
 * @param size The size of the new vector
 * @return vector Double pointer to a new vector
 */
vector new_vector(unsigned int size);

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
 * @brief Function to save a vector
 *
 * @param vec
 */
void save_vector(vector vec, const char* filename);

/**
 * @brief Function to read the saved vector from a file
 *
 * @param filename 
 */
vector read_vector(const char* filename);

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
