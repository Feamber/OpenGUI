//DO NOT MODIFY THIS FILE
//generated from EnumParser.cpp.mako
#include "OpenGUI/Style2/Parse.h"
%for header in db.headers:
#include "${header}"
%endfor

namespace OGUI::CSSParser
{
void SetupEnumParser()
{
    static auto grammar = R"(
    %for enum in db.enums:
        ${enum.raw_name} <- ${" / ".join(["'" + enumerator.syntax + "'" for enumerator in enum.enumerators])}
    %endfor
    )";
    RegisterGrammar(grammar, [](peg::parser& parser)
    {
    %for enum in db.enums:
        parser["${enum.raw_name}"] = [](peg::SemanticValues& vs, std::any& dt){
            switch(vs.choice())
            {
            %for i, enumerator in enumerate(enum.enumerators):
                case ${i} : return ${enumerator.name};
            %endfor
            }
            throw peg::parse_error("fail to parse ${enum.raw_name}");
        };
    %endfor
    });
}
}