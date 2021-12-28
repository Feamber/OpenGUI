#include "catch.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Style2/generated/position.h"
#include <iostream>

OGUI::StyleSheet* LoadStyleSheet2()
{
    using namespace OGUI;
    const char* exampleCSS = 
R"(#TestElement {
    left:100px;
    bottom:100px; font-size:24

        
})";
    auto res = CSSParser::Parse(exampleCSS);
    res->Initialize();
    return res;
}

TEST_CASE("StyleParse", "[Parser][NameSelector]")
{
    using namespace OGUI;
    RegisterBuiltinStructs();
    auto ve = std::make_unique<VisualElement>();
    ve->_name = "TestElement";
    ve->_styleSheets.push_back(LoadStyleSheet2());
    Context::Get().styleSystem.Update(ve.get(), true);
    REQUIRE(StylePosition::Get(ve->_style).left.value == 100.f);
    REQUIRE(StyleText::Get(ve->_style).fontSize == 24.f);
}