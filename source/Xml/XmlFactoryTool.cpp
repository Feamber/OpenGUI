#include <memory>
#define DLL_IMPLEMENTATION
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include "OpenGUI/Text/TextElement.h"

namespace OGUI
{

    void IXmlFactory_TextValue::Internal_Init()
    {
        OGUI::IXmlFactory_VisualElement::Internal_Init();
    }

    bool IXmlFactory_TextValue::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory_VisualElement::Internal_InitAttribute(attr_name_hash, attr);
        return false;
    }

    VisualElement* IXmlFactory_TextValue::Create(const XmlElement& asset, CreationContext& context)
    {
        auto textVisualElement = dynamic_cast<OGUI::TextElement*>(context.stack.front());
        if (textVisualElement)
            textVisualElement->AddText(ostr::string(asset.text));
        return nullptr;
    }

    bool IXmlFactory_TextValue::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return true;
    }
}

