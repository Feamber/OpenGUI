//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/transition.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/ComputedStyle.h"

void OGUI::TransitionStyle::Initialize()
{
    transitionProperty = {};
    transitionDuration = 1.f;
    transitionDelay = 0.f;
    transitionTimingFunction = {};
}

void OGUI::TransitionStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            switch(prop.id)
            {
                case Ids::transitionProperty:
                    transitionProperty = {};
                    break;
                case Ids::transitionDuration:
                    transitionDuration = 1.f;
                    break;
                case Ids::transitionDelay:
                    transitionDelay = 0.f;
                    break;
                case Ids::transitionTimingFunction:
                    transitionTimingFunction = {};
                    break;
                default: break;
            }
        }
        else
        {
            switch(prop.id)
            {
                case Ids::transitionProperty:
                    transitionProperty = sheet.Get<size_t>(prop.value);
                    break;
                case Ids::transitionDuration:
                    transitionDuration = sheet.Get<float>(prop.value);
                    break;
                case Ids::transitionDelay:
                    transitionDelay = sheet.Get<float>(prop.value);
                    break;
                case Ids::transitionTimingFunction:
                    transitionTimingFunction = sheet.Get<AnimTimingFunction>(prop.value);
                    break;
                default: break;
            }
        }
    }
}


void OGUI::TransitionStyle::SetupParser()
{
    {
        using namespace CSSParser;
        static const auto grammar = "transition-property <- 'transition-property' _ ':' _ Name (_ ',' _ Name)*";
        RegisterProperty("transition-property");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::transitionProperty;
            parser["transition-property"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& tras = ctx.rule->transition;
                if(tras.size() != 0)
                    throw peg::parse_error("transition-property is already specified.");
                tras.resize(vs.size());
                for(int i=0; i<vs.size(); ++i)
                    tras[i].property = ComputedStyle::GetProperty(ostr::string::decode_from_utf8(std::any_cast<const std::string_view&>(vs[i])));
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "transition-durationValue <- GlobalValue / Time  (_ ',' _ Time)*\ntransition-duration <- 'transition-duration' _ ':' _ transition-durationValue";
        RegisterProperty("transition-duration");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::transitionDuration;
            parser["transition-durationValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& tras = ctx.rule->transition;
                if(tras.size() == 0)
                    throw peg::parse_error("transition-property must be specified first.");
                auto size = tras.size();
                
                if(vs.choice() == 0)
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[i%vs.size()])});
                else
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[i%vs.size()]))});

            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "transition-delayValue <- GlobalValue / Time  (_ ',' _ Time)*\ntransition-delay <- 'transition-delay' _ ':' _ transition-delayValue";
        RegisterProperty("transition-delay");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::transitionDelay;
            parser["transition-delayValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& tras = ctx.rule->transition;
                if(tras.size() == 0)
                    throw peg::parse_error("transition-property must be specified first.");
                auto size = tras.size();
                
                if(vs.choice() == 0)
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[i%vs.size()])});
                else
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[i%vs.size()]))});

            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "transition-timing-functionValue <- GlobalValue / AnimTimingFunction  (_ ',' _ AnimTimingFunction)*\ntransition-timing-function <- 'transition-timing-function' _ ':' _ transition-timing-functionValue";
        RegisterProperty("transition-timing-function");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::transitionTimingFunction;
            parser["transition-timing-functionValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& tras = ctx.rule->transition;
                if(tras.size() == 0)
                    throw peg::parse_error("transition-property must be specified first.");
                auto size = tras.size();
                
                if(vs.choice() == 0)
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[i%vs.size()])});
                else
                    for(int i=0; i<size; ++i)
                        tras[i].properties.push_back({hash, ctx.storage->Push<AnimTimingFunction>(std::any_cast<AnimTimingFunction&>(vs[i%vs.size()]))});

            };
        });
    }
}