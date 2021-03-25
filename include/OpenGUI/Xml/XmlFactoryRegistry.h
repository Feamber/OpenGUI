#pragma once
#include <map>
#include <string_view>

namespace OGUI
{
	class IXmlFactory;

	class XmlFactoryRegistry
	{
	public:
		static std::map<std::string, IXmlFactory*> factories;

		static void RegisterFactory(IXmlFactory* factory);

		static IXmlFactory* FindFactory(const std::string& Xml_qualified_name);

	private:
		static void RegisterEngineFactories();
	};
}