//		 name,					index,	type,					id,								initial
ANIMPROP(animName,				0,		std::string,			"animation-name",				"")
ANIMPROP(animDuration,			1,		float,					"animation-duration",			1.f)
ANIMPROP(animDelay,				2,		float,					"animation-delay",				0.f)
ANIMPROP(animDirections,		3,		EAnimDirection,			"animation-direction",			EAnimDirection::Normal)
ANIMPROP(animIterCount,			4,		float,					"animation-iteration-count",	1.f)
ANIMPROP(animPlayState,			5,		EAnimPlayState,			"animation-play-state",			EAnimPlayState::Running)
ANIMPROP(animTimingFunction,	6,		AnimTimingFunction,		"animation-timing-function",	AnimLinearFuncion)
ANIMPROP(animFillMode,			7,		EAnimFillMode,			"animation-fill-mode",			EAnimFillMode::Forwards)
ANIMPROP(animYieldMode,			8,		EAnimYieldMode,			"animation-yield-mode",			EAnimYieldMode::Goback)
ANIMPROP(animResumeMode,		9,		EAnimResumeMode,		"animation-resume-mode",		EAnimResumeMode::Resume)
#undef ANIMPROP