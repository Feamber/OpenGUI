#pragma once
#include <string>
#include <string_view>
#include <list>
#include <iostream>
#include <set>
#include "OpenGUI/Xml/XmlAsset.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI
{
    class VisualElement;

    // 实例化 XmlAsset 过程中的上下文
    struct CreationContext
    {
        struct Template
        {
            XmlElement &xml_root;
            class TemplateContainer& template_container;

            // key = element-name
            // 当前模板需要覆盖的属性
            std::map<std::string_view , std::set<XmlAttribute>> attribute_overrides;

            // <engine:Template path="/Assets/Portrait.xml" name="Portrait"/>
            // key = name
            // value = path指向的模板root节点
            std::map<std::string, XmlElement*> templates_alias;

            Template(XmlElement &xml_root, class TemplateContainer& template_container) :
                    xml_root(xml_root),
                    template_container(template_container)
            {}
        };

        bool is_error = false;

        // IXmlFactory::Create 处理时第一个是上一级节点
        std::list<VisualElement*> stack = {};

        // IXmlFactory::Create 处理时第一个是当前模板文件
        std::list<Template> stack_template = {};

        // 解析<Instance>时缓存
        XmlElement const * instance_asset;

        // 生成过程中生成的所有VisualElement
        std::list<VisualElement*> all;

        template<class T>
        T* New()
        {
            static_assert(std::is_base_of_v<VisualElement, T>);
            auto new_element = new T();
            all.push_front(new_element);
            return new_element;
        }
    };

    class IXmlFactory
    {
    public:
        std::string_view xml_name;
        std::string_view xml_namespace;
        std::string_view xml_qualified_name;

        virtual bool InitAttribute(VisualElement& new_element, const XmlElement& Asset, CreationContext& context) = 0;
        virtual VisualElement* Create(const XmlElement& asset, CreationContext& context) = 0;

        void Internal_Init(){};
        bool Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr){ return true; };
    };


    template<class TFactoryInterface = IXmlFactory>
    VisualElement* XmlFactoryEmptyCreate(TFactoryInterface& factory, const XmlElement& asset, CreationContext& context)
    {
        static_assert(std::is_base_of_v<IXmlFactory, TFactoryInterface>);

        factory.Internal_Init();
        for(const XmlAttribute& attr : asset.attributes)
        {
            factory.Internal_InitAttribute(hash_(attr.name), attr);
        }
        return nullptr;
    }

    template<class TFactoryInterface = IXmlFactory, class TVisualElement = VisualElement>
    VisualElement* XmlFactoryCreate(TFactoryInterface& factory, const XmlElement& asset, CreationContext& context)
    {
        static_assert(std::is_base_of_v<IXmlFactory, TFactoryInterface>);
        static_assert(std::is_base_of_v<VisualElement, TVisualElement>);

        VisualElement* new_element = context.New<TVisualElement>();
        factory.Internal_Init();
        for(const XmlAttribute& attr : asset.attributes)
        {
            factory.Internal_InitAttribute(hash_(attr.name), attr);
        }
        if(!factory.InitAttribute(*new_element, asset, context))
        {
            context.is_error = true;
            std::cerr << "InitAttribute失败 xml_qualified_name： " << factory.xml_qualified_name << std::endl;
            return nullptr;
        }
        return new_element;
    }
}