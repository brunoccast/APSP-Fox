/* util.hpp
 *
 * Util functions.
 *
 **/

#ifndef UTIL_H
#define UTIL_H

/*
 * Return square of number.
 *
 **/
inline int sqr(int n)
{
    return n*n;
}

/*
 * Return a 2D coord projected as 1D coord.
 *
 **/
inline int proj(int row, int col, int sz)
{
    return row*sz + col;
}

/*
 * Multiply 2 given matrices of the same order.
 * The product is given by the minimum of two
 * values, ence the "special multiply".
 * 
 * Ref: 
 *   - http://users.cecs.anu.edu.au/~Alistair.Rendell/Teaching/apac_comp3600/module4/all_pairs_shortest_paths.xhtml
 *
 * Input:
 *   a    - matrix A to use in the product
 *   b    - matrix B to use in the product
 *   c    - matrix-result of multiply A and B
 *   size - matrix order
 **/
void special_matrix_multiply(double*, double*, double**, int);

#endif
