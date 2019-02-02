#include "mem.hpp"

#include <cstdlib>
#include <cstring>

#define SQR(n) ((n)*(n))

void matrix_alloc(double **mx, int size)
{
    if(*mx != nullptr)
	return;
    
    *mx = (double*) malloc(SQR(size) * sizeof(double)); 
}

void matrix_copy(double **dest, double **source, int size)
{
    if(*dest == nullptr || *source == nullptr)
	return;

    for(int i=0; i!=SQR(size); ++i)
	(*dest)[i] = (*source)[i];
}

void matrix_free(double **mx)
{
    if(*mx != nullptr)
	free(*mx);
    
    *mx = nullptr;
}
