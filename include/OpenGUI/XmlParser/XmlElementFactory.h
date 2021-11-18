#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/XmlParser/XmlParser.h"

namespace OGUI
{
    class OGUI_API XmlElementFactory
    {
    public:
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
        // 初始化OGUI对象
        virtual bool OnInitElement(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent);
        // 初始化完子级元素的时候
        virtual bool OnInitElementChildPost(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent);
    };
}