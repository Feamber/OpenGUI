#define DLL_IMPLEMENTATION
#include "OpenGUI/XmlParser/XmlElementFactory.h"

namespace OGUI
{
    bool XmlElementFactory::OnParseXmlElement(ParseXmlState&, XmlElement&)
    {
        return true;
    };

    bool XmlElementFactory::OnParseXmlElementSiblingPost(ParseXmlState&, XmlElement&)
    {
        return true;
    };

    bool XmlElementFactory::OnParseXmlElementChildPost(ParseXmlState&, XmlElement&)
    {
        return true;
    };

    bool XmlElementFactory::OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*&, VisualElement*)
    {
        return true;
    };

    bool XmlElementFactory::PushChild(InstantiateXmlState&, XmlElement&, VisualElement* element, XmlElement&, VisualElement* child)
    {
        if(child->GetHierachyParent())
        {
            olog::Error(u"XmlElementFactory::PushChild 失败，child已经有parent！"_o);
            return false;
        }
        element->PushChild(child);
        return true;
    };

    bool XmlElementFactory::OnInitElementHierarchy(InstantiateXmlState&, XmlElement&, VisualElement* element, VisualElement* parent, bool& isAutoPushToParent)
    {
        return true;
    }

    bool XmlElementFactory::OnInitElement(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*)
    {
        return true;
    };

    bool XmlElementFactory::OnInitElementChildPost(InstantiateXmlState& state, XmlElement& xe, VisualElement* e, VisualElement* p)
    {
        return true;
    };
}