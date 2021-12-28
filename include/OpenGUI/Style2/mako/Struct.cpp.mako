//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako
<%! 
    from tool.style_codegen import to_small_camel_case, to_camel_case
%>

#include <memory>
#include "${struct.include_path}"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
%if struct.shorthands:
#include "${struct.name}_shorthands.h"
%endif

const OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetDefault()
{
    struct Helper
    {
        OGUI::Style${struct.ident} value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::Style${struct.ident}* OGUI::Style${struct.ident}::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::Style${struct.ident}*)iter->second.ptr.get();
    }
}

OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::Style${struct.ident}>();
        value->Initialize();
        style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
        return *value.get();
    }
    else 
    {
        return *(OGUI::Style${struct.ident}*)iter->second.ptr.get();
    }

}

void OGUI::Style${struct.ident}::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::Style${struct.ident}::Initialize()
{
%for prop in struct.longhands:
    ${prop.ident} = ${prop.initial_value};
%endfor
}

void OGUI::Style${struct.ident}::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::Style${struct.ident}* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::Style${struct.ident}*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>(*st);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
        %if not struct.inherited:
            || prop.value.index == (int)StyleKeyword::Unset
        %endif
             )
            {
                switch(prop.id)
                {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:{
                    auto v = fget();
                    v->${prop.ident} = ${prop.initial_value};
                    break;
                    }
            %endfor
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:{
                    auto v = fget();
                    v->${prop.ident} = pst->${prop.ident};
                    break;
                    }
            %endfor
                default: break;
                }
            }
        }
        else
        {
            switch(prop.id)
            {
            %for prop in struct.longhands:
                case Ids::${prop.ident}:{
                    auto v = fget();
                %if prop.is_vector:
                    v->${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.value));
                %else:
                    v->${prop.ident} = sheet.Get<${prop.view_type}>(prop.value);
                %endif
                    break;
                    }
            %endfor
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::Style${struct.ident}::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::Style${struct.ident}* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::Style${struct.ident}*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::Style${struct.ident}>(*st);
            style.structs.erase(iter);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
        %for prop in struct.longhands:
            case Ids::${prop.ident}:{
                auto v = fget();
            %if prop.restyle_damage and not prop.is_vector:
                auto prevValue = v->${prop.ident};
            %endif
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                %if prop.is_vector:
                    v->${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.from));
                %else:
                    v->${prop.ident} = sheet.Get<${prop.view_type}>(prop.from);
                %endif
                else if(prop.alpha == 1.f)
                %if prop.is_vector:
                    v->${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.to));
                %else:
                    v->${prop.ident} = sheet.Get<${prop.view_type}>(prop.to);
                %endif
                else if(prop.from == prop.to)
                    v->${prop.ident} = OGUI::Lerp(v->${prop.ident}, sheet.Get<${prop.view_type}>(prop.to), prop.alpha);
                else
                    v->${prop.ident} = OGUI::Lerp(sheet.Get<${prop.view_type}>(prop.from), sheet.Get<${prop.view_type}>(prop.to), prop.alpha);
                
            %if prop.restyle_damage:
            %if not prop.is_vector:
                if(prevValue != v->${prop.ident})
            %endif
                    damage |= ${"|".join(["RestyleDamage::" + x for x in prop.restyle_damage.split("|")])};
            %endif
                break;
                }
        %endfor
            default: break;
        }
    }
    return damage;
}

void OGUI::Style${struct.ident}::SetupParser()
{
%if struct.shorthands:
%for prop in struct.shorthands:
    CSSParser::Register${to_camel_case(prop.name)}();
%endfor
%endif
%for prop in struct.longhands:
	{
        using namespace CSSParser;
    %if prop.is_vector:
        static const auto grammar = "${prop.name}Value <- GlobalValue / (${prop.valueRule} (_ ',' _ ${prop.valueRule})*) \n${prop.name} <- '${prop.name}' _ ':' _ ${prop.name}Value";
    %else:
        static const auto grammar = "${prop.name}Value <- GlobalValue / ${prop.valueRule} \n${prop.name} <- '${prop.name}' _ ':' _ ${prop.name}Value";
    %endif
        RegisterProperty("${prop.name}");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::${prop.ident};
            parser["${prop.name}Value"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                %if prop.is_vector:
                {
                    ${prop.storage_type} value;
                    for(auto& e : vs)
                        value.emplace_back(any_move<${prop.parsed_type}>(e));
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(value)});
                }
                %else:
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(std::any_cast<${prop.parsed_type}&>(vs[0]))});
                %endif
            };
        });
    }
%endfor
}