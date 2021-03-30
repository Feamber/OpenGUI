#pragma once
#include <vector>
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Animation/VisualAnimationSystem.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/PointerEvent.h"


namespace OGUI
{
	class VisualElement;
	class VisualWindow;
	class IOThread;
	class Context;
	class RenderTextureManager;

	using WindowHandle = void*;
	class WindowContext
	{
		friend class Context;
	public:
		inline float GetWindowX() const { return X; }
		inline float GetWindowY() const { return Y; }
		inline WindowHandle GetWindowHandle() const { return window; }
	protected:
		float X; float Y;
		WindowHandle window;
	};

	class Context
	{
	public:
		Context();
		//Initialize
		void Initialize(InputInterface*,SystemInterface*,RenderInterface*,FileInterface*,BitmapParserInterface*);

		//Windows
		std::shared_ptr<VisualWindow> desktops;
		std::shared_ptr<VisualWindow> dialogs;
		std::weak_ptr<VisualElement> currentFocus;

		//Systems
		VisualStyleSystem styleSystem;
		VisualAnimationSystem animSystem;

		//Global States
		float _deltaTime = 0.f;
		bool _layoutDirty = true;
	
		//APIs
		void Update(const WindowHandle window, float dt);
		void Render(const WindowHandle window);

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(const WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseUp(const WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseDoubleClick(const WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseMove(bool relative, int32 x, int32 y);
		bool OnMouseMoveHP(bool relative, float x, float y);
		bool OnMouseWheel(float delta);
			
		static Context& Get();
		std::weak_ptr<VisualElement> _elementUnderCursor;
	public:
		//Hooks
		std::unique_ptr<InputInterface>  inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		std::unique_ptr<RenderInterface> renderImpl;
		std::unique_ptr<FileInterface>   fileImpl;
		std::unique_ptr<BitmapParserInterface>   bmParserImpl;
		//Components
		std::unique_ptr<IOThread>        ioThread;
		std::vector<WindowContext>       windowContexts; // Update Per Frame
		std::unique_ptr<RenderTextureManager> textureManager;
		//States
		bool initialized = false;
		int pointerDownCount = 0;
	protected:
		const WindowContext& GetWindowContext(const WindowHandle window) const;
		WindowContext& GetOrRegisterWindowContext(const WindowHandle window);
	};
}