/* fox.hpp
 *
 * Functions related to fox algorithm.
 *
 * Both GRID and setup_grid() are based on
 * the same constructs described by Peter
 * S. Pacheco in "A User's Guide to MPI.".
 *
 **/

#ifndef FOX_H
#define FOX_H

#include <mpi.h>

/*
 * Util container for information about the
 * processors cartesian, row & col comms and
 * the corresponding ranks.
 *
 **/
struct GRID
{
    int p;              // #processors
    MPI_Comm comm;      // grid comm
    MPI_Comm row_comm;  // row comm
    MPI_Comm col_comm;  // column comm
    int q;              // grid order
    int my_row;         // row number
    int my_col;         // column number
    int my_rank;        // grid comm rank
};

/*
 * Validate program input according to Fox rules:
 *   - #processors must be a perfect square
 *   - Square root of #processors must divide
 *     matrix size
 *
 * Input:
 *   n - matrix size
 *   p - #processors
 *
 * Return:
 *   true on successful validation, false
 *   otherwise.
 *
 **/
bool validate_input(int,int);

/*
 * Setup GRID for current processor's context.
 * 
 * Input:
 *   p - #processors
 *   g - target GRID to setup
 *
 **/
void setup_grid(int, GRID*);

#endif // FOX_H
