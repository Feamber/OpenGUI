#include "OpenGUI/Xml/XsdGen/OGUI.h"
#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Xml/XmlFactoryTool.h"

namespace FactoryTool
{
    using namespace std;
    void split(const string& s, vector<string>& tokens, const string_view& delimiters = " ")
    {
        string::size_type lastPos = s.find_first_not_of(delimiters, 0);
        string::size_type pos = s.find_first_of(delimiters, lastPos);
        while (string::npos != pos || string::npos != lastPos)
        {
            auto substr = s.substr(lastPos, pos - lastPos);
            tokens.push_back(substr);//use emplace_back after C++11
            lastPos = s.find_first_not_of(delimiters, pos);
            pos = s.find_first_of(delimiters, lastPos);
        }
    }
}

namespace OGUI
{
    VisualElement* IXmlFactory_Root::Create(const XmlElement& asset, CreationContext& context)
    {
        return XmlFactoryEmptyCreate(*this, asset, context);
    }

    bool IXmlFactory_Root::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return true;
    }

    VisualElement* IXmlFactory_Style::Create(const XmlElement& asset, CreationContext& context)
    {
        XmlFactoryEmptyCreate(*this, asset, context);
        if(context.is_error) return nullptr;
        //todo: cache css file
        auto value = ParseCSSFile({path.begin(), path.end()});
        if (!value)
            return nullptr;//todo: log error
        auto target = context.stack.front();
        if (!target)
            return nullptr;//todo: log error
        auto sheet = new StyleSheet{value.value()};
        sheet->Initialize();
        target->_styleSheets.push_back(sheet);
        // <Style> 这个元素没有实际控件
        return nullptr;
    }

    bool IXmlFactory_Style::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return true;
    }

    VisualElement* IXmlFactory_Template::Create(const XmlElement& asset, CreationContext& context)
    {
        XmlFactoryEmptyCreate(*this, asset, context);

        auto new_asset = XmlAsset::LoadXmlFile({path.begin(), path.end()});
        if(new_asset)
        {
            context.main_asset->all_child_asset.emplace_back(new_asset);
            if(!name.empty())
            {
                const std::string sname = {name.begin(), name.end()};
                auto& templates_alias = context.stack_template.front().templates_alias;
                auto result = templates_alias.find(sname);
                if(result == templates_alias.end())
                {
                    templates_alias[sname] = &new_asset->root;
                    return nullptr;
                }
                std::cerr << "重复的<Template>别名 name: " << name << std::endl;
            }
        }

        context.is_error = true;
        // <Template> 这个元素没有实际控件
        return nullptr;
    }

    bool IXmlFactory_Template::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return true;
    }

    VisualElement* IXmlFactory_AttributeOverrides::Create(const XmlElement& asset, CreationContext& context)
    {
        return XmlFactoryEmptyCreate(*this, asset, context);
    }

    bool IXmlFactory_AttributeOverrides::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return true;
    }

    VisualElement* IXmlFactory_VisualElement::Create(const XmlElement& asset, CreationContext& context)
    {
        return XmlFactoryCreate<IXmlFactory_VisualElement, VisualElement>(*this, asset, context);
    }

    bool IXmlFactory_VisualElement::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        if(name.has_value()) new_element._name = name.value();
        if(path.has_value()) new_element._path = path.value();
        if(style.has_value()) new_element.InitInlineStyle(style.value());
        if(class_tag.has_value())
            FactoryTool::split({class_tag.value().begin(), class_tag.value().end()}, new_element._styleClasses, ",");

        if(slot_name.has_value())
        {
            std::string _slot_name = {slot_name.value().begin(), slot_name.value().end()};
            auto& template_container = context.stack_template.front().template_container;
            template_container.slots[_slot_name] = &new_element;
        }

        if(slot.has_value())
        {
            std::string _slot = {slot.value().begin(), slot.value().end()};
            auto parent_node = context.stack.front();
            if(parent_node->IsA("TemplateContainer"))
            {
                auto template_container = (TemplateContainer*)parent_node;
                auto find_slots = template_container->slots.find(_slot);
                if(find_slots != template_container->slots.end())
                    find_slots->second->PushChild(&new_element);
            }
        }
        return true;
    }

    VisualElement* IXmlFactory_Instance::Create(const XmlElement& asset, CreationContext& context)
    {
        XmlFactoryEmptyCreate(*this, asset, context);
        if(context.is_error) return nullptr;

        auto& templates_alias = context.stack_template.front().templates_alias;
        auto result = templates_alias.find({template_name.begin(), template_name.end()});
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

            Internal_Init();
            for(const XmlAttribute& attr : asset.attributes)
            {
                Internal_InitAttribute(hash_(attr.name), attr);
            }
            if(!InitAttribute(*new_template_container, asset, context))
            {
                context.is_error = true;
                std::cerr << "InitAttribute失败 xml_qualified_name： " << xml_qualified_name << std::endl;
                return nullptr;
            }

            return new_template_container;
        }

        context.is_error = true;
        std::cerr << "<Instance>没找到模板别名 template_name: " << template_name << std::endl;
        return nullptr;
    }

    bool IXmlFactory_Instance::InitAttribute(VisualElement &new_element, const XmlElement &asset, CreationContext &context)
    {
        return IXmlFactory_VisualElement::InitAttribute(new_element, asset, context);
    }
}

