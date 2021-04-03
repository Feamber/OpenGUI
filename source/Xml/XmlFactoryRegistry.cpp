#include "OpenGUI/Xml/XmlFactoryRegistry.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include "OpenGUI/Xml/XsdGen/OGUI.h"
#include "OpenGUI/Core/Containers/vector.hpp"

namespace OGUI
{
	std::map<std::string, IXmlFactory*> OGUI::XmlFactoryRegistry::factories = {};
}

void OGUI::XmlFactoryRegistry::RegisterFactory(IXmlFactory* factory)
{
    std::string xml_qualified_name = {factory->xml_qualified_name.begin(), factory->xml_qualified_name.end()};
	auto result = factories.find(xml_qualified_name);
	if (result == factories.end())
	{
		factories[xml_qualified_name] = factory;
	}
	// TODO 日志：重复注册 XmlFactory
}

OGUI::IXmlFactory* OGUI::XmlFactoryRegistry::FindFactory(const std::string& Xml_qualified_name)
{
	struct Initial
	{
		Initial()
		{
			RegisterEngineFactories();
		}
	};
	static Initial init;

	auto result = factories.find(Xml_qualified_name);
	if (result != factories.end()) return result->second;
	return nullptr;
}

void OGUI::XmlFactoryRegistry::RegisterEngineFactories()
{
	std::vector<IXmlFactory*> engine_factories =
	{
		new IXmlFactory_Root(),
        new IXmlFactory_Style(),
        new IXmlFactory_Template(),
        new IXmlFactory_AttributeOverrides(),
        new IXmlFactory_VisualElement(),
        new IXmlFactory_Instance(),
        new IXmlFactory_Text(),
        new IXmlFactory_TextValue(),
	};

	for (auto factory : engine_factories)
	{
		RegisterFactory(factory);
	}
}
