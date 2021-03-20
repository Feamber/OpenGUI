#pragma once

namespace OGUI
{
	class VisualElement;
	struct VisualSystemBase
	{
		virtual void Update(VisualElement* Tree) = 0;
	};
}

