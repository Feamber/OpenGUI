#define DLL_IMPLEMENTATION
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Math.h"
#include <type_traits>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Xml/XmlFactoryTool.h"
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Animation/AnimStyle.h"
#include "OpenGUI/Context.h"

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
	//auto rectPixelPos = GetRect();
	Rect rect = GetRect();
	//Rect rect_origin = GetRect();
	//Rect rect = rectPixelPosToScreenPos(rect_origin, Ctx.resolution);

	auto transform = _worldTransform;
	//transform.M[3][0] /= Ctx.resolution.X;
	//transform.M[3][1] /= Ctx.resolution.Y;
	if (!_style.backgroundImage.empty())
	{
		//start new load
		if (!backgroundImageResource || !backgroundImageResource->valid() || backgroundImageUrl != _style.backgroundImage)
		{
			backgroundImageResource = Ctx.Window.textureManager->RequireFromFileSystem(_style.backgroundImage);
			backgroundImageUrl = _style.backgroundImage;
		}
	}
	else //release old texture
		backgroundImageResource = nullptr;
	
	BeginDraw(Ctx.prims);
	Rect uv = {Vector2f::vector_zero(), Vector2f::vector_one()};
	RoundBoxParams params {rect, uv, _style.backgroundColor };
	TextureInterface* tex = nullptr;
	if (backgroundImageResource && backgroundImageResource->valid())
	{
		tex = backgroundImageResource->Get();
	}
	params.radius[0] = _style.borderTopLeftRadius.value;// / Ctx.resolution.Y;
	params.radius[1] = _style.borderTopRightRadius.value;// / Ctx.resolution.Y;
	params.radius[2] = _style.borderBottomRightRadius.value;// / Ctx.resolution.Y;
	params.radius[3] = _style.borderBottomLeftRadius.value;// / Ctx.resolution.Y;
	PrimitiveDraw::PrimitiveDraw<RoundBoxShape2>(tex, Ctx.prims, params, 20);
	EndDraw(Ctx.prims, transform, Ctx.resolution);
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
			EndDraw(Ctx.prims, float4x4(), Ctx.resolution);
		}
	}
}

void OGUI::VisualElement::ApplyClipping(OGUI::PrimitiveDraw::DrawContext & Ctx)
{}

void OGUI::VisualElement::CreateYogaNode()
{
	auto config = YGConfigGetDefault();
	_ygnode = YGNodeNewWithConfig(config);
	YGNodeSetContext(_ygnode, this);
}

OGUI::VisualElement::VisualElement()
{
	CreateYogaNode();
	_style = Style::GetInitialStyle();
	RegisterFocusedEvent();
}

OGUI::VisualElement::~VisualElement()
{
	if (_ygnode)
	{
		YGNodeFree(_ygnode);
	}
}

void OGUI::VisualElement::DrawPrimitive(OGUI::PrimitiveDraw::DrawContext& Ctx)
{
	DrawBackgroundPrimitive(Ctx);
	DrawBorderPrimitive(Ctx);
	DrawDebugPrimitive(Ctx);
	ApplyClipping(Ctx);
}

OGUI::VisualElement* OGUI::VisualElement::GetParent()
{
	return _logical_parent;
}

OGUI::VisualElement* OGUI::VisualElement::GetHierachyParent()
{
	return _physical_parent;
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

void OGUI::VisualElement::PushChild(VisualElement* child)
{
	InsertChild(child, _children.size());
}

void OGUI::VisualElement::InsertChild(VisualElement* child, int index)
{
	child->_physical_parent = this;
	YGNodeInsertChild(_ygnode, child->_ygnode, index);
	_children.insert(_children.begin() + index, child);
}

void OGUI::VisualElement::RemoveChild(VisualElement* child)
{
    child->_physical_parent = nullptr;
    YGNodeRemoveChild(_ygnode, child->_ygnode);
	auto end = std::remove(_children.begin(), _children.end(), child);
	_children.erase(end, _children.end());
}

OGUI::VisualElement* OGUI::VisualElement::GetRoot()
{
	if(auto Parent = GetHierachyParent())
	{
		return Parent->GetRoot();
	}
	return this;
}

void OGUI::VisualElement::CalculateLayout()
{
	//TODO: mark transform dirty
	YGNodeCalculateLayout(_ygnode, YGUndefined, YGUndefined, YGNodeStyleGetDirection(_ygnode));
	YGNodeSetHasNewLayout(_ygnode, false);
}

void OGUI::VisualElement::UpdateWorldTransform()
{
	using namespace math;
	auto layout = GetLayout();
	auto parent = GetHierachyParent();
	if (parent)
	{
		auto playout = parent->GetLayout();
		auto offset = (layout.min + layout.max)/2 -(playout.max - playout.min) / 2;
		_worldTransform = math::make_transform_2d(offset + _style.translation, _style.rotation, _style.scale);
		_worldTransform = multiply(_worldTransform, parent->_worldTransform);
	}
	else
	{
		auto offset = -(layout.max - layout.min) / 2;
		_worldTransform = math::make_transform_2d(_style.translation, _style.rotation, _style.scale);
	}
	_worldPosition = {_worldTransform.M[3][0], _worldTransform.M[3][1]};
	_transformDirty = false;
}

OGUI::Rect OGUI::VisualElement::GetLayout()
{
	Vector2f LB = {YGNodeLayoutGetLeft(_ygnode), YGNodeLayoutGetTop(_ygnode)};
	Vector2f WH = {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
	return
	{
		LB,
		LB + WH,
	};
}

OGUI::Rect OGUI::VisualElement::GetRect()
{
	Vector2f LB = Vector2f::vector_zero();
	Vector2f WH = {YGNodeLayoutGetWidth(_ygnode), YGNodeLayoutGetHeight(_ygnode)};
	return
	{
		- WH / 2,
		WH / 2,
	};
}

void OGUI::VisualElement::SyncYogaStyle()
{
	if (!_ygnode)
		return;
	Context::Get()._layoutDirty = true;
	YGNodeStyleSetFlex(_ygnode, _style.flex);
	YGNodeStyleSetFlexGrow(_ygnode, _style.flexGrow);
	YGNodeStyleSetFlexShrink(_ygnode, _style.flexShrink);
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

	SetYGValueAuto(YGNodeStyleSetFlexBasis, _style.flexBasis);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeLeft, _style.left);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeTop, _style.top);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeRight, _style.right);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeBottom, _style.bottom);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeLeft, _style.marginLeft);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeTop, _style.marginTop);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeRight, _style.marginRight);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeBottom, _style.marginBottom);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeLeft, _style.paddingLeft);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeTop, _style.paddingTop);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeRight, _style.paddingRight);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeBottom, _style.paddingBottom);
	YGNodeStyleSetBorder(_ygnode, YGEdgeLeft, _style.borderLeftWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeTop, _style.borderTopWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeRight, _style.borderRightWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeBottom, _style.borderBottomWidth);

	SetYGValueAuto(YGNodeStyleSetWidth, _style.width);
	SetYGValueAuto(YGNodeStyleSetHeight, _style.height);
	YGNodeStyleSetPositionType(_ygnode, _style.position);
	YGNodeStyleSetOverflow(_ygnode, _style.overflow);
	YGNodeStyleSetAlignSelf(_ygnode, _style.alignSelf);
	SetYGValue(YGNodeStyleSetMaxWidth, _style.maxWidth);
	SetYGValue(YGNodeStyleSetMaxHeight, _style.maxHeight);
	SetYGValue(YGNodeStyleSetMinWidth, _style.minWidth);
	SetYGValue(YGNodeStyleSetMinHeight, _style.minHeight);
	YGNodeStyleSetFlexDirection(_ygnode, _style.flexDirection);
	YGNodeStyleSetAlignContent(_ygnode, _style.alignContent);
	YGNodeStyleSetAlignItems(_ygnode, _style.alignItems);
	YGNodeStyleSetJustifyContent(_ygnode, _style.justifyContent);
	YGNodeStyleSetFlexWrap(_ygnode, _style.wrap);
	YGNodeStyleSetDisplay(_ygnode, _style.display);
}

bool OGUI::VisualElement::ContainClass(std::string_view cls)
{
	return std::find(_styleClasses.begin(), _styleClasses.end(), cls) != _styleClasses.end();
}

void OGUI::VisualElement::_ResetStyles()
{
	_sharedStyle = nullptr;
	_selectorDirty = true;
	//dosent clean this cause we want to inherit anim context
	//_animContext.clear();
	//_animStyles.clear();
	_triggerPseudoMask = 0;
	_dependencyPseudoMask = 0;
}

void OGUI::VisualElement::ResetStyles()
{
	_ResetStyles();
	if (_beforeElement)
		_beforeElement->_ResetStyles();
	if (_afterElement)
		_afterElement->_ResetStyles();
}

namespace std
{
	void split(const string& s, vector<string>& tokens, const string_view& delimiters = " ")
	{
		string::size_type lastPos = s.find_first_not_of(delimiters, 0);
		string::size_type pos = s.find_first_of(delimiters, lastPos);
		while (string::npos != pos || string::npos != lastPos)
		{
			auto substr = s.substr(lastPos, pos - lastPos);
			tokens.push_back(substr);//use emplace_back after C++11
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
		}
	}
}

bool OGUI::VisualElement::Intersect(Vector2f point)
{
	auto rect = GetRect();
	return rect.IntersectPoint(point);
}

void OGUI::VisualElement::SetPseudoClass(PseudoStates state, bool b)
{
	if (b)
		_pseudoMask |= (uint32_t)state;
	else
		_pseudoMask &= ~(uint32_t)state;
	if ((_triggerPseudoMask & _pseudoMask) != 0 || (_dependencyPseudoMask & ~_pseudoMask) != 0)
		_selectorDirty = true;
}

#include "OpenGUI/CSSParser/CSSParser.h"
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
	_eventHandler.Register<PreGotKeyboardFocusEvent, &VisualElement::OnPreGotKeyboardFocus_Internal>(this);
	_eventHandler.Register<PreLostKeyboardFocusEvent, &VisualElement::OnPreLostKeyboardFocus_Internal>(this);
	_eventHandler.Register<GotKeyboardFocusEvent, &VisualElement::OnGotKeyboardFocus_Internal>(this);
	_eventHandler.Register<LostKeyboardFocusEvent, &VisualElement::OnLostKeyboardFocus_Internal>(this);
	_eventHandler.Register<PreGotFocusEvent, &VisualElement::OnPreGotFocus_Internal>(this);
	_eventHandler.Register<PreLostFocusEvent, &VisualElement::OnPreLostFocus_Internal>(this);
	_eventHandler.Register<GotFocusEvent, &VisualElement::OnGotFocus_Internal>(this);
	_eventHandler.Register<LostFocusEvent, &VisualElement::OnLostFocus_Internal>(this);
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

bool OGUI::VisualElement::OnGotKeyboardFocus_Internal(struct GotKeyboardFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::KeyboardFocus, true);
	OnGotKeyboardFocus(event);
	return false;
}

bool OGUI::VisualElement::OnLostKeyboardFocus_Internal(struct LostKeyboardFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::KeyboardFocus, false);
	OnLostKeyboardFocus(event);
	return false;
}

bool OGUI::VisualElement::OnGotFocus_Internal(struct GotFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::Focus, true);
	OnGotFocus(event);
	return false;
}

bool OGUI::VisualElement::OnLostFocus_Internal(struct LostFocusEvent& event)
{
	if(event.currentPhase == EventRoutePhase::Reach)
		SetPseudoClass(PseudoStates::Focus, false);
	OnLostFocus(event);
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

		Quad ToBoundingBox()
		{
			float minX = RU.X < RB.X && RU.X < LU.X && RU.X < LB.X ? RU.X : RB.X < LU.X && RB.X < LB.X ? RB.X : LU.X < LB.X ? LU.X : LB.X;
			float minY = RU.Y < RB.Y && RU.Y < LU.Y && RU.Y < LB.Y ? RU.Y : RB.Y < LU.Y && RB.Y < LB.Y ? RB.Y : LU.Y < LB.Y ? LU.Y : LB.Y;
			float maxX = RU.X > RB.X && RU.X > LU.X && RU.X > LB.X ? RU.X : RB.X > LU.X && RB.X > LB.X ? RB.X : LU.X > LB.X ? LU.X : LB.X;
			float maxY = RU.Y > RB.Y && RU.Y > LU.Y && RU.Y > LB.Y ? RU.Y : RB.Y > LU.Y && RB.Y > LB.Y ? RB.Y : LU.Y > LB.Y ? LU.Y : LB.Y;
			return Quad({Vector2f(minX, minY), Vector2f(maxX, maxY)});
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
	auto currentOrigin = this;
	auto currentQuad = Quad(this);
	auto currentCollision = this;
	auto currentWorldTransform = currentOrigin->_worldTransform;
	auto currentWorldPosition = currentOrigin->_worldPosition;
	bool isCycleMode = false;
	std::vector<VisualElement*> ignore;
	ignore.push_back(this);
	
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
			if(find(ignore.begin(), ignore.end(), element) != ignore.end()) continue;
			ignore.push_back(element);

			auto elementQuad = Quad(element).ToBoundingBox();
			if(FocusNavDebugState != NoDebug)
			{
				time_t seconds = time(NULL);
				element->navDebugLastUpdate = seconds;
				element->navDebugRect = elementQuad.ToRect();
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

			auto elementAABB = elementQuad.ToBoundingBox();
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
			ignore.clear();
			direction = direction == ENavDirection::Left ? ENavDirection::Right : ENavDirection::Left;
		}
		else if(!isCycleMode && prevFocusScope && prevFocusScope->navCycleMode == ENavCycleMode::Vertical && (direction == ENavDirection::Down || direction == ENavDirection::Up))
		{
			isCycleMode = true;
			ignore.clear();
			direction = direction == ENavDirection::Down ? ENavDirection::Up : ENavDirection::Down;
		}
		else if(!isCycleMode && prevFocusScope && prevFocusScope->navCycleMode == ENavCycleMode::Automatic)
		{
			isCycleMode = true;
			ignore.clear();
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
			ignore.clear();
		}
	}

	return nullptr;
}