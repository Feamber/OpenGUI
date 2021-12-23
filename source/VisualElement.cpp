

#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/OMath.h"
#include <type_traits>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/generated/border.h"
#include "Yoga.h"
#include "YGValue.h"
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Context.h"
#include <gsl/span>


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
		if (!backgroundImageResource || backgroundImageUrl != bg.backgroundImage)
		{
			backgroundImageResource = ctx.textureManager->RequireFromFileSystem(bg.backgroundImage);
			backgroundImageUrl = bg.backgroundImage;
		}
	}
	else //release old texture
		backgroundImageResource = nullptr;
	
	BeginDraw(Ctx.prims);
	Rect uv = {Vector2f::vector_zero(), Vector2f::vector_one()};
	auto bgcolor = bg.backgroundColor;
	bgcolor.w *= _opacity;
	TextureInterface* tex = nullptr;
	if (backgroundImageResource)
	{
		if(backgroundImageResource->valid())
			tex = backgroundImageResource->Get();
		else 
			bgcolor.w = 0;
	}
	RoundBoxParams params {rect, uv, bgcolor};
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

	if((FocusNavDebugState == CollisionBox || FocusNavDebugState == ElementQuad) && Context::Get().IsFocusNavDebug())
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

OGUI::Matrix4x4 OGUI::VisualElement::ApplyClipping()
{
	auto rect = GetRect();
	auto offset = (rect.min + rect.max) / 2;
	auto size = (rect.max - rect.min) / 2;
	size.x = 1 / size.x;
	size.y = 1 / size.y;
	return math::multiply(_invTransform, math::make_transform_2d(-offset, 0.f, size));
}

bool OGUI::VisualElement::CheckClip(const Matrix4x4& rect)
{
	auto transform = math::multiply(_worldTransform, rect);
	auto quad = Quad(GetRect(), transform);
	if(
		(quad.LB.x == std::clamp(quad.LB.x, -1.f, 1.f)&&
		quad.LB.y == std::clamp(quad.LB.y, -1.f, 1.f))||
		(quad.RB.x == std::clamp(quad.RB.x, -1.f, 1.f)&&
		quad.RB.y == std::clamp(quad.RB.y, -1.f, 1.f))||
		(quad.LU.x == std::clamp(quad.LU.x, -1.f, 1.f)&&
		quad.LU.y == std::clamp(quad.LU.y, -1.f, 1.f))||
		(quad.RU.x == std::clamp(quad.RU.x, -1.f, 1.f)&&
		quad.RU.y == std::clamp(quad.RU.y, -1.f, 1.f))
	)
		return false;
	return true;
}

bool OGUI::VisualElement::IsClippingChildren()
{
	auto& pos = StylePosition::Get(_style);
	return pos.overflow != StyleOverflow::Visible;
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
	if(_physicalParent)
		_physicalParent->RemoveChild(this);
	if(_logicalParent)
		_logicalParent->RemoveChild(this);
	if (_ygnode)
	{
		YGNodeFree(_ygnode);
	}
	auto& Ctx = Context::Get();
	if(!_styleSheets.empty())
		Ctx.InvalidateCssCache();
	auto& allElementHandle = Ctx._allElementHandle;
	allElementHandle.erase(this);
	if(Ctx._keyboardFocused == this)
		Ctx._keyboardFocused = nullptr;
	for(auto sheet : _styleSheets)
		delete sheet;
}

void OGUI::VisualElement::DrawPrimitive(OGUI::PrimitiveDraw::DrawContext& Ctx)
{
	DrawBackgroundPrimitive(Ctx);
	DrawBorderPrimitive(Ctx);
	DrawDebugPrimitive(Ctx);
}

OGUI::VisualElement* OGUI::VisualElement::GetParent()
{
	return _logicalParent;
}

OGUI::VisualElement* OGUI::VisualElement::GetHierachyParent()
{
	return _physicalParent;
}

bool OGUI::VisualElement::IsParent(OGUI::VisualElement* e)
{
	if(!e || !_physicalParent || e == this)
		return false;
	if(e == _physicalParent)
		return true;
	return _physicalParent->IsParent(e);
}

void OGUI::VisualElement::MarkDirty(DirtyReason reason)
{
	Context::Get().MarkDirty(this, reason);
}

const char* OGUI::VisualElement::GetName()
{
	return _name.c_str();
}

void OGUI::VisualElement::SetName(const char* name)
{
	_name = name;
}

void OGUI::VisualElement::DestoryTree(VisualElement* element)
{
	std::vector<VisualElement*> toDestroy;
	std::vector<VisualElement*> children;
	toDestroy.push_back(element);
	while (toDestroy.size() > 0)
	{
		auto back = toDestroy.back(); toDestroy.pop_back();
		children.clear();
		back->GetChildren(children);
		for (auto child : children)
		{
			child->_logicalParent = child->_physicalParent = nullptr;
			toDestroy.push_back(child);
		}
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
	child->_physicalParent = this;
	InsertChild(child, _children.size());\
}

void OGUI::VisualElement::InsertChild(VisualElement* child, int index)
{
	_scrollSizeDirty = true;
	child->_physicalParent = this;
	if(_ygnode)
		YGNodeInsertChild(_ygnode, child->_ygnode, index);
	_children.insert(_children.begin() + index, child);
	child->_layoutType = LayoutType::Flex;
	child->_selectorDirty = true;
	UpdateRoot(child);
	auto& Ctx = Context::Get();
	Ctx._layoutDirty = true;
}

void OGUI::VisualElement::RemoveChild(VisualElement* child)
{
	_scrollSizeDirty = true;
    child->_physicalParent = nullptr;
	if(_ygnode)
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

void OGUI::VisualElement::CalculateLayout(float width, float height)
{
	YGNodeCalculateLayout(_ygnode, width, height, YGNodeStyleGetDirection(_ygnode));
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
		Vector2f offset;
		auto playout = parent->GetLayout();
		offset = (layout.min + layout.max)/2 - (playout.max - playout.min) / 2;
		offset.y = -offset.y;
		auto& pos = StylePosition::Get(_style);
		if(_scrollable)
			offset += parent->GetScrollPos();
		_worldTransform = multiply(GetStyleTransform(), ComputedTransform::translate(offset)).to_3D();
		_worldTransform = math::multiply(_worldTransform, parent->_worldTransform);
	}
	else
	{
		auto offset = -(layout.max - layout.min) / 2;
		_worldTransform = GetStyleTransform().to_3D();
	}
	_invTransform = math::inverse(_worldTransform);
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
	return {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
}

void OGUI::VisualElement::MarkTransformDirty()
{
	_transformDirty = true;
	if(_physicalParent)
		_physicalParent->_scrollSizeDirty = true;
}

void OGUI::VisualElement::MarkStyleTransformDirty()
{
	_styleTransformDirty = true;
	MarkTransformDirty();
}

void OGUI::VisualElement::MarkLayoutDirty(bool visiblity)
{
}

void OGUI::VisualElement::NotifyLayoutDirty(bool visiblity)
{
	Context::Get()._layoutDirty = true;
	auto p = this;
	while(p!=nullptr)
	{
		p = p->GetLayoutRoot();
		p->MarkLayoutDirty(visiblity);
		p = p->GetHierachyParent();
		visiblity = false;
	}
}

namespace OGUI
{
	YGOverflow ToYGOverflow(StyleOverflow o)
	{
		switch(o)
		{
			case StyleOverflow::Auto:
				return YGOverflowScroll;
			case StyleOverflow::Clip:
				return YGOverflowVisible;
			case StyleOverflow::Visible:
				return YGOverflowVisible;
			case StyleOverflow::Scroll:
				return YGOverflowScroll;
			case StyleOverflow::Hidden:
				return YGOverflowHidden;
			default:
				return YGOverflowVisible;
		}
	}	
}

void OGUI::VisualElement::SyncYogaStyle()
{
	if (!_ygnode)
		return;
	auto& pos = StylePosition::Get(_style);
	auto& bd = StyleBorder::Get(_style);
#define SetYGEdge(node, function, edge, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(node, edge, v.value); \
	else \
		function(node, edge, v.value)
#define SetYGEdgeAuto(node, function, edge, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(node, edge, v.value); \
	else if(v.unit == YGUnitAuto) \
		function##Auto(node, edge); \
	else \
		function(node, edge, v.value)
#define SetYGValueAuto(node, function, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(node, v.value); \
	else if(v.unit == YGUnitAuto) \
		function##Auto(node); \
	else \
		function(node, v.value)
#define SetYGValue(node, function, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(node, v.value); \
	else \
		function(node, v.value)

	SetYGEdge(_ygnode, YGNodeStyleSetPadding, YGEdgeLeft, pos.paddingLeft);
	SetYGEdge(_ygnode, YGNodeStyleSetPadding, YGEdgeTop, pos.paddingTop);
	SetYGEdge(_ygnode, YGNodeStyleSetPadding, YGEdgeRight, pos.paddingRight);
	SetYGEdge(_ygnode, YGNodeStyleSetPadding, YGEdgeBottom, pos.paddingBottom);
	YGNodeStyleSetBorder(_ygnode, YGEdgeLeft, bd.borderLeftWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeTop, bd.borderTopWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeRight, bd.borderRightWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeBottom, bd.borderBottomWidth);
	YGNodeStyleSetFlexWrap(_ygnode, pos.flexWrap);
	YGNodeStyleSetAlignItems(_ygnode, pos.alignItems);
	YGNodeStyleSetAlignContent(_ygnode, pos.alignContent);
	YGNodeStyleSetJustifyContent(_ygnode, pos.justifyContent);
	YGNodeStyleSetFlexDirection(_ygnode, pos.flexDirection);
	YGNodeStyleSetFlexGrow(_ygnode, pos.flexGrow);
	YGNodeStyleSetFlexShrink(_ygnode, pos.flexShrink);
	SetYGValueAuto(_ygnode, YGNodeStyleSetFlexBasis, pos.flexBasis);
	SetYGEdge(_ygnode, YGNodeStyleSetPosition, YGEdgeLeft, pos.left);
	SetYGEdge(_ygnode, YGNodeStyleSetPosition, YGEdgeTop, pos.top);
	SetYGEdge(_ygnode, YGNodeStyleSetPosition, YGEdgeRight, pos.right);
	SetYGEdge(_ygnode, YGNodeStyleSetPosition, YGEdgeBottom, pos.bottom);
	SetYGEdgeAuto(_ygnode, YGNodeStyleSetMargin, YGEdgeLeft, pos.marginLeft);
	SetYGEdgeAuto(_ygnode, YGNodeStyleSetMargin, YGEdgeTop, pos.marginTop);
	SetYGEdgeAuto(_ygnode, YGNodeStyleSetMargin, YGEdgeRight, pos.marginRight);
	SetYGEdgeAuto(_ygnode, YGNodeStyleSetMargin, YGEdgeBottom, pos.marginBottom);

	SetYGValueAuto(_ygnode, YGNodeStyleSetWidth, pos.width);
	SetYGValueAuto(_ygnode, YGNodeStyleSetHeight, pos.height);
	YGNodeStyleSetPositionType(_ygnode, pos.position);
	YGNodeStyleSetOverflow(_ygnode, ToYGOverflow(pos.overflow));
	YGNodeStyleSetAlignSelf(_ygnode, pos.alignSelf);
	YGNodeStyleSetAspectRatio(_ygnode, pos.aspectRatio);
	SetYGValue(_ygnode, YGNodeStyleSetMaxWidth, pos.maxWidth);
	SetYGValue(_ygnode, YGNodeStyleSetMaxHeight, pos.maxHeight);
	SetYGValue(_ygnode, YGNodeStyleSetMinWidth, pos.minWidth);
	SetYGValue(_ygnode, YGNodeStyleSetMinHeight, pos.minHeight);
	
	bool visChanged = YGNodeStyleGetDisplay(_ygnode) != (_visible ? pos.flexDisplay : YGDisplayNone);
	YGNodeStyleSetDisplay(_ygnode, _visible ? pos.flexDisplay : YGDisplayNone);
	NotifyLayoutDirty(visChanged);
}

void OGUI::VisualElement::UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss)
{
	if(HasFlag(damage, RestyleDamage::Layout))
		SyncYogaStyle();
	if(HasFlag(damage, RestyleDamage::Transform))
		MarkStyleTransformDirty();
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
		Bindable::Notify(iter->second, force);
}

bool OGUI::VisualElement::Visible() const
{
	return !(StylePosition::Get(_style).flexDisplay == YGDisplayNone) && !_clipped && _visible;
}

void OGUI::VisualElement::SetVisibility(bool visible)
{
	_visible = visible;
	NotifyLayoutDirty(true);
}

bool OGUI::VisualElement::Intersect(Vector2f point)
{
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
	_eventHandler.Register<PointerDownEvent, &VisualElement::_OnPointerDown>(this);
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

bool OGUI::VisualElement::_OnPointerDown(struct PointerDownEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach && focusable)
	{
		Context::Get().SetFocus(this);
		return true;
	}
	return false;
}

OGUI::VisualElement* OGUI::VisualElement::GetPrevFocusScope()
{
	VisualElement* current = this;
	while (current) 
	{
		if(current->_physicalParent && current->_physicalParent->isFocusScope)
			return current->_physicalParent;
		current = current->_physicalParent;
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
		current = Context::Get().IsElementValid(current->currentFocused) ? current->currentFocused : nullptr;
	}
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
			if(element == currentOrigin || !element->Visible() || !navCollisionBox.Intersect(elementQuad)) continue;

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
    for(VisualElement* e = this; e; e=e->_logicalParent)
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
	for(auto& anim : _procedureAnims)
		if(anim.style.animationName == name)
			anim.SetTime(time);
			
	for(auto& anim : _anims)
		if(anim.style.animationName == name)
			anim.SetTime(time);
}


bool OGUI::VisualElement::ScrollOnRow() const
{
	auto& pos = StylePosition::Get(_style);
	bool row = pos.flexDirection == YGFlexDirectionRow || pos.flexDirection == YGFlexDirectionRowReverse;
	bool wrap = pos.flexWrap != YGWrapNoWrap;
	return row != wrap;
}

void OGUI::VisualElement::UpdateScrollSize()
{
	if(!CanScroll())
		return;
	if(_scrollSizeDirty)
	{
		Rect rect = {Vector2f::vector_zero(), {YGNodeLayoutGetContentWidth(_ygnode), YGNodeLayoutGetContentHeight(_ygnode)}};
		float axisY = GetScrollingAxisY();
		float axisX = GetScrollingAxisX();
		{
			float minY = rect.max.y * (axisY * 0.5f - 0.5f) + rect.min.y * (axisY * 0.5f + 0.5f);
			float maxY = rect.max.y * (axisY * 0.5f + 0.5f) + rect.min.y * (axisY * 0.5f - 0.5f);
			float minX = rect.max.x * (axisX * 0.5f - 0.5f) + rect.min.x * (axisX * 0.5f + 0.5f);
			float maxX = rect.max.x * (axisX * 0.5f + 0.5f) + rect.min.x * (axisX * 0.5f - 0.5f);
			rect = {{minX, minY}, {maxX, maxY}};
		}
		Rect clientRect =  {Vector2f::vector_zero(), GetSize() };
		{
			float minY = clientRect.max.y * (axisY * 0.5f - 0.5f) + clientRect.min.y * (axisY * 0.5f + 0.5f);
			float maxY = clientRect.max.y * (axisY * 0.5f + 0.5f) + clientRect.min.y * (axisY * 0.5f - 0.5f);
			float minX = clientRect.max.x * (axisX * 0.5f - 0.5f) + clientRect.min.x * (axisX * 0.5f + 0.5f);
			float maxX = clientRect.max.x * (axisX * 0.5f + 0.5f) + clientRect.min.x * (axisX * 0.5f - 0.5f);
			clientRect = {{minX, minY}, {maxX, maxY}};
		}
		Traverse([&](VisualElement* child)
		{
			if(!child->_scrollable)
				return;
			auto& pos = StylePosition::Get(child->_style);
			if(pos.position != YGPositionTypeRelative)
				return;
			if(pos.transform.empty())
				return;
			auto baseLayout = child->GetLayout();
			auto rect = child->GetRect();
			auto offset = (baseLayout.max + baseLayout.min)/2;
			auto styleTransform = child->GetStyleTransform();
			auto quad = Quad(rect, styleTransform.to_3D());
			quad.RU += offset;
    		quad.RB += offset;
    		quad.LU += offset;
    		quad.LB += offset;
			auto relativeRect = quad.ToBoundingBox();
			rect.min.x = std::min(rect.min.x, relativeRect.min.x);
			rect.min.y = std::min(rect.min.y, relativeRect.min.y);
			rect.max.x = std::max(rect.max.x, relativeRect.max.x);
			rect.max.y = std::max(rect.max.y, relativeRect.max.y);
		});
		_scrollMax = rect.max - clientRect.max;
		_scrollMin = rect.min - clientRect.min;
		if(_scrollMax.y > 0 || _scrollMin.y < 0)
		{
			float minY = _scrollMax.y * (axisY * 0.5f - 0.5f) + _scrollMin.y * (axisY * 0.5f + 0.5f);
			float maxY = _scrollMax.y * (axisY * 0.5f + 0.5f) + _scrollMin.y * (axisY * 0.5f - 0.5f);
			_scrollOffset.y = std::clamp(_scrollOffset.y, minY, maxY);
		}
		else
			_scrollOffset.y = 0;
		if(_scrollMax.x > 0 || _scrollMin.x < 0)
		{
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
	if(!CanScroll())
		return false;
	return _scrollMax.x > 0 || _scrollMin.x < 0; 
}

bool OGUI::VisualElement::IsScrollingY() const
{
	auto& pos = StylePosition::Get(_style);
	if(!CanScroll())
		return false;
	return _scrollMax.y > 0 || _scrollMin.y < 0; 
}


bool OGUI::VisualElement::IsScrolling() const
{
	if(!CanScroll())
		return false;
	return IsScrollingX() || IsScrollingY(); 
}

bool OGUI::VisualElement::CanScroll() const
{
	auto& pos = StylePosition::Get(_style);
	if(pos.overflow == StyleOverflow::Clip || pos.overflow == StyleOverflow::Visible)
		return false;
	return true;
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
		switch(pos.alignItems)
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

void OGUI::VisualElement::SetXmlFilter(const char* key, const char* filterTag)
{
	auto result = _localXmlFiltersMap.try_emplace(key, filterTag);
	bool isSet = result.first->second != filterTag;
	if(isSet)
		result.first->second = filterTag;

	if(result.second || isSet)
	{
		UpdataXmlFilterCache();
		std::map<Name, int> localXmlFilters;
		Context::Get().RecursionUpdataFilter(this, localXmlFilters);
	}
}

void OGUI::VisualElement::CleanXmlFilter(const char* key)
{
	_localXmlFiltersMap.erase(key);
	UpdataXmlFilterCache();
}

void OGUI::VisualElement::UpdataXmlFilterCache()
{
	_localXmlFiltersCache.clear();
	for(auto pair : _localXmlFiltersMap)
		_localXmlFiltersCache.insert(pair.second);
}

bool OGUI::VisualElement::HasFilterTag(const char* filterTag) const
{
	return HasFilterTag(Name(filterTag));
}

bool OGUI::VisualElement::HasFilterTag(Name filterTag) const
{
	return HasFilterTag((VisualElement*)this, filterTag);
}

bool OGUI::VisualElement::HasFilterTag(OGUI::VisualElement* element, const Name& filterTag) const
{
	if(!element)
		return false;
	if(element->_localXmlFiltersCache.count(filterTag))
		return true;
	return HasFilterTag(element->GetHierachyParent(), filterTag);
}


void OGUI::BindTree(VisualElement* element, Bindable& bindable)
{
	element->Bind(bindable);
	bindable.Bind(*element);
	element->Traverse([&](VisualElement* next)
	{
		BindTree(next, bindable);
	});
}