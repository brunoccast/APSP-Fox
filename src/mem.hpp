/* mem.hpp
 *
 * Functions related to memory management.
 *
 **/

#ifndef MEM_H
#define MEM_H

/*
 * Allocate memory for a 1D matrix of
 * size*size doubles.
 *
 * Input:
 *   mx   - target matrix to allocate
 *   size - order of the matrix
 *
 **/
void matrix_alloc(double**, int);

/*
 * Copies the values of a source 1D matrix 
 * to a destination 1D matrix.
 *
 * Input:
 *   dest   - destination matrix
 *   source - source matrix
 *   size   - order of matrices
 *
 **/
void matrix_copy(double**, double**, int);

/*
 * Free matrix memory previously allocated
 * by matrix_alloc.
 *
 * Input:
 *   mx - target matrix to free
 *
 */
void matrix_free(double**);

#endif
