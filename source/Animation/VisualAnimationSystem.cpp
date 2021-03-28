#include "OpenGUI/Animation/VisualAnimationSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Utilities/ipair.hpp"

void OGUI::VisualAnimationSystem::Traverse(VisualElement* element)
{
	for (auto&& [i, anim] : ipair(element->_animStyles))
	{
		if (anim.animPlayState != EAnimPlayState::Paused)
			element->_animContext[i].Time += cachedCtx->_deltaTime;
	}
	element->Traverse([this](VisualElement* next) { Traverse(next); });
}

void OGUI::VisualAnimationSystem::Update(VisualElement* Tree)
{
	cachedCtx = &Context::Get();
	Traverse(Tree);
}
