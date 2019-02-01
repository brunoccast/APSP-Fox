#include "fox.hpp"

#include <mpi.h>
#include <cstdlib>
#include <iostream>

#define ROOT 0
#define SQR(n) ((n)*(n))                    // square of number
#define PROJ(row,col,sz) ((row)*(sz)+(col)) // 2d array coord -> 1d array coord

using namespace std;

int main(int argc, char **argv)
{
    GRID grid;
    int sz_mx, my_rank, nproc;
    double *mx, *a, *b, *c;
    bool err;
    
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
	    mx = (double*) malloc(SQR(sz_mx) * sizeof(double));
	    
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
    
    a = (double*) malloc(SQR(mx_part) * sizeof(double));
    b = (double*) malloc(SQR(mx_part) * sizeof(double));
    c = (double*) malloc(SQR(mx_part) * sizeof(double));
    
    // share work
    if(my_rank == ROOT)
    {
	int dest = 0;
	double *tmp = (double*) malloc(SQR(mx_part) * sizeof(double));
	
        for(int r=0; r!=grid.q; ++r)
	    for(int c=0; c!=grid.q; ++c)
	    {
		for(int i=0; i!=mx_part; ++i)
		    for(int j=0; j!=mx_part; ++j)
			tmp[ PROJ(i,j,mx_part) ] = mx[ PROJ((r*mx_part)+i,(c*mx_part)+j,sz_mx) ];

		MPI_Send(tmp, SQR(mx_part), MPI_DOUBLE, dest++, 0, MPI_COMM_WORLD);
	    }
    }

    // receive work
    MPI_Recv(b, SQR(mx_part), MPI_DOUBLE, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    
    MPI_Finalize();
    return 0;
}
