#pragma once

namespace OGUI
{
    enum class EAnimDirection
	{
		Normal = 1,
		Reverse = 2,
		Alternate = 3,
		AlternateReverse = 4
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
		bool operator==(const AnimTimingFunction& other) { return type == other.type; }
	};

	//when animation is already actived
	enum class EAnimResumeMode
	{
		Reset,
		Resume
	};

	//when rule is deatived
	enum class EAnimYieldMode
	{
		Stop, // stop 
		GoBack, // reverse and stop
		Keep // play until end
	};

	enum class EAnimFillMode
	{
		None,
		Forwards = 1,
		Backwards = 2,
		Both = Forwards | Backwards,
	};
}