#include "catch.hpp"
#include "OpenGUI/Xml/XmlAsset.h"

TEST_CASE("XmlParser", "[XmlAsset]")
{
    using namespace OGUI;
    auto asset = XmlAsset::LoadXmlFile("XmlParserTest.xml");
    REQUIRE(asset);
    auto ins = XmlAsset::Instantiate(asset->id);

    REQUIRE(ins);
}
