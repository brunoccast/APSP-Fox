#include "util.hpp"

#include <algorithm>

void special_matrix_multiply(double *a, double *b, double **c, int size)
{
    for(int i=0; i!=size; ++i)
	for(int j=0; j!=size; ++j)
	    for(int k=0; k!=size; ++k)
	    {
		(*c)[proj(i,j,size)] = std::min( (*c)[proj(i,j,size)],
						 a[proj(i,k,size)] + b[proj(k,j,size)] );
	    }
}
