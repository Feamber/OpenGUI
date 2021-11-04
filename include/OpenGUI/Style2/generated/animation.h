//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/AnimTypes.h"
namespace OGUI
{
    struct AnimStyle
    {
        constexpr static size_t hash = 2075082312401384495U;
        //Resolved reference
		StyleSheet* sheet;
		StyleKeyframes* keyframes;
        struct Id
        {
            static constexpr size_t animationName = 3244085226358175677U;
            static constexpr size_t animationDuration = 16528445173470680616U;
            static constexpr size_t animationDelay = 3711333784419848324U;
            static constexpr size_t animationDirection = 15181145096571222690U;
            static constexpr size_t animationIterationCount = 6209108401425347064U;
            static constexpr size_t animationPlayState = 13888980212020061235U;
            static constexpr size_t animationFillMode = 773048627929703309U;
            static constexpr size_t animationYieldMode = 13179971152880940527U;
            static constexpr size_t animationResumeMode = 13493091705672928255U;
        };
        std::string animationName;
        float animationDuration;
        float animationDelay;
        EAnimDirection animationDirection;
        float animationIterationCount;
        AnimTimingFunction animationPlayState;
        EAnimFillMode animationFillMode;
        EAnimYieldMode animationYieldMode;
        EAnimResumeMode animationResumeMode;
        void Initialize();
        static void ApplyProperties(std::vector<AnimStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg, int animCount);
    };
}