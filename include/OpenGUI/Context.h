#pragma once
#include <vector>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Interface/InputInterface.h"
#include "OpenGUI/Interface/SystemInterface.h"
#include "OpenGUI/Interface/RenderInterface.h"
#include "OpenGUI/Event/PointerEvent.h"

namespace OGUI
{
	class VisualWindow;
	struct Context
	{
		//Windows
		std::vector<std::shared_ptr<VisualWindow>> desktops;
		std::vector<std::shared_ptr<VisualWindow>> dialogs;

		//Systems
		VisualStyleSystem styleSystem;

		//Hooks
		std::unique_ptr<InputInterface> inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		std::unique_ptr<RenderInterface> renderImpl;

		//Global States
		float _deltaTime;

		//APIs
		void Update(int window, float dt);
		void Render(int window);

		//Message Handling
		//reference : UE4 Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
		bool OnMouseDown(int window, ButtonId button, int32 x, int32 y);
		bool OnMouseUp(int window, ButtonId button, int32 x, int32 y);
		bool OnMouseDoubleClick(int window, ButtonId button, int32 x, int32 y);
		bool OnMouseMove(bool relative, int32 x, int32 y);
		bool OnMouseWheel(float delta);

		static Context& Get();
	};
}