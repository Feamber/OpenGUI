#pragma once
#include "yoga/Yoga.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/position.h"




    
namespace OGUI::CSSParser
{
    void RegisterMargin()
    {
        static const auto grammar = "marginValue <- GlobalValue / LengthPercentage (w LengthPercentage){0, 3} \nmargin <- 'margin' _ ':' _ marginValue";
        RegisterProperty("margin");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            parser["marginValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                {
                    int keyword = (int)std::any_cast<StyleKeyword>(vs[0]);

                    ctx.rule->properties.push_back({StylePosition::Ids::marginTop, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginRight, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginBottom, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginLeft, keyword});
                }
                else
                {
                    auto& v0 = vs[0];
                    auto& v1 = vs.size() > 1 ? vs[1] : v0;
                    auto& v2 = vs.size() > 2 ? vs[2] : v0;
                    auto& v3 = vs.size() > 3 ? vs[3] : v1;
                    
                    ctx.rule->properties.push_back({StylePosition::Ids::marginTop, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v0))});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginRight, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v1))});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginBottom, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v2))});
                    ctx.rule->properties.push_back({StylePosition::Ids::marginLeft, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v3))});
                }
            };
        });
    }
}



    
namespace OGUI::CSSParser
{
    void RegisterPadding()
    {
        static const auto grammar = "paddingValue <- GlobalValue / LengthPercentage (w LengthPercentage){0, 3} \npadding <- 'padding' _ ':' _ paddingValue";
        RegisterProperty("padding");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            parser["paddingValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                {
                    int keyword = (int)std::any_cast<StyleKeyword>(vs[0]);

                    ctx.rule->properties.push_back({StylePosition::Ids::paddingTop, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingRight, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingBottom, keyword});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingLeft, keyword});
                }
                else
                {
                    auto& v0 = vs[0];
                    auto& v1 = vs.size() > 1 ? vs[1] : v0;
                    auto& v2 = vs.size() > 2 ? vs[2] : v0;
                    auto& v3 = vs.size() > 3 ? vs[3] : v1;
                    
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingTop, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v0))});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingRight, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v1))});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingBottom, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v2))});
                    ctx.rule->properties.push_back({StylePosition::Ids::paddingLeft, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v3))});
                }
            };
        });
    }
}
