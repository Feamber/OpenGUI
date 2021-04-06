#include "catch.hpp"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/CSSParser/CSSParser.h"
#include <iostream>

OGUI::StyleSheet LoadStyleSheet2()
{
    using namespace OGUI;
    const char* exampleCSS = 
R"(#TestElement {
    left:100px;
    bottom:100px; font-size:24
})";
    auto res = ParseCSS(exampleCSS);

    return res.value();
}

TEST_CASE("StyleParse", "[Parser][NameSelector]")
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_unique<VisualElement>();
    auto styleSt = LoadStyleSheet2();
    styleSt.Initialize();
    ve->_name = "TestElement";
    ve->_styleSheets.push_back(&styleSt);
    styleSys.Update(ve.get());
    REQUIRE(ve->_style.left.value == 100.f);
    REQUIRE(ve->_style.fontSize == 24.f);
}