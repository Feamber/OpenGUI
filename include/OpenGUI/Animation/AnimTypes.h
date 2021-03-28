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
	};

	inline constexpr AnimTimingFunction AnimLinearFuncion{};

	enum class EAnimFillMode
	{
		None,
		Forwards = 1,
		Backwords = 2,
		Both = Forwards | Backwords,
	};
}