#include "OpenGUI/Style2/peglib.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Properties.h"
#include <string_view>


namespace OGUI
{
    struct AnimValueParser
    {
        static std::string_view GetGrammar();
        static void SetupAction(peg::parser& parser);
    };
    namespace CSSParser
    {
        void SetupAnimValueParser()
        {
            RegisterGrammar(AnimValueParser::GetGrammar(), &AnimValueParser::SetupAction);
        }
    }
}

std::string_view OGUI::AnimValueParser::GetGrammar()
{
    static auto grammar = R"(
        Time <- Number ('s' / 'ms')
        AnimTimingFunction <- 'linear'
        AnimIterationCount <- Number / 'infinite'
    )";
    return grammar;
}

constexpr OGUI::AnimTimingFunction AnimLinearFuncion{};
void OGUI::AnimValueParser::SetupAction(peg::parser &parser)
{
    using namespace peg;
    parser["Time"] = [](SemanticValues& vs)
    {
        float value = std::any_cast<float>(vs[0]);
        return vs.choice() == 0 ? value : (value / 1000);
    };
    parser["AnimTimingFunction"] = [](SemanticValues& vs)
    {
        return AnimLinearFuncion; //TODO
    };
    parser["AnimIterationCount"] = [](SemanticValues& vs)
    {
        return vs.choice() == 0 ? std::any_cast<float>(vs[0]) : -1.f;
    };
}