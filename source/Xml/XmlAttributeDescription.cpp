#include "OpenGUI/Xml/XmlAttributeDescription.h"

bool XmlAttributeDescription::GetValueString(std::string& out, const VisualElementAsset& element)
{
	return element.GetValueString(name, out);
}

std::string XmlStringAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? out : dedefault_value;
}

float XmlFloatAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? std::stof(out) : dedefault_value;
}

double XmlDoubleAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? std::stod(out) : dedefault_value;
}

int XmlIntAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? std::stoi(out) : dedefault_value;
}

long XmlLongAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? std::stol(out) : dedefault_value;
}

bool XmlBoolAttributeDescription::GetValue(const VisualElementAsset& element)
{
	std::string out;
	return GetValueString(out, element) ? out == "true" : dedefault_value;
}
