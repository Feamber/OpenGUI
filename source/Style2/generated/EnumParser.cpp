//DO NOT MODIFY THIS FILE
//generated from EnumParser.cpp.mako
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Text/TextTypes.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/AnimTypes.h"

namespace OGUI::CSSParser
{
void SetupEnumParser()
{
    static auto grammar = R"(
        InlineAlign <- 'baseline' / 'top' / 'middle'
        FlexOverflow <- 'visible' / 'hidden' / 'clip' / 'scroll' / 'auto'
        AnimDirection <- 'normal' / 'reverse' / 'alternate' / 'alternate-reverse'
        AnimPlayState <- 'running' / 'paused'
        AnimResumeMode <- 'reset' / 'resume'
        AnimYieldMode <- 'stop' / 'go-back' / 'keep'
        AnimFillMode <- 'none' / 'forwards' / 'backwards' / 'both'
        TextStyle <- 'normal' / 'italic'
        TextAlign <- 'start' / 'end' / 'left' / 'right' / 'center' / 'justify'
    )";
    RegisterGrammar(grammar, [](peg::parser& parser)
    {
        parser["InlineAlign"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EInlineAlign::Baseline;
                case 1 : return OGUI::EInlineAlign::Top;
                case 2 : return OGUI::EInlineAlign::Middle;
            }
            throw peg::parse_error("fail to parse InlineAlign");
        };
        parser["FlexOverflow"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EFlexOverflow::Visible;
                case 1 : return OGUI::EFlexOverflow::Hidden;
                case 2 : return OGUI::EFlexOverflow::Clip;
                case 3 : return OGUI::EFlexOverflow::Scroll;
                case 4 : return OGUI::EFlexOverflow::Auto;
            }
            throw peg::parse_error("fail to parse FlexOverflow");
        };
        parser["AnimDirection"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EAnimDirection::Normal;
                case 1 : return OGUI::EAnimDirection::Reverse;
                case 2 : return OGUI::EAnimDirection::Alternate;
                case 3 : return OGUI::EAnimDirection::AlternateReverse;
            }
            throw peg::parse_error("fail to parse AnimDirection");
        };
        parser["AnimPlayState"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EAnimPlayState::Running;
                case 1 : return OGUI::EAnimPlayState::Paused;
            }
            throw peg::parse_error("fail to parse AnimPlayState");
        };
        parser["AnimResumeMode"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EAnimResumeMode::Reset;
                case 1 : return OGUI::EAnimResumeMode::Resume;
            }
            throw peg::parse_error("fail to parse AnimResumeMode");
        };
        parser["AnimYieldMode"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EAnimYieldMode::Stop;
                case 1 : return OGUI::EAnimYieldMode::GoBack;
                case 2 : return OGUI::EAnimYieldMode::Keep;
            }
            throw peg::parse_error("fail to parse AnimYieldMode");
        };
        parser["AnimFillMode"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::EAnimFillMode::None;
                case 1 : return OGUI::EAnimFillMode::Forwards;
                case 2 : return OGUI::EAnimFillMode::Backwards;
                case 3 : return OGUI::EAnimFillMode::Both;
            }
            throw peg::parse_error("fail to parse AnimFillMode");
        };
        parser["TextStyle"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::ETextStyle::Normal;
                case 1 : return OGUI::ETextStyle::Italic;
            }
            throw peg::parse_error("fail to parse TextStyle");
        };
        parser["TextAlign"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
                case 0 : return OGUI::ETextAlign::Start;
                case 1 : return OGUI::ETextAlign::End;
                case 2 : return OGUI::ETextAlign::Left;
                case 3 : return OGUI::ETextAlign::Right;
                case 4 : return OGUI::ETextAlign::Center;
                case 5 : return OGUI::ETextAlign::Justify;
            }
            throw peg::parse_error("fail to parse TextAlign");
        };
    });
}
}