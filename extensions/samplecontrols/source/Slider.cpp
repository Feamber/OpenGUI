#include "Slider.h"
#include "OpenGUI/XmlParser/XmlParser.h"

void SampleControls::Slider::RegisterXml()
{
    using namespace XmlParserHelper;
    RegisterXmlParser(
            XmlBase::RootName,
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new Slider();
                return true;
            },
            [](InstantiateXmlState& state, XmlElement& xe, VisualElement* element, VisualElement* parent)
            {
                if(!XmlBase::OnInitElement_VisualElement(state, xe, element, parent))
                    return false;
                auto ve = (Slider*)element;
                FindAttribute(xe, "min-value", ve->_minValue, ve, [ve](bool valid){ve->OnMinMaxChanged();});
                FindAttribute(xe, "max-value", ve->_maxValue, ve, [ve](bool valid){ve->OnMinMaxChanged();});
                FindAttribute(xe, "value", ve->_value, ve, [ve](bool valid){ve->OnValueChanged();});
                
                static const std::map<ostr::string, SliderDirection> Directions = 
                {
                    {"horizontal",        SliderDirection::Horizontal},
                    {"vertical",   SliderDirection::Vertical},
                };
                FindAttributeEnum(xe, "direction", Directions, ve->_direction);
                return true;
            },
            OnInstantiateXmlElement_Empty
        );
}

SampleControls::Slider::Slider()
{
    AddSource({
        "value",
        &_value
    });
}