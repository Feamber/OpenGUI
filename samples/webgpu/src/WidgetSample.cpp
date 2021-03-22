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

#define SimpleSelector(t, v, r) \
{ \
    StyleComplexSelector complexSel; \
    StyleSelector selector; \
    selector.type = StyleSelector::t; \
    selector.value = v; \
    complexSel.priority = line++; \
    complexSel.ruleIndex = r; \
    complexSel.UpdateSpecificity(); \
    complexSel.selectors.push_back(std::move(selector)); \
    styleSt.styleSelectors.push_back(std::move(complexSel)); \
}

namespace OGUI
{
    void TransformRec(VisualElement* element)
    {
        element->UpdateWorldTransform();
        element->Traverse([&](VisualElement* next, int depth) { TransformRec(next); }, 0);
    }
}

OGUI::StyleSheet LoadStyleSheet()
{
    using namespace OGUI;
    StyleSheet styleSt;
    int line = 0;

    BeginRule(rule1)
        Prop("width", YGPoint(300.f));
        Prop("height", YGPoint(300.f));
        Prop("justify-content", YGJustifyCenter);
        Prop("color", Color4f(0.6f, 0.6f, 0.6f, 1.f));
    EndRule();

    BeginRule(rule2)
        //Prop("width", YGPoint(100.f));
        //Prop("height", YGPoint(100.f));
        Prop("flex-direction", YGFlexDirectionRow);
        Prop("justify-content", YGJustifyCenter);
        Prop("padding-top", YGPoint(10.f));
        Prop("padding-bottom", YGPoint(10.f));
        Prop("color", Color4f(0.8f, 0.5f, 0.8f, 1.f));
    EndRule();

    BeginRule(rule3)
        Prop("width", YGPoint(100.f));
        Prop("height", YGPoint(100.f));
        Prop("margin-right", YGPoint(10.f));
        Prop("color", Color4f(1.f, 1.f, 1.f, 1.f));
    EndRule();

    SimpleSelector(Name, "TestElement", rule1);
    SimpleSelector(Name, "TestElement2", rule2);
    SimpleSelector(Class, "Child", rule3);

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
    c1->_name = "TestElement2";
    ve->PushChild(c1.get());

    auto c2 = std::make_unique<VisualElement>();
    c2->_name = "Child1";
    c2->_styleClasses.push_back("Child");
    c1->PushChild(c2.get());

    auto c3 = std::make_unique<VisualElement>();
    c3->_name = "Child2";
    c3->_styleClasses.push_back("Child");
    c1->PushChild(c3.get());
    nodes.push_back(std::move(c1));
    nodes.push_back(std::move(c2));
    nodes.push_back(std::move(c3));

    styleSys.Update(ve.get());
    ve->CalculateLayout();
    OGUI::TransformRec(ve.get());
    tree = std::move(ve);
}
