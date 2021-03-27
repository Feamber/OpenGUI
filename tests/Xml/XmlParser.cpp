#include "catch.hpp"
#include "OpenGUI/Xml/XmlAsset.h"
#include "OpenGUI/VisualElement.h"

TEST_CASE("XmlParser", "[XmlAsset]")
{
    using namespace OGUI;
    auto asset = XmlAsset::LoadXmlFile("XmlParserTest.xml");
    REQUIRE(!asset.expired());
    std::shared_ptr<VisualElement> ins = XmlAsset::Instantiate(asset.lock()->id);

    auto A = ins->_children[0];
    REQUIRE(A->_name == "A");
    REQUIRE(A->_children[2]->_name == "B3");

    auto B1 = A->_children[0];
    REQUIRE(B1->_name == "B1");

    auto AA1 = B1->_children[0];
    REQUIRE(AA1->_name == "AA1");

    auto BB2 = AA1->_children[1];
    REQUIRE(BB2->_name == "BB2");
    REQUIRE(BB2->_styleClasses[0] == "aaaaaa");
    REQUIRE(BB2->_path == "aaaa");

    auto BBB = BB2->_children[0]->_children[0];
    REQUIRE(BBB->_name == "BBB");
    REQUIRE(BBB->_children[0]->_name == "BB2_1");
}
