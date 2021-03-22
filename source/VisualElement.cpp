#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Style/Style.h"

OGUI::Rect rectPixelPosToScreenPos(const OGUI::Rect& rect, const OGUI::Vector2f resolution)
{
	OGUI::Rect result = rect;
	result.max /= resolution;
	result.min /= resolution;
	return result;
}

void OGUI::VisualElement::DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx)
{
	using namespace PrimitiveDraw;
	auto rectPixelPos = GetRect();
	Rect rect = rectPixelPosToScreenPos(rectPixelPos, Ctx.resolution);
	//TODO: Apply Transform
	PrimitiveDraw::DrawBox(Ctx.prims,
		BoxParams::MakeSolid(rect, Color4f(.6f, .6f, .6f, 1.f), _worldTransform));
}

void OGUI::VisualElement::DrawBorderPrimitive(PrimitiveDraw::DrawContext & Ctx)
{}

void OGUI::VisualElement::ApplyClipping(PrimitiveDraw::DrawContext & Ctx)
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
}

OGUI::VisualElement::~VisualElement()
{
	if (_ygnode)
	{
		YGNodeFree(_ygnode);
	}
}

void OGUI::VisualElement::DrawPrimitive(PrimitiveDraw::DrawContext& Ctx)
{
	DrawBackgroundPrimitive(Ctx);
	DrawBorderPrimitive(Ctx);
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
	//Context.Get().BroadcastDirtyEvent(this, reason);
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

void OGUI::VisualElement::SetSharedStyle(Style* style)
{
	if (style == _sharedStyle)
		return;
	_sharedStyle = style;
	_style = *style;
	if (_inlineSheet)
		_style.ApplyProperties(_inlineSheet->Storage, _inlineRule.properties);
	style->ApplyProperties(_procedureSheet, _procedureRule.properties);

	_inheritedStylesHash = _style.GetInheritedHash();
	SyncYogaStyle();
}

void OGUI::VisualElement::CalculateLayout()
{
	YGNodeCalculateLayout(_ygnode,YGUndefined, YGUndefined, YGNodeStyleGetDirection(_ygnode));
	YGNodeSetHasNewLayout(_ygnode, false);
}

OGUI::float4x4 make_transform(OGUI::Vector2f pos2d, float rot2d, OGUI::Vector2f scale2d)
{
	using namespace OGUI;
	Vector3f pos{pos2d.X, pos2d.Y, 0};
	Quaternion rot = math::quaternion_from_axis(Vector3f{0.f,0.f,1.f}, rot2d);
	Vector3f scale{scale2d.X, scale2d.Y, 1};
	return math::make_transform(pos, scale, rot);
}

void OGUI::VisualElement::UpdateWorldTransform()
{
	using namespace math;
	auto layout = GetLayout();
	auto parent = GetHierachyParent();
	auto localMat = ::make_transform(layout.min + _renderPosition, _renderRotation, _renderScale);
	if (parent)
		_worldTransform = math::multiply(parent->_worldTransform, localMat);
	else
		_worldTransform = localMat;
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
		LB,
		WH,
	};
}

void OGUI::VisualElement::SyncYogaStyle()
{
	if (!_ygnode)
		return;
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
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeBottom, _style.buttom);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeLeft, _style.marginLeft);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeTop, _style.marginTop);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeRight, _style.marginRight);
	SetYGEdgeAuto(YGNodeStyleSetMargin, YGEdgeBottom, _style.marginButtom);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeLeft, _style.paddingLeft);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeTop, _style.paddingTop);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeRight, _style.paddingRight);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeBottom, _style.paddingButtom);
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

void OGUI::VisualElement::SetPseudoMask(uint32_t mask)
{
	if (_pseudoMask != mask)
	{
		_pseudoMask = mask;
		if ((mask & _triggerPseudoMask) != 0 || (mask & ~_dependencyPseudoMask) != 0)
			MarkDirty(DirtyReason::StyleSheet);
	}
}

bool OGUI::VisualElement::ContainClass(std::string_view cls)
{
	return std::find(_styleClasses.begin(), _styleClasses.end(), cls) != _styleClasses.end();
}