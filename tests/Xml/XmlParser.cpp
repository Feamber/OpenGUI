#include <OpenGUI/Xml/XmlFactoryTool.h>
#include "OpenGUI/Core/Types.h"
#include "catch.hpp"
#include "OpenGUI/Xml/XmlAsset.h"
#include "OpenGUI/VisualElement.h"

#include <fstream>

TEST_CASE("XmlParser", "[XmlAsset]")
{
    using namespace OGUI;
    // for (int i = 0; i < 1000; ++i) {
        auto asset = XmlAsset::LoadXmlFile("XmlParserTest.xml");
        REQUIRE(asset);
        VisualElement* ins = asset->Instantiate();
        REQUIRE(ins);

        auto Ve666 = ins->_children[1];
        REQUIRE(((TextValue*)Ve666->_children[0])->text == "66666");

        auto Text = ins->_children[2];
        REQUIRE(Text->_name == "Text");
        REQUIRE(((TextValue*)Text->_children[0])->text == "aaaa");
        REQUIRE(((TextValue*)Text->_children[1]->_children[0]->_children[0])->text == "ssss");
        REQUIRE(((TextValue*)Text->_children[2])->text == "bbbb");

        auto A = ins->_children[0];
        REQUIRE(A->_name == "A");
        REQUIRE(A->_children[2]->_name == "B3");
        REQUIRE(A->focusable == true);
        REQUIRE(A->navMode == ENavMode::Horizontal);
        REQUIRE(A->isFocusScope == true);
        REQUIRE(A->navCycleMode == ENavCycleMode::Automatic);

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
    // }
}
