#pragma once
#include <string>
#include <string_view>
#include <optional>
#include <functional>
#include "OpenGUI/Configure.h"
#include "OpenGUI/Xml/XmlFactory.h"


namespace OGUI
{
	using namespace OGUI;
    class OGUI_API IXmlFactory_Root : public OGUI::IXmlFactory
    {
    public:


        IXmlFactory_Root()
        {
            xml_name = "Root";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.Root";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_Style : public OGUI::IXmlFactory
    {
    public:
        std::string_view path;

        IXmlFactory_Style()
        {
            xml_name = "Style";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.Style";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_Template : public OGUI::IXmlFactory
    {
    public:
        std::string_view name;
        std::string_view path;

        IXmlFactory_Template()
        {
            xml_name = "Template";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.Template";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_AttributeOverrides : public OGUI::IXmlFactory
    {
    public:
        std::string_view element_name;

        IXmlFactory_AttributeOverrides()
        {
            xml_name = "AttributeOverrides";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.AttributeOverrides";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_VisualElement : public OGUI::IXmlFactory
    {
    public:
        std::optional<std::string_view> name;
        std::optional<std::string_view> path;
        std::optional<std::string_view> style;
        std::optional<std::string_view> class_tag;
        std::optional<std::string_view> slot_name;
        std::optional<std::string_view> slot;

        IXmlFactory_VisualElement()
        {
            xml_name = "VisualElement";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.VisualElement";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_Instance : public OGUI::IXmlFactory_VisualElement
    {
    public:
        std::string_view template_name;

        IXmlFactory_Instance()
        {
            xml_name = "Instance";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.Instance";
            
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };

    class OGUI_API IXmlFactory_Text : public OGUI::IXmlFactory_VisualElement
    {
    public:


        IXmlFactory_Text()
        {
            xml_name = "Text";
            xml_namespace = "OGUI";
            xml_qualified_name = "OGUI.Text";
            mixed = true;
        }

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) override;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        void Internal_Init();
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr);
    };


}