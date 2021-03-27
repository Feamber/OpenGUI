#pragma once
#include "OpenGUI/Style/Style.h"

namespace OGUI
{
	Style Lerp(const Style& a, const Style& b, float alpha);


	enum class EAnimDirection
	{
		Normal = 1,
		Reverse = 2,
		Alternate = 4,
	};

	enum class EAnimPlayState
	{
		Running,
		Paused
	};

	struct AnimTimingFunction
	{
		enum
		{
			Linear
		} type;
	};

	inline constexpr AnimTimingFunction AnimLinearFuncion{};

	enum class EAnimFillMode
	{
		None,
		Forwards,
		Backwords,
		Both,
	};

	struct AnimationStyle
	{
		std::vector<std::string> animName;
		std::vector<float> animDuration;
		std::vector<float> animDelay;
		std::vector<EAnimDirection> animDirections;
		std::vector<float> animIterCount;
		std::vector<EAnimPlayState> animPlayState;
		std::vector<AnimTimingFunction> animTimingFunction;
		std::vector<EAnimFillMode> animFillMode;
	};
}