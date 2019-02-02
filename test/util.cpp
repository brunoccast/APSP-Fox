#include "../src/util.hpp"
#include "../src/mem.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Matrix multiply works correctly" )
{
    int size = 2;
    double inf = 1000.0;
    double *A = nullptr,
	   *B = nullptr,
	   *C = nullptr;

    double expected[] = {2,3,4,5};

    matrix_alloc(&A,size);
    matrix_alloc(&B,size);
    matrix_alloc(&C,size);

    matrix_fill(&C, size, inf);
    
    for(int i=0; i!=size*size; ++i)
	A[i] = B[i] = i+1;
    
    special_matrix_multiply(A, B, &C, 2);
    
    for(int i=0; i!=size*size; ++i)
	REQUIRE( C[i] == expected[i] );

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
}
