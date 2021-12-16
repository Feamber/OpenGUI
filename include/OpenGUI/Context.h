#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Core/OName.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/VisualStyleSystem.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/FocusEvent.h"
#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>
#include <set>

namespace godot
{
	class TextServer;
}

namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	};

	class VisualElement;
	class VisualWindow;
	class IOThread;
	class Context;
	class RenderTextureManager;

	class WindowContext
	{
		friend class Context;
	public:
		WindowContext(WindowHandle window);
		~WindowContext();

		inline float GetWidth() const { return window->GetWidth(); }
		inline float GetHeight() const { return window->GetHeight(); }
		inline WindowHandle GetWindowHandle() const { return window; }
		inline VisualWindow* GetWindowUI() const { return ui; }

		std::shared_ptr<PrimitiveDraw::DrawContext> currentDrawCtx;
		bool _cssCacheInvalidated = true;
	protected:
		WindowHandle window;
		VisualWindow* ui;
	};

	class OGUI_API Context
	{
	public:
		Context();
		~Context();

		std::set<VisualElement*> _allElementHandle;
		inline bool IsElementValid(VisualElement*) const;

#pragma region FocusNavigation
		std::vector<EKeyCode> keyNavigation_Up {EKeyCode::W, EKeyCode::Up};
		std::vector<EKeyCode> keyNavigation_Down {EKeyCode::S, EKeyCode::Down};
		std::vector<EKeyCode> keyNavigation_Left {EKeyCode::A, EKeyCode::Left};
		std::vector<EKeyCode> keyNavigation_Right {EKeyCode::D, EKeyCode::Right};

		bool OnKeyNavigation(VisualElement* element, ENavDirection direction);
#pragma endregion

#pragma region FocusController
		VisualWindow* _currentActivateWindow = nullptr;
		VisualElement* _keyboardFocused = nullptr;
		
		//可能激活失败，比如：在弹出的消息窗口没关闭时没法操作其他窗口
		bool ActivateWindow(VisualWindow* newWindow);
		bool SetFocus(VisualElement* element, FocusChangeCause cause = FocusChangeCause::UserActions, std::string describe = "");
#pragma endregion

#pragma region XmlFilter
		std::map<Name, Name> _globalXmlFiltersMap;
		std::set<Name> _globalXmlFiltersCache;

		void SetXmlFilter_Global(const char* key, const char* filterTag);
		void CleanXmlFilter_Global(const char* key);
		void UpdataXmlFilterCache_Global();
		bool HasFilterTag_Global(const char* filterTag) const;
		bool HasFilterTag_Global(Name filterTag) const;

		std::map<VisualElement*, std::map<Name, Name>> _localXmlFiltersMap;
		std::map<VisualElement*, std::set<Name>> _localXmlFiltersCache;
		void SetXmlFilter_Local(VisualElement* element, const char* key, const char* filterTag);
		void CleanXmlFilter_Local(VisualElement* element, const char* key);
		void UpdataXmlFilterCache_Local();
		bool HasFilterTag_Local(VisualElement* element, const char* filterTag) const;
		bool HasFilterTag_Local(VisualElement* element, const Name& filterTag) const;

		bool UpdataFilter(VisualElement* element, std::map<Name, int>& localXmlFilters);
		void RecursionUpdataFilter(VisualElement* element, std::map<Name, int>& localXmlFilters);
#pragma endregion

		//Systems
		VisualStyleSystem styleSystem;

		//Global States
		float _deltaTime = 0.f;
		bool _layoutDirty = true;
	
		//APIs
		WindowContext& Create(const WindowHandle window);
		void Remove(const WindowHandle window);
		void Update(const WindowHandle window, float dt);
		void PreparePrimitives(const WindowHandle window);
		void Render(const WindowHandle window);
		void MarkDirty(VisualElement* element, DirtyReason reason);
		void InvalidateCssCache();

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(const WindowHandle window, EMouseKey button, float x, float y);
		bool OnMouseUp(const WindowHandle window, EMouseKey button, float x, float y);
		bool OnMouseDoubleClick(const WindowHandle window, EMouseKey button, float x, float y);
		bool OnMouseMove(const OGUI::WindowHandle window, float x, float y, float relativeMotionX, float relativeMotionY);
		bool OnMouseMoveHP(const WindowHandle window, bool relative, float x, float y);
		bool OnMouseWheel(const WindowHandle window, float delta);
		bool OnMouseEnter(const WindowHandle window);
		bool OnMouseLeave(const WindowHandle window);

		void UpdateHover(VisualElement* element);
		VisualElement* PickElement(const WindowHandle window, Vector2f point);
		void CapturePointer(int id, VisualElement* element);
		void ReleasePointer(int id);

		bool OnKeyDown(const WindowHandle window, EKeyCode keyCode);
		bool OnKeyUp(const WindowHandle window, EKeyCode keyCode);

		void OnActivateWindow(const WindowHandle window);
			
		static Context& Get();
		WindowHandle _windowUnderCursor = nullptr;
		VisualElement* _elementUnderCursor = nullptr;
		VisualElement* _elementCapturingCursor = nullptr;
	public:
		std::unique_ptr<godot::TextServer> _textServer;
		std::unique_ptr<RenderTextureManager> textureManager;
		//Hooks
		std::unique_ptr<InputInterface>  inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		std::unique_ptr<LogInterface> logImpl;
		std::unique_ptr<RenderInterface> renderImpl;
		
		std::unique_ptr<FileInterface>   fileImpl;
		std::unique_ptr<BitmapParserInterface>   bmParserImpl;
		//Components
		std::unique_ptr<IOThread>        ioThread;
		std::vector<std::unique_ptr<WindowContext>>   windowContexts; // Update Per Frame
		godot::TextServer* GetTextServer();
		//States
		bool initialized = false;
		int pointerDownCount = 0;
	protected:
		WindowContext& GetWindowContext(const WindowHandle window);
		WindowContext& GetOrRegisterWindowContext(const WindowHandle window);
	};
}