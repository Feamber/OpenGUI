#pragma once
#include <map>
#include <string_view>
#include "OpenGUI/Core/Name.h"

namespace OGUI
{
	class IXmlFactory;

	class XmlFactoryRegistry
	{
	public:
		static std::map<Name, IXmlFactory*> factories;

		static void RegisterFactory(IXmlFactory* factory);

		static IXmlFactory* FindFactory(const Name& Xml_qualified_name);

	private:
		static void RegisterEngineFactories();
	};
}