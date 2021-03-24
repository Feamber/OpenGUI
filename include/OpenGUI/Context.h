#pragma once
#include <vector>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Interface/InputInterface.h"
#include "OpenGUI/Interface/SystemInterface.h"

namespace OGUI
{
	class VisualWindow;
	struct Context
	{
		std::vector<std::shared_ptr<VisualWindow>> desktops;
		std::vector<std::shared_ptr<VisualWindow>> dialogs;
		VisualStyleSystem styleSystem;
		std::unique_ptr<InputInterface> inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		float deltaTime;

		void Update(int window, float dt);
		void Render(int window);
		static Context& Get();
	};
}