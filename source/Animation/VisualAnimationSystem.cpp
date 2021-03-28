#include "OpenGUI/Animation/VisualAnimationSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Utilities/ipair.hpp"

template <typename T, typename Iter>
void remove_indices(std::vector<T>& v, Iter begin, Iter end)
// requires std::is_convertible_v<std::iterator_traits<Iter>::value_type, std::size_t>
{
	auto rm_iter = begin;
	std::size_t current_index = 0;

	const auto pred = [&](const T&)
	{
		// any more to remove?
		if (rm_iter == end) { return false; }
		// is this one specified?
		if (*rm_iter == current_index++) { return ++rm_iter, true; }
		return false;
	};

	v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());
}

void OGUI::VisualAnimationSystem::Traverse(VisualElement* element)
{
	std::vector<int> toStop;
	for (auto&& [i, anim] : ipair(element->_animStyles))
	{
		auto& ctx = element->_animContext[i];
		float maxTime = anim.animDuration * anim.animIterCount + anim.animDelay;
		if (anim.animPlayState != EAnimPlayState::Paused)
		{
			if(ctx.Reversed)
				ctx.time = std::max(0.f, ctx.time - cachedCtx->_deltaTime);
			else
				ctx.time = std::min(cachedCtx->_deltaTime, maxTime);
		}
		if (ctx.Yielding)
		{
			bool shouldStop = false;
			if (!ctx.Reversed && anim.animIterCount > 0 && ctx.time >= maxTime)
				shouldStop = true;
			else if(ctx.Reversed && ctx.time <= anim.animDelay)
				shouldStop = true;
			if (shouldStop)
				toStop.emplace_back(i);
		}
	}
	remove_indices(element->_animStyles, toStop.begin(), toStop.end());
	remove_indices(element->_animContext, toStop.begin(), toStop.end());
	element->Traverse([this](VisualElement* next) { Traverse(next); });
}

void OGUI::VisualAnimationSystem::Update(VisualElement* Tree)
{
	cachedCtx = &Context::Get();
	Traverse(Tree);
}
