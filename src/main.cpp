#include "fox.hpp"
#include "mem.hpp"

#include <mpi.h>
#include <iostream>

#define ROOT 0
#define PROJ(row,col,sz) ((row)*(sz)+(col)) // 2d array coord -> 1d array coord

using namespace std;

int main(int argc, char **argv)
{
    int sz_mx, my_rank, nproc;
    double *mx, *a, *b, *c;
    bool err;
    GRID grid;

    mx = a = b = c = nullptr;
    
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
		    cin >> mx[ PROJ(i,j,sz_mx) ];
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
	double *tmp;
	matrix_alloc(&tmp, mx_part);
	
        for(int r=0; r!=grid.q; ++r)
	    for(int c=0; c!=grid.q; ++c)
	    {
		for(int i=0; i!=mx_part; ++i)
		    for(int j=0; j!=mx_part; ++j)
			tmp[ PROJ(i,j,mx_part) ] = mx[ PROJ((r*mx_part)+i,(c*mx_part)+j,sz_mx) ];

		MPI_Send(tmp, SQR(mx_part), MPI_DOUBLE, dest++, 0, MPI_COMM_WORLD);
	    }

	matrix_free(&tmp);
    }

    // receive work
    MPI_Recv(a, SQR(mx_part), MPI_DOUBLE, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // APSP: repeated squaring + fox
    int m = 1;

    while(m < sz_mx-1)
    {
	matrix_copy(&b, &a, mx_part);
	m *= 2;
    }

    // free allocated memory
    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&c);
    
    MPI_Finalize();
    return 0;
}
