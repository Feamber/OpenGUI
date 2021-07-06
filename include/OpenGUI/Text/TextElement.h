#ifndef UE4Runtime
#pragma once

//https://www.w3.org/TR/2021/CRD-css-text-3-20210316
//https://github1s.com/mozilla/gecko-dev/blob/HEAD/gfx/thebes/gfxTextRun.h
//https://github1s.com/mozilla/gecko-dev/blob/HEAD/gfx/thebes/gfxTextRun.cpp
//https://github1s.com/mozilla/gecko-dev/blob/HEAD/layout/generic/nsTextFrame.h
//https://github1s.com/mozilla/gecko-dev/blob/HEAD/layout/generic/nsTextFrame.cpp
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{
    enum class TextOverflow
    {
        Clip, Elipsis
    };
    enum class WhiteSpace
    {
        Normal,
        NoWrap,
        Pre,
        PreWrap,
        BreakSpaces,
        PreLine
    };
    enum class WritingMode
    {
        HorizontalTB,
        VerticalRL,
        VerticalLR
    };
    enum class LineBreak
    {
        Auto,
        Loose,
        Normal,
        Strict,
        Anywhere
    };
    enum class WordBreak
    {
        Normal,
        BreakAll,
        KeepAll
    };
    enum class OverflowWrap
    {
        Normal,
        Anywhere,
        Breakword,
    };
    enum class Hyphens
    {
        None,
        Manual,
        Auto
    };
    
#define TEXTPROP(F) \
F(tabSize,   float, "tab-size", 4.f) \
F(whiteSpace, WhiteSpace, "white-space", WhiteSpace::Normal)\
F(textOverflow, TextOverflow, "text-overflow", TextOverflow::Clip)\

    class TextElement : public VisualElement
    {
        TextElement();
        ~TextElement();
        std::vector<ostr::string> _texts;
        struct Content
        {
            int index : 31;
            bool isElement : 1;
        };
        std::vector<Content> _contents;
    };
}
#endif