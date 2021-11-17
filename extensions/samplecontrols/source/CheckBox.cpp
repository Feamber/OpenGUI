#include "CheckBox.h"
#define DLL_IMPLEMENTATION
#include "OpenGUI/Context.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/XmlParser/XmlParser.h"

void SampleControls::CheckBox::RegisterXml()
{
    using namespace XmlParserHelper;
    RegisterXmlParser(
            "SampleControls:CheckBox",
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new CheckBox();
                return true;
            },
            XmlBase::OnInitElement_VisualElement,
            OnInstantiateXmlElement_Empty
        );
}

static OGUI::Name checkField = "check";

SampleControls::CheckBox::CheckBox()
{
    AddSource({checkField, &_checked});
}

void SampleControls::CheckBox::OnClicked()
{
    _checked = !_checked;
    Notify(checkField);
}