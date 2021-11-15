#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/Selector.h"
#include "event/event.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Event/EventHandler.h"
#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Bind/AttributeBind.h"
#include "OpenGUI/Bind/EventBind.h"

namespace OGUI
{
	using namespace ostr::literal;
	namespace PrimitiveDraw
	{
		struct OGUI_API DrawContext;
	}

	struct Matrix4x4f{};

	struct OGUI_API Interpolation
	{
		float duration;
		float time = 0;
		void reset() { time = 0; }
		void forward(float deltaTime) { time = std::clamp(time + deltaTime, 0.f, duration); }
		void reverse() { time = duration - time; }
		float alpha() { return std::clamp(time / duration, 0.f, 1.f); }
	};

	enum class LayoutType
	{
		None,
		Flex,
		Inline
	};

	class OGUI_API VisualElement : public AttrBag
	{
	public:
		VisualElement();
		virtual ~VisualElement();
		virtual void DrawPrimitive(PrimitiveDraw::DrawContext& Ctx);
		VisualElement* GetParent();
		VisualElement* GetHierachyParent();
		bool IsParent(VisualElement*);
		const std::vector<StyleSheet*>& GetStyleSheets() { return _styleSheets; }

		virtual bool IsA(std::string_view type) { return GetTypeName() == type; }
		virtual std::string_view GetTypeName() { return "VisualElement"; }
		virtual std::string_view GetFullTypeName() { return "OGUI::VisualElement"; }

	public:
		void DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void DrawBorderPrimitive(PrimitiveDraw::DrawContext& Ctx);
		virtual void DrawDebugPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		void CreateYogaNode();
		void MarkDirty(DirtyReason reason);
		std::string _name;
		static void DestoryTree(VisualElement* element);
		virtual void GetChildren(std::vector<VisualElement*>& children);

#pragma region Hierachy
		void UpdateRoot(VisualElement* child);
		void PushChild(VisualElement* child);
		void InsertChild(VisualElement* child, int index);
		void RemoveChild(VisualElement* child);
		VisualElement* GetRoot();
		VisualElement* GetLayoutRoot();

		std::vector<VisualElement*> _children;
		
		VisualElement* _root = nullptr;
		VisualElement* _layoutRoot = nullptr;
		VisualElement* _physical_parent = nullptr;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logical_parent = nullptr;
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
		Vector2f GetSize();
		Rect _inlineLayout;
		//Rect _layout;
#pragma endregion

#pragma region Style
	public:
		LayoutType _layoutType = LayoutType::Flex;
		YGNodeRef _ygnode;
		PseudoStates _triggerPseudoMask = PseudoStates::None;
		PseudoStates _dependencyPseudoMask = PseudoStates::None;
		PseudoStates _pseudoMask = PseudoStates::None;

		//TODO: should we merge these two
		std::unique_ptr<InlineStyle> _inlineStyle;
		std::unique_ptr<InlineStyle> _procedureStyle;
		std::shared_ptr<class AsyncRenderTexture> backgroundImageResource;
		
		std::string backgroundImageUrl;

		bool _selectorDirty = true;
		bool _styleDirty = false;
		bool _transformDirty = false;
		bool _layoutDirty = false;
		Rect _prevLayout;
		ComputedStyle _style;
		ComputedStyle _preAnimatedStyle;
		std::vector<StyleSheet*> _styleSheets;
		std::vector<std::string> _styleClasses;

		virtual void MarkLayoutDirty();
		void SetPseudoClass(PseudoStates state, bool b);
		void InitInlineStyle(std::string_view str);
		void CalculateLayout();
		virtual void SyncYogaStyle();
		bool ContainClass(std::string_view c);
		void _ResetStyles();
		void ResetStyles();
#pragma endregion

#pragma region Animation
	public:
		std::vector<ComputedAnim> _anims;
		std::vector<ComputedAnim> _procedureAnims;
		bool _prevEvaluating = false;
		bool PlayAnimation(const AnimStyle& style);
		void SetAnimationTime(std::string_view name, float time);
#pragma endregion

#pragma region PseudoElement
	public:
		bool _isPseudoElement = false;
		VisualElement* _beforeElement = nullptr;
		VisualElement* _afterElement = nullptr;
		VisualElement* GetBeforePseudoElement();
		void ReleaseBeforePseudoElement();
		VisualElement* GetAfterPseudoElement();
		void ReleaseAfterPseudoElement();
#pragma endregion

#pragma region Bind
	public:
		std::unordered_map<Name, Name> _bindBag;
		EventBind::EventBag _eventBag;
		EventHandler _eventHandler;
		Name* GetEventBind(Name event);
		void Notify(Name prop, bool force = false);
		virtual bool Intersect(Vector2f point);
#pragma endregion

#pragma region Focused
	public:
		bool focusable = false;
		ENavMode navMode = ENavMode::Automatic;

		// 当有效时会覆盖正常的方向性导航
		// 使用的是选择器语法，参考 StyleSelector.h -> QueryFirst()
		std::string navExplicitUp = "";
		std::string navExplicitDown = "";
		std::string navExplicitLeft = "";
		std::string navExplicitRight = "";

		void RegisterFocusedEvent();

		bool _OnGotKeyboardFocus(struct GotKeyboardFocusEvent& event);
		bool _OnLostKeyboardFocus(struct LostKeyboardFocusEvent& event);

		bool _OnGotFocus(struct GotFocusEvent& event);
		bool _OnLostFocus(struct LostFocusEvent& event);
#pragma endregion

#pragma region Hover
	public:
		bool _OnMouseEnter(struct PointerEnterEvent& event);
		bool _OnMouseLeave(struct PointerLeaveEvent& event);
		int hovered = 0;
#pragma endregion

#pragma region FocusScope
	public:
		bool isFocusScope = false;
		// 是否在焦点空间失去焦点后保持 currentFocused
		bool isKeeyScopeFocused = false;
		VisualElement* currentFocused = nullptr;
		ENavCycleMode navCycleMode = ENavCycleMode::None;
		
		// 获取当前焦点空间下的所有可聚焦元素
		std::vector<VisualElement*> GetFocusScopeChildren();
		VisualElement* GetPrevFocusScope();
		static void GetRelativeFocusedPath(VisualElement* element, std::vector<VisualElement*>& out);
#pragma endregion

#pragma region FocusNavigation
		enum 
		{
			NoDebug,
			Null,
			CollisionBox,
			ElementQuad,
		}FocusNavDebugState = Null;
		Rect navDebugRect;
		time_t navDebugLastUpdate = 0;

		VisualElement* FindNextNavTarget(ENavDirection direction);
#pragma endregion

#pragma region Scroll
		bool _OnMouseScroll(struct PointerEnterEvent& event);
        Vector2f _scrollPos;
#pragma endregion
	};
}


#include "VisualElement.inl"