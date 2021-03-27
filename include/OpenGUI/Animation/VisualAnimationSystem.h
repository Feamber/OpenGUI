#pragma once
#include "OpenGUI/VisualSystem.h"

namespace OGUI
{
	class Context;
	class VisualAnimationSystem : public VisualSystemBase
	{
		void Traverse(VisualElement* element);

	public:
		Context* cachedCtx;
		virtual void Update(VisualElement* Tree);
	};
}