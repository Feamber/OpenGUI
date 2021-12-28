#include "OpenGUI/Style2/peglib.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Properties.h"
#include "yoga/Yoga.h"


namespace OGUI
{
    struct LayoutValueParser
    {
        static std::string_view GetGrammar();
        static void SetupAction(peg::parser& parser);
    };
    namespace CSSParser
    {
        void SetupLayoutValueParser()
        {
            RegisterGrammar(LayoutValueParser::GetGrammar(), &LayoutValueParser::SetupAction);
        }
    }
}


std::string_view OGUI::LayoutValueParser::GetGrammar()
{   
    static auto grammar = R"(
        Length				 <- (Number 'px') / '0'
        LengthPercentage <- Percentage / Length
        Width	<-  Percentage / Length / 'auto'
        AspectRatio <- Number / Number '/' Number
        FlexPosition <- 'static' / 'relative' / 'absolute'
        FlexAlign <- 'auto' / 'flex-start' / 'center' / 'flex-end' / 'stretch' / 'baseline' / 'space-between' / 'space-around'
        FlexDirection <- 'column' / 'column-reverse' / 'row' / 'row-reverse'
        FlexJustify <- 'flex-start' / 'center' / 'flex-end' / 'space-between' / 'space-around' / 'space-evenly'
        FlexWrap <- 'no-wrap' / 'wrap' / 'wrap-reverse'
        FlexDisplay <- 'flex' / 'none'
    )";
    return grammar;
};

void OGUI::LayoutValueParser::SetupAction(peg::parser &parser)
{
    using namespace peg;
    parser["Length"] = [](SemanticValues& vs)
    {
        if(vs.choice() == 1)
            return 0.f;
        else
            return std::any_cast<float>(vs[0]);
    };
    parser["LengthPercentage"] = [](SemanticValues& vs)
    {
        YGValue value;
        if(vs.choice() == 0)
        {
            value.value = std::any_cast<float>(vs[0]) * 100.f;
            value.unit = YGUnitPercent;
        }
        else 
        {
            value.value = std::any_cast<float>(vs[0]);
            value.unit =  YGUnitPoint;
        }
       
        return value;
    };
    parser["Width"] = [](SemanticValues& vs)
    {
        YGValue value;
        if(vs.choice() == 0)
        {
            value.value = std::any_cast<float>(vs[0]) * 100.f;
            value.unit = YGUnitPercent;
        }
        else if(vs.choice() == 1)
        {
            value.value = std::any_cast<float>(vs[0]);
            value.unit =  YGUnitPoint;
        }
        else 
        {
            value.unit =  YGUnitAuto;
        }
        return value;
    };
    parser["AspectRatio"] = [](SemanticValues& vs)
    {
        if(vs.choice() == 0)
            return std::any_cast<float>(vs[0]);
        else
            return std::any_cast<float>(vs[0]) / std::any_cast<float>(vs[1]);
    };
    parser["FlexPosition"] = [](SemanticValues& vs)
    {
        return (YGPositionType)vs.choice();
    }; 
    parser["FlexAlign"] = [](SemanticValues& vs)
    {
        return (YGAlign)vs.choice();
    };
    parser["FlexDirection"] = [](SemanticValues& vs)
    {
        return (YGFlexDirection)vs.choice();
    };
    parser["FlexJustify"] = [](SemanticValues& vs)
    {
        return (YGJustify)vs.choice();
    };
    parser["FlexWrap"] = [](SemanticValues& vs)
    {
        return (YGWrap)vs.choice();
    };
    parser["FlexDisplay"] = [](SemanticValues& vs)
    {
        return (YGDisplay)vs.choice();
    };
}

