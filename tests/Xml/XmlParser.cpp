#include "OpenGUI/XmlParser/XmlParser.h"
#include "OpenGUI/Core/Types.h"
#include "catch.hpp"
#include "OpenGUI/VisualElement.h"

#include <fstream>

TEST_CASE("XmlParser", "[XmlAsset]")
{
    using namespace OGUI;
    XmlBase::RegisterOGUIXmlParser();
    for (int i = 0; i < 1000; ++i) {
        ParseXmlState xmlState;
        auto asset = LoadXmlFile("XmlParserTest.xml", xmlState);
        REQUIRE(asset);
        // asset->root->DebugPrint();
        InstantiateXmlState InstantState;
        VisualElement* ins = asset->Instantiate(InstantState);
        REQUIRE(ins);

        // auto Text = (TextVisualElement*)ins->_children[1];
        // REQUIRE(Text->_name == "Text");
        // REQUIRE(Text->_texts[0] == "aaaa");
        // auto TextChild = (TextVisualElement*)(Text->_children[0]->_children[0]);
        // REQUIRE(TextChild->_texts[0] == "ssss");
        // REQUIRE(Text->_texts[1] == "bbbb");

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

        auto BBB = BB2->_children[0]->_children[0];
        REQUIRE(BBB->_name == "BBB");
        REQUIRE(BBB->_children[0]->_name == "BB2_1");
    }
}