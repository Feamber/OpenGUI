#pragma once
#include <string>
#include <string_view>
#include <xercesc/dom/DOMElement.hpp>
#include "OpenGUI/Xml/XmlTypeRestriction.h"
#include "nameof/nameof.hpp"

using namespace XERCES_CPP_NAMESPACE;
namespace OGUI
{
	/*
	class XmlTraits : public AXmlTraits
	{
	public:
#define ATTRS \
		PARENT_CLASS(AXmlTraits) \
		ATTR(XmlFloatAttributeDescription, angle, 45.f, XmlAttributeUse::Optional)\
		ATTR(XmlStringAttributeDescription, describe, "描述", XmlAttributeUse::Required)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h.h"
	}
	*/

	enum class XmlAttributeUse
	{
		Optional,	// 可选
		Required,	// 必须使用
	};

	class XmlGenericAttributeNames
	{
	public:
		inline static const std::string name = "name";
		inline static const std::string path = "path";
	};

	class XmlAttributeDescription
	{
	public:
		inline static const std::string_view xml_schema_namespace = "http://www.w3.org/2001/XMLSchema";

		std::string_view name;
		std::string_view type;
		std::string_view type_namespace;
		std::string default_value_string;
		XmlAttributeUse use;
		XmlTypeRestriction restriction;

		XmlAttributeDescription(std::string_view name, std::string_view type, std::string_view type_namespace, std::string default_value_string, XmlAttributeUse use) :
			name(name),
			type(type),
			type_namespace(type_namespace),
			default_value_string(default_value_string),
			use(use)
		{}

		bool GetValueString(std::string& out, const DOMElement& element);
	};

	template<class T>
	class TypeXmlAttributeDescription : public XmlAttributeDescription
	{
	public:
		T default_value;
		TypeXmlAttributeDescription(std::string_view name, std::string_view type, std::string_view type_namespace, std::string default_value_string, XmlAttributeUse use, T default_value) :
			XmlAttributeDescription(name, type, type_namespace, default_value_string, use),
			default_value(default_value) {}

		virtual T GetValue(const DOMElement& element) = 0;
	};

	class XmlStringAttributeDescription : public TypeXmlAttributeDescription<std::string>
	{
	public:
		XmlStringAttributeDescription(std::string_view name, std::string default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"string",
				xml_schema_namespace,
				default_value,
				use,
				default_value
			)
		{
		}

		std::string GetValue(const DOMElement& element) override;
	};

	class XmlFloatAttributeDescription : public TypeXmlAttributeDescription<float>
	{
	public:
		XmlFloatAttributeDescription(std::string_view name, float default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"float",
				xml_schema_namespace,
				std::to_string(default_value),
				use,
				default_value
			)
		{
		}

		float GetValue(const DOMElement& element) override;
	};

	class XmlDoubleAttributeDescription : public TypeXmlAttributeDescription<double>
	{
	public:
		XmlDoubleAttributeDescription(std::string_view name, double default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"double",
				xml_schema_namespace,
				std::to_string(default_value),
				use,
				default_value
			)
		{
		}

		double GetValue(const DOMElement& element) override;
	};

	class XmlIntAttributeDescription : public TypeXmlAttributeDescription<int>
	{
	public:
		XmlIntAttributeDescription(std::string_view name, int default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"int",
				xml_schema_namespace,
				std::to_string(default_value),
				use,
				default_value
			)
		{
		}

		int GetValue(const DOMElement& element) override;
	};

	class XmlLongAttributeDescription : public TypeXmlAttributeDescription<long>
	{
	public:
		XmlLongAttributeDescription(std::string_view name, long default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"long",
				xml_schema_namespace,
				std::to_string(default_value),
				use,
				default_value
			)
		{
		}

		long GetValue(const DOMElement& element) override;
	};

	class XmlBoolAttributeDescription : public TypeXmlAttributeDescription<bool>
	{
	public:
		XmlBoolAttributeDescription(std::string_view name, bool default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription(
				name,
				"boolean",
				xml_schema_namespace,
				default_value ? "true" : "false",
				use,
				default_value
			)
		{
		}

		bool GetValue(const DOMElement& element) override;
	};
	
	template<class E>
	class XmlEnumAttributeDescription 
		: public TypeXmlAttributeDescription<std::enable_if_t<std::is_enum_v<E>, E>>
	{
	public:
		virtual const std::vector<std::pair<std::string, E>>& GetEnumEntries() = 0;

		XmlEnumAttributeDescription(std::string_view name, E default_value, XmlAttributeUse use) :
			TypeXmlAttributeDescription<E>(
				name,
				"string",
				XmlAttributeDescription::xml_schema_namespace,
				NAMEOF_ENUM(default_value),
				use,
				default_value
			)
		{
			auto entries = GetEnumEntries();
			XmlAttributeDescription::restriction.type = XmlTypeRestriction::Type::Enum;
			XmlAttributeDescription::restriction.enums.resize(entries.size());
			for (auto entrie : entries)
			{
				XmlAttributeDescription::restriction.enums.push_back(entrie.first);
			}
		}

		bool GetValue(const DOMElement& element) override
		{
			std::string out;
			if (XmlAttributeDescription::GetValueString(out, element))
			{
				auto entries = GetEnumEntries();
				for (auto entrie : entries)
				{
					if (out == entrie.first)
						return entrie.second;
				}
			}
			return true;
			//return default_value;
		}
	};
}