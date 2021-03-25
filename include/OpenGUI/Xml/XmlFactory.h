#pragma once
#include <string>
#include <string_view>
#include <list>
#include <iostream>
#include <set>
#include "XmlTool.h"
#include "XmlAttributeDescription.h"
#include "XmlChildElementDescription.h"

namespace OGUI
{
    class VisualElement;

    // 实例化 XmlAsset 过程中的上下文
    struct CreationContext
    {
        struct AttributeOverrid
        {
            std::string name;
            std::string value;
            bool operator<(const struct AttributeOverrid& right)const
            {
                return name < right.name;
            }

            AttributeOverrid(const std::string &name, const std::string &value) : name(name), value(value) {}
        };

        struct Template
        {
            DOMElement &xml_root;
            class TemplateContainer& template_container;

            // key = element-name
            // 当前模板需要覆盖的属性
            std::map<std::string, std::set<AttributeOverrid>> attribute_overrides;

            // <engine:Template path="/Assets/Portrait.xml" name="Portrait"/>
            // key = name
            // value = path指向的模板root节点
            std::map<std::string, DOMElement*> templates_alias;

            Template(DOMElement &xml_root, class TemplateContainer& template_container) :
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
        DOMElement const* instance_asset;

        // 生成过程中生成的所有VisualElement
        std::list<std::shared_ptr<VisualElement>> all;

        template<class T>
        T* New()
        {
            static_assert(std::is_base_of_v<VisualElement, T>);
            auto new_element = std::make_shared<T>();
            all.push_front(new_element);
            return new_element.get();
        }
    };

    // XmlTraits描述了从 VisualElement 派生的类的XML属性和子元素类型。
    // XmlFactory 使用它在读取XML文档时将XML属性映射到C++类属性。
    // XmlTraits 还用于生成 XML schema。
    class XmlTraits
    {
    public:
        // 描述元素的XML属性
        std::vector<XmlAttributeDescription*> attributes_desc;

        // 描述在XML文件中可以作为该元素的子元素出现的元素的类型
        std::vector<XmlChildElementDescription*> child_desc;

        // 子类需要在这将自定义XML属性解析到C++类属性上
        // 注意先调用父类
        bool InitAttribute(VisualElement& new_element, const DOMElement& Asset, CreationContext& context) {return true;};

        // GenXmlAttrs.h
        void GetAllAttr(std::vector<XmlAttributeDescription*>& result) {};

        // GenXmlChildDesc.h
        void GetAllChild(std::vector<XmlChildElementDescription*>& result) {};
    };

    class IXmlFactory
    {
    public:
        // XML文件中的元素名
        // 在命名空间中是唯一
        std::string xml_name;

        // XML元素的命名空间 xx.xx.xx
        std::string xml_namespace;

        // XML文件中的元素名
        // 在整个项目中唯一
        std::string xml_qualified_name;

        // 是否可以接受任何属性 对应 XmlSchema.anyAttribute
        bool any_attribute = false;

        // 创建新 VisualElement
        // 有些XML元素不会生成 VisualElement 只是用来辅助生成 XML schema，比如 <Root> <Template>
        virtual VisualElement* Create(const DOMElement& asset, CreationContext& context) = 0;
    };

    // 每种 VisualElement 都应该有个对应的 XmlFactory
    template<class TCreatedType, class TTraits>
    class XmlFactory : public IXmlFactory
    {
        static_assert(std::is_base_of_v<VisualElement, TCreatedType>);
        static_assert(std::is_base_of_v<XmlTraits, TTraits>);

    public:
        VisualElement* Create(const DOMElement& asset, CreationContext& context) override
        {
            VisualElement* new_element = context.New<TCreatedType>();
            if(!_traits.InitAttribute(*new_element, asset, context))
            {
                context.is_error = true;
                std::cerr << "InitAttribute失败 xml_qualified_name： " << xml_qualified_name << std::endl;
                delete new_element;
                return nullptr;
            }
            return new_element;
        }

    protected:
        XmlFactory()
        {
//            xml_name = NAMEOF_SHORT_TYPE(TCreatedType).data();
//            xml_namespace = XmlTool::GetTypeNamespace<TCreatedType>();
//            xml_qualified_name = xml_namespace == "" ? xml_name : xml_namespace + '.' + xml_name;
            _traits.GetAllAttr(_traits.attributes_desc);
            _traits.GetAllChild(_traits.child_desc);
        }

        TTraits _traits;
    };
}