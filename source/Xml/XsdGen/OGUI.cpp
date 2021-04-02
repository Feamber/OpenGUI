#include "OpenGUI/Xml/XsdGen/OGUI.h"


namespace OGUI
{
    void IXmlFactory_Root::Internal_Init()
    {
        OGUI::IXmlFactory::Internal_Init();

    }

    bool IXmlFactory_Root::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory::Internal_InitAttribute(attr_name_hash, attr);

        return false;
    }

    void IXmlFactory_Style::Internal_Init()
    {
        OGUI::IXmlFactory::Internal_Init();
        path = "";
    }

    bool IXmlFactory_Style::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory::Internal_InitAttribute(attr_name_hash, attr);
        const std::string_view& str = attr.name;
        switch (attr_name_hash)
        {
            casestr("path") path = attr.value; return true;
            default: return false;
        }
        return false;
    }

    void IXmlFactory_Template::Internal_Init()
    {
        OGUI::IXmlFactory::Internal_Init();
        name = "";
        path = "";
    }

    bool IXmlFactory_Template::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory::Internal_InitAttribute(attr_name_hash, attr);
        const std::string_view& str = attr.name;
        switch (attr_name_hash)
        {
            casestr("name") name = attr.value; return true;
            casestr("path") path = attr.value; return true;
            default: return false;
        }
        return false;
    }

    void IXmlFactory_AttributeOverrides::Internal_Init()
    {
        OGUI::IXmlFactory::Internal_Init();
        element_name = "";
    }

    bool IXmlFactory_AttributeOverrides::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory::Internal_InitAttribute(attr_name_hash, attr);
        const std::string_view& str = attr.name;
        switch (attr_name_hash)
        {
            casestr("element_name") element_name = attr.value; return true;
            default: return false;
        }
        return false;
    }

    void IXmlFactory_VisualElement::Internal_Init()
    {
        OGUI::IXmlFactory::Internal_Init();
        name.reset();
        path.reset();
        style.reset();
        class_tag.reset();
        slot_name.reset();
        slot.reset();
    }

    bool IXmlFactory_VisualElement::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory::Internal_InitAttribute(attr_name_hash, attr);
        const std::string_view& str = attr.name;
        switch (attr_name_hash)
        {
            casestr("name") name.emplace(attr.value); return true;
            casestr("path") path.emplace(attr.value); return true;
            casestr("style") style.emplace(attr.value); return true;
            casestr("class_tag") class_tag.emplace(attr.value); return true;
            casestr("slot_name") slot_name.emplace(attr.value); return true;
            casestr("slot") slot.emplace(attr.value); return true;
            default: return false;
        }
        return false;
    }

    void IXmlFactory_Instance::Internal_Init()
    {
        OGUI::IXmlFactory_VisualElement::Internal_Init();
        template_name = "";
    }

    bool IXmlFactory_Instance::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory_VisualElement::Internal_InitAttribute(attr_name_hash, attr);
        const std::string_view& str = attr.name;
        switch (attr_name_hash)
        {
            casestr("template_name") template_name = attr.value; return true;
            default: return false;
        }
        return false;
    }

    void IXmlFactory_Text::Internal_Init()
    {
        OGUI::IXmlFactory_VisualElement::Internal_Init();

    }

    bool IXmlFactory_Text::Internal_InitAttribute(size_t attr_name_hash, const XmlAttribute& attr)
    {
        OGUI::IXmlFactory_VisualElement::Internal_InitAttribute(attr_name_hash, attr);

        return false;
    }


}