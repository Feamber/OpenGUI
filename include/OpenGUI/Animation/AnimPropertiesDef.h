//		 name,			type,					id,								initial
#define ANIMPROP(F) \
F(animName,				std::string,			"animation-name",				""							) \
F(animDuration,			float,					"animation-duration",			1.f							) \
F(animDelay,			float,					"animation-delay",				0.f							) \
F(animDirections,		EAnimDirection,			"animation-direction",			EAnimDirection::Normal		) \
F(animIterCount,		float,					"animation-iteration-count",	1.f							) \
F(animPlayState,		EAnimPlayState,			"animation-play-state",			EAnimPlayState::Running		) \
F(animTimingFunction,	AnimTimingFunction,		"animation-timing-function",	AnimLinearFuncion			) \
F(animFillMode,			EAnimFillMode,			"animation-fill-mode",			EAnimFillMode::Forwards		) \
F(animYieldMode,		EAnimYieldMode,			"animation-yield-mode",			EAnimYieldMode::Goback		) \
F(animResumeMode,		EAnimResumeMode,		"animation-resume-mode",		EAnimResumeMode::Resume		) 