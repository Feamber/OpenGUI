#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "peglib.h"

namespace OGUI
{
    std::optional<StyleComplexSelector> ParseSelector(std::string_view str)
	{
		auto grammar = R"(
			ComplexSelector		<- Selector ComplexPart*
			ComplexPart			<- ([ ]+ Selector) / (w '>' w Selector)
			Selector			<- SelectorPart+
			SelectorPart		<- "*" / ('.' <IDENT>) / ('#' <IDENT>) / <IDENT> / (':' <IDENT>)
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			~w					<- [ ]*
		)"; 
		using namespace peg;
		using namespace std;
		parser parser;
		StyleSheet sheet;

		parser.log = [](size_t line, size_t col, const string& msg)
		{
			cerr << line << ":" << col << ": " << msg << "\n";
		};

		auto ok = parser.load_grammar(grammar);
		if (!ok)
			return {};
		parser["SelectorPart"] = [](SemanticValues& vs)
		{
			string value;
			if (vs.tokens.size() != 0) value = {vs.tokens[0].begin(), vs.tokens[0].end()};
			return StyleSelector::Part{(StyleSelector::Kind)vs.choice(), value};
		};
		parser["Selector"] = [](SemanticValues& vs)
		{
			StyleSelector selector;
			for (auto& p : vs)
			{
				auto part = any_move<StyleSelector::Part>(p);
				if (part.type == StyleSelector::PseudoClass)
					selector.AddPseudoClass(part.value);
				else
					selector.parts.push_back(std::move(part));
			}
			return selector;
		};
		struct ComplexPart { StyleSelector selector; };
		parser["ComplexPart"] = [](SemanticValues& vs)
		{
			StyleSelector selector = any_move<StyleSelector>(vs[0]);
			selector.relationship = vs.choice() == 0 ? StyleSelectorRelationship::Descendent : StyleSelectorRelationship::Child;
			return selector;
		};
		parser["ComplexSelector"] = [](SemanticValues& vs)
		{
			StyleComplexSelector complexSelector;
			for (auto& p : vs)
				complexSelector.selectors.push_back(any_move<StyleSelector>(p));
			complexSelector.ruleIndex = vs.line_info().first;
			complexSelector.UpdateSpecificity();
			return complexSelector;
		};
		StyleComplexSelector value;
		if (parser.parse(str, value))
			return value;
		return {};
	}
}