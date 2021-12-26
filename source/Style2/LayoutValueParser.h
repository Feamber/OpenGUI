#pragma once
#include "OpenGUI/Style2/peglib.h"
namespace OGUI
{
    struct LayoutValueParser
    {
        static std::string_view GetGrammar();
        static void SetupAction(peg::parser& parser);
    };
}