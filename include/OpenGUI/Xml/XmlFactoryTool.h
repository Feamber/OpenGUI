#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Xml/XsdGen/OGUI.h"

namespace OGUI
{
    class OGUI_API TemplateContainer : public VisualElement
    {
    public:
        std::map<std::string, VisualElement*> slots;

        virtual std::string_view GetTypeName() { return "TemplateContainer"; }
        virtual std::string_view GetFullTypeName() { return "OGUI::TemplateContainer"; }
    };

    class OGUI_API TextVisualElement : public VisualElement
    {
    public:
        std::vector<ostr::string> _texts;
        
        virtual std::string_view GetTypeName() { return "Text"; }
        virtual std::string_view GetFullTypeName() { return "OGUI::Text"; }
    };

    // 这是专门用来解析字符串节点的特殊控件
    class OGUI_API IXmlFactory_TextValue : public OGUI::IXmlFactory_VisualElement
    {
    public:
        IXmlFactory_TextValue()
        {
            xml_name = "TextValue";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.TextValue";
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };
}