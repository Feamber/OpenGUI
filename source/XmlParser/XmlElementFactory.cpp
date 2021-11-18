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

    bool XmlElementFactory::OnInitElement(InstantiateXmlState&, XmlElement&, VisualElement*, VisualElement*)
    {
        return true;
    };

    namespace XmlElementFactoryHelper 
    {
        void PushChild(XmlElement& xe, VisualElement& e)
        {
            if(xe.tempElement)
            {
                if(!xe.tempElement->GetHierachyParent())
                    e.PushChild(xe.tempElement);
                return;
            }
            for(auto& childXe : xe.children)
                PushChild(*childXe, e);
        }
    }

    bool XmlElementFactory::OnInitElementChildPost(InstantiateXmlState& state, XmlElement& xe, VisualElement* e, VisualElement* p)
    {
        if(e)
            for(auto& childXe : xe.children)
                XmlElementFactoryHelper::PushChild(*childXe, *e);
        return true;
    };
}