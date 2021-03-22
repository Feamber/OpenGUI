#pragma once
#include <string>
#include "XmlTool.h"

namespace OGUI
{
	/*
	class XmlTraits : public AXmlTraits
	{
	public:
#define CHILDREN \
		PARENT_CLASS(AXmlTraits) \
		CHILD(child1, VisualElementA)\
		CHILD(child2, VisualElementB)\
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

		template<class T>
		static XmlChildElementDescription Build()
		{
			static_assert(std::is_base_of_v<class VisualElement, T>);
			return XmlChildElementDescription(NAMEOF_SHORT_TYPE(T), XmlTool::GetTypeNamespace<T>());
		}
	};
}