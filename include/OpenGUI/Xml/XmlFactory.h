#pragma once
#include <string>
#include <string_view>
#include "XmlTool.h"
#include "XmlAttributeDescription.h"
#include "XmlChildElementDescription.h"

namespace OGUI
{
    class VisualElement;
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
        void InitAttribute(VisualElement& new_element, const VisualElementAsset& Asset) {};

        // GenXmlAttrs.h
        void GetAllAttr(std::vector<XmlAttributeDescription*>& result) {};

        // GenXmlChildDesc.h
        void GetAllChild(std::vector<XmlAttributeDescription*>& result) {};
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
        virtual VisualElement* Create(const VisualElementAsset& asset) = 0;
    };

    // 每种 VisualElement 都应该有个对应的 XmlFactory
    template<class TCreatedType, class TTraits>
    class XmlFactory : public IXmlFactory
    {
        static_assert(std::is_base_of_v<VisualElement, TCreatedType>);
        static_assert(std::is_base_of_v<XmlTraits, TTraits>);
    private:
        TTraits _traits;

    public:
        VisualElement* Create(const VisualElementAsset& asset) override
        {
            VisualElement* new_element = new TCreatedType();
            _traits.InitAttribute(*new_element, asset);
            return new_element;
        }

    private:
        XmlFactory()
        {
            xml_name = NAMEOF_SHORT_TYPE(TCreatedType).data();
            xml_namespace = xmlTool::GetTypeNamespace<TCreatedType>();
            xml_qualified_name = xml_namespace == "" ? xml_name : xml_namespace + '.' + xml_name;
            _traits.GetAllAttrDesc(_traits.attributes_desc);
        }
    };
}