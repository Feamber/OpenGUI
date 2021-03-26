#include "OpenGUI/CSSParser/CSSParser.h"
#include "peglib.h"
#include <fstream>

namespace OGUI
{
	template<class T>
	std::remove_cv_t<T> any_move(std::any& any)
	{
		return std::any_cast<T>(std::move(any));
	}
	std::optional<StyleSheet> ParseCSS(std::string_view str)
	{
		auto grammar = R"(
			Stylesheet		<- StyleRule*
			StyleRule		<- SelectorList _ '{' _ PropertyList '}' _
			SelectorList	<- ComplexSelector (',' w ComplexSelector)* _
			ComplexSelector <- Selector ComplexPart*
			ComplexPart		<- ([ ]+ Selector) / (w '>' w Selector)
			Selector		<- SelectorPart+ (':' <IDENT>)*
			SelectorPart	<- "*" / ('.' <IDENT>) / ('#' <IDENT>) / <IDENT>
			PropertyList	<- Property? (';' _ Property)*
			Property		<- IDENT w ':' w Value
			Value			<- SizeList / SIZE / COLOR / IDENT / NUM _
			SizeList		<- <SIZE (',' w SIZE){3}> _
			SIZE			<- (<NUM ('px' / '%')>) / <'0'>
			NUM				<- < ([0-9]*"."[0-9]+) / ([0-9]+) >
			COLOR			<- 'rgba('< w NUM ',' w NUM ',' w NUM ',' w NUM >')' _
			IDENT			<- < [a-zA-Z] [a-zA-Z0-9-]* >
			~_				<- [ \t\r\n]*
			~w				<- [ ]*
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

		auto token = [](SemanticValues& vs)
		{
			return vs.token(0);
		};
		auto forward = [](SemanticValues& vs)
		{
			return vs[0];
		};
		
		parser["SIZE"] = token;
		parser["COLOR"] = token;
		parser["NUM"] = token;
		parser["IDENT"] = token;
		parser["SizeList"] = token;
		parser["Value"] = forward;
		parser["Property"] = [](SemanticValues& vs)
		{
			auto name = any_move<string_view>(vs[0]);
			auto value = any_move<string_view>(vs[1]);
			return make_pair(name, value);
		};
		using property_list_t = vector<pair<string_view, string_view>>;
		parser["PropertyList"] = [&](SemanticValues& vs)
		{
			property_list_t pairs;
			for (auto& p : vs)
				pairs.push_back(any_move<pair<string_view, string_view>>(p));
			return pairs;
		};
		parser["SelectorPart"] = [](SemanticValues& vs)
		{
			string value;
			if(vs.tokens.size() != 0) value = {vs.tokens[0].begin(), vs.tokens[0].end()};
			return StyleSelector::Part{(StyleSelector::Kind)vs.choice(), value};
		};
		parser["Selector"] = [](SemanticValues& vs)
		{
			StyleSelector selector;
			for (auto& p : vs)
				if(p.type() == typeid(StyleSelector::Part))
					selector.parts.push_back(any_move<StyleSelector::Part>(p));
			for (auto& p : vs.tokens)
				selector.AddPseudoClass(p);
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
		parser["SelectorList"] = [](SemanticValues& vs)
		{
			vector<StyleComplexSelector> selectorList;
			for (auto& p : vs)
				selectorList.push_back(any_move<StyleComplexSelector>(p));
			return selectorList;
		};
		parser["StyleRule"] = [&](SemanticValues& vs)
		{
			StyleRule rule;
			auto& list = any_move<property_list_t>(vs[1]);
			for (auto& pair : list)
			{
				const char* errorMsg;
				ParseErrorType errorType;
				if (!ParseProperty(sheet.storage, pair.first, pair.second, rule, errorMsg, errorType))
				{
					throw parse_error(errorMsg);
				}
			}
			int ruleIndex = sheet.styleRules.size();
			sheet.styleRules.push_back(std::move(rule));
			auto& selectorList = any_move<vector<StyleComplexSelector>>(vs[0]);
			for (auto& sel : selectorList)
			{
				sel.ruleIndex = ruleIndex;
				sheet.styleSelectors.push_back(sel);
			}
		};

		//parser.enable_packrat_parsing(); // Enable packrat parsing.
		if (parser.parse(str))
			return sheet;
		return {};
	}

	std::optional<InlineStyle> ParseInlineStyle(std::string_view str)
	{
		auto grammar = R"(
			PropertyList	<- Property? (';' _ Property)*
			Property		<- IDENT w ':' w Value
			Value			<- SizeList / SIZE / COLOR / IDENT / NUM _
			SizeList		<- <SIZE (',' w SIZE){3}> _
			SIZE			<- (<NUM ('px' / '%')>) / <'0'>
			NUM				<- < ([0-9]*"."[0-9]+) / ([0-9]+) >
			COLOR			<- 'rgba('< w NUM ',' w NUM ',' w NUM ',' w NUM >')' _
			IDENT			<- < [a-zA-Z] [a-zA-Z0-9-]* >
			~_				<- [ \t\r\n]*
			~w				<- [ ]*
		)";
		using namespace peg;
		using namespace std;
		parser parser;
		InlineStyle sheet;

		parser.log = [](size_t line, size_t col, const string& msg)
		{
			cerr << line << ":" << col << ": " << msg << "\n";
		};

		auto ok = parser.load_grammar(grammar);

		auto token = [](SemanticValues& vs)
		{
			return vs.token(0);
		};
		auto forward = [](SemanticValues& vs)
		{
			return vs[0];
		};

		parser["SIZE"] = token;
		parser["COLOR"] = token;
		parser["NUM"] = token;
		parser["IDENT"] = token;
		parser["SizeList"] = token;
		parser["Value"] = forward;
		parser["Property"] = [](SemanticValues& vs)
		{
			auto name = any_move<string_view>(vs[0]);
			auto value = any_move<string_view>(vs[1]);
			return make_pair(name, value);
		};
		using property_list_t = vector<pair<string_view, string_view>>;
		parser["PropertyList"] = [&](SemanticValues& vs)
		{
			property_list_t pairs;
			for (auto& p : vs)
				pairs.push_back(any_move<pair<string_view, string_view>>(p));

			for (auto& pair : pairs)
			{
				const char* errorMsg;
				ParseErrorType errorType;
				if (!ParseProperty(sheet.storage, pair.first, pair.second, sheet.rule, errorMsg, errorType))
				{
					throw parse_error(errorMsg);
				}
			}
		};

		//parser.enable_packrat_parsing(); // Enable packrat parsing.
		if (parser.parse(str))
			return sheet;
		return {};
	}

	std::optional<StyleSheet> ParseCSSFile(std::string path)
	{
		std::ifstream ifs(path);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		return ParseCSS(content);
	}
}

