
#include "CheckBox.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Event/PointerEvent.h"

static OGUI::Name checkField = "check";

SampleControls::CheckBox::CheckBox()
{
    AddSource({checkField, _checked});
}

void SampleControls::CheckBox::OnClicked()
{
    _checked = !_checked;
    Notify(checkField);
}

const OGUI::Name& SampleControls::CheckBoxXmlFactory::GetFullName()
{
    static Name name = "SampleControls:CheckBox";
    return name;
}

bool SampleControls::CheckBoxXmlFactory::OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement*)
{
    outNewElement = new CheckBox();
    return true;
}