#include "fox.hpp"
#include "mem.hpp"
#include "util.hpp"

#include <cmath>
#include <iostream>

struct Grid;

bool validate_input(int n, int p)
{
    bool ret = true;

    // perfect square?
    double q = sqrt(p);
    
    if(floor(q) != q)
    {
	std::cout << "The number of processes must be a perfect square.\n";
	ret = false;
    }

    // even division?
    if(n%(int)q != 0)
    {
	std::cout << "Square root of the number of processes isn't divisible by the matrix size.\n";
	ret = false;
    }
    
    return ret;
}

void setup_grid(int p, GRID* g)
{
    int dimensions[2];
    int periods[2];
    int coords[2];

    // #processor & grid order
    g->p = p;
    g->q = (int)sqrt(p);

    // global grid comm (cartesian)
    dimensions[0] = dimensions[1] = g->q;
    periods[0] = periods[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 1, &(g->comm));

    // ranks
    MPI_Comm_rank(g->comm, &(g->my_rank));
    MPI_Cart_coords(g->comm, g->my_rank, 2, coords);
    g->my_row = coords[0];
    g->my_col = coords[1];
    
    // row comm
    dimensions[0] = 0;
    dimensions[1] = 1;
    MPI_Cart_sub(g->comm, dimensions, &(g->row_comm));

    // col comm
    dimensions[0] = 1;
    dimensions[1] = 0;
    MPI_Cart_sub(g->comm, dimensions, &(g->col_comm));
}

void fox_algorithm(int n, const GRID& g, double** a, double** b, double** c)
{
    int mx_part, source, dest, bcast_root;
    double *tmp_a = nullptr;
    
    mx_part = n/g.q;

    // addresses for circular shift of B
    source = (g.my_row + 1) % g.q;
    dest   = (g.my_row + g.q - 1) % g.q;

    // storage for the broadcast of A
    matrix_alloc(&tmp_a, mx_part);

    for(int step=0; step<g.q; ++step)
    {
	bcast_root = (g.my_row + step) % g.q;

	if(bcast_root == g.my_col)
	{
	    MPI_Bcast(*a, sqr(mx_part), MPI_DOUBLE, bcast_root, g.row_comm);
	    special_matrix_multiply(*a, *b, c, mx_part);
	}
	else
	{
	    MPI_Bcast(tmp_a, sqr(mx_part), MPI_DOUBLE, bcast_root, g.row_comm);
	    special_matrix_multiply(tmp_a, *b, c, mx_part);
	}

	MPI_Send(*b, sqr(mx_part), MPI_DOUBLE, dest, 0, g.col_comm);
	MPI_Recv(*b, sqr(mx_part), MPI_DOUBLE, source, MPI_ANY_TAG, g.col_comm, MPI_STATUS_IGNORE);
    }

    matrix_free(&tmp_a);
}
