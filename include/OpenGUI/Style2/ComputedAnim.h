#pragma once
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/generated/animation.h"

namespace OGUI
{
    struct ComputedAnim
    {
		float time = 0.f;
		bool goingback = false;
		bool yielding = false;
		bool evaluating = true;

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
        void Apply(ComputedStyle& style);
    };
}