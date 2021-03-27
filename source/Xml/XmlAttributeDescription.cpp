#include "OpenGUI/Xml/XmlAttributeDescription.h"
#include "OpenGUI/Xml/XmlAsset.h"

namespace OGUI
{
	bool XmlAttributeDescription::GetValueString(std::string& out, const XmlElement& element)
	{
        auto arrt = element.FindAttribute(name);
        if(arrt == nullptr) return false;

        out = arrt->value;
		return true;
	}

	std::string XmlStringAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? out : default_value;
	}

	float XmlFloatAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stof(out) : default_value;
	}

	double XmlDoubleAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stod(out) : default_value;
	}

	int XmlIntAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stoi(out) : default_value;
	}

	long XmlLongAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stol(out) : default_value;
	}

	bool XmlBoolAttributeDescription::GetValue(const XmlElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? out == "true" : default_value;
	}
}
