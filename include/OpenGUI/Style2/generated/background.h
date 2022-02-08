//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.h.mako

#pragma once
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"

namespace OGUI
{
    using namespace ostr::literal;
    struct OGUI_API StyleBackground
    {
        constexpr static ostr::string_view name = u"background"_o;
        constexpr static size_t hash = OGUI::hash(name);
        static void SetEntry(size_t e);
        static size_t GetEntry();
        constexpr static bool inherited = false;
        struct Ids
        {
            static constexpr size_t backgroundColor = OGUI::hash(u"background-color"_o);
            static constexpr size_t backgroundImage = OGUI::hash(u"background-image"_o);
            static constexpr size_t backgroundMaterial = OGUI::hash(u"background-material"_o);
        };
        Color4f backgroundColor;
        ostr::string backgroundImage;
        ostr::string backgroundMaterial;
        void Initialize();
        static const StyleBackground& GetDefault();
        static const StyleBackground& Get(const ComputedStyle& style);
        static StyleBackground* TryGet(const ComputedStyle& style);
        static StyleBackground& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static RestyleDamage ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static void SetupParser();
    };
}