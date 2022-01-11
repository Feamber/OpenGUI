
#include "ScrollView.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Context.h"
#include "YGValue.h"

namespace SampleControls
{
    const YGValue YGValueZero = {0, YGUnitPoint};
    const YGValue YGValueUndefined = {YGUndefined, YGUnitUndefined};
    const YGValue YGValueAuto = {YGUndefined, YGUnitAuto};
}

SampleControls::ScrollView::ScrollView()
{
	_eventHandler.Register<PointerScrollEvent, &ScrollView::_OnMouseScroll>(this);
    _eventHandler.Register<GotFocusEvent, &ScrollView::OnGotFocusEvent>(this);
}

void SampleControls::ScrollView::InitializeChildren()
{
    slider = new VisualElement();
    scrollbar = new VisualElement();
    scrollbar->_styleOverriding.push_back([=]()
    {
        auto& pos = StylePosition::GetOrAdd(scrollbar->_style);
        pos.position = YGPositionTypeAbsolute;
        pos.right = YGValue{0.f, YGUnitPoint};
        pos.height = YGValue{100.f, YGUnitPercent};
        //pos.top = YGValue{0.f, YGUnitPoint};
        //pos.bottom = YGValue{0.f, YGUnitPoint};
        pos.justifyContent = YGJustifyCenter;
        return RestyleDamage::None;
    });
    scrollbar->_scrollable = false;
    slider->_styleOverriding.push_back([=]()
    {
        auto& pos = StylePosition::GetOrAdd(slider->_style);
        pos.bottom = YGValueUndefined;
        if(pos.height == YGValueUndefined || pos.height == YGValueAuto)
            _autoSliderHeight = true;
        if(_autoSliderHeight)
        {
            float ratio = GetSize().y / (_scrollMax.y - _scrollMin.y);
            pos.height = YGValue{ratio * 100.f, YGUnitPercent};
            pos.top = YGValue{(1-ratio) * _sliderProgress * 100.f, YGUnitPercent};
        }
        else 
        {
            float gap = GetSize().y - pos.height.value;
            pos.top = YGValue{_sliderProgress * gap, YGUnitPoint};
        }
        if(_sliderPosDirty)
        {
            _sliderPosDirty = false;
            return RestyleDamage::Layout;
        }
        return RestyleDamage::None;
    });
    slider->_styleClasses.push_back("Slider");
    scrollbar->_styleClasses.push_back("ScrollBar");
    scrollbar->PushChild(slider);
    PushChild(scrollbar);
    
    scrollbar->_eventHandler.Register<PointerDownEvent, &ScrollView::OnMouseDown>(this);
    scrollbar->_eventHandler.Register<PointerMoveEvent, &ScrollView::OnMouseMove>(this);
    scrollbar->_eventHandler.Register<PointerUpEvent, &ScrollView::OnMouseUp>(this);
}

void SampleControls::ScrollView::UpdateScrollSize()
{
    auto scrollSizeDirty = _scrollSizeDirty;
    VisualElement::UpdateScrollSize();
    if(!scrollSizeDirty)
        return;
    if(IsScrollingY())
    {
        Vector2f progress = (_scrollOffset - _scrollMin)  / (_scrollMax - _scrollMin);
        _sliderProgress = progress.y;
        _sliderPosDirty = true;
        SetPseudoClass(PseudoStates::Scroll, true);
    }
    else 
    {
        SetPseudoClass(PseudoStates::Scroll, false);
    }
}

bool SampleControls::ScrollView::_OnMouseScroll(PointerScrollEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach || event.currentPhase == EventRoutePhase::BubbleUp)
	{
		bool scrollX = IsScrollingX();
		bool scrollY = IsScrollingY();
		if(!scrollX && !scrollY)
			return false;
		float delta = -event.wheelOrGestureDelta.y;
		if(scrollY) //滚动竖直方向
			AddScroll({0, delta * 10.f});
		return true;
	}
	return false;
}

void SampleControls::ScrollView::SetScroll(Vector2f offset)
{
    VisualElement::SetScroll(offset);
    Vector2f progress = (_scrollOffset - _scrollMin)  / (_scrollMax - _scrollMin);
    _sliderProgress = progress.y;
    _sliderPosDirty = true;
}

bool SampleControls::ScrollView::OnMouseDown(struct PointerDownEvent& event)
{
    if(event.currentPhase == EventRoutePhase::TrickleDown)
        return false;
    _dragStartPos = event.position;
    if(event.currentPhase == EventRoutePhase::Reach) // direct pointer down
    {
        auto offset = event.position - _worldPosition;
        offset.y = -offset.y;
        auto rect = GetRect();
        float sliderY = slider->GetSize().y;
        float gap = GetSize().y - sliderY;
        _sliderProgress = ((offset.Y - rect.min.Y) - sliderY/2) / gap;
        _sliderProgress = std::clamp(_sliderProgress, 0.f, 1.f);
        _sliderPosDirty = true;
        VisualElement::SetScroll({_scrollOffset.x, _scrollMin.y + _sliderProgress * (_scrollMax.y - _scrollMin.y)});
    }
    _dragging = true;
    _dragStartValue = _sliderProgress;
    Context::Get().CapturePointer(event.pointerId, scrollbar); // start drag
    return true;
}

bool SampleControls::ScrollView::OnMouseMove(struct PointerMoveEvent& event)
{
    if(event.currentPhase != EventRoutePhase::Reach || !_dragging)
        return false;
    auto offset = event.position - _dragStartPos;
    auto _invRotation = _invTransform;
    _invRotation.M[3][0] = _invRotation.M[3][1] = _invRotation.M[3][2] = 0.f;
    Transform(offset, _invRotation);
    offset.y = -offset.y;
    auto gap = GetSize().y - slider->GetSize().y;
    _sliderProgress = _dragStartValue + offset.y / gap;
    _sliderProgress = std::clamp(_sliderProgress, 0.f, 1.f);
    _sliderPosDirty = true;
    VisualElement::SetScroll({_scrollOffset.x, _scrollMin.y + _sliderProgress * (_scrollMax.y - _scrollMin.y)});
    return true;
}

bool SampleControls::ScrollView::OnMouseUp(struct PointerUpEvent& event)
{
    if(event.currentPhase != EventRoutePhase::Reach || !_dragging)
        return false;
    _dragging = false;
    Context::Get().ReleasePointer(event.pointerId);
    return true;
}

bool SampleControls::ScrollView::OnGotFocusEvent(GotFocusEvent& event)
{
    if(event.cause != EFocusChangeCause::FocusNavigation || event.currentPhase != EventRoutePhase::TrickleDown)
        return false;
    ScrollIntoView(event.currentFocusedPath->back());
    return false;
}

const OGUI::Name& SampleControls::ScrollViewXmlFactory::GetFullName()
{
    static Name name = "SampleControls:ScrollView";
    return name;
}

bool SampleControls::ScrollViewXmlFactory::OnCreateElement(InstantiateXmlState&, XmlElement&, VisualElement*& outNewElement, VisualElement*)
{
    outNewElement = new ScrollView();
    return true;
}

bool SampleControls::ScrollViewXmlFactory::OnInitElementChildPost(InstantiateXmlState& state, XmlElement& xe, VisualElement* e, VisualElement* p)
{
    if(!VisualElementXmlFactory::OnInitElementChildPost(state, xe, e, p))
        return false;
    ((ScrollView*)e)->InitializeChildren();
        return true;
    return true;
}