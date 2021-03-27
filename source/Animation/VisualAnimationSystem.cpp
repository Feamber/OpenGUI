#include "OpenGUI/Animation/VisualAnimationSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"

void OGUI::VisualAnimationSystem::Traverse(VisualElement* element)
{
	element->ApplySharedStyle(cachedCtx->_deltaTime);
	element->Traverse([this](VisualElement* next) { Traverse(next); });
}

void OGUI::VisualAnimationSystem::Update(VisualElement* Tree)
{
	cachedCtx = &Context::Get();
	Traverse(Tree);
}
