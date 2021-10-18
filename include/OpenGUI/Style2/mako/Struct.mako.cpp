#include <memory>
<% from data import struct %>
#include "OpenGUI/Style2/generated/${struct.name}.h"

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

const OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::Get(Style& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

const OGUI::Style${struct.ident}* OGUI::Style${struct.ident}::TryGet(Style& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::Style${struct.ident}*)iter.second->get();
    }
}

OGUI::Style${struct.ident}& OGUI::Style${struct.ident}::GetOrAdd(Style& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::Style${struct.ident}>();
        value->Initialize();
        style.structs.insert(hash, std::static_pointer_cast<void>(value));
        return *value->get();
    }
    else 
    {
        return *(OGUI::Style${struct.ident}*)iter.second->get();
    }

}

void OGUI::Style${struct.ident}::Dispose(Style& style)
{
    style.structs.erase(hash);
}

void OGUI::Style${struct.ident}::Merge(Style& style, const Style& other, gsl::span<StyleProperty>& props);
{
    auto st = TryGet(style);
    auto ost = TryGet(other);
    if(!ost)
        return;
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            %for prop in struct.longhands:
                case ${prop.hash}:
                    if(!st)
                    {
                        style.structs.insert(hash, other.structs.find(hash).second);
                        return;
                    }
                    st->${prop.ident} = ost->${prop.ident};
                    break;
            %endfor
        }
    }
}

void OGUI::Style${struct.ident}::Initialize(Style& style)
{
    %for prop in struct.longhands:
        ${prop.ident} = {prop.initial_value};
    %endfor
}

void OGUI::Style${struct.ident}::ApplySS(Style& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style& parent)
{
    auto pst = TryGet(parent);
    OGUI::Style${struct.ident}* value;
    auto fget = [&]
    {
        if(!value)
            value = &GetOrAdd(style);
        return value;
    }
    
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial
            %if !struct.inherited
                || prop.value.index == (int)StyleKeyword::Unset || !pst
            %endif
             )
            {
                %for prop in struct.longhands:
                    case ${prop.hash}:
                        auto v = fget();
                        v->${prop.ident} = sheet.Get<${prop.type}>(prop.value);
                        break;
                %endfor
            }
            else
            { 
                %for prop in struct.longhands:
                    case ${prop.hash}:
                        auto v = fget();
                        v->${prop.ident} = pst->${prop.ident};
                        break;
                %endfor
            }
        }
        else
        {
            switch(prop.id)
            {
                %for prop in struct.longhands:
                    case ${prop.hash}:
                        auto v = fget();
                        v->${prop.ident} = sheet.Get<${prop.type}>(prop.value);
                        break;
                %endfor
            }
        }
    }
}