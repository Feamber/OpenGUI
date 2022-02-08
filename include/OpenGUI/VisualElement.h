#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <algorithm>
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/Transform.h"
#include "YGValue.h"
#include "event/event.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Event/EventHandler.h"
#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Bind/Bind.h"

namespace OGUI reflect
{
	using namespace ostr::literal;
	struct OGUI_API PrimDrawContext;

	struct Matrix4x4f{};

	enum class LayoutType
	{
		None,
		Flex,
		Inline
	};

	class OGUI_API reflect attr("rtti":true)
	VisualElement : public Bindable
	{
	public:
		VisualElement();
		virtual ~VisualElement();
		VisualElement* GetParent();
		VisualElement* GetHierachyParent();
		bool IsSibling(VisualElement*);
		bool IsParent(VisualElement*);
		const std::vector<StyleSheet*>& GetStyleSheets() { return _styleSheets; }

		virtual bool IsA(ostr::string_view type) { return GetTypeName() == type; }
		virtual ostr::string_view GetTypeName() { return u"VisualElement"; }
		virtual ostr::string_view GetFullTypeName() { return u"OGUI::VisualElement"; }

	public:
#pragma region Rendering
		attr("script":true)
		bool Visible() const;
		TextureInterface* GetBackgroundImage(const struct StyleBackground& bg);
		virtual void DrawPrimitive(PrimDrawContext& Ctx);
		void DrawBackgroundPrimitive(PrimDrawContext& Ctx);
		void DrawBorderPrimitive(PrimDrawContext& Ctx);
		virtual void DrawDebugPrimitive(PrimDrawContext& Ctx);
		Matrix4x4 ApplyClipping();
		bool CheckClip(const Matrix4x4& rect);
		float _opacity = 1.f;
#pragma endregion
		void CreateYogaNode();
		void MarkDirty(DirtyReason reason);
		ostr::string _name;
		attr("script":true)
		const ostr::string& GetName();
		void SetName(ostr::string_view name);
		attr("script":true)
		void SetName(ostr::string name) { SetName(ostr::string_view{name}); }
		static void DestoryTree(VisualElement* element);
		virtual void GetChildren(std::vector<VisualElement*>& children);
		attr("script":true)
		void SetVisibility(bool visible);
		attr("script":true)
		bool IsClippingChildren();
		bool _clipped = false;
		bool _visible = true;
#pragma region Hierachy
		void UpdateRoot(VisualElement* child);
		attr("script":true)
		void PushChild(VisualElement* child);
		attr("script":true)
		void InsertChild(VisualElement* child, int index);
		attr("script":true)
		void RemoveChild(VisualElement* child);
		attr("script":true)
		VisualElement* GetRoot();
		attr("script":true)
		VisualElement* GetLayoutRoot();

		std::vector<VisualElement*> _children;
		
		VisualElement* _root = nullptr;
		VisualElement* _layoutRoot = nullptr;
		VisualElement* _physicalParent = nullptr;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logicalParent = nullptr;
		bool _rerouteEvent;
		template<class F>
		void Traverse(F&& f);
#pragma endregion

#pragma region Transform
		Vector2f _worldPosition;
		mutable ComputedTransform _styleTransform;
		ComputedTransform GetStyleTransform() const;
		float4x4 _worldTransform;
		float4x4 _invTransform;
		void UpdateWorldTransform();
		Rect GetLayout() const;
		Rect GetRect() const;
		Vector2f GetSize() const;
		Rect _inlineLayout;
		Rect _prevLayout;
		//Rect _layout;
#pragma endregion

#pragma region Style
	public:
		LayoutType _layoutType = LayoutType::Flex;
		YGNodeRef _ygnode;
		PseudoStates _triggerPseudoMask = PseudoStates::None;
		PseudoStates _dependencyPseudoMask = PseudoStates::None;
		PseudoStates _pseudoMask = PseudoStates::None;
		bool _scrollable = true; //use this with absolute position

		//TODO: should we merge these two
		std::unique_ptr<InlineStyle> _inlineStyle;
		std::unique_ptr<InlineStyle> _procedureStyle;
		std::shared_ptr<class AsyncRenderTexture> backgroundImageResource;
		MaterialHandle backgroundMaterial = nullptr;
		
		ostr::string backgroundImageUrl;
		ostr::string backgroundMaterialUrl;

		bool _depended = false;
		bool _selectorDirty = true;
		bool _styleDirty = false;
		bool _transformDirty = false;
		mutable bool _styleTransformDirty = true;
		bool _layoutDirty = false;
		ComputedStyle _style;
		ComputedStyle _preAnimatedStyle;
		std::vector<StyleSheet*> _styleSheets;
		std::vector<ostr::string> _styleClasses;

		attr("script":true)
		void AddStyleClass(ostr::string_view styleClass);
		attr("script":true)
		void RemoveStyleClass(ostr::string_view styleClass);
		void MarkTransformDirty();
		void MarkStyleTransformDirty();
		virtual void MarkLayoutDirty(bool visibility);
		void NotifyLayoutDirty(bool visiblity);
		attr("script":true)
		void SetPseudoClass(PseudoStates state, bool b);
		void InitInlineStyle(std::string_view str);
		void CalculateLayout(float width = YGUndefined, float height = YGUndefined);
		void SyncYogaStyle();
		virtual void UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss);
		RestyleDamage ApplyProcedureStyle();
		using StyleOverridingFunc = std::function<RestyleDamage()>;
		std::vector<StyleOverridingFunc> _styleOverriding;
		bool ContainClass(ostr::string_view c);
		void _ResetStyles();
		void ResetStyles();
#pragma endregion

#pragma region Animation
	public:
		std::vector<ComputedAnim> _anims;
		std::vector<ComputedAnim> _procedureAnims;
		bool _prevEvaluating = false;
		bool PlayAnimation(const AnimStyle& style);
		void SetAnimationTime(ostr::string_view name, float time);
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
		EventBag _eventBag;
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
		ostr::string navExplicitUp = u"";
		ostr::string navExplicitDown = u"";
		ostr::string navExplicitLeft = u"";
		ostr::string navExplicitRight = u"";

		void RegisterFocusedEvent();

		bool _OnGotKeyboardFocus(struct GotKeyboardFocusEvent& event);
		bool _OnLostKeyboardFocus(struct LostKeyboardFocusEvent& event);

		bool _OnGotFocus(struct GotFocusEvent& event);
		bool _OnLostFocus(struct LostFocusEvent& event);

		bool _OnPointerDown(struct PointerDownEvent& event);
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
		virtual void UpdateScrollSize();
		bool CanScroll() const;
		bool IsScrollingX() const;
		bool IsScrollingY() const;
		bool IsScrolling() const;
		float GetScrollingAxisX() const;
		float GetScrollingAxisY() const;
		bool ScrollOnRow() const;
		void AddScroll(Vector2f delta);
		virtual void SetScroll(Vector2f offset);
		void ScrollIntoView(VisualElement* child);
		bool _scrollSizeDirty = true;
		Vector2f GetScrollPos();
        Vector2f _scrollOffset;
		Vector2f _scrollMin;
		Vector2f _scrollMax;
#pragma endregion 

#pragma region Xml
		std::map<ostr::string, VisualElement*> _slots;
		bool _isXmlRoot = false;
#pragma endregion 

#pragma region XmlFilter
		std::set<Name> _xmlFilters;
		std::map<Name, Name> _localXmlFiltersMap;
		std::set<Name> _localXmlFiltersCache;

		attr("script":true)
		void SetXmlFilter(const char* key, const char* filterTag);
		attr("script":true)
		void CleanXmlFilter(const char* key);
		void UpdataXmlFilterCache();
		bool HasFilterTag(const char* filterTag) const;
		bool HasFilterTag(Name filterTag) const;
		bool HasFilterTag(VisualElement* element, const Name& filterTag) const;
#pragma endregion 
	};

	attr("script":true)
	OGUI_API void BindTree(VisualElement* element, Bindable& bindable);
}


#include "VisualElement.inl"