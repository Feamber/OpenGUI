#include "catch.hpp"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleHelpers.h"
#include "OpenGUI/VisualElement.h"

#include "OpenGUI/CSSParser/CSSParser.h"
#include <iostream>

template<class T>
void SetProp(OGUI::StyleSheet& sheet, OGUI::StyleRule& rule, std::string_view name, const T& value)
{
    auto id = OGUI::PropertyNameToId(name);
    auto prop = OGUI::AddProperty(sheet.Storage, id, value);
    rule.properties.push_back(prop);
}

YGValue YGPoint(float v)
{
    return {v, YGUnitPoint};
}
#define SimpleSelector(t, v, r) \
{ \
    StyleComplexSelector complexSel; \
    complexSel.priority = line++; \
    complexSel.ruleIndex = r; \
    StyleSelector selector; \
    StyleSelector::Part selPart; \
    selPart.type = StyleSelector::t; \
    selPart.value = v; \
    selector.parts.push_back(std::move(selPart)); \
    complexSel.selectors.push_back(std::move(selector)); \
    complexSel.UpdateSpecificity(); \
    styleSt.styleSelectors.push_back(std::move(complexSel)); \
}

#define BeginRule(name) \
    auto name = styleSt.styleRules.size(); \
    styleSt.styleRules.push_back([&](){StyleRule __rule;
#define EndRule() \
    return __rule;}())
#define Prop(name, value) \
    SetProp(styleSt, __rule, name, value)

OGUI::StyleSheet LoadStyleSheet()
{
    using namespace OGUI;
    StyleSheet styleSt;
    int line = 0;

    BeginRule(rule1)
        Prop("left", YGPoint(100.f));
        Prop("bottom", YGPoint(100.f));
        Prop("fontSize", 24.f);
    EndRule();

    SimpleSelector(Name, "TestElement", rule1);

    styleSt.Initialize();
    return styleSt;
}

TEST_CASE("StyleRuntime", "[SharedStyle][NameSelector]")
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_unique<VisualElement>();
    auto styleSt = LoadStyleSheet();
    ve->_name = "TestElement";
    ve->_styleSheets.push_back(&styleSt);
    styleSys.Update(ve.get());
    REQUIRE(ve->_style.left.value == 100.f);
    REQUIRE(ve->_style.fontSize == 24.f);

    std::cout << "CSSParser test here!\n";
    std::cout << "--------------------------------------------------------------\n";

    std::string text = 
R"(selector : keyword {
        left : 0.0 | auto | test;
        top: 0.0 | auto;
        right : 640.0 | auto|fuck;bottom: 320.0 | auto;
    }
)";

	std::string text1 =
R"(selector {
        left : 0.0 | auto | test;
    }
)";

    OGUI::Lexer lexer(text);
   
    while (true)
    {
        Token t = lexer.GetNextToken();
        if (t.type == TokenType::TEOF)
            break;
        std::cout << t.value << std::endl;
    };

    std::cout << "--------------------------------------------------------------\n";
}