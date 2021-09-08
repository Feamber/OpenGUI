#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include <array>
#include <cstddef>
#include <string>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Xml/XmlFactory.h"
#include "OpenGUI/Event/EventHandler.h"
#include "OpenGUI/Animation/AnimStyle.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/FocusEvent.h"

namespace OGUI
{
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

	class OGUI_API VisualElement
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
		virtual void DrawDebugPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		void CreateYogaNode();
		void MarkDirty(DirtyReason reason);
		std::string _name;
        std::string _path;
		static void DestoryTree(VisualElement* element);
		virtual void GetChildren(std::vector<VisualElement*>& children);

#pragma region Hierachy
		void PushChild(VisualElement* child);
		void InsertChild(VisualElement* child, int index);
		void RemoveChild(VisualElement* child);
		VisualElement* GetRoot();

		std::vector<VisualElement*> _children;
		
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

		bool _selectorDirty = true;
		bool _styleDirty = false;
		bool _sharedDirty = false;
		bool _transformDirty = false;
		bool _procedureStyleDirty = false;
		Rect _prevLayout;
		Style _style;
		Style _preAnimatedStyle;
		struct CachedStyle* _sharedStyle = nullptr;
		std::vector<StyleSheet*> _styleSheets;
		std::vector<std::string> _styleClasses;

		void SetPseudoClass(PseudoStates state, bool b);
		void InitInlineStyle(std::string_view str);
		void CalculateLayout();
		void SyncYogaStyle();
		bool ContainClass(std::string_view c);
		void _ResetStyles();
		void ResetStyles();
#pragma endregion

#pragma region Animation
	public:
		std::vector<AnimationStyle> _animStyles;
		std::vector<AnimRunContext> _animContext;
		bool _prevEvaluating = false;
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

#pragma region Event
	public:
		EventHandler _eventHandler;

		bool Intersect(Vector2f point);
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

		// OnPre @return 返回true将阻止KeyboardFocus改变
		virtual bool OnPreGotKeyboardFocus(struct PreGotKeyboardFocusEvent& event) { return false; };
		virtual bool OnPreLostKeyboardFocus(struct PreLostKeyboardFocusEvent& event) { return false; };
		virtual void OnGotKeyboardFocus(struct GotKeyboardFocusEvent& event) {};
		virtual void OnLostKeyboardFocus(struct LostKeyboardFocusEvent& event) {};

		// OnPre @return 返回true将阻止Focus改变
		virtual bool OnPreGotFocus(struct PreGotFocusEvent& event) { return false; };
		virtual bool OnPreLostFocus(struct PreLostFocusEvent& event) { return false; };
		virtual void OnGotFocus(struct GotFocusEvent& event) {};
		virtual void OnLostFocus(struct LostFocusEvent& event) {};

		bool OnPreGotKeyboardFocus_Internal(struct PreGotKeyboardFocusEvent& event) { return OnPreGotKeyboardFocus(event); };
		bool OnPreLostKeyboardFocus_Internal(struct PreLostKeyboardFocusEvent& event) { return OnPreLostKeyboardFocus(event); };
		bool OnGotKeyboardFocus_Internal(struct GotKeyboardFocusEvent& event);
		bool OnLostKeyboardFocus_Internal(struct LostKeyboardFocusEvent& event);

		bool OnPreGotFocus_Internal(struct PreGotFocusEvent& event) { return OnPreGotFocus(event); };
		bool OnPreLostFocus_Internal(struct PreLostFocusEvent& event) { return OnPreLostFocus(event); };
		bool OnGotFocus_Internal(struct GotFocusEvent& event);
		bool OnLostFocus_Internal(struct LostFocusEvent& event);
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

#pragma region DataBind
	public:
		std::vector<std::shared_ptr<int>> _allListenHandle;
#pragma endregion

	};
}


#include "VisualElement.inl"