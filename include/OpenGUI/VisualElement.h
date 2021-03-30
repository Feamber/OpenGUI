#pragma once
#include <vector>
#include <string>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Core/Types.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Xml/XmlAttributeDescription.h"
#include "OpenGUI/Xml/XmlChildElementDescription.h"
#include "OpenGUI/Event/EventHandler.h"
#include "Animation/AnimStyle.h"

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

	struct Interpolation
	{
		float duration;
		float time = 0;
		void reset() { time = 0; }
		void forward(float deltaTime) { time = std::clamp(time + deltaTime, 0.f, duration); }
		void reverse() { time = duration - time; }
		float alpha() { return std::clamp(time / duration, 0.f, 1.f); }
	};

	class PseudoElement
	{

	};

	class VisualElement : public std::enable_shared_from_this<VisualElement>
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
        std::string _path;

#pragma region Xml
	public:
		class Traits : public XmlTraits
		{
        public:
#define ATTRS \
			PARENT_CLASS(XmlTraits) \
			ATTR(XmlStringAttributeDescription, name, "", XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, path, "", XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, style, "", XmlAttributeUse::Optional)\
			ATTR(XmlStringAttributeDescription, class_tag, "", XmlAttributeUse::Optional)\
            ATTR(XmlStringAttributeDescription, slot_name, "", XmlAttributeUse::Optional)\
            ATTR(XmlStringAttributeDescription, slot, "", XmlAttributeUse::Optional)
#include "OpenGUI/Xml/GenXmlAttrsDesc.h"

            bool InitAttribute(VisualElement& new_element, const XmlElement& asset, CreationContext& context);
		};

		class Factory : public XmlFactory<VisualElement, Traits>
		{
		public:
            Factory()
            {
                xml_name = "VisualElement";
                xml_namespace = "OGUI";
                xml_qualified_name = xml_namespace + '.' + xml_name;
            }
		};
#pragma endregion

#pragma region Hierachy
		void PushChild(VisualElement* child);
		void InsertChild(VisualElement* child, int index);
		void RemoveChild(VisualElement* child);

		std::vector<std::shared_ptr<VisualElement>> _children;
		
		std::weak_ptr<VisualElement> _physical_parent;
		//There could be some node between logical parent and this widget for layout
		std::weak_ptr<VisualElement> _logical_parent;
		bool _rerouteEvent;
		template<class F>
		void Traverse(F&& f);
#pragma endregion

#pragma region Transform
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

		//TODO: should we merge these two
		std::unique_ptr<InlineStyle> _inlineStyle;
		std::unique_ptr<InlineStyle> _procedureStyle;
		std::shared_ptr<class AsyncRenderTexture> backgroundImageResource;
		
		std::string backgroundImageUrl;

		bool _styleDirty = false;
		bool _transformDirty = false;
		bool _procedureStyleDirty = false;
		Rect _prevLayout;
		Style _style;
		Style _preAnimatedStyle;
		Style* _sharedStyle = nullptr;
		std::vector<StyleSheet*> _styleSheets;
		std::vector<std::string> _styleClasses;

		void InitInlineStyle(std::string_view str);
		void SetPseudoMask(uint32_t mask);
		void CalculateLayout();
		void SyncYogaStyle();
		bool ContainClass(std::string_view c);
#pragma endregion

#pragma region Animation
	public:
		std::vector<AnimationStyle> _animStyles;
		std::vector<AnimRunContext> _animContext;
		bool _prevEvaluating = false;

		void UpdateAnimList();
#pragma endregion

#pragma region Event
	public:
		EventHandler _eventHandler;

		bool Intersect(Vector2f point);
#pragma endregion
	};
}


#include "VisualElement.inl"