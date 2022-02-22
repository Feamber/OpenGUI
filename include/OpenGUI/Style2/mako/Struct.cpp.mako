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
#include "OpenGUI/VisualElement.h"
%if struct.shorthands:
#include "${struct.name}_shorthands.h"
%endif

size_t Style${struct.ident}Entry = 0;

size_t OGUI::Style${struct.ident}::GetEntry()
{
    return Style${struct.ident}Entry;
}

void OGUI::Style${struct.ident}::SetEntry(size_t e)
{
    Style${struct.ident}Entry = e;
}

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
    auto& s = style.structs[Style${struct.ident}Entry];
    return (Style${struct.ident}*)s.ptr.get();
}

OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[Style${struct.ident}Entry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::Style${struct.ident}>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::Style${struct.ident}>(*(OGUI::Style${struct.ident}*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(Style${struct.ident}*)s.ptr.get();
}

void OGUI::Style${struct.ident}::Dispose(ComputedStyle& style)
{
    style.structs[Style${struct.ident}Entry].ptr.reset();
}

void OGUI::Style${struct.ident}::Initialize()
{
%for prop in struct.longhands:
    ${prop.ident} = ${prop.initial_value};
%endfor
}

void OGUI::Style${struct.ident}::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    auto mask = override[Style${struct.ident}Entry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
        %for i, prop in enumerate(struct.longhands):
            case Ids::${prop.ident}: if(mask & (1ull<<${i})) continue; break;
        %endfor
        }
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
                case Ids::${prop.ident}: {
                    auto& v = GetOrAdd(style);
                    v.${prop.ident} = ${prop.initial_value};
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
                    auto& v = GetOrAdd(style);
                    v.${prop.ident} = pst->${prop.ident};
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
                    auto& v = GetOrAdd(style);
                %if prop.is_vector:
                    v.${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.value));
                %else:
                    v.${prop.ident} = sheet.Get<${prop.view_type}>(prop.value);
                %endif
                    break;
                    }
            %endfor
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::Style${struct.ident}::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[Style${struct.ident}Entry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
        %for i, prop in enumerate(struct.longhands):
            case Ids::${prop.ident}: if(mask & (1ull<<${i})) continue; break;
        %endfor
        }
        switch(prop.id)
        {
        %for prop in struct.longhands:
            case Ids::${prop.ident}:{
                auto& v = GetOrAdd(style);
            %if prop.restyle_damage and not prop.is_vector:
                auto prevValue = v.${prop.ident};
            %endif
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                %if prop.is_vector:
                    v.${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.from));
                %else:
                    v.${prop.ident} = sheet.Get<${prop.view_type}>(prop.from);
                %endif
                else if(prop.alpha == 1.f)
                %if prop.is_vector:
                    v.${prop.ident} = ToOwned(sheet.Get<${prop.view_type}>(prop.to));
                %else:
                    v.${prop.ident} = sheet.Get<${prop.view_type}>(prop.to);
                %endif
                else if(prop.from == prop.to)
                    v.${prop.ident} = OGUI::Lerp(v.${prop.ident}, sheet.Get<${prop.view_type}>(prop.to), prop.alpha);
                else
                    v.${prop.ident} = OGUI::Lerp(sheet.Get<${prop.view_type}>(prop.from), sheet.Get<${prop.view_type}>(prop.to), prop.alpha);
                
            %if prop.restyle_damage:
            %if not prop.is_vector:
                if(prevValue != v.${prop.ident})
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


OGUI::RestyleDamage OGUI::Style${struct.ident}::ApplyTransitionProperties(ComputedStyle& style, const ComputedStyle& srcS, const ComputedStyle& dstS, 
    const gsl::span<TransitionProperty>& props, const StyleMasks& override)
{
    RestyleDamage damage = RestyleDamage::None;
    
    auto mask = override[Style${struct.ident}Entry];
    auto& src = Get(srcS);
    auto& dst = Get(dstS);

    for(auto& prop : props)
    {
        switch(prop.id)
        {
        %for i, prop in enumerate(struct.longhands):
            case Ids::${prop.ident}: if(mask & (1ull<<${i})) continue; break;
        %endfor
        }
        switch(prop.id)
        {
        %for prop in struct.longhands:
            case Ids::${prop.ident}:{
                auto& v = GetOrAdd(style);
            %if prop.restyle_damage and not prop.is_vector:
                auto prevValue = v.${prop.ident};
            %endif
                if(prop.alpha == 1.f)
                    v.${prop.ident} = dst.${prop.ident};
                else
                    v.${prop.ident} = OGUI::Lerp(src.${prop.ident}, dst.${prop.ident}, prop.alpha);
                
            %if prop.restyle_damage:
            %if not prop.is_vector:
                if(prevValue != v.${prop.ident})
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

void OGUI::Style${struct.ident}::Merge(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[Style${struct.ident}Entry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
%for i, prop in enumerate(struct.longhands):
    if(mask & (1ull << ${i}))
        s.${prop.ident} = po->${prop.ident};
%endfor
}

void OGUI::Style${struct.ident}::MergeId(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    for(auto prop : override)
    {
        switch(prop)
        {
        %for prop in struct.longhands:
            case Ids::${prop.ident}: {
                 auto& v = GetOrAdd(style);
                 v.${prop.ident} = po->${prop.ident};
                 break;
            }
        %endfor
        }
    }
}

size_t OGUI::Style${struct.ident}::GetProperty(ostr::string_view pname)
{
    switchstr(pname)
    {
%for prop in struct.longhands:
        casestr("${prop.name}") return Ids::${prop.ident};
%endfor
        default: return -1;
    }
    return -1;
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
                    %if prop.is_string:
                        value.emplace_back(ostr::string::decode_from_utf8(any_move<${prop.parsed_type}>(e)));
                    %else:
                        value.emplace_back(any_move<${prop.parsed_type}>(e));
                    %endif
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(value)});
                }
                %elif prop.is_string:
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(ostr::string::decode_from_utf8(std::any_cast<${prop.parsed_type}&>(vs[0])))});
                %else:
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<${prop.view_type}>(std::any_cast<${prop.parsed_type}&>(vs[0]))});
                %endif
            };
        });
    }
%endfor
}


%for i, prop in enumerate(struct.longhands):
void OGUI::SetStyle${to_camel_case(prop.name)}(VisualElement* element, ${prop.reference_type} value)
{
    element->_procedureOverrides[Style${struct.ident}Entry] |= 1ull<<${i};
%if prop.is_vector:
    Style${struct.ident}::GetOrAdd(element->_style).${prop.ident} = ToOwned(value);
%else:
    Style${struct.ident}::GetOrAdd(element->_style).${prop.ident} = value;
%endif
%if prop.restyle_damage:
    RestyleDamage damage = ${"|".join(["RestyleDamage::" + x for x in prop.restyle_damage.split("|")])};
%else:
    RestyleDamage damage = RestyleDamage::None;
%endif
    element->UpdateStyle(damage);
}
%if prop.type == "YGValue":
void OGUI::SetStyle${to_camel_case(prop.name)}Pixel(VisualElement* element, float value)
{
    SetStyle${to_camel_case(prop.name)}(element, YGValue{value, YGUnitPoint});
}
void OGUI::SetStyle${to_camel_case(prop.name)}Percentage(VisualElement* element, float value)
{
    SetStyle${to_camel_case(prop.name)}(element, YGValue{value, YGUnitPercent});
}
%if prop.valueRule == "Width":
void OGUI::SetStyle${to_camel_case(prop.name)}Auto(VisualElement* element)
{
    SetStyle${to_camel_case(prop.name)}(element, YGValueAuto);
}
%endif
%endif
void OGUI::ResetStyle${to_camel_case(prop.name)}(VisualElement* element)
{
    element->_procedureOverrides[Style${struct.ident}Entry] &= ~(1ull<<${i});
}
%endfor