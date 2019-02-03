#include "fox.hpp"
#include "mem.hpp"
#include "util.hpp"

#include <mpi.h>
#include <cfloat>
#include <iostream>

#define ROOT 0
#define INF DBL_MAX

using namespace std;

int main(int argc, char **argv)
{
    int sz_mx, my_rank, nproc;
    double *mx, *a, *b, *c, *tmp;
    bool err;
    GRID grid;

    mx = a = b = c = tmp = nullptr;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    // Root takes care of input
    if(my_rank == ROOT)
    {
	cin >> sz_mx;
	err = !validate_input(sz_mx, nproc);

	if(!err)
	{
	    // basic input validated, alloc & read matrix
	    matrix_alloc(&mx, sz_mx);
	    
	    for(int i=0; i!=sz_mx; ++i)
		for(int j=0; j!=sz_mx; ++j)
		{
		    int pos = proj(i,j,sz_mx);
		    cin >> mx[pos];

		    // no connection (i,j) in the graph?
		    if(i != j && mx[pos] == 0)
			mx[pos] = INF;
		}
	}
    }

    // broadcast err to avoid terminating (if needed)
    // execution with MPI_Abort()
    MPI_Bcast(&err, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(err)
    {
	MPI_Finalize();
	return 0;
    }

    setup_grid(nproc, &grid);
    
    // alloc local matrices
    int mx_part = sz_mx/grid.q;

    matrix_alloc(&a, mx_part);
    matrix_alloc(&b, mx_part);
    matrix_alloc(&c, mx_part);

    // share work
    if(my_rank == ROOT)
    {
	int dest = 0;
	matrix_alloc(&tmp, mx_part);

        for(int row=0; row!=grid.q; ++row)
	    for(int col=0; col!=grid.q; ++col)
	    {
		for(int i=0; i!=mx_part; ++i)
		    for(int j=0; j!=mx_part; ++j)
			tmp[ proj(i, j, mx_part) ] = mx[ proj((row*mx_part)+i, (col*mx_part)+j, sz_mx) ];

		MPI_Send(tmp, sqr(mx_part), MPI_DOUBLE, dest++, 0, MPI_COMM_WORLD);
	    }
    }

    // receive work
    MPI_Recv(a, sqr(mx_part), MPI_DOUBLE, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    matrix_copy(&b, a, mx_part);
    matrix_fill(&c, mx_part, INF);
    
    // APSP: repeated squaring + fox
    int m = 1;
    while(m < sz_mx-1)
    {
	fox_algorithm(sz_mx, grid, &a, &b, &c);

	matrix_copy(&a, c, mx_part);
	matrix_copy(&b, c, mx_part);
	
	m *= 2;
    }
    
    // gather results
    if(my_rank != ROOT)
	MPI_Send(a, sqr(mx_part), MPI_DOUBLE, ROOT, 0, MPI_COMM_WORLD);
    else
    {
	int source = 0;
	matrix_copy(&tmp, a, mx_part);

	for(int row=0; row!=grid.q; ++row)
	    for(int col=0; col!=grid.q; ++col)
	    {
		if(source != ROOT)
		    MPI_Recv(tmp, sqr(mx_part), MPI_DOUBLE, source, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		for(int i=0; i!=mx_part; ++i)
		    for(int j=0; j!=mx_part; ++j)
		    {
			int dest_pos = proj(i, j, mx_part);
			
			mx[ proj((row*mx_part)+i, (col*mx_part)+j, sz_mx) ] =
			    (tmp[dest_pos] == INF) ? 0 : tmp[dest_pos];
		    }

		source++;
	    }

	// print matrix-result
	cout << "Results:\n";
	for(int i=0; i!=sz_mx; ++i)
	    for(int j=0; j!=sz_mx; ++j)
		cout << mx[ proj(i, j, sz_mx) ] << " \n"[j==sz_mx-1];
	
	matrix_free(&tmp);
	matrix_free(&mx);
    }
    
    // free allocated memory
    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&c);

    MPI_Finalize();
    return 0;
}
