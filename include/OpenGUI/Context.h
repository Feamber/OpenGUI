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

	using WindowHandle = int;
	struct Context
	{
		//Initialize
		void Initialize(InputInterface*,SystemInterface*,RenderInterface*,FileInterface*);

		//Windows
		std::shared_ptr<VisualWindow> desktops;
		std::shared_ptr<VisualWindow> dialogs;
		std::weak_ptr<VisualElement> currentFocus;

		//Systems
		VisualStyleSystem styleSystem;
		VisualAnimationSystem animSystem;

		//Global States
		float _deltaTime;
	
		//APIs
		void Update(WindowHandle window, float dt);
		void Render(WindowHandle window);

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseUp(WindowHandle window, EMouseKey button, int32 x, int32 y);
		bool OnMouseDoubleClick(WindowHandle window, EMouseKey button, int32 x, int32 y);
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
		std::unique_ptr<IOThread>        ioThread;
		bool initialized = false;
	};
}