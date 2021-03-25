#include "WidgetSample.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleHelpers.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/CSSParser/CSSParser.h"

template<class T>
void SetProp(OGUI::StyleSheet& sheet, OGUI::StyleRule& rule, std::string_view name, const T& value)
{
    auto id = OGUI::PropertyNameToId(name);
    auto prop = OGUI::AddProperty(sheet.storage, id, value);
    rule.properties.push_back(prop);
}

YGValue YGPoint(float v)
{
    return {v, YGUnitPoint};
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

    auto res = ParseCSSFile("test.css");

    return res.value();
}

void WidgetSample::Initialize()
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_shared<VisualElement>();
    auto styleSt = LoadStyleSheet();
    styleSt.Initialize();
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
}
