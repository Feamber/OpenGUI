#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/XmlParser/XmlParser.h"
#include "OpenGUI/XmlParser/XmlElementFactory.h"

namespace OGUI
{
    void OGUI_API RegisterOGUIXmlParser();

    class OGUI_API VisualElementXmlFactory : public XmlElementFactory
    {
    public:
        static const Name& GetFullName();

        static const Name& Attr_Name();
        static const Name& Attr_Class();
        static const Name& Attr_Style();
        static const Name& Attr_InsertSlot();
        static const Name& Attr_Slot();
        static const Name& Attr_FocusScope();
        static const Name& Attr_KeepFocused();
        static const Name& Attr_NavCycleMode();
        static const Name& Attr_Focusable();
        static const Name& Attr_NavMode();
        static const Name& Attr_NavUp();
        static const Name& Attr_NavDown();
        static const Name& Attr_NavLeft();
        static const Name& Attr_NavRight();
        static const Name& Attr_Filters();
        static const Name& Attr_Retained();
        static const Name& Attr_RenderTarget();

        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
        virtual bool OnInitElementHierarchy(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent, bool& isAutoPushToParent) override;
        virtual bool OnInitElement(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent) override;
    };

    class OGUI_API StyleXmlFactory : public XmlElementFactory
    {
    public:
        static const Name& GetFullName();

        static const Name& Attr_Path();

        virtual bool OnParseXmlElement(ParseXmlState&, XmlElement&) override;
        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
    
    
    };

    class OGUI_API InstanceXmlFactory : public XmlElementFactory
    {
    public:
        static const Name& GetFullName();

         static const Name& Attr_XmlPath();

        virtual bool OnParseXmlElementChildPost(ParseXmlState&, XmlElement&) override;
    private:
        void AttributeOverrides(XmlElement& xe, const ostr::string& target, std::map<Name, ostr::string>& override);
    };

    class OGUI_API AttributeOverridesXmlFactory : public XmlElementFactory
    {
    public:
        static const Name& GetFullName();

        static const Name& Attr_ElementName();

        virtual bool OnParseXmlElement(ParseXmlState&, XmlElement&) override;
    };

    class OGUI_API TextXmlFactory : public VisualElementXmlFactory
    {
    public:
        static const Name& GetFullName();

        virtual bool OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement* parent) override;
        bool PushChild(InstantiateXmlState&, XmlElement& elementXml, VisualElement* element, XmlElement& childXml, VisualElement* child) override;
        virtual bool OnInitElementChildPost(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent) override;
    
    private:
        void TextPushChild(XmlElement& xe, class TextElement& e);
    };
}