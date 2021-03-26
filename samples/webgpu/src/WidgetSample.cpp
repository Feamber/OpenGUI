#include "OpenGUI/Core/Primitive.h"
#include "utils.h"
#include "WidgetSample.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleHelpers.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Xml/XmlAsset.h"

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
void WidgetSample::Initialize()
{
    using namespace OGUI;
    VisualStyleSystem styleSys;
    auto ve = std::make_shared<VisualElement>();

    styleSheet = ParseCSSFile("test.css").value();
    styleSheet.Initialize();

    auto asset = XmlAsset::LoadXmlFile("test.xml");
    ve = XmlAsset::Instantiate(asset->id);
    ve->_styleSheets.push_back(&styleSheet);
    ve->_pseudoMask |= (int)PseudoStates::Root;

    styleSys.Update(ve.get());
    ve->CalculateLayout();
    OGUI::TransformRec(ve.get());
    tree = std::move(ve);

    OGUI::BitMap bm;
    createBitMapFromJPG("test.jpg", bm);
    
    freeBitMap(bm);
}
