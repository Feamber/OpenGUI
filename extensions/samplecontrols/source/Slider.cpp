#define DLL_IMPLEMENTATION
#include "Slider.h"
#include "OpenGUI/Context.h"

static OGUI::Name VALUE = "value";

void SampleControls::Slider::InitializeChildren()
{
    std::vector<StyleSheet*> sheets;
    for(VisualElement* e = this->_physicalParent; e; e=e->_physicalParent)
        for(auto sheet : e->_styleSheets)
            sheets.push_back(sheet);
    Initialize(this, sheets);
}

void SampleControls::Slider::Initialize(VisualElement* child, std::vector<StyleSheet*>& sheets)
{
    for(auto sheet : child->_styleSheets)
        sheets.push_back(sheet);
    ComputedAnim anim;
    anim.style.Initialize();
    anim.style.animationName =  child->_name + "-slide";
    anim.style.animationDuration = 1.f;
    anim.style.animationPlayState = EAnimPlayState::Paused;
    if(anim.Init(sheets))
    {
        child->_procedureAnims.push_back(std::move(anim));
        _animatedChildren.push_back(child);
    }
    child->Traverse([&](VisualElement* next) { Initialize(next, sheets); });
}

SampleControls::Slider::Slider()
{
    _eventHandler.Register<PointerDownEvent, &Slider::OnMouseDown>(this);
    _eventHandler.Register<PointerMoveEvent, &Slider::OnMouseMove>(this);
    _eventHandler.Register<PointerUpEvent, &Slider::OnMouseUp>(this);
}

void SampleControls::Slider::OnMinMaxChanged()
{
    auto oldValue = _value;
    _value = std::clamp(_value, _minValue, _maxValue);
    if(_value != oldValue)
    {
        OnValueChanged();
        Notify(VALUE);
    }
}

void SampleControls::Slider::OnValueChanged()
{
    float percentage = (_value - _minValue) / (_maxValue - _minValue);
    for(auto& child : _animatedChildren)
    {
        std::string name =  child->_name + "-slide";
        child->SetAnimationTime(name, percentage);
    }
}

bool SampleControls::Slider::OnMouseDown(struct PointerDownEvent& event)
{
    if(event.currentPhase == EventRoutePhase::TrickleDown)
        return false;
    _dragStartPos = event.position;
    _dragStartValue = _value;
    if(event.currentPhase == EventRoutePhase::Reach) // direct pointer down
    {
        auto offset = event.position - _worldPosition;
        auto rect = GetRect();
        float percentage = (offset.X - rect.min.X) / (rect.max.X - rect.min.X);
        _value = (_maxValue - _minValue) * percentage + _minValue;
        _value = std::clamp(_value, _minValue, _maxValue);
        _dragStartValue = _value;
        Notify(VALUE);
        OnValueChanged();
    }
    _dragging = true;
    Context::Get().CapturePointer(event.pointerId, this); // start drag
    return true;
}

bool SampleControls::Slider::OnMouseMove(struct PointerMoveEvent& event)
{
    if(event.currentPhase != EventRoutePhase::Reach || !_dragging)
        return false;
    auto offset = event.position - _dragStartPos;
    auto _invRotation = _invTransform;
    _invRotation.M[3][0] = _invRotation.M[3][1] = _invRotation.M[3][2] = 0.f;
    Transform(offset, _invRotation);
    auto size = GetSize();
    float percentage = offset.X / size.X;
    _value = _dragStartValue + (_maxValue - _minValue) * percentage + _minValue;
    _value = std::clamp(_value, _minValue, _maxValue);
    Notify(VALUE);
    OnValueChanged();
    return true;
}

bool SampleControls::Slider::OnMouseUp(struct PointerUpEvent& event)
{
    if(event.currentPhase != EventRoutePhase::Reach || !_dragging)
        return false;
    _dragging = false;
    Context::Get().ReleasePointer(event.pointerId);
    return true;
}

const OGUI::Name& SampleControls::SliderXmlFactory::GetFullName()
{
    static Name name = "SampleControls:Slider";
    return name;
}

bool SampleControls::SliderXmlFactory::OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement*)
{
    outNewElement = new Slider();
    return true;
}

bool SampleControls::SliderXmlFactory::OnInitElement(InstantiateXmlState& state, XmlElement& xe, VisualElement* element, VisualElement* p)
{
    using namespace XmlParserHelper;
    if(!VisualElementXmlFactory::OnInitElement(state, xe, element, p))
        return false;
    auto ve = (Slider*)element;
    FindAttribute(xe, "min-value", ve->_minValue, ve, [ve](bool valid){ve->OnMinMaxChanged();});
    ve->_value = ve->_minValue;
    FindAttribute(xe, "max-value", ve->_maxValue, ve, [ve](bool valid){ve->OnMinMaxChanged();});
    FindAttribute(xe, "value", ve->_value, ve, [ve](bool valid){ve->OnValueChanged();}, true) ;
    
    static const std::map<ostr::string, SliderDirection> Directions = 
    {
        {"horizontal",        SliderDirection::Horizontal},
        {"vertical",   SliderDirection::Vertical},
    };
    FindAttributeEnum(xe, "direction", Directions, ve->_direction);
    return true;
}

bool SampleControls::SliderXmlFactory::OnInitElementChildPost(InstantiateXmlState& state, XmlElement& xe, VisualElement* e, VisualElement* p)
{
    if(!VisualElementXmlFactory::OnInitElementChildPost(state, xe, e, p))
        return false;
    auto ve = (Slider*)e;
    ve->InitializeChildren();
    return true;
}