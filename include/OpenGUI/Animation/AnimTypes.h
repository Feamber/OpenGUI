#pragma once

namespace OGUI
{
	enum class EAnimDirection
	{
		Normal = 1,
		Reverse = 2,
		Alternate = 4,
		Default = Normal,
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
}