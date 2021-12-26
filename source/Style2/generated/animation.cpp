//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Rule.h"

void OGUI::AnimStyle::Initialize()
{
    animationName = {};
    animationDuration = 1.f;
    animationDelay = 0.f;
    animationDirection = EAnimDirection::Normal;
    animationIterationCount = 1.f;
    animationPlayState = {};
    animationTimingFunction = {};
    animationFillMode = EAnimFillMode::Forwards;
    animationYieldMode = EAnimYieldMode::GoBack;
    animationResumeMode = EAnimResumeMode::Resume;
}

void OGUI::AnimStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            switch(prop.id)
            {
                case Ids::animationName:
                    animationName = {};
                    break;
                case Ids::animationDuration:
                    animationDuration = 1.f;
                    break;
                case Ids::animationDelay:
                    animationDelay = 0.f;
                    break;
                case Ids::animationDirection:
                    animationDirection = EAnimDirection::Normal;
                    break;
                case Ids::animationIterationCount:
                    animationIterationCount = 1.f;
                    break;
                case Ids::animationPlayState:
                    animationPlayState = {};
                    break;
                case Ids::animationTimingFunction:
                    animationTimingFunction = {};
                    break;
                case Ids::animationFillMode:
                    animationFillMode = EAnimFillMode::Forwards;
                    break;
                case Ids::animationYieldMode:
                    animationYieldMode = EAnimYieldMode::GoBack;
                    break;
                case Ids::animationResumeMode:
                    animationResumeMode = EAnimResumeMode::Resume;
                    break;
                default: break;
            }
        }
        else
        {
            switch(prop.id)
            {
                case Ids::animationName:
                    animationName = sheet.Get<std::string>(prop.value);
                    break;
                case Ids::animationDuration:
                    animationDuration = sheet.Get<float>(prop.value);
                    break;
                case Ids::animationDelay:
                    animationDelay = sheet.Get<float>(prop.value);
                    break;
                case Ids::animationDirection:
                    animationDirection = sheet.Get<EAnimDirection>(prop.value);
                    break;
                case Ids::animationIterationCount:
                    animationIterationCount = sheet.Get<float>(prop.value);
                    break;
                case Ids::animationPlayState:
                    animationPlayState = sheet.Get<EAnimPlayState>(prop.value);
                    break;
                case Ids::animationTimingFunction:
                    animationTimingFunction = sheet.Get<AnimTimingFunction>(prop.value);
                    break;
                case Ids::animationFillMode:
                    animationFillMode = sheet.Get<EAnimFillMode>(prop.value);
                    break;
                case Ids::animationYieldMode:
                    animationYieldMode = sheet.Get<EAnimYieldMode>(prop.value);
                    break;
                case Ids::animationResumeMode:
                    animationResumeMode = sheet.Get<EAnimResumeMode>(prop.value);
                    break;
                default: break;
            }
        }
    }
}


void OGUI::AnimStyle::SetupParser()
{
    {
        using namespace CSSParser;
        std::string grammar = "animation-name <- 'animation-name' _ ':' _ Name+";
        RegisterProperty("animation-name");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationName;
            parser["animation-name"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(vs.size() < anim.size())
                    throw peg::parse_error("animation-name dose not match animation properties count.");
                anim.resize(vs.size());
                for(int i=0; i<vs.size(); ++i)
                    anim[i].name = std::any_cast<std::string&>(vs[i]);
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-duration <- 'animation-duration' _ ':' _ (GlobalValue / Time+)";
        RegisterProperty("animation-duration");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationDuration;
            parser["animation-duration"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-duration dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-delay <- 'animation-delay' _ ':' _ (GlobalValue / Time+)";
        RegisterProperty("animation-delay");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationDelay;
            parser["animation-delay"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-delay dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-direction <- 'animation-direction' _ ':' _ (GlobalValue / AnimDirection+)";
        RegisterProperty("animation-direction");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationDirection;
            parser["animation-direction"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-direction dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<EAnimDirection>(std::any_cast<EAnimDirection&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-iteration-count <- 'animation-iteration-count' _ ':' _ (GlobalValue / AnimIterationCount+)";
        RegisterProperty("animation-iteration-count");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationIterationCount;
            parser["animation-iteration-count"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-iteration-count dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-play-state <- 'animation-play-state' _ ':' _ (GlobalValue / AnimPlayState+)";
        RegisterProperty("animation-play-state");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationPlayState;
            parser["animation-play-state"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-play-state dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<EAnimPlayState>(std::any_cast<EAnimPlayState&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-timing-function <- 'animation-timing-function' _ ':' _ (GlobalValue / AnimTimingFunction+)";
        RegisterProperty("animation-timing-function");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationTimingFunction;
            parser["animation-timing-function"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-timing-function dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<AnimTimingFunction>(std::any_cast<AnimTimingFunction&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-fill-mode <- 'animation-fill-mode' _ ':' _ (GlobalValue / AnimFillMode+)";
        RegisterProperty("animation-fill-mode");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationFillMode;
            parser["animation-fill-mode"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-fill-mode dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<EAnimFillMode>(std::any_cast<EAnimFillMode&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-yield-mode <- 'animation-yield-mode' _ ':' _ (GlobalValue / AnimYieldMode+)";
        RegisterProperty("animation-yield-mode");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationYieldMode;
            parser["animation-yield-mode"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-yield-mode dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<EAnimYieldMode>(std::any_cast<EAnimYieldMode&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        std::string grammar = "animation-resume-mode <- 'animation-resume-mode' _ ':' _ (GlobalValue / AnimResumeMode+)";
        RegisterProperty("animation-resume-mode");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::animationResumeMode;
            parser["animation-resume-mode"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                auto& anim = ctx.rule->animation;
                if(anim.size() > 0 && !anim[0].name.empty() && vs.size() > anim.size())
                    throw peg::parse_error("animation-resume-mode dose not match animation-name count.");
                anim.resize(std::max(anim.size(), vs.size()));
                
                if(vs.choice() == 0)
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    for(int i=0; i<vs.size(); ++i)
                        anim[i].properties.push_back({hash, ctx.storage->Push<EAnimResumeMode>(std::any_cast<EAnimResumeMode&>(vs[0]))});
            };
        });
    }
}