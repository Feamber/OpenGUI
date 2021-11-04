#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/DataBind/PropertyManager.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Animation/VisualAnimationSystem.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Core/Types.h"
#include <algorithm>
#include <memory>
#include <vector>


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
		WindowContext();
		~WindowContext();

		inline float GetWidth() const { return window->GetWidth(); }
		inline float GetHeight() const { return window->GetHeight(); }
		inline WindowHandle GetWindowHandle() const { return window; }
		inline VisualWindow* GetWindowUI() const { return ui; }

		std::shared_ptr<PrimitiveDraw::DrawContext> currentDrawCtx;
		std::shared_ptr<RenderInterface> renderImpl;
		std::unique_ptr<RenderTextureManager> textureManager;
	protected:
		float X; float Y;
		WindowHandle window;
		VisualWindow* ui;
	};

	class OGUI_API Context
	{
	public:
		Context();
		~Context();

#pragma region FocusNavigation
		std::vector<EKeyCode> keyNavigation_Up {EKeyCode::W, EKeyCode::Up};
		std::vector<EKeyCode> keyNavigation_Down {EKeyCode::S, EKeyCode::Down};
		std::vector<EKeyCode> keyNavigation_Left {EKeyCode::A, EKeyCode::Left};
		std::vector<EKeyCode> keyNavigation_Right {EKeyCode::D, EKeyCode::Right};

		void OnKeyNavigation(VisualElement* element, ENavDirection direction);
#pragma endregion

#pragma region FocusController
		VisualWindow* _currentActivateWindow = nullptr;
		VisualElement* _keyboardFocused = nullptr;
		
		//可能激活失败，比如：在弹出的消息窗口没关闭时没法操作其他窗口
		bool ActivateWindow(VisualWindow* newWindow);
		bool SetFocus(VisualElement* element, FocusChangeCause cause = UserActions, std::string describe = "");
#pragma endregion

		//Systems
		VisualStyleSystem styleSystem;
		VisualAnimationSystem animSystem;

		//Global States
		float _deltaTime = 0.f;
		bool _layoutDirty = true;
	
		//APIs
		WindowContext& Create(const WindowHandle window);
		void Remove(const WindowHandle window);
		void Update(const WindowHandle window, float dt);
		void Render(const WindowHandle window);
		void MarkDirty(VisualElement* element, DirtyReason reason);

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(const WindowHandle window, float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y);
		bool OnMouseUp(const WindowHandle window, float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y);
		bool OnMouseDoubleClick(const WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseMove(const WindowHandle window, bool relative, int32 x, int32 y);
		bool OnMouseMoveHP(const WindowHandle window, bool relative, float x, float y);
		bool OnMouseWheel(const WindowHandle window, float delta);

		bool OnKeyDown(const WindowHandle window, EKeyCode keyCode);
		bool OnKeyUp(const WindowHandle window, EKeyCode keyCode);

		void OnActivateWindow(const WindowHandle window);
			
		static Context& Get();
		VisualElement* _elementUnderCursor = nullptr;
	public:
		std::unique_ptr<godot::TextServer> _textServer;
		//Hooks
		std::unique_ptr<InputInterface>  inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		std::unique_ptr<LogInterface> logImpl;
		
		std::unique_ptr<FileInterface>   fileImpl;
		std::unique_ptr<BitmapParserInterface>   bmParserImpl;
		//Components
		std::unique_ptr<IOThread>        ioThread;
		std::vector<std::unique_ptr<WindowContext>>   windowContexts; // Update Per Frame
		std::vector<std::unique_ptr<RenderInterface>>   renderImples; 
		godot::TextServer* GetTextServer();
		//States
		bool initialized = false;
		int pointerDownCount = 0;

		PropertyManager propeManager;
	protected:
		WindowContext& GetWindowContext(const WindowHandle window);
		WindowContext& GetOrRegisterWindowContext(const WindowHandle window);
	};
}