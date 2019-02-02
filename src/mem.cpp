#include "mem.hpp"
#include "util.hpp" // sqr

#include <cstdlib>
#include <cstring>

void matrix_alloc(double **mx, int size)
{
    if(*mx != nullptr)
	return;
    
    *mx = (double*) malloc(sqr(size) * sizeof(double)); 
}

void matrix_copy(double **dest, double *source, int size)
{
    if(*dest == nullptr || source == nullptr)
	return;

    for(int i=0; i!=sqr(size); ++i)
	(*dest)[i] = source[i];
}

void matrix_free(double **mx)
{
    if(*mx != nullptr)
	free(*mx);
    
    *mx = nullptr;
}

void matrix_fill(double **mx, int size, double val)
{
    if(*mx == nullptr)
	return;

    for(int i=0; i!=sqr(size); ++i)
	(*mx)[i] = val;
}
