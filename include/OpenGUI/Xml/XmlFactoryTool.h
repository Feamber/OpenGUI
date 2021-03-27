#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlAttributeDescription.h"
#include "OpenGUI/Xml/XmlChildElementDescription.h"
#include "OpenGUI/Xml/XmlAsset.h"

namespace OGUI
{
	// root -----------------
	class XmlRootElementTraits : public XmlTraits
	{
	public:
#define CHILDREN \
		PARENT_CLASS(XmlTraits) \
		CHILD(child1, "VisualElement", "OGUI")
#include "OpenGUI/Xml/GenXmlChildDesc.h"
	};

	class XmlRootElementFactory : public XmlFactory<VisualElement, XmlRootElementTraits> 
	{
	public:
		inline static const std::string_view element_name = "Root";

		XmlRootElementFactory()
		{
			xml_name = element_name;
            xml_namespace = "OGUI";
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const XmlElement& asset, CreationContext& context) override
		{
			// <Root> 这个元素没有实际控件
			return nullptr;
		}
	};

	// style ----------------------------
	class XmlStyleElementTraits : public XmlTraits
	{
	public:
#define ATTRS \
		PARENT_CLASS(XmlTraits) \
		ATTR(XmlStringAttributeDescription, path, "", XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"
	};

	class XmlStyleElementFactory : public XmlFactory<VisualElement, XmlStyleElementTraits>
	{
	public:
		inline static const std::string_view element_name = "Style";

		XmlStyleElementFactory()
		{
			xml_name = element_name;
            xml_namespace = "OGUI";
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const XmlElement& asset, CreationContext& context) override
		{
			// <Style> 这个元素没有实际控件
			return nullptr;
		}
	};

	// template -------------------------
	class XmlTemplateElementTraits : public XmlTraits
	{
	public:
#define ATTRS \
		PARENT_CLASS(XmlTraits) \
		ATTR(XmlStringAttributeDescription, name, "", XmlAttributeUse::Required)\
		ATTR(XmlStringAttributeDescription, path, "", XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"
	};

	class XmlTemplateElementFactory : public XmlFactory<VisualElement, XmlTemplateElementTraits>
	{
	public:
		inline static const std::string_view element_name = "Template";

		XmlTemplateElementFactory()
		{
			xml_name = element_name;
            xml_namespace = "OGUI";
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
	};

	// AttributeOverrides -----------------------------------------
	class XmlAttributeOverridesElementTraits : public XmlTraits
	{
	public:
#define ATTRS \
		PARENT_CLASS(XmlTraits) \
		ATTR(XmlStringAttributeDescription, element_name, "", XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"

#define CHILDREN \
		PARENT_CLASS(XmlTraits) \
		CHILD(child1, "VisualElement", "OGUI")
#include "OpenGUI/Xml/GenXmlChildDesc.h"
	};

	class XmlAttributeOverridesElementFactory : public XmlFactory<VisualElement, XmlAttributeOverridesElementTraits>
	{
	public:
		inline static const std::string_view element_name = "AttributeOverrides";

		XmlAttributeOverridesElementFactory()
		{
			xml_name = element_name;
            xml_namespace = "OGUI";
			xml_qualified_name = xml_namespace + '.' + xml_name;
			any_attribute = true;
		}

		VisualElement* Create(const XmlElement& asset, CreationContext& context) override
        {
            // <AttributeOverrides> 这个元素没有实际控件
            return nullptr;
        }
	};

	// TemplateContainer ------------------------------
    class TemplateContainer : public VisualElement
    {
    public:
        std::map<std::string, VisualElement*> slots;
		TemplateContainer();

        virtual std::string_view GetTypeName() { return "TemplateContainer"; }
        virtual std::string_view GetFullTypeName() { return "OGUI::TemplateContainer"; }

        class Traits : public VisualElement::Traits
        {
        public:
#define ATTRS \
			PARENT_CLASS(VisualElement::Traits) \
			ATTR(XmlStringAttributeDescription, template_name, "", XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"

            bool InitAttribute(VisualElement& new_element, const XmlElement& asset, CreationContext& context);
        };

        class Factory : public XmlFactory<VisualElement, Traits>
        {
        public:
            Factory()
            {
                xml_name = "Instance";
                xml_namespace = "OGUI";
                xml_qualified_name = xml_namespace + '.' + xml_name;
            }

            VisualElement* Create(const XmlElement& asset, CreationContext& context) override;
        };
    };
}