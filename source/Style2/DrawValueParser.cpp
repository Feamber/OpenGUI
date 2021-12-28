#include "OpenGUI/Style2/peglib.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Shadow.h"
#include <string_view>


namespace OGUI
{
    struct DrawValueParser
    {
        static std::string_view GetGrammar();
        static void SetupAction(peg::parser& parser);
    };
    namespace CSSParser
    {
        void SetupDrawValueParser()
        {
            RegisterGrammar(DrawValueParser::GetGrammar(), &DrawValueParser::SetupAction);
        }
    }
}

std::string_view OGUI::DrawValueParser::GetGrammar()
{
    static auto grammar = R"(
        TextShadow <- Length w Length w Color
        TextWeight <- 'normal' / 'bold' / Integer
    )";
    return grammar;
}

void OGUI::DrawValueParser::SetupAction(peg::parser &parser)
{
    using namespace peg;
    parser["TextShadow"] = [](SemanticValues& vs)
    {
        return TextShadow{{std::any_cast<float>(vs[0]), std::any_cast<float>(vs[1])}, std::any_cast<Color4f>(vs[2])};
    };
    parser["TextWeight"] = [](SemanticValues& vs)
    {
        if(vs.choice() == 0)
            return (int)500;
        else if (vs.choice() == 1)
            return (int)700;
        else
            return std::any_cast<int>(vs[0]);
    };
}