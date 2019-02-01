#include "../src/fox.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Input is validated", "[validate_input]" )
{
    // true validations
    REQUIRE( validate_input(4,4) == true );
    REQUIRE( validate_input(10,4) == true );
    REQUIRE( validate_input(6,4) == true );

    // false validations
    REQUIRE( validate_input(4,2) == false );
    REQUIRE( validate_input(10,3) == false );
}
