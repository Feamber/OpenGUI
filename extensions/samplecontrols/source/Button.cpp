#define DLL_IMPLEMENTATION
#include "Button.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/XmlParser/XmlParser.h"

void SampleControls::Button::RegisterXml()
{
    using namespace XmlParserHelper;
    RegisterXmlParser(
            "SampleControls:Button",
            OnParseXmlElement_Empty,
            OnParseXmlElement_Empty, 
            OnParseXmlElement_Empty, 
            [](InstantiateXmlState&, XmlElement&, VisualElement*& out, VisualElement*)
            {
                out = new Button();
                return true;
            },
            XmlBase::OnInitElement_VisualElement,
            OnInstantiateXmlElement_Empty
        );
}


bool SampleControls::Button::OnPointerDown(PointerDownEvent& event)
{
    if(event.currentPhase == EventRoutePhase::Reach || event.currentPhase == EventRoutePhase::BubbleUp)
    {
        SetPseudoClass(PseudoStates::Active, true);
        Context::Get().CapturePointer(event.pointerId, this);
        _pressed = true;
        return true;
    }
    return false;
}
bool SampleControls::Button::OnPointerMove(PointerMoveEvent& event)
{
    if(_pressed && (event.currentPhase == EventRoutePhase::Reach || event.currentPhase == EventRoutePhase::BubbleUp))
    {
        bool intersect = Intersect(event.position);
        if(intersect != Active())
            SetPseudoClass(PseudoStates::Active, intersect);
        return true;
    }
    return false;
}
bool SampleControls::Button::OnPointerUp(PointerUpEvent& event)
{
    if(_pressed)
    {	
        if(Active())
        {
            SetPseudoClass(PseudoStates::Active, false);
            PointerClickEvent click { (PointerData&)event };
            click.currentPhase = EventRoutePhase::None;
            RouteEvent(this, click);
            OnClicked();
        }
        _pressed = false;
        Context::Get().ReleasePointer(event.pointerId);
        return true;
    }
    return false;
}

SampleControls::Button::Button()
{
    _eventHandler.Register<PointerDownEvent, &Button::OnPointerDown>(this);
    _eventHandler.Register<PointerMoveEvent, &Button::OnPointerMove>(this);
    _eventHandler.Register<PointerUpEvent, &Button::OnPointerUp>(this);
}

bool SampleControls::Button::Active()
{
    return (_pseudoMask & PseudoStates::Active) == PseudoStates::Active;
}