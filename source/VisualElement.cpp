#define DLL_IMPLEMENTATION
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Math.h"
#include <type_traits>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/generated/border.h"
#include "Yoga.h"
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Context.h"
#include <gsl/span>

OGUI::Rect rectPixelPosToScreenPos(const OGUI::Rect& rect, const OGUI::Vector2f resolution)
{
	OGUI::Rect result = rect;
	result.max /= resolution;
	result.min /= resolution;
	return result;
}

void OGUI::VisualElement::DrawBackgroundPrimitive(OGUI::PrimitiveDraw::DrawContext& Ctx)
{
	using namespace PrimitiveDraw;
	Rect rect = GetRect();
	auto&& ctx = Context::Get();
	auto& bg = StyleBackground::Get(_style);
	auto& bd = StyleBorder::Get(_style);
	auto transform = _worldTransform;
	if (!bg.backgroundImage.empty())
	{
		//start new load
		if (!backgroundImageResource || !backgroundImageResource->valid() || backgroundImageUrl != bg.backgroundImage)
		{
			backgroundImageResource = ctx.textureManager->RequireFromFileSystem(bg.backgroundImage);
			backgroundImageUrl = bg.backgroundImage;
		}
	}
	else //release old texture
		backgroundImageResource = nullptr;
	
	BeginDraw(Ctx.prims);
	Rect uv = {Vector2f::vector_zero(), Vector2f::vector_one()};
	RoundBoxParams params {rect, uv, bg.backgroundColor };
	TextureInterface* tex = nullptr;
	if (backgroundImageResource && backgroundImageResource->valid())
	{
		tex = backgroundImageResource->Get();
	}
	params.radius[0] = bd.borderTopLeftRadius.value;// / Ctx.resolution.Y;
	params.radius[1] = bd.borderTopRightRadius.value;// / Ctx.resolution.Y;
	params.radius[2] = bd.borderBottomRightRadius.value;// / Ctx.resolution.Y;
	params.radius[3] = bd.borderBottomLeftRadius.value;// / Ctx.resolution.Y;
	PrimitiveDraw::PrimitiveDraw<RoundBoxShape2>(tex, Ctx.prims, params, 20);
	EndDraw(Ctx.prims, transform);
}

void OGUI::VisualElement::DrawBorderPrimitive(OGUI::PrimitiveDraw::DrawContext & Ctx)
{}

void OGUI::VisualElement::DrawDebugPrimitive(OGUI::PrimitiveDraw::DrawContext & Ctx)
{
	using namespace PrimitiveDraw;

	if((FocusNavDebugState == CollisionBox || FocusNavDebugState == ElementQuad))
	{
		time_t seconds = time(NULL);
		if(seconds - navDebugLastUpdate < 3)
		{
			BeginDraw(Ctx.prims);
			BoxParams params;
			params.uv = {Vector2f::vector_zero(), Vector2f::vector_one()};
			params.rect = navDebugRect;
			params.color = FocusNavDebugState == CollisionBox ? 
				Color4f(225, 0, 0, 0.3f) : 
				Color4f(46, 225, 225, 0.3f);

			PrimitiveDraw::PrimitiveDraw<BoxShape>(nullptr, Ctx.prims, params);
			EndDraw(Ctx.prims, float4x4());
		}
	}
}

OGUI::ClipRect OGUI::VisualElement::ApplyClipping(OGUI::PrimitiveDraw::DrawContext & Ctx)
{
	return {GetRect(), _worldTransform};
}

bool OGUI::VisualElement::IsClipping()
{
	auto& pos = StylePosition::Get(_style);
	return pos.overflow != YGOverflow::YGOverflowVisible;
}

void OGUI::VisualElement::CreateYogaNode()
{
	auto config = YGConfigGetDefault();
	_ygnode = YGNodeNewWithConfig(config);
	YGNodeSetContext(_ygnode, this);
}

void OGUI::VisualElement::GetChildren(std::vector<VisualElement *>& children)
{
	children.insert(children.end(), _children.begin(), _children.end());
}

OGUI::VisualElement::VisualElement()
{
	Context::Get()._allElementHandle.insert(this);
	CreateYogaNode();
	_style = ComputedStyle::Create(nullptr);
	RegisterFocusedEvent();
	
	_eventHandler.Register<PointerEnterEvent, &VisualElement::_OnMouseEnter>(this);
	_eventHandler.Register<PointerLeaveEvent, &VisualElement::_OnMouseLeave>(this);
}

OGUI::VisualElement::~VisualElement()
{
	if (_ygnode)
	{
		YGNodeFree(_ygnode);
	}

	auto& allElementHandle = Context::Get()._allElementHandle;
	allElementHandle.erase(this);
}

void OGUI::VisualElement::DrawPrimitive(OGUI::PrimitiveDraw::DrawContext& Ctx)
{
	DrawBackgroundPrimitive(Ctx);
	DrawBorderPrimitive(Ctx);
	DrawDebugPrimitive(Ctx);
}

OGUI::VisualElement* OGUI::VisualElement::GetParent()
{
	return _logical_parent;
}

OGUI::VisualElement* OGUI::VisualElement::GetHierachyParent()
{
	return _physical_parent;
}

bool OGUI::VisualElement::IsParent(OGUI::VisualElement* e)
{
	if(!e || !_physical_parent || e == this)
		return false;
	if(e == _physical_parent)
		return true;
	return _physical_parent->IsParent(e);
}

void OGUI::VisualElement::MarkDirty(DirtyReason reason)
{
	Context::Get().MarkDirty(this, reason);
}


void OGUI::VisualElement::DestoryTree(VisualElement* element)
{
	std::vector<VisualElement*> toDestroy;
	toDestroy.push_back(element);
	while (toDestroy.size() > 0)
	{
		auto back = toDestroy.back(); toDestroy.pop_back();
		for (auto child : back->_children)
			toDestroy.push_back(child);
		delete back;
	}
}

void OGUI::VisualElement::UpdateRoot(VisualElement* child)
{
	child->_root = GetRoot();
	if(child->_layoutType == _layoutType)
		child->_layoutRoot = GetLayoutRoot();
	for(auto grandson : child->_children)
		child->UpdateRoot(grandson);
}

void OGUI::VisualElement::PushChild(VisualElement* child)
{
	_scrollSizeDirty = true;
	child->_physical_parent = this;
	InsertChild(child, _children.size());
	child->_layoutType = LayoutType::Flex;
	UpdateRoot(child);
}

void OGUI::VisualElement::InsertChild(VisualElement* child, int index)
{
	_scrollSizeDirty = true;
	child->_physical_parent = this;
	YGNodeInsertChild(_ygnode, child->_ygnode, index);
	_children.insert(_children.begin() + index, child);
	child->_layoutType = LayoutType::Flex;
	UpdateRoot(child);
}

void OGUI::VisualElement::RemoveChild(VisualElement* child)
{
	_scrollSizeDirty = true;
    child->_physical_parent = nullptr;
    YGNodeRemoveChild(_ygnode, child->_ygnode);
	auto end = std::remove(_children.begin(), _children.end(), child);
	_children.erase(end, _children.end());
	child->_root = child->_layoutRoot = nullptr;
	child->_layoutType = LayoutType::None;
}

OGUI::VisualElement* OGUI::VisualElement::GetRoot()
{
	return _root ? _root : this;
}

OGUI::VisualElement* OGUI::VisualElement::GetLayoutRoot()
{
	return _layoutRoot ? _layoutRoot : this;
}

void OGUI::VisualElement::CalculateLayout()
{
	YGNodeCalculateLayout(_ygnode, YGUndefined, YGUndefined, YGNodeStyleGetDirection(_ygnode));
}

OGUI::ComputedTransform OGUI::VisualElement::GetStyleTransform() const
{
	if(_styleTransformDirty)
	{
		_styleTransformDirty = false;
		auto& pos = StylePosition::Get(_style);
		return _styleTransform = evaluate(pos.transform);
	}
	return _styleTransform;
}

void OGUI::VisualElement::UpdateWorldTransform()
{
	using namespace math;
	auto layout = GetLayout();
	auto parent = GetHierachyParent();
	if (parent)
	{
		auto playout = parent->GetLayout();
		auto offset = (layout.min + layout.max)/2 - (playout.max - playout.min) / 2;
		offset.y = -offset.y;
		auto& pos = StylePosition::Get(_style);
		if(pos.position == YGPositionTypeRelative)
			offset += parent->GetScrollPos();
		_worldTransform = multiply(GetStyleTransform(), ComputedTransform::translate(offset)).to_3D();
		_worldTransform = math::multiply(_worldTransform, parent->_worldTransform);
	}
	else
	{
		auto offset = -(layout.max - layout.min) / 2;
		_worldTransform = GetStyleTransform().to_3D();
	}
	_worldPosition = {_worldTransform.M[3][0], _worldTransform.M[3][1]};
	_transformDirty = false;
}

OGUI::Rect OGUI::VisualElement::GetLayout() const
{
	if(_layoutType == LayoutType::Flex)
	{
		Vector2f LB = {YGNodeLayoutGetLeft(_ygnode), YGNodeLayoutGetTop(_ygnode)};
		Vector2f WH = {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
		return
		{
			LB,
			LB + WH,
		};
	}
	else if(_layoutType == LayoutType::Inline)
	{
		return _inlineLayout;
	}
	else
		return Rect();
}

OGUI::Rect OGUI::VisualElement::GetRect() const
{
	if(_layoutType == LayoutType::Flex)
	{
		Vector2f LB = Vector2f::vector_zero();
		Vector2f WH = {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
		return
		{
			- WH / 2,
			WH / 2,
		};
	}
	else if(_layoutType == LayoutType::Inline)
	{
		auto size = _inlineLayout.max - _inlineLayout.min;
		return 
		{
			- size / 2,
			size / 2
		};
	}
	else
		return Rect();
}

OGUI::Vector2f OGUI::VisualElement::GetSize() const
{
	
	if(_layoutType == LayoutType::Flex)
		return {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
	else if(_layoutType == LayoutType::Inline)
		return _inlineLayout.max - _inlineLayout.min;
	else
		return Vector2f();
}

void OGUI::VisualElement::MarkTransformDirty()
{
	_transformDirty = true;
	if(_physical_parent)
		_physical_parent->_scrollSizeDirty = true;
}

void OGUI::VisualElement::MarkStyleTransformDirty()
{
	_styleTransformDirty = true;
	MarkTransformDirty();
}

void OGUI::VisualElement::MarkLayoutDirty()
{
}

void OGUI::VisualElement::SyncYogaStyle()
{
	if (!_ygnode)
		return;
	Context::Get()._layoutDirty = true;
	auto p = this;
	while(p!=nullptr)
	{
		p = p->GetLayoutRoot();
		p->MarkLayoutDirty();
		p = p->GetHierachyParent();
	}
	auto& pos = StylePosition::Get(_style);
	auto& bd = StyleBorder::Get(_style);
	YGNodeStyleSetFlexGrow(_ygnode, pos.flexGrow);
	YGNodeStyleSetFlexShrink(_ygnode, pos.flexShrink);
#define SetYGEdge(function, edge, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, edge, v.value); \
	else \
		function(_ygnode, edge, v.value)
#define SetYGEdgeAuto(function, edge, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, edge, v.value); \
	else if(v.unit == YGUnitAuto) \
		function##Auto(_ygnode, edge); \
	else \
		function(_ygnode, edge, v.value)
#define SetYGValueAuto(function, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, v.value); \
	else if(v.unit == YGUnitAuto) \
		function##Auto(_ygnode); \
	else \
		function(_ygnode, v.value)
#define SetYGValue(function, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, v.value); \
	else \
		function(_ygnode, v.value)

	SetYGValueAuto(YGNodeStyleSetFlexBasis, pos.flexBasis);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeLeft, pos.left);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeTop, pos.top);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeRight, pos.right);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeBottom, pos.bottom);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeLeft, pos.marginLeft);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeTop, pos.marginTop);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeRight, pos.marginRight);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeBottom, pos.marginBottom);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeLeft, pos.paddingLeft);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeTop, pos.paddingTop);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeRight, pos.paddingRight);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeBottom, pos.paddingBottom);
	YGNodeStyleSetBorder(_ygnode, YGEdgeLeft, bd.borderLeftWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeTop, bd.borderTopWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeRight, bd.borderRightWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeBottom, bd.borderBottomWidth);

	SetYGValueAuto(YGNodeStyleSetWidth, pos.width);
	SetYGValueAuto(YGNodeStyleSetHeight, pos.height);
	YGNodeStyleSetPositionType(_ygnode, pos.position);
	YGNodeStyleSetOverflow(_ygnode, pos.overflow);
	YGNodeStyleSetAlignSelf(_ygnode, pos.alignSelf);
	SetYGValue(YGNodeStyleSetMaxWidth, pos.maxWidth);
	SetYGValue(YGNodeStyleSetMaxHeight, pos.maxHeight);
	SetYGValue(YGNodeStyleSetMinWidth, pos.minWidth);
	SetYGValue(YGNodeStyleSetMinHeight, pos.minHeight);
	YGNodeStyleSetFlexDirection(_ygnode, pos.flexDirection);
	YGNodeStyleSetAlignContent(_ygnode, pos.alignContent);
	YGNodeStyleSetAlignItems(_ygnode, pos.alignItems);
	YGNodeStyleSetJustifyContent(_ygnode, pos.justifyContent);
	YGNodeStyleSetFlexWrap(_ygnode, pos.flexWrap);
	YGNodeStyleSetDisplay(_ygnode, pos.flexDisplay);
}

OGUI::RestyleDamage OGUI::VisualElement::ApplyProcedureStyle()
{
	RestyleDamage damage = RestyleDamage::None;
	for(auto& ovr : _styleOverriding)
		damage |= ovr();
	return damage;
}

bool OGUI::VisualElement::ContainClass(std::string_view cls)
{
	return std::find(_styleClasses.begin(), _styleClasses.end(), cls) != _styleClasses.end();
}

void OGUI::VisualElement::_ResetStyles()
{
	_selectorDirty = true;
	_style = ComputedStyle();
	_preAnimatedStyle = ComputedStyle();
	//dosent clean this cause we want to inherit anim context
	//_animContext.clear();
	//_animStyles.clear();
	_triggerPseudoMask = PseudoStates::None;
	_dependencyPseudoMask = PseudoStates::None;
}

void OGUI::VisualElement::ResetStyles()
{
	_ResetStyles();
	if (_beforeElement)
		_beforeElement->_ResetStyles();
	if (_afterElement)
		_afterElement->_ResetStyles();
}

OGUI::Name* OGUI::VisualElement::GetEventBind(Name event)
{
	auto iter = _eventBag.find(event);
	if(iter == _eventBag.end())
		return nullptr;
	return &iter->second;
}

void OGUI::VisualElement::Notify(Name prop, bool force)
{
	auto iter =_bindBag.find(prop);
	if(iter != _bindBag.end())
		AttrBag::Notify(iter->second, force);
}

bool OGUI::VisualElement::Visible() const
{
	return !(StylePosition::Get(_style).flexDisplay == YGDisplayNone);
}

bool OGUI::VisualElement::Intersect(Vector2f point)
{
	if(!Visible())
		return false;
	auto invTransform = math::inverse(_worldTransform);
	Vector4f dummy = {point.X, point.Y, 0.f, 1.f};
	const Vector4f result = math::multiply(dummy, invTransform);
	Vector2f localPoint = {result.X, result.Y};
	auto rect = GetRect();
	return rect.IntersectPoint(localPoint);
}

void OGUI::VisualElement::SetPseudoClass(PseudoStates state, bool b)
{
	if (b)
	{
		_pseudoMask |= state;
		if((_triggerPseudoMask & state) != PseudoStates::None)
			_selectorDirty = true;
	}
	else
	{
		_pseudoMask &= ~state;
		if((_dependencyPseudoMask & state) != PseudoStates::None)
			_selectorDirty = true;
	}	
}

void OGUI::VisualElement::InitInlineStyle(std::string_view str)
{
	auto res = ParseInlineStyle(str);
	if(res)
		_inlineStyle = std::make_unique<InlineStyle>(std::move(res.value()));
}

namespace OGUI
{
	VisualElement* CreatePseudoElement()
	{
		auto res = new VisualElement();
		res->_isPseudoElement = true;
		return res;
	}
}

OGUI::VisualElement* OGUI::VisualElement::GetAfterPseudoElement()
{
	if (!_beforeElement)
	{
		_beforeElement = CreatePseudoElement();
		InsertChild(_beforeElement, 0);
	}
	return _beforeElement;
}

void OGUI::VisualElement::ReleaseAfterPseudoElement()
{
	if (_beforeElement)
	{
		RemoveChild(_beforeElement);
		delete _beforeElement;
		_beforeElement = nullptr;
	}
}

OGUI::VisualElement* OGUI::VisualElement::GetBeforePseudoElement()
{
	if (!_afterElement)
	{
		_afterElement = CreatePseudoElement();
		PushChild(_afterElement);
	}
	return _afterElement;
}

void OGUI::VisualElement::ReleaseBeforePseudoElement()
{
	if (_afterElement)
	{
		RemoveChild(_afterElement);
		delete _afterElement;
		_afterElement = nullptr;
	}
}

void OGUI::VisualElement::RegisterFocusedEvent()
{
	_eventHandler.Register<GotKeyboardFocusEvent, &VisualElement::_OnGotKeyboardFocus>(this);
	_eventHandler.Register<LostKeyboardFocusEvent, &VisualElement::_OnLostKeyboardFocus>(this);
	_eventHandler.Register<GotFocusEvent, &VisualElement::_OnGotFocus>(this);
	_eventHandler.Register<LostFocusEvent, &VisualElement::_OnLostFocus>(this);
}

std::vector<OGUI::VisualElement*> OGUI::VisualElement::GetFocusScopeChildren()
{
	auto prevFocusScope = GetPrevFocusScope();
	std::vector<VisualElement*> out;
	if(prevFocusScope)
	{
		std::vector<VisualElement*> current {prevFocusScope->_children};
		std::vector<VisualElement*> next;
		while (current.size() > 0) 
		{
			for (auto element : current)
			{
				if(element->focusable)
				{
					out.push_back(element);
					continue;
				}
				next.insert(next.end(), element->_children.begin(), element->_children.end());
			}
			current.clear();
			std::swap(current, next);
		}
	}
	return out;
}

bool OGUI::VisualElement::_OnGotKeyboardFocus(struct GotKeyboardFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::KeyboardFocus, true);
	return false;
}

bool OGUI::VisualElement::_OnLostKeyboardFocus(struct LostKeyboardFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::KeyboardFocus, false);
	return false;
}

bool OGUI::VisualElement::_OnGotFocus(struct GotFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::Focus, true);
	return false;
}

bool OGUI::VisualElement::_OnLostFocus(struct LostFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::Focus, false);
	return false;
}

bool OGUI::VisualElement::_OnMouseEnter(struct PointerEnterEvent &event)
{
	if(event.currentPhase == EventRoutePhase::Reach || event.currentPhase == EventRoutePhase::BubbleUp)
	{
		hovered++;
		SetPseudoClass(PseudoStates::Hover, true);
	}
	return false;
}

bool OGUI::VisualElement::_OnMouseLeave(struct PointerLeaveEvent &event)
{
	if(event.currentPhase == EventRoutePhase::Reach || event.currentPhase == EventRoutePhase::BubbleUp)
		hovered--;
	if(hovered == 0)
		SetPseudoClass(PseudoStates::Hover, false);
	return false;
}

OGUI::VisualElement* OGUI::VisualElement::GetPrevFocusScope()
{
	VisualElement* current = this;
	while (current) 
	{
		if(current->_physical_parent && current->_physical_parent->isFocusScope)
			return current->_physical_parent;
		current = current->_physical_parent;
	}
	return nullptr;
}

void OGUI::VisualElement::GetRelativeFocusedPath(OGUI::VisualElement* element, std::vector<OGUI::VisualElement*>& out)
{
	auto prevFocusScope = element->GetPrevFocusScope();
	VisualElement* current = element->isFocusScope ? element : prevFocusScope;
	while (current) 
	{
		out.push_back(current);
		current = current->currentFocused;
	}
}

namespace OGUI
{
	struct Quad
	{
		Vector2f RU;
		Vector2f RB;
		Vector2f LU;
		Vector2f LB;

		const Vector2f Transform(Vector2f p, const float4x4& transform)
    	{
        	const Vector4f dummy = Vector4f(p.X, p.Y, 0.f, 1.f);
        	const Vector4f result = math::multiply(dummy, transform);
        	p.X = result.X;
        	p.Y = result.Y;
        	return p;
    	}

		Quad(Vector2f RU, Vector2f RB, Vector2f LU, Vector2f LB) : RU(RU), RB(RB), LU(LU), LB(LB) {}

		Quad(VisualElement* element)
		{
			Rect rect = element->GetRect();
			RU = Transform(rect.max, element->_worldTransform);
			RB = Transform({rect.max.X, rect.min.Y}, element->_worldTransform);
			LU = Transform({rect.min.X, rect.max.Y}, element->_worldTransform);
			LB = Transform(rect.min, element->_worldTransform);
		}

		Quad(Rect rect, float4x4 worldTransform)
		{
			RU = Transform(rect.max, worldTransform);
			RB = Transform({rect.max.X, rect.min.Y}, worldTransform);
			LU = Transform({rect.min.X, rect.max.Y}, worldTransform);
			LB = Transform(rect.min, worldTransform);
		}

		Quad(Rect rect)
		{
			RU = {rect.max.X, rect.max.Y};
    		RB = {rect.max.X, rect.max.X};
    		LU = {rect.min.X, rect.max.Y};
    		LB = {rect.min.X, rect.min.Y};
		}

		Rect ToBoundingBox()
		{
			float minX = RU.X < RB.X && RU.X < LU.X && RU.X < LB.X ? RU.X : RB.X < LU.X && RB.X < LB.X ? RB.X : LU.X < LB.X ? LU.X : LB.X;
			float minY = RU.Y < RB.Y && RU.Y < LU.Y && RU.Y < LB.Y ? RU.Y : RB.Y < LU.Y && RB.Y < LB.Y ? RB.Y : LU.Y < LB.Y ? LU.Y : LB.Y;
			float maxX = RU.X > RB.X && RU.X > LU.X && RU.X > LB.X ? RU.X : RB.X > LU.X && RB.X > LB.X ? RB.X : LU.X > LB.X ? LU.X : LB.X;
			float maxY = RU.Y > RB.Y && RU.Y > LU.Y && RU.Y > LB.Y ? RU.Y : RB.Y > LU.Y && RB.Y > LB.Y ? RB.Y : LU.Y > LB.Y ? LU.Y : LB.Y;
			return Rect({Vector2f(minX, minY), Vector2f(maxX, maxY)});
		}

		Quad ToNavCollisionBox(ENavDirection direction, float width, float height)
		{
			Quad aabb = ToBoundingBox();
			float aabbWidth = aabb.Width()+0.01;
			float aabbHeight = aabb.Height()+0.01;
			
			if(direction == ENavDirection::Up)
			{
				aabb.LU.Y = FLT_MAX;
				aabb.RU.Y = FLT_MAX;
				aabb.LB.Y += height / 2.f + aabbHeight / 2.f;
				aabb.RB.Y += height / 2.f + aabbHeight / 2.f;
			}
			else if(direction == ENavDirection::Down)
			{
				aabb.LU.Y -= height / 2.f + aabbHeight / 2.f;
				aabb.RU.Y -= height / 2.f + aabbHeight / 2.f;
				aabb.LB.Y = -FLT_MAX;
				aabb.RB.Y = -FLT_MAX;
			}
			else if(direction == ENavDirection::Left)
			{
				aabb.LU.X = -FLT_MAX;
				aabb.LB.X = -FLT_MAX;
				aabb.RU.X -= width / 2.f + aabbWidth / 2.f;
				aabb.RB.X -= width / 2.f + aabbWidth / 2.f;
			}
			else if(direction == ENavDirection::Right)
			{
				aabb.LU.X += width / 2.f + aabbWidth / 2.f;
				aabb.LB.X += width / 2.f + aabbWidth / 2.f;
				aabb.RU.X = FLT_MAX;
				aabb.RB.X = FLT_MAX;
			}

			return aabb;
		}

		bool Intersect(const Quad& other)
		{
			if ((LB.X > other.RU.X) || (other.LB.X > RU.X))
			{
				return false;
			}

			if ((LB.Y > other.RU.Y) || (other.LB.Y > RU.Y))
			{
				return false;
			}

			return true;
		}

		Rect ToRect()
		{
			return {Vector2f(LB.X, LB.Y), Vector2f(RU.X, RU.Y)};
		}

		float Width()
		{
			return RU.X - LU.X;
		}

		float Height()
		{
			return LU.Y - LB.Y;
		}
	};
}

OGUI::VisualElement* OGUI::VisualElement::FindNextNavTarget(ENavDirection direction)
{
	VisualElement* explicitTarget = nullptr;
	if(direction == ENavDirection::Up && navExplicitUp != "")
		explicitTarget = QueryFirst(GetRoot(), navExplicitUp);
	else if(direction == ENavDirection::Down && navExplicitDown != "")
		explicitTarget = QueryFirst(GetRoot(), navExplicitDown);
	else if(direction == ENavDirection::Left && navExplicitLeft != "")
		explicitTarget = QueryFirst(GetRoot(), navExplicitLeft);
	else if(direction == ENavDirection::Right && navExplicitRight != "")
		explicitTarget = QueryFirst(GetRoot(), navExplicitRight);
	
	if(explicitTarget) return explicitTarget;
	auto currentOrigin = this;
	auto currentQuad = Quad(this);
	auto currentCollision = this;
	auto currentWorldTransform = currentOrigin->_worldTransform;
	auto currentWorldPosition = currentOrigin->_worldPosition;
	bool isCycleMode = false;
	std::vector<VisualElement*> ignore_;
	ignore_.push_back(this);
	
	while (currentOrigin) 
	{
		if(currentOrigin->navMode == ENavMode::None) break;

		if(currentOrigin->navMode == ENavMode::Horizontal 
		&& (direction == ENavDirection::Down || direction == ENavDirection::Up)) break;

		if(currentOrigin->navMode == ENavMode::Vertical 
		&& (direction == ENavDirection::Left || direction == ENavDirection::Right)) break;

		auto currentScope = currentOrigin->GetFocusScopeChildren();
		auto navCollisionBox = Quad(currentCollision->GetHierachyParent() == nullptr ? Rect{{-100000, -100000},{100000, 100000}} : currentCollision->GetRect(), currentWorldTransform)
			.ToNavCollisionBox(direction, currentQuad.Width(), currentQuad.Height());

		VisualElement* currentPriorityGoals = nullptr;
		float minDistance = FLT_MAX;
		float maxDistance = 0.f;
		float maxOverlap = 0.f;
		for(auto element : currentScope)
		{
			// 先比较距离(只和最近的比，防止很多连续相近的)，当两个距离很接近时，就比较垂直方向占比
			if(find(ignore_.begin(), ignore_.end(), element) != ignore_.end()) continue;
			ignore_.push_back(element);

			auto elementQuad = Quad(element).ToBoundingBox();
			if(FocusNavDebugState != NoDebug)
			{
				time_t seconds = time(NULL);
				element->navDebugLastUpdate = seconds;
				element->navDebugRect = elementQuad;
				element->FocusNavDebugState = ElementQuad;

				currentOrigin->navDebugLastUpdate = seconds;
				currentOrigin->navDebugRect = navCollisionBox.ToRect();
				currentOrigin->FocusNavDebugState = CollisionBox;
			}
			if(element == currentOrigin || !navCollisionBox.Intersect(elementQuad)) continue;

			float distance = (element->_worldPosition - currentWorldPosition).length();
			const float near = 10.f;
			if(!isCycleMode && distance >= (minDistance == FLT_MAX ? FLT_MAX : minDistance + near)) continue;
			if(isCycleMode && distance <= (maxDistance == 0.f ? 0.f : maxDistance - near)) continue;

			Quad elementAABB = elementQuad;
			float overlap = 0.f;
			if(direction == ENavDirection::Up || direction == ENavDirection::Down)
			{
				if(elementAABB.LU.X <= navCollisionBox.LU.X && elementAABB.RU.X <= navCollisionBox.RU.X)
					overlap = std::abs(elementAABB.RU.X - navCollisionBox.LU.X);
				else if(elementAABB.LU.X >= navCollisionBox.LU.X && elementAABB.RU.X <= navCollisionBox.RU.X)
					overlap = std::abs(elementAABB.RU.X - elementAABB.LU.X);
				else if(elementAABB.LU.X >= navCollisionBox.LU.X && elementAABB.RU.X >= navCollisionBox.RU.X)
					overlap = std::abs(navCollisionBox.RU.X - elementAABB.LU.X);
				else if(elementAABB.LU.X < navCollisionBox.LU.X && elementAABB.RU.X > navCollisionBox.RU.X)
					overlap = std::abs(navCollisionBox.RU.X - navCollisionBox.LU.X);
			}
			else if(direction == ENavDirection::Left || direction == ENavDirection::Right)
			{
				if(elementAABB.LU.Y <= navCollisionBox.LU.Y && elementAABB.LB.Y <= navCollisionBox.LB.Y)
					overlap = std::abs(elementAABB.LU.Y - navCollisionBox.LB.Y);
				else if(elementAABB.LU.Y >= navCollisionBox.LU.Y && elementAABB.LB.Y <= navCollisionBox.LU.Y)
					overlap = std::abs(navCollisionBox.LU.Y - elementAABB.LB.Y);
				else if(elementAABB.LU.Y <= navCollisionBox.LU.Y && elementAABB.LB.Y >= navCollisionBox.LB.Y)
					overlap = std::abs(elementAABB.LU.Y - elementAABB.LB.Y);
				else if(elementAABB.LU.Y >= navCollisionBox.LU.Y && elementAABB.LB.Y <= navCollisionBox.LB.Y)
					overlap = std::abs(navCollisionBox.LU.Y - navCollisionBox.LB.Y);
			}

			if(!isCycleMode && distance < minDistance)
			{
				if(minDistance - distance > near) maxOverlap = 0.f;
				minDistance = distance;
			}
			else if(isCycleMode && distance > maxDistance)
			{
				if(distance - maxDistance > near) maxOverlap = 0.f;
				maxDistance = distance;
			}

			if(overlap > maxOverlap || 
				(isCycleMode && overlap == maxOverlap && distance > maxDistance) ||
				(!isCycleMode && overlap == maxOverlap && distance < minDistance))
			{
				currentPriorityGoals = element;
				maxOverlap = overlap;
			}
		}

		auto prevFocusScope = currentOrigin->GetPrevFocusScope();
		if(currentPriorityGoals)
		{
			return currentPriorityGoals;
		}
		else if(!isCycleMode && prevFocusScope && prevFocusScope->navCycleMode == ENavCycleMode::Horizontal && (direction == ENavDirection::Left || direction == ENavDirection::Right))
		{
			isCycleMode = true;
			ignore_.clear();
			direction = direction == ENavDirection::Left ? ENavDirection::Right : ENavDirection::Left;
		}
		else if(!isCycleMode && prevFocusScope && prevFocusScope->navCycleMode == ENavCycleMode::Vertical && (direction == ENavDirection::Down || direction == ENavDirection::Up))
		{
			isCycleMode = true;
			ignore_.clear();
			direction = direction == ENavDirection::Down ? ENavDirection::Up : ENavDirection::Down;
		}
		else if(!isCycleMode && prevFocusScope && prevFocusScope->navCycleMode == ENavCycleMode::Automatic)
		{
			isCycleMode = true;
			ignore_.clear();
			if(direction == ENavDirection::Up) direction = ENavDirection::Down;
			else if(direction == ENavDirection::Down) direction = ENavDirection::Up;	
			else if(direction == ENavDirection::Left) direction = ENavDirection::Right;	
			else if(direction == ENavDirection::Right) direction = ENavDirection::Left;	
		}
		else if(prevFocusScope)
		{
			currentOrigin = prevFocusScope;
		}
		else
		{
			currentOrigin = this;
			auto nextCollision = currentCollision->GetPrevFocusScope();
			if(nextCollision == nullptr || currentCollision == nextCollision) return nullptr;
			currentCollision = nextCollision;
			ignore_.clear();
		}
	}

	return nullptr;
}

bool OGUI::VisualElement::PlayAnimation(const AnimStyle& style)
{
	std::vector<StyleSheet*> sheets;
    for(VisualElement* e = this; e; e=e->_logical_parent)
        for(auto sheet : e->_styleSheets)
            sheets.push_back(sheet);
	ComputedAnim anim;
	anim.style = style;
	if(!anim.Init(sheets))
		return false;
	_procedureAnims.push_back(std::move(anim));
	return true;
}

void OGUI::VisualElement::SetAnimationTime(std::string_view name, float time)
{
	olog::Info(u"{}"_o.format(time));
	for(auto& anim : _procedureAnims)
		if(anim.style.animationName == name)
			anim.SetTime(time);
			
	for(auto& anim : _anims)
		if(anim.style.animationName == name)
			anim.SetTime(time);
}

void OGUI::VisualElement::UpdateScrollSize()
{
	if(_scrollSizeDirty)
	{
		Rect scrollRect{};
		Traverse([&](VisualElement* child)
		{
			auto& pos = StylePosition::Get(child->_style);
			if(pos.position != YGPositionTypeRelative)
				return;
			auto baseLayout = child->GetLayout();
			auto offset = (baseLayout.max + baseLayout.min)/2;
			auto rect = child->GetRect();
			auto styleTransform = child->GetStyleTransform();
			auto quad = Quad(rect, styleTransform.to_3D());
			quad.RU += offset;
    		quad.RB += offset;
    		quad.LU += offset;
    		quad.LB += offset;
			auto relativeRect = quad.ToBoundingBox();
			scrollRect.min.x = std::min(scrollRect.min.x, relativeRect.min.x);
			scrollRect.min.y = std::min(scrollRect.min.y, relativeRect.min.y);
			scrollRect.max.x = std::max(scrollRect.max.x, relativeRect.max.x);
			scrollRect.max.y = std::max(scrollRect.max.y, relativeRect.max.y);
		});
		_scrollMax = scrollRect.max - GetSize();
		_scrollMax.x = std::max(0.f, _scrollMax.x);
		_scrollMax.y = std::max(0.f, _scrollMax.y);
		_scrollMin = scrollRect.min;
		if(_scrollMax.y > 0 || _scrollMin.y < 0)
		{
			float axisY = GetScrollingAxisY();
			float minY = _scrollMax.y * (axisY * 0.5f - 0.5f) + _scrollMin.y * (axisY * 0.5f + 0.5f);
			float maxY = _scrollMax.y * (axisY * 0.5f + 0.5f) + _scrollMin.y * (axisY * 0.5f - 0.5f);
			_scrollOffset.y = std::clamp(_scrollOffset.y, minY, maxY);
		}
		else
			_scrollOffset.y = 0;
		if(_scrollMax.x > 0 || _scrollMin.x < 0)
		{
			float axisX = GetScrollingAxisX();
			float minX = _scrollMax.x * (axisX * 0.5f - 0.5f) + _scrollMin.x * (axisX * 0.5f + 0.5f);
			float maxX = _scrollMax.x * (axisX * 0.5f + 0.5f) + _scrollMin.x * (axisX * 0.5f - 0.5f);
			_scrollOffset.x = std::clamp(_scrollOffset.x, minX, maxX);
		}
		else
			_scrollOffset.x = 0;
		
		_scrollSizeDirty = false;
	}
}
bool OGUI::VisualElement::IsScrollingX() const
{
	auto& pos = StylePosition::Get(_style);
	if(pos.overflow == YGOverflow::YGOverflowVisible)
		return false;
	return _scrollMax.x > 0 || _scrollMin.x < 0; 
}

bool OGUI::VisualElement::IsScrollingY() const
{
	auto& pos = StylePosition::Get(_style);
	if(pos.overflow == YGOverflow::YGOverflowVisible)
		return false;
	return _scrollMax.y > 0 || _scrollMin.y < 0; 
}


bool OGUI::VisualElement::IsScrolling() const
{
	auto& pos = StylePosition::Get(_style);
	if(pos.overflow == YGOverflow::YGOverflowVisible)
		return false;
	return IsScrollingX() || IsScrollingY(); 
}

namespace OGUI
{
	float GetMainAxisScroll(const StylePosition& pos)
	{
		switch(pos.justifyContent)
		{
			case YGJustifyFlexStart:
				return 1.f;
			case YGJustifyFlexEnd:
				return -1.f;
			case YGJustifyCenter:
			case YGJustifySpaceAround:
			case YGJustifySpaceBetween:
			case YGJustifySpaceEvenly:
			default:
				return 0.f;
		}
	}

	float GetCrossAxisScroll(const StylePosition& pos)
	{
		switch(pos.alignContent)
		{
			case YGAlignStretch:
			case YGAlignFlexStart:
				return 1.f;
			case YGAlignFlexEnd:
				return -1.f;
			case YGAlignCenter:
			case YGAlignSpaceAround:
			case YGAlignSpaceBetween:
			default:
				return 0.f;
		}
	}
}

float OGUI::VisualElement::GetScrollingAxisX() const
{
	auto& pos = StylePosition::Get(_style);
	switch(pos.flexDirection)
	{
		case YGFlexDirectionRow:
			return GetMainAxisScroll(pos);
		case YGFlexDirectionRowReverse:
			return -1.f * GetMainAxisScroll(pos);
		case YGFlexDirectionColumn:
			return GetCrossAxisScroll(pos);
		case YGFlexDirectionColumnReverse:
			return -1.f * GetCrossAxisScroll(pos);
		default:
			return 0.f;
	}
}

float OGUI::VisualElement::GetScrollingAxisY() const
{
	auto& pos = StylePosition::Get(_style);
	switch(pos.flexDirection)
	{
		case YGFlexDirectionRow:
			return GetCrossAxisScroll(pos);
		case YGFlexDirectionRowReverse:
			return -1.f * GetCrossAxisScroll(pos);
		case YGFlexDirectionColumn:
			return GetMainAxisScroll(pos);
		case YGFlexDirectionColumnReverse:
			return -1.f * GetMainAxisScroll(pos);
		default:
			return 0.f;
	}
}

void OGUI::VisualElement::AddScroll(Vector2f delta)
{
	SetScroll(_scrollOffset + delta);
}

void OGUI::VisualElement::SetScroll(Vector2f offset)
{
	if(IsScrollingY())
	{
		_scrollOffset.y = offset.y;
		float axisY = GetScrollingAxisY();
		float minY = _scrollMax.y * (axisY * 0.5f - 0.5f) + _scrollMin.y * (axisY * 0.5f + 0.5f);
		float maxY = _scrollMax.y * (axisY * 0.5f + 0.5f) + _scrollMin.y * (axisY * 0.5f - 0.5f);
		_scrollOffset.y = std::clamp(_scrollOffset.y, minY, maxY);
	}
	if(IsScrollingX())
	{
		_scrollOffset.x = offset.x;
		float axisX = GetScrollingAxisX();
		float minX = _scrollMax.x * (axisX * 0.5f - 0.5f) + _scrollMin.x * (axisX * 0.5f + 0.5f);
		float maxX = _scrollMax.x * (axisX * 0.5f + 0.5f) + _scrollMin.x * (axisX * 0.5f - 0.5f);
		_scrollOffset.x = std::clamp(_scrollOffset.x, minX, maxX);
	}
	if(IsScrolling())
		Traverse([](VisualElement* child)
		{
			child->_transformDirty = true;
		});
}

OGUI::Vector2f OGUI::VisualElement::GetScrollPos()
{
	//Vector2f axis = {GetScrollingAxisX(), GetScrollingAxisY()};
	//Vector2f pivot = Vector2f{-axis.x/2 + 0.5f, -axis.y/2 + 0.5f} * _scrollSize;
	return _scrollOffset;
}