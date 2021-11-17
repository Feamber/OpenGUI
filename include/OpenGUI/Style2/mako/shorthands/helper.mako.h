<%! 
    from tool.style_codegen import to_small_camel_case, to_camel_case
    from tool.style_codegen import (LOGICAL_CORNERS, PHYSICAL_CORNERS, LOGICAL_SIDES,
                      PHYSICAL_SIDES, LOGICAL_SIZES, LOGICAL_AXES)
%>
<%def name="shorthand(name, sub_properties)">
    <% struct.add_shorthand(name, sub_properties.split()) %>
    ${caller.body()}
</%def>

<%def name="four_sides_shorthand(name, sub_property_pattern)">
    <% sub_properties=' '.join(sub_property_pattern % side for side in PHYSICAL_SIDES) %>
    <%call expr="self.shorthand(name, sub_properties=sub_properties)">
namespace OGUI
{
    namespace Parse
    {
        bool Parse${to_camel_case(name)}(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
        {
            YGValue values[4];
            if(!ParseFourSides(value, values[0], values[1], values[2], values[3]))
            {
                errorMsg = "failed to parse ${name} value!";
                return false;
            }
        %for index, side in enumerate(["left", "top", "right", "bottom"]):
            rule.properties.push_back(StyleProperty{Style${struct.ident}::Ids::${to_small_camel_case(sub_property_pattern % side)}, sheet.Push(values[${index}])});
        %endfor
            return true;
        }
    }
}
    </%call>
</%def>

<%def name="four_corners_shorthand(name, sub_property_pattern)">
    <% sub_properties=' '.join(sub_property_pattern % side for side in PHYSICAL_CORNERS) %>
    <%call expr="self.shorthand(name, sub_properties=sub_properties)">
namespace OGUI
{
    namespace Parse
    {
        bool Parse${to_camel_case(name)}(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
        {
            YGValue values[4];
            if(!ParseFourSides(value, values[0], values[1], values[2], values[3]))
            {
                errorMsg = "failed to parse ${name} value!";
                return false;
            }
        %for index, side in enumerate(["top-left", "top-right", "bottom-right", "bottom-left"]):
            rule.properties.push_back(StyleProperty{Style${struct.ident}::Ids::${to_small_camel_case(sub_property_pattern % side)}, sheet.Push(values[${index}])});
        %endfor
            return true;
        }
    }
}
    </%call>
</%def>