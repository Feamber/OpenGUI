#include "catch.hpp"

unsigned int Draw( unsigned int number ) {
    return 1;
}

TEST_CASE( "Draw", "[draw]" ) {
    REQUIRE( Draw(1) == 1 );
}