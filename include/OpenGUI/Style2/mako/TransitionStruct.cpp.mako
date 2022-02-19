//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako
<%! 
    from tool.style_codegen import to_small_camel_case, to_camel_case
%>

#include <memory>
#include "${struct.include_path}"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/ComputedStyle.h"

void OGUI::TransitionStyle::Initialize()
{
%for prop in struct.longhands:
    ${prop.ident} = ${prop.initial_value};
%endfor
}

void OGUI::TransitionStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:
                    ${prop.ident} = ${prop.initial_value};
                    break;
            %endfor
                default: break;
            }
        }
        else
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:
                    ${prop.ident} = sheet.Get<${prop.view_type}>(prop.value);
                    break;
            %endfor
                default: break;
            }
        }
    }
}


void OGUI::TransitionStyle::SetupParser()
{
%if struct.shorthands:
%for prop in struct.shorthands:
    CSSParser::Register${to_camel_case(prop.name)}();
%endfor
%endif
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
%for prop in struct.longhands:
    %if prop.name!="transition-property":
	{
        using namespace CSSParser;
        static const auto grammar = "${prop.name}Value <- GlobalValue / ${prop.valueRule}  (_ ',' _ ${prop.valueRule})*\n${prop.name} <- '${prop.name}' _ ':' _ ${prop.name}Value";
        RegisterProperty("${prop.name}");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::${prop.ident};
            parser["${prop.name}Value"] = [](peg::SemanticValues& vs, std::any& dt){
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
                    %if prop.is_string:
                        tras[i].properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(ostr::string::decode_from_utf8(std::any_cast<${prop.parsed_type}&>(vs[i%vs.size()])))});
                    %else:
                        tras[i].properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(std::any_cast<${prop.parsed_type}&>(vs[i%vs.size()]))});
                    %endif

            };
        });
    }
    %endif
%endfor
}