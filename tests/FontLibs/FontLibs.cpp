#include "catch.hpp"
#include "FreeTypeLib.h"
#include "HBSharper.h"

TEST_CASE( "FreeTypeInit", "[freetype]" ) {
    // the font rasterizing library
    FreeTypeLib lib;
}

TEST_CASE( "HBShaper", "[harfbuzz]" ) {
    // the font rasterizing library
    FreeTypeLib lib;
    HBShaper hanShaper("fireflysung.ttf", &lib);
    hanShaper.init();

}