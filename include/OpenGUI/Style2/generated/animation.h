//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Parse/Common.h"
#include "OpenGUI/Style2/Parse/Anim.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse/Math.h"
#include "OpenGUI/Style2/Lerp/Math.h"
#include "OpenGUI/Style2/AnimTypes.h"
namespace OGUI
{
    using namespace std::literals::string_view_literals;
    struct OGUI_API AnimStyle
    {
        constexpr static size_t hash = OGUI::hash("animation"sv);
        struct Id
        {
            static constexpr size_t animationName = OGUI::hash("animation-name"sv);
            static constexpr size_t animationDuration = OGUI::hash("animation-duration"sv);
            static constexpr size_t animationDelay = OGUI::hash("animation-delay"sv);
            static constexpr size_t animationDirection = OGUI::hash("animation-direction"sv);
            static constexpr size_t animationIterationCount = OGUI::hash("animation-iteration-count"sv);
            static constexpr size_t animationPlayState = OGUI::hash("animation-play-state"sv);
            static constexpr size_t animationTimingFunction = OGUI::hash("animation-timing-function"sv);
            static constexpr size_t animationFillMode = OGUI::hash("animation-fill-mode"sv);
            static constexpr size_t animationYieldMode = OGUI::hash("animation-yield-mode"sv);
            static constexpr size_t animationResumeMode = OGUI::hash("animation-resume-mode"sv);
        };
        std::string animationName;
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
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount);
    };
}