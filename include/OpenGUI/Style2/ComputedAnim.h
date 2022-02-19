#pragma once
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Style2/generated/transition.h"

namespace OGUI
{
	OGUI_API float ApplyTimingFunction(AnimTimingFunction function, float percentage);

    struct OGUI_API ComputedTransition
    {
        float time = 0.f;
        TransitionStyle style;
    };

    struct OGUI_API ComputedAnim
    {
		float time = 0.f;
		bool goingback = false;
		bool yielding = false;
		bool evaluating = true;

        void SetTime(float t)
        {
            time = t;
            evaluating = true;
        }

        AnimStyle style;
        StyleSheet* sheet;
        struct Frame
        {
            //TODO: animation timing function
            float percentage;
		    VariantHandle value;
        };
        struct Track
        {
            size_t id;
            std::vector<Frame> frames;
        };
        std::vector<Track> tracks;

        bool Init(const gsl::span<StyleSheet*>& sheets);
        bool GetPercentage(float& percent);
        RestyleDamage Apply(ComputedStyle& style, const StyleMasks& override);
    };
}