#pragma once
#include <string>
#include <string_view>
#include "OpenGUI/Xml/XmlTypeRestriction.h"
#include "OpenGUI/Xml/VisualElementAsset.h"
#include "nameof/nameof.hpp"

namespace OGUI
{
//#define Attribute(type, attribute_name, constructors) type attribute_name constructors;
//#define Attribute(type, attribute_name, constructors, ...)	type attribute_name constructors; AttributeFor(__VA_ARGS__)
//#define Attributes(type, attribute_name, constructors, ...) struct\
//	{\
//		AttributeFor()
//	} attributes;

	class XmlAttributeDescription
	{
	public:
		inline static const std::string_view xml_schema_namespace = "http://www.w3.org/2001/XMLSchema";

		enum class Use
		{
			Optional,	// 可选
			Required,	// 必须使用
		};

		std::string_view name;
		std::string_view type;
		std::string_view type_namespace;
		std::string default_value_string;
		Use use;
		XmlTypeRestriction restriction;

		XmlAttributeDescription(std::string_view name, std::string_view type, std::string_view type_namespace, std::string default_value_string, Use use) :
			name(name),
			type(type),
			type_namespace(type_namespace),
			default_value_string(default_value_string),
			use(use)
		{}

		bool GetValueString(std::string& out, const VisualElementAsset& element);
	};

	template<typename T>
	class TypeXmlAttributeDescription : public XmlAttributeDescription
	{
	public:
		T default_value;
		TypeXmlAttributeDescription(std::string_view name, std::string_view type, std::string_view type_namespace, std::string default_value_string, Use use, T default_value) :
			XmlAttributeDescription(name, type, type_namespace, default_value_string, use),
			default_value(default_value) {}

		virtual T GetValue(const VisualElementAsset& element) = 0;
	};

	class XmlStringAttributeDescription : public TypeXmlAttributeDescription<std::string>
	{
	public:
		XmlStringAttributeDescription(std::string_view name, std::string default_value, Use use) :
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

		std::string GetValue(const VisualElementAsset& element) override;
	};

	class XmlFloatAttributeDescription : public TypeXmlAttributeDescription<float>
	{
	public:
		XmlFloatAttributeDescription(std::string_view name, float default_value, Use use) :
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

		float GetValue(const VisualElementAsset& element) override;
	};

	class XmlDoubleAttributeDescription : public TypeXmlAttributeDescription<double>
	{
	public:
		XmlDoubleAttributeDescription(std::string_view name, double default_value, Use use) :
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

		double GetValue(const VisualElementAsset& element) override;
	};

	class XmlIntAttributeDescription : public TypeXmlAttributeDescription<int>
	{
	public:
		XmlIntAttributeDescription(std::string_view name, int default_value, Use use) :
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

		int GetValue(const VisualElementAsset& element) override;
	};

	class XmlLongAttributeDescription : public TypeXmlAttributeDescription<long>
	{
	public:
		XmlLongAttributeDescription(std::string_view name, long default_value, Use use) :
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

		long GetValue(const VisualElementAsset& element) override;
	};

	class XmlBoolAttributeDescription : public TypeXmlAttributeDescription<bool>
	{
	public:
		XmlBoolAttributeDescription(std::string_view name, bool default_value, Use use) :
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

		bool GetValue(const VisualElementAsset& element) override;
	};
	
	template<typename E>
	class XmlEnumAttributeDescription : public TypeXmlAttributeDescription<E>
	{
	public:
		virtual const std::vector<std::pair<std::string, E>>& GetEnumEntries() = 0;

		XmlEnumAttributeDescription(std::string_view name, E default_value, Use use) :
			TypeXmlAttributeDescription(
				name,
				"string",
				xml_schema_namespace,
				NAMEOF_ENUM(DefaultValue),
				use,
				default_value
			)
		{
			static_assert(std::is_enum_v<E>);

			auto entries = GetEnumEntries();
			restriction.type = XmlTypeRestriction::Type::Enum;
			restriction.enums.resize(entries.size());
			for (auto entrie : entries)
			{
				restriction.enums.push_back(entrie.first);
			}
		}

		bool GetValue(const VisualElementAsset& element) override
		{
			std::string out;
			if (GetValueString(out, element))
			{
				auto entries = GetEnumEntries();
				for (auto entrie : entries)
				{
					if (out == entrie.first)
						return entrie.second;
				}
			}

			return default_value;
		}
	};
}