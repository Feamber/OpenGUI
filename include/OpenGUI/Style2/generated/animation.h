//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/AnimTypes.h"
namespace OGUI
{
    using namespace ostr::literal;
    struct OGUI_API AnimStyle
    {
        struct Ids
        {
            static constexpr size_t animationName = OGUI::hash(u"animation-name"_o);
            static constexpr size_t animationDuration = OGUI::hash(u"animation-duration"_o);
            static constexpr size_t animationDelay = OGUI::hash(u"animation-delay"_o);
            static constexpr size_t animationDirection = OGUI::hash(u"animation-direction"_o);
            static constexpr size_t animationIterationCount = OGUI::hash(u"animation-iteration-count"_o);
            static constexpr size_t animationPlayState = OGUI::hash(u"animation-play-state"_o);
            static constexpr size_t animationTimingFunction = OGUI::hash(u"animation-timing-function"_o);
            static constexpr size_t animationFillMode = OGUI::hash(u"animation-fill-mode"_o);
            static constexpr size_t animationYieldMode = OGUI::hash(u"animation-yield-mode"_o);
            static constexpr size_t animationResumeMode = OGUI::hash(u"animation-resume-mode"_o);
        };
        ostr::string animationName;
        float animationDuration;
        float animationDelay;
        EAnimDirection animationDirection;
        float animationIterationCount;
        EAnimPlayState animationPlayState;
        AnimTimingFunction animationTimingFunction;
        EAnimFillMode animationFillMode;
        EAnimYieldMode animationYieldMode;
        EAnimResumeMode animationResumeMode;
        void Initialize();
        static void ApplyProperties(std::vector<AnimStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static void SetupParser();
    };
}