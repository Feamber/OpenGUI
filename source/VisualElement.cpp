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

void OGUI::VisualElement::DrawBackgroundPrimitive(
	PrimitiveDraw::DrawContext& Ctx)
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
			backgroundImageResource = Context::Get().textureManager->Require(_style.backgroundImage);
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
	Context::Get().MarkDirty(this, reason);
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

bool OGUI::VisualElement::Traits::InitAttribute(OGUI::VisualElement &new_element, const XmlElement &asset, CreationContext& context)
{
    if(!XmlTraits::InitAttribute(new_element, asset, context)) return false;

    new_element._name = name.GetValue(asset);
    new_element._path = path.GetValue(asset);
	std::string inlineStyleStr;
	if(style.GetValueString(inlineStyleStr, asset))
		new_element.InitInlineStyle(inlineStyleStr);
	std::split(class_tag.GetValue(asset), new_element._styleClasses, ",");

    auto _slot_name = slot_name.GetValue(asset);
    auto _slot = slot.GetValue(asset);

    if(!_slot_name.empty())
    {
        auto& template_container = context.stack_template.front().template_container;
        template_container.slots[_slot_name] = &new_element;
    }

    if(!_slot.empty())
    {
        auto parent_node = context.stack.front();
        if(parent_node->IsA("TemplateContainer"))
        {
            auto template_container = (TemplateContainer*)parent_node;
            auto find_slots = template_container->slots.find(_slot);
            if(find_slots != template_container->slots.end())
                find_slots->second->PushChild(&new_element);
        }
    }

    return true;
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

bool OGUI::VisualElement::Intersect(Vector2f point)
{
	auto rect = GetRect();
	return rect.IntersectPoint(point);
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
	}
}
