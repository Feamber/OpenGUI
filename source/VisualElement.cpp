#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/PrimitiveDraw.h"

void OGUI::VisualElement::DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx)
{}

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
	YGNodeCalculateLayout(_ygnode, YGUndefined, YGUndefined, YGNodeStyleGetDirection(_ygnode));
	YGNodeSetHasNewLayout(_ygnode, false);
}

OGUI::Rect OGUI::VisualElement::GetLayout()
{
	return
	{
		{YGNodeLayoutGetLeft(_ygnode), YGNodeLayoutGetBottom(_ygnode)},
		{YGNodeLayoutGetRight(_ygnode), YGNodeLayoutGetTop(_ygnode)},
	};
}

void OGUI::VisualElement::SyncYogaStyle()
{
	YGNodeStyleSetFlex(_ygnode, _style.flex);
	YGNodeStyleSetFlexGrow(_ygnode, _style.flexGrow);
	YGNodeStyleSetFlexShrink(_ygnode, _style.flexShrink);
	YGNodeStyleSetFlexBasis(_ygnode, _style.flexBasis);
#define SetYGEdge(function, edge, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, edge, v.value); \
	else \
		function(_ygnode, edge, v.value)
#define SetYGValue(function, v) \
	if (v.unit == YGUnitPercent) \
		function##Percent(_ygnode, v.value); \
	else \
		function(_ygnode, v.value)

	SetYGEdge(YGNodeStyleSetPosition, YGEdgeLeft, _style.left);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeTop, _style.top);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeRight, _style.right);
	SetYGEdge(YGNodeStyleSetPosition, YGEdgeBottom, _style.buttom);
	SetYGEdge(YGNodeStyleSetMargin, YGEdgeLeft, _style.marginLeft);
	SetYGEdge(YGNodeStyleSetMargin, YGEdgeTop, _style.marginTop);
	SetYGEdge(YGNodeStyleSetMargin, YGEdgeRight, _style.marginRight);
	SetYGEdge(YGNodeStyleSetMargin, YGEdgeBottom, _style.marginButtom);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeLeft, _style.paddingLeft);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeTop, _style.paddingTop);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeRight, _style.paddingRight);
	SetYGEdge(YGNodeStyleSetPadding, YGEdgeBottom, _style.paddingButtom);
	YGNodeStyleSetBorder(_ygnode, YGEdgeLeft, _style.borderLeftWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeTop, _style.borderTopWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeRight, _style.borderRightWidth);
	YGNodeStyleSetBorder(_ygnode, YGEdgeBottom, _style.borderBottomWidth);

	SetYGValue(YGNodeStyleSetWidth, _style.width);
	SetYGValue(YGNodeStyleSetHeight, _style.height);
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