#pragma once
#include "OpenGUI/Configure.h"

namespace OGUI reflect
{
    enum class attr("style-enum":true)
	EAnimDirection
	{
		Normal = 1,
		Reverse = 2,
		Alternate = 3,
		AlternateReverse = 4
	};

	enum class attr("style-enum":true)
	EAnimPlayState
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
		bool operator==(const AnimTimingFunction& other) { return type == other.type; }
	};

	//when animation is already actived
	enum class attr("style-enum":true)
	EAnimResumeMode
	{
		Reset,
		Resume
	};

	//when rule is deatived
	enum class attr("style-enum":true)
	EAnimYieldMode
	{
		Stop, // stop 
		GoBack, // reverse and stop
		Keep // play until end
	};

	enum class attr("style-enum":true)
	EAnimFillMode
	{
		None,
		Forwards = 1,
		Backwards = 2,
		Both = Forwards | Backwards,
	};
}