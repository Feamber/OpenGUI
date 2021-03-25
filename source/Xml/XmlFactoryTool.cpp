#include <iostream>
#include "OpenGUI/Xml/XmlFactoryTool.h"

namespace OGUI
{
    VisualElement *XmlTemplateElementFactory::Create(const DOMElement &asset, CreationContext &context)
    {
        auto new_asset = XmlAsset::LoadXmlFile(_traits.path.GetValue(asset));
        if(new_asset != nullptr)
        {
            std::string name = _traits.name.GetValue(asset);
            if(!name.empty())
            {
                auto& templates_alias = context.stack_template.front().templates_alias;
                auto result = templates_alias.find(name);
                if(result == templates_alias.end())
                {
                    templates_alias[name] = &new_asset->root;
                    return nullptr;
                }
                std::cerr << "重复的<Template>别名 name: " << name << std::endl;
            }
        }

        context.is_error = true;
        // <Template> 这个元素没有实际控件
        return nullptr;
    }

    VisualElement *TemplateContainer::Factory::Create(const DOMElement &asset, CreationContext &context)
    {
        auto& templates_alias = context.stack_template.front().templates_alias;
        std::string template_name = _traits.template_name.GetValue(asset);
        auto result = templates_alias.find(template_name);
        if(result != templates_alias.end())
        {
            context.instance_asset = &asset;
            auto new_template_container = XmlAsset::ParseTemplate(*result->second, context);
            if(new_template_container == nullptr)
            {
                context.is_error = true;
                std::cerr << "ParseTemplate失败 aliasName： " << template_name << std::endl;
                return nullptr;
            }
            if(!_traits.InitAttribute(*new_template_container, asset, context))
            {
                context.is_error = true;
                std::cerr << "ParseTemplate InitAttribute失败 aliasName： " << template_name << std::endl;
                return nullptr;
            }

            return new_template_container;
        }

        context.is_error = true;
        std::cerr << "<Instance>没找到模板别名 template_name: " << template_name << std::endl;
        return nullptr;
    }

    bool TemplateContainer::Traits::InitAttribute(VisualElement &new_element, const DOMElement &asset, CreationContext& context)
    {
        return VisualElement::Traits::InitAttribute(new_element, asset, context);
    }
    TemplateContainer::TemplateContainer()
    {
        
    }
}

