//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.h

#pragma once
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Core/Math.h"
namespace OGUI
{
    struct StylePosition
    {
        constexpr static size_t hash = 5530202806555891530U;
        struct Id
        {
            static constexpr size_t rotate = 12970377039170710697U;
            static constexpr size_t scale = 2092905813020566909U;
            static constexpr size_t translate = 6289005345924523389U;
            static constexpr size_t flexGrow = 13170296350314741562U;
            static constexpr size_t flexShrink = 10365817147375943713U;
            static constexpr size_t flexBasis = 4314523119058039089U;
            static constexpr size_t left = 17155065611775311164U;
            static constexpr size_t top = 8014166908184336511U;
            static constexpr size_t right = 6657230345862557259U;
            static constexpr size_t bottom = 6701519901935332996U;
            static constexpr size_t marginLeft = 7196095289661758432U;
            static constexpr size_t marginTop = 9128238604532076424U;
            static constexpr size_t marginRight = 12311009162633855753U;
            static constexpr size_t marginBottom = 12677486643599222917U;
            static constexpr size_t paddingLeft = 14954591363078587756U;
            static constexpr size_t paddingTop = 13009014907411704783U;
            static constexpr size_t paddingRight = 3739237915405526423U;
            static constexpr size_t paddingBottom = 414457025258337092U;
            static constexpr size_t width = 7212975389898416544U;
            static constexpr size_t height = 10231501292519662331U;
            static constexpr size_t position = 6693393657162939416U;
            static constexpr size_t overflow = 8391640804579568494U;
            static constexpr size_t alignSelf = 6778698307008250384U;
            static constexpr size_t maxWidth = 12833965934808178275U;
            static constexpr size_t maxHeight = 3720051466868152577U;
            static constexpr size_t minWidth = 5537486059816424318U;
            static constexpr size_t minHeight = 14711625092876590978U;
            static constexpr size_t flexDirection = 14640126859953853289U;
            static constexpr size_t alignContent = 9118343503683952186U;
            static constexpr size_t alignItems = 13007453979135775982U;
            static constexpr size_t justifyContent = 10311501824482019454U;
            static constexpr size_t flexWrap = 15725690929436419896U;
            static constexpr size_t flexDisplay = 3933629464269167308U;
            static constexpr size_t margin = 8162270687675663346U;
        };
        float rotate;
        Vector2f scale;
        Vector2f translate;
        float flexGrow;
        float flexShrink;
        YGValue flexBasis;
        YGValue left;
        YGValue top;
        YGValue right;
        YGValue bottom;
        YGValue marginLeft;
        YGValue marginTop;
        YGValue marginRight;
        YGValue marginBottom;
        YGValue paddingLeft;
        YGValue paddingTop;
        YGValue paddingRight;
        YGValue paddingBottom;
        YGValue width;
        YGValue height;
        YGPositionType position;
        YGOverflow overflow;
        YGAlign alignSelf;
        YGValue maxWidth;
        YGValue maxHeight;
        YGValue minWidth;
        YGValue minHeight;
        YGFlexDirection flexDirection;
        YGAlign alignContent;
        YGAlign alignItems;
        YGJustify justifyContent;
        YGWrap flexWrap;
        YGDisplay flexDisplay;
        void Initialize();
        static const StylePosition& GetDefault();
        static const StylePosition& Get(const ComputedStyle& style);
        static StylePosition* TryGet(const ComputedStyle& style);
        static StylePosition& GetOrAdd(ComputedStyle& style);
        static void Dispose(ComputedStyle& style);
        static void ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
            const ComputedStyle* parent);
        static void ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props);
        static bool ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg);
    };
}