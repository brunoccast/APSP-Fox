#include "../src/mem.hpp"

#include <cstdlib>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

void populate(double**, int);

TEST_CASE( "Allocation works correctly" )
{
    double *mx = nullptr;
    matrix_alloc(&mx, 2);
    
    REQUIRE( mx != nullptr );

    SECTION( "Re-allocation changes nothing" )
    {
	populate(&mx, 2);
	matrix_alloc(&mx, 2);
    
	for(int i=0; i!=4; ++i)
	    REQUIRE( mx[i] == i+1 );
    }
    
    if(mx != nullptr) free(mx);
}

TEST_CASE( "Copy works correctly" )
{
    double *mx1 = nullptr;
    double *mx2 = nullptr;

    matrix_alloc(&mx1, 2);
    populate(&mx1, 2);

    SECTION( "Copying to null matrices changes nothing" )
    {
	matrix_copy(&mx2, mx1, 2);
	REQUIRE(mx2 == nullptr);
    }

    SECTION( "Copy works for allocated matrices" )
    {
	matrix_alloc(&mx2, 2);
	matrix_copy(&mx2, mx1, 2);

	for(int i=0; i!=4; ++i)
	    REQUIRE(mx1[i] == mx2[i]);
    }

    if(mx1 != nullptr) free(mx1);
    if(mx2 != nullptr) free(mx2);
}

TEST_CASE( "Free works correctly" )
{
    double *mx = nullptr;

    matrix_alloc(&mx, 2);
    REQUIRE( mx != nullptr );

    matrix_free(&mx);
    REQUIRE( mx == nullptr );
}

TEST_CASE( "Fill works correctly" )
{
    double *mx = nullptr;

    matrix_alloc(&mx, 2);
    matrix_fill(&mx, 2, 10);

    for(int i=0; i!=4; ++i)
	REQUIRE( mx[i] == 10 );

    matrix_free(&mx);
}

void populate(double **mx, int size)
{
    for(int i=0; i!=size*size; ++i)
	(*mx)[i] = i+1;
}
