#pragma once
#include <vector>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Interface/InputInterface.h"
#include "OpenGUI/Interface/SystemInterface.h"

namespace OGUI
{
	class VisualSystem;
	class VisualElement;
	struct Context
	{
		std::vector<std::shared_ptr<VisualElement>> desktops;
		std::vector<std::shared_ptr<VisualElement>> dialogs;
		VisualStyleSystem styleSystem;
		std::unique_ptr<InputInterface> inputImpl;
		std::unique_ptr<SystemInterface> systemImpl;
		float deltaTime;

		void Update(float dt);
		static Context& Get();
	};
}