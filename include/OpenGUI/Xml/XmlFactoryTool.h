#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlAttributeDescription.h"
#include "OpenGUI/Xml/XmlChildElementDescription.h"

namespace OGUI
{
	// root -----------------
	class XmlRootElementTraits : public XmlTraits
	{
	public:
#define CHILDREN \
		PARENT_CLASS(XmlTraits) \
		CHILD(child1, VisualElement)
#include "OpenGUI/Xml/GenXmlChildDesc.h"
	};

	class XmlRootElementFactory : XmlFactory<VisualElement, XmlRootElementTraits> 
	{
	public:
		inline static const std::string_view element_name = "Root";

		XmlRootElementFactory()
		{
			xml_name = element_name;
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const VisualElementAsset& asset) override
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
		ATTR(XmlStringAttributeDescription, path, XmlGenericAttributeNames::path, XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"
	};

	class XmlStyleElementFactory : XmlFactory<VisualElement, XmlStyleElementTraits>
	{
	public:
		inline static const std::string_view element_name = "Style";

		XmlStyleElementFactory()
		{
			xml_name = element_name;
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const VisualElementAsset& asset) override
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
		ATTR(XmlStringAttributeDescription, name, XmlGenericAttributeNames::name, XmlAttributeUse::Optional)\
		ATTR(XmlStringAttributeDescription, path, XmlGenericAttributeNames::path, XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"
	};

	class XmlTemplateElementFactory : XmlFactory<VisualElement, XmlTemplateElementTraits>
	{
	public:
		inline static const std::string_view element_name = "Template";

		XmlTemplateElementFactory()
		{
			xml_name = element_name;
			xml_qualified_name = xml_namespace + '.' + xml_name;
		}

		VisualElement* Create(const VisualElementAsset& asset) override
		{
			// <Template> 这个元素没有实际控件
			return nullptr;
		}
	};

	// AttributeOverrides -----------------------------------------
	class XmlAttributeOverridesElementTraits : public XmlTraits
	{
	public:
#define CHILDREN \
		PARENT_CLASS(XmlTraits) \
		CHILD(child1, VisualElement)
#include "OpenGUI/Xml/GenXmlChildDesc.h"
	};

	class XmlAttributeOverridesElementFactory : XmlFactory<VisualElement, XmlAttributeOverridesElementTraits>
	{
	public:
		inline static const std::string_view element_name = "AttributeOverrides";

		XmlAttributeOverridesElementFactory()
		{
			xml_name = element_name;
			xml_qualified_name = xml_namespace + '.' + xml_name;
			any_attribute = true;
		}

		VisualElement* Create(const VisualElementAsset& asset) override
		{
			// <AttributeOverrides> 这个元素没有实际控件
			return nullptr;
		}
	};
}