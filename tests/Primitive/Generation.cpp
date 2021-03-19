#include "catch.hpp"
#include "OpenGUI/Core/Primitive.h"

TEST_CASE( "Vertex", "[vertex]" ) {
    OGUI::Vertex v = {};
    v.position = {0.1f, 0.23f};
    REQUIRE( v.position.X == 0.1f );
}