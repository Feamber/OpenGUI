#include "OpenGUI/Xml/XmlAttributeDescription.h"

namespace OGUI
{
	bool XmlAttributeDescription::GetValueString(std::string& out, const VisualElementAsset& element)
	{
		return element.GetAttributeValue(out, name);
	}

	std::string XmlStringAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? out : default_value;
	}

	float XmlFloatAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stof(out) : default_value;
	}

	double XmlDoubleAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stod(out) : default_value;
	}

	int XmlIntAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stoi(out) : default_value;
	}

	long XmlLongAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stol(out) : default_value;
	}

	bool XmlBoolAttributeDescription::GetValue(const VisualElementAsset& element)
	{
		std::string out;
		return GetValueString(out, element) ? out == "true" : default_value;
	}
}
