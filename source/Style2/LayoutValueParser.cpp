#include "LayoutValueParser.h"
#include "OpenGUI/Style2/Parse.h"
#include "yoga/Yoga.h"

std::string_view OGUI::LayoutValueParser::GetGrammar()
{   
    static auto grammar = R"(
			Length				 <- Number ('px')
            LengthPercentage <- Length / Percentage
			Width	<- Length / Percentage / 'auto''
    )";
    return grammar;
};

void OGUI::LayoutValueParser::SetupAction(peg::parser &parser)
{
    using namespace peg;
    parser["Length"] = [](SemanticValues& vs)
    {
        return std::any_cast<float>(vs[0]);
    };
    parser["LengthPercentage"] = [](SemanticValues& vs)
    {
        YGValue value;
        if(vs.choice() < 2)
            value.value = std::any_cast<float>(vs[0]);
        value.unit =  (YGUnit)(vs.choice()+1);
        return value;
    };
     parser["Width"] = [](SemanticValues& vs)
    {
        YGValue value;
        value.value = std::any_cast<float>(vs[0]);
        value.unit =  (YGUnit)(vs.choice()+1);
        return value;
    };
}

