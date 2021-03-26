#include "OpenGUI/Core/Primitive.h"
#include "utils.h"
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

namespace OGUI
{
    void TransformRec(VisualElement* element)
    {
        element->UpdateWorldTransform();
        element->Traverse([&](VisualElement* next) { TransformRec(next); });
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
        Prop("align-items", YGAlignCenter);
        Prop("color", Color4f(0.6f, 0.6f, 0.6f, 1.f));
    EndRule();

    BeginRule(rule2)
        Prop("width", YGPoint(100.f));
        Prop("height", YGPoint(100.f));
        Prop("margin-right", YGPoint(10.f));
        Prop("color", Color4f(1.f, 1.f, 1.f, 1.f));
    EndRule();

    SimpleSelector(Name, "TestElement", rule1);
    SimpleSelector(Class, "Child", rule2);

    styleSt.Initialize();
    return styleSt;
}

void WidgetSample::Initialize()
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_shared<VisualElement>();
    auto styleSt = LoadStyleSheet();
    ve->_name = "TestElement";
    ve->_styleSheets.push_back(&styleSt);\

    auto c1 = std::make_shared<VisualElement>();
    c1->_name = "Child1";
    c1->_styleClasses.push_back("Child");
    ve->PushChild(c1.get());

    auto c2 = std::make_shared<VisualElement>();
    c2->_name = "Child2";
    c2->_styleClasses.push_back("Child");
    ve->PushChild(c2.get());
    nodes.push_back(std::move(c1));
    nodes.push_back(std::move(c2));

    styleSys.Update(ve.get());
    ve->CalculateLayout();
    OGUI::TransformRec(ve.get());
    tree = std::move(ve);

    OGUI::BitMap bm;
    createBitMapFromJPG("test.jpg", bm);
    
    freeBitMap(bm);
}
