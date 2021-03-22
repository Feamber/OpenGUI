#pragma once
#include <map>
#include <string_view>

namespace OGUI
{
	class IXmlFactory;

	class XmlFactoryRegistry
	{
	public:
		static std::map<std::string_view, IXmlFactory*> factories;

		static void RegisterFactory(IXmlFactory* factory);

		static IXmlFactory* FindFactory(std::string_view Xml_qualified_name);

	private:
		static void RegisterEngineFactories();
	};
}