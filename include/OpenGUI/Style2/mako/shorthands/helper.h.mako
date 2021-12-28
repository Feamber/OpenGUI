<%! 
    from tool.style_codegen import to_small_camel_case, to_camel_case
%>
<%def name="four_sides_shorthand(name, sub_property_pattern, sides)">
    <% 
        prop = struct.add_shorthand(name, [sub_property_pattern % side for side in sides])
        valueRule = prop.sub_properties[0].valueRule
    %>
namespace OGUI::CSSParser
{
    void Register${to_camel_case(name)}()
    {
        static const auto grammar = "${prop.name}Value <- GlobalValue / ${valueRule} (w ${valueRule}){0, 3} \n${prop.name} <- '${prop.name}' _ ':' _ ${prop.name}Value";
        RegisterProperty("${prop.name}");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            parser["${prop.name}Value"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                {
                    int keyword = (int)std::any_cast<StyleKeyword>(vs[0]);

                %for subprop in prop.sub_properties:
                    ctx.rule->properties.push_back({Style${struct.ident}::Ids::${subprop.ident}, keyword});
                %endfor
                }
                else
                {
                    auto& v0 = vs[0];
                    auto& v1 = vs.size() > 1 ? vs[1] : v0;
                    auto& v2 = vs.size() > 2 ? vs[2] : v0;
                    auto& v3 = vs.size() > 3 ? vs[3] : v1;
                    
                %for i, subprop in enumerate(prop.sub_properties):
                    ctx.rule->properties.push_back({Style${struct.ident}::Ids::${subprop.ident}, ctx.storage->Push<${subprop.view_type}>(std::any_cast<${subprop.storage_type}>(v${i}))});
                %endfor
                }
            };
        });
    }
}
</%def>