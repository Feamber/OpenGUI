#pragma once
#include <string>

namespace OGUI
{
	/*
	class XmlTraits : public AXmlTraits
	{
	public:
#define CHILDREN \
		PARENT_CLASS(AXmlTraits) \
		CHILD(child1, "VisualElementA", "OGUI")\
		CHILD(child2, "VisualElementB", "OGUI")\
#include "OpenGUI/Xml/GenXmlChildDesc.h"
	}
	*/
	struct XmlChildElementDescription
	{
		std::string_view elementName;
		std::string elementNamespace;

		XmlChildElementDescription(std::string_view elementName, std::string&& elementNamespace) :
			elementName(elementName),
			elementNamespace(elementNamespace)
		{}
	};
}