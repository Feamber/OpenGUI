#pragma once
#include <vector>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Event/PointerEvent.h"

namespace OGUI
{
	class VisualWindow;
	struct Context
	{
		//Initialize
		void Initialize(InputInterface*,SystemInterface*,RenderInterface*,FileInterface*);

		//Windows
		std::vector<std::shared_ptr<VisualWindow>> desktops;
		std::vector<std::shared_ptr<VisualWindow>> dialogs;

		//Systems
		VisualStyleSystem styleSystem;

		//Global States
		float _deltaTime;
	
		//APIs
		void Update(int window, float dt);
		void Render(int window);

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(int window, EMouseKey button, int32 x, int32 y);
		bool OnMouseUp(int window, EMouseKey button, int32 x, int32 y);
		bool OnMouseDoubleClick(int window, EMouseKey button, int32 x, int32 y);
		bool OnMouseMove(bool relative, int32 x, int32 y);
		bool OnMouseMoveHP(bool relative, float x, float y);
		bool OnMouseWheel(float delta);

		static Context& Get();
		std::weak_ptr<VisualElement> _elementUnderCursor;

	protected:
		//Hooks
		std::unique_ptr<InputInterface>  inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		std::unique_ptr<RenderInterface> renderImpl;
		std::unique_ptr<FileInterface>   fileImpl;
		bool initialized = false;
	};
}