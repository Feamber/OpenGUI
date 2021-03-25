#include "OpenGUI/Xml/XmlFactoryRegistry.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include <vector>

namespace OGUI
{
	std::map<std::string, IXmlFactory*> OGUI::XmlFactoryRegistry::factories = {};
}

void OGUI::XmlFactoryRegistry::RegisterFactory(IXmlFactory* factory)
{
	auto result = factories.find(factory->xml_qualified_name);
	if (result == factories.end())
	{
		factories[factory->xml_qualified_name] = factory;
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
		new VisualElement::Factory(),
		new XmlRootElementFactory(),
		new XmlStyleElementFactory,
		new XmlTemplateElementFactory(),
		new XmlAttributeOverridesElementFactory(),
        new TemplateContainer::Factory(),
	};

	for (auto factory : engine_factories)
	{
		RegisterFactory(factory);
	}
}
