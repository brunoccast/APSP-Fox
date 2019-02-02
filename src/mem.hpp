/* mem.hpp
 *
 * Functions related to matrix memory management.
 * Matrices are assumed to be 1-dimensional,
 * being then treated as arrays. 
 *
 **/

#ifndef MEM_H
#define MEM_H

/*
 * Allocate memory for a matrix of
 * size*size doubles.
 *
 * Input:
 *   mx   - target matrix to allocate
 *   size - order of the matrix
 *
 **/
void matrix_alloc(double**, int);

/*
 * Copies the values of a source matrix 
 * to a destination matrix.
 *
 * Input:
 *   dest   - destination matrix
 *   source - source matrix
 *   size   - order of matrices
 *
 **/
void matrix_copy(double**, double*, int);

/*
 * Free matrix memory previously allocated
 * by matrix_alloc.
 *
 * Input:
 *   mx - target matrix to free
 *
 **/
void matrix_free(double**);

/*
 * Fill matrix with provided value.
 *
 * Input:
 *   mx   - target matrix to fill
 *   size - matrix order
 *   val  - value to fill with
 *
 **/
void matrix_fill(double**, int, double);

#endif
