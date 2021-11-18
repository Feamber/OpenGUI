#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/XmlParser/XmlParser.h"

namespace OGUI
{
    class OGUI_API XmlElementFactory
    {
    public:
        virtual ~XmlElementFactory() {}
        //static const Name& GetFullName();

        // 返回 false 表示解析失败，最好每个失败都有日志输出原因

        // 解析xml元素
        virtual bool OnParseXmlElement(ParseXmlState&, XmlElement&);
        // 解析完同级xml元素的时候
        virtual bool OnParseXmlElementSiblingPost(ParseXmlState&, XmlElement&);
        // 解析完子级xml元素的时候
        virtual bool OnParseXmlElementChildPost(ParseXmlState&, XmlElement&);

        // 创建OGUI元素
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent);
        // 这里主要写约束子元素类型
        // 在实例化阶段一定要通过调用父元素的 XmlElementFactory::PushChild 来添加
        virtual bool PushChild(InstantiateXmlState&, XmlElement& elementXml, VisualElement* element, XmlElement& childXml, VisualElement* child);
        // 初始化层级结构
        // 如果已经Push到其他元素上 isAutoPushToParent 失效
        virtual bool OnInitElementHierarchy(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent, bool& isAutoPushToParent);
        // 初始化OGUI对象
        virtual bool OnInitElement(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent);
        // 初始化完子级元素的时候
        virtual bool OnInitElementChildPost(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent);
    };
}