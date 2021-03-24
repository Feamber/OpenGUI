#pragma once
#include <vector>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Interface/InputInterface.h"
#include "OpenGUI/Interface/SystemInterface.h"
#include "OpenGUI/Interface/RenderInterface.h"

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
		static Context& Get();
	};
}