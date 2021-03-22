#include "WidgetSample.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleHelpers.h"
#include "OpenGUI/VisualElement.h"

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

    BeginRule(rule1)
        Prop("width", YGPoint(500.f));
        Prop("height", YGPoint(500.f));
        Prop("justify-content", YGJustifyCenter);
    EndRule();

    BeginRule(rule2)
        Prop("width", YGPoint(100.f));
        Prop("height", YGPoint(100.f));
    EndRule();

    {
        StyleComplexSelector complexSel;
        StyleSelector selector;
        selector.type = StyleSelector::Name;
        selector.value = "TestElement";
        complexSel.priority = 0;
        complexSel.ruleIndex = rule1;
        complexSel.UpdateSpecificity();
        complexSel.selectors.push_back(std::move(selector));
        styleSt.styleSelectors.push_back(std::move(complexSel));
    }

    {
        StyleComplexSelector complexSel;
        StyleSelector selector;
        selector.type = StyleSelector::Class;
        selector.value = "Child";
        complexSel.priority = 1;
        complexSel.ruleIndex = rule2;
        complexSel.UpdateSpecificity();
        complexSel.selectors.push_back(std::move(selector));
        styleSt.styleSelectors.push_back(std::move(complexSel));
    }

    styleSt.Initialize();
    return styleSt;
}

void WidgetSample::Initialize()
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_unique<VisualElement>();
    auto styleSt = LoadStyleSheet();
    ve->_name = "TestElement";
    ve->_styleSheets.push_back(&styleSt);

    auto c1 = std::make_unique<VisualElement>();
    c1->_name = "Child1";
    c1->_styleClasses.push_back("Child");
    ve->PushChild(c1.get());

    auto c2 = std::make_unique<VisualElement>();
    c2->_name = "Child2";
    c2->_styleClasses.push_back("Child");
    c1->PushChild(c2.get());
    nodes.push_back(std::move(c1));
    nodes.push_back(std::move(c2));

    styleSys.Update(ve.get());
    ve->CalculateLayout();
    tree = std::move(ve);
}
