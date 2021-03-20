#pragma once
#include <string>
#include "XmlTypeRestriction.h"
#include "VisualElementAsset.h"
#include "../nameof/nameof.hpp"
#include "../magic_enum/magic_enum.hpp"

class XmlAttributeDescription
{
public:
	static const std::string xml_schema_namespace = "http://www.w3.org/2001/XMLSchema";

	enum class Use
	{
		Optional,	// 可选
		Required,	// 必须使用
	};

	Use use = Use::Optional;

	std::string name;
	std::string type;
	std::string type_namespace;
	std::string default_value_string;
	XmlTypeRestriction restriction;

	bool GetValueString(std::string& out, const VisualElementAsset& element);
};

template<typename T>
class TypeXmlAttributeDescription
{
public:
	T default_value;

	virtual T GetValue(const VisualElementAsset& element) = 0;
};

class XmlStringAttributeDescription : public TypeXmlAttributeDescription<std::string>
{
public:
	XmlStringAttributeDescription()
	{
		type = "string";
		type_namespace = xml_schema_namespace;
		default_value = "";
		default_value_string = "";
	}

	std::string GetValue(const VisualElementAsset& element) override;
};

class XmlFloatAttributeDescription : public TypeXmlAttributeDescription<float>
{
public:
	static std::string AsString();

	XmlStringAttributeDescription()
	{
		type = "float";
		type_namespace = xml_schema_namespace;
		default_value = 0.0f;
		default_value_string = std::to_string(default_value);
	}

	float GetValue(const VisualElementAsset& element) override;
};

class XmlDoubleAttributeDescription : public TypeXmlAttributeDescription<double>
{
public:
	XmlStringAttributeDescription()
	{
		type = "double";
		type_namespace = xml_schema_namespace;
		default_value = 0.0;
		default_value_string = std::to_string(default_value);
	}

	double GetValue(const VisualElementAsset& element) override;
};

class XmlIntAttributeDescription : public TypeXmlAttributeDescription<int>
{
public:
	XmlStringAttributeDescription()
	{
		type = "int";
		type_namespace = xml_schema_namespace;
		default_value = 0;
		default_value_string = std::to_string(default_value);
	}

	int GetValue(const VisualElementAsset& element) override;
};

class XmlLongAttributeDescription : public TypeXmlAttributeDescription<long>
{
public:
	XmlStringAttributeDescription()
	{
		type = "long";
		type_namespace = xml_schema_namespace;
		default_value = 0;
		default_value_string = std::to_string(default_value);
	}

	long GetValue(const VisualElementAsset& element) override;
};

class XmlBoolAttributeDescription : public TypeXmlAttributeDescription<bool>
{
public:
	XmlStringAttributeDescription()
	{
		type = "boolean";
		type_namespace = xml_schema_namespace;
		default_value = false;
		default_value_string = dedefault_value ? "true" : "false";
	}

	bool GetValue(const VisualElementAsset& element) override;
};

template<typename E, std::enable_if_t<std::is_enum_v<E>, E> DefaultValue>
class XmlEnumAttributeDescription : public TypeXmlAttributeDescription<E>
{
public:
	constexpr auto& entries = magic_enum::enum_entries<E>();

	XmlStringAttributeDescription()
	{
		type = "string";
		type_namespace = xml_schema_namespace;
		default_value = DefaultValue;
		default_value_string = NAMEOF_ENUM(DefaultValue);

		restriction.type = XmlTypeRestriction::Type::Enumeration;
		restriction.enums.resize(entries.size());
		for (auto entrie : entries)
		{
			restriction.enums.push_back(entrie.second);
		}
	}

	bool GetValue(const VisualElementAsset& element) override
	{
		std::string out;
		if (GetValueString(out, element))
		{
			auto e = magic_enum::enum_cast<E>(out);
			if (e.has_value()) return e.value();
		}
		
		return dedefault_value;
	}
};
