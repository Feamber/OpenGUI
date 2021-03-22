#pragma once
#include <vector>
#include <string>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/Style.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlAttributeDescription.h"
#include "OpenGUI/Xml/XmlChildElementDescription.h"


namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	}

	enum class DirtyReason : int
	{
		// Some data was bound
		Bindings = 1 << 0,
		// persistent data ready
		ViewData = 1 << 1,
		// changes to hierarchy
		Hierarchy = 1 << 2,
		// changes to properties that may have an impact on layout
		Layout = 1 << 3,
		// changes to StyleSheet, USS class
		StyleSheet = 1 << 4,
		// changes to styles, colors and other render properties
		Styles = 1 << 5,
		Overflow = 1 << 6,
		BorderRadius = 1 << 7,
		BorderWidth = 1 << 8,
		// changes that may impact the world transform (e.g. laid out position, local transform)
		Transform = 1 << 9,
		// changes to the size of the element after layout has been performed, without taking the local transform into account
		Size = 1 << 10,
		// The visuals of the element have changed
		Repaint = 1 << 11,
		// The opacity of the element have changed
		Opacity = 1 << 12,
	};

	struct Matrix4x4f{};

	struct StyleSheet;

	class VisualElement
	{
	public:
		VisualElement();
		virtual ~VisualElement();
		virtual void DrawPrimitive(PrimitiveDraw::DrawContext& Ctx);
		VisualElement* GetParent();
		VisualElement* GetHierachyParent();
		const std::vector<StyleSheet*>& GetStyleSheets() { return _styleSheets; }

		virtual bool IsA(std::string_view type) { return GetTypeName() == type; }
		virtual std::string_view GetTypeName() { return "VisualElement"; }
		virtual std::string_view GetFullTypeName() { return "OGUI::VisualElement"; }

	public:
		void DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void DrawBorderPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		void CreateYogaNode();
		void MarkDirty(DirtyReason reason);
		std::string _name;

#pragma region Xml
	public:
		class Traits : public XmlTraits
		{
#define ATTRS \
			PARENT_CLASS(XmlTraits) \
			ATTR(XmlStringAttributeDescription, name, XmlGenericAttributeNames::name, XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, path, XmlGenericAttributeNames::path, XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, style, "style", XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, class_tag, "class", XmlAttributeUse::Optional)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"
		};

		class Factory : public XmlFactory<VisualElement, Traits> {};
#pragma endregion

#pragma region Hierachy
		void PushChild(VisualElement* child);
		void InsertChild(VisualElement* child, int index);

		std::vector<VisualElement*> _children;
		VisualElement* _physical_parent = nullptr;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logical_parent = nullptr;
#pragma endregion

#pragma region Transform
		Vector2f _localPosition = Vector2f::vector_zero();
		float _localRotation = 0.f;
		Vector2f _localScale = Vector2f::vector_one();
		Vector2f _worldPosition;
		float4x4 _worldTransform;
		void UpdateWorldTransform();
		Rect GetLayout();
		Rect GetRect();
		//Rect _layout;
#pragma endregion

#pragma region Style
	public:
		YGNodeRef _ygnode;
		uint32_t _triggerPseudoMask = 0;
		uint32_t _dependencyPseudoMask = 0;
		uint32_t _pseudoMask = 0;
		int _inheritedStylesHash = 0;
		StyleRule _inlineRule;
		StyleSheet* _inlineSheet = nullptr;
		StyleSheetStorage _procedureSheet;
		StyleRule _procedureRule;

		Style _style;
		Style* _sharedStyle = nullptr;
		std::vector<StyleSheet*> _styleSheets;
		std::vector<std::string_view> _styleClasses;

		void SetPseudoMask(uint32_t mask);
		void CalculateLayout();
		void SetSharedStyle(Style* style);
		void SyncYogaStyle();
		bool ContainClass(std::string_view c);
#pragma endregion

	public:
		template<class F>
		void Traverse(F&& f, int depth = 0);
	};
}


#include "VisualElement.inl"