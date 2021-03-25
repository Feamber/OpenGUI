#include "xercesc/util/XMLString.hpp"
#include "xercesc/dom/DOMAttr.hpp"
#include "OpenGUI/Xml/XmlAttributeDescription.h"

namespace OGUI
{
	bool XmlAttributeDescription::GetValueString(std::string& out, const DOMElement& element)
	{
        auto node = element.getAttributeNode(XMLString::transcode(name.data()));
        if(node == nullptr) return false;

        out = XMLString::transcode(node->getValue());
		return true;
	}

	std::string XmlStringAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? out : default_value;
	}

	float XmlFloatAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stof(out) : default_value;
	}

	double XmlDoubleAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stod(out) : default_value;
	}

	int XmlIntAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stoi(out) : default_value;
	}

	long XmlLongAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? std::stol(out) : default_value;
	}

	bool XmlBoolAttributeDescription::GetValue(const DOMElement& element)
	{
		std::string out;
		return GetValueString(out, element) ? out == "true" : default_value;
	}
}
