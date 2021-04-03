#define DLL_IMPLEMENTATION
#include "OpenGUI/Xml/XmlFactoryTool.h"

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
        return XmlFactoryCreate<IXmlFactory_TextValue, TextValue>(*this, asset, context);
    }

    bool IXmlFactory_TextValue::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        ((TextValue&)new_element).text = asset.text;
        return IXmlFactory_VisualElement::InitAttribute(new_element, asset, context);
    }
}

