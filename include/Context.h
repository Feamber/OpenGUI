#pragma once
#include <vector>


namespace OGUI
{
	class VisualSystem;
	class VisualElement;
	class Context
	{
		std::vector<VisualElement*> Desktops;
		std::vector<VisualElement*> Dialogs;
		std::vector<VisualSystem*> Systems;
	};
}