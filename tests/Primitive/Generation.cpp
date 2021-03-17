#include "catch.hpp"
#include "Core/Primitive.h"

TEST_CASE( "Vertex", "[vertex]" ) {
    OGUI::Vertex v = {};
    v.position = {0.1f, 0.23f};
    REQUIRE( v.position.x == 0.1f );
}