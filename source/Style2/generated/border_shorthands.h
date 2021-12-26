#pragma once
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/border.h"




    
namespace OGUI::CSSParser
{
    void RegisterBorderRadius()
    {
        std::string grammar = "border-radius <- 'border-radius' _ ':' _ (GlobalValue / YGValueZero{1, 4})";
        RegisterProperty("border-radius");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            parser["border-radius"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                {
                    int keyword = (int)std::any_cast<StyleKeyword>(vs[0]);

                    ctx.rule->properties.push_back({StyleBorder::Ids::borderTopLeftRadius, keyword});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderTopRightRadius, keyword});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderBottomRightRadius, keyword});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderBottomLeftRadius, keyword});
                }
                else
                {
                    auto& v0 = vs[0];
                    auto& v1 = vs.size() > 1 ? vs[1] : v0;
                    auto& v2 = vs.size() > 2 ? vs[2] : v0;
                    auto& v3 = vs.size() > 3 ? vs[3] : v1;
                    
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderTopLeftRadius, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v0))});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderTopRightRadius, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v1))});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderBottomRightRadius, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v2))});
                    ctx.rule->properties.push_back({StyleBorder::Ids::borderBottomLeftRadius, ctx.storage->Push<YGValue>(std::any_cast<YGValue>(v3))});
                }
            };
        });
    }
}
