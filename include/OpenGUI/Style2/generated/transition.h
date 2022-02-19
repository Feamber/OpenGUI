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
    struct OGUI_API TransitionStyle
    {
        struct Ids
        {
            static constexpr size_t transitionProperty = OGUI::hash(u"transition-property"_o);
            static constexpr size_t transitionDuration = OGUI::hash(u"transition-duration"_o);
            static constexpr size_t transitionDelay = OGUI::hash(u"transition-delay"_o);
            static constexpr size_t transitionTimingFunction = OGUI::hash(u"transition-timing-function"_o);
        };
        size_t transitionProperty;
        float transitionDuration;
        float transitionDelay;
        AnimTimingFunction transitionTimingFunction;
        void Initialize();
        static void ApplyProperties(std::vector<TransitionStyle>& group, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
        void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props);
		void ResolveReference(const gsl::span<StyleSheet*>& sheets);
        static void SetupParser();
    };
}