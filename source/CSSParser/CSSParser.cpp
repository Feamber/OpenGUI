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
			COLOR			<- (IDENT '(' < CNUM (',' CNUM)* > ')') / (<'#' NUM>)
			CNUM			<- NUM (IDENT / '%')
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
		if (!ok)
			return {};

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
			COLOR			<- (IDENT '(' < CNUM (',' CNUM)* > ')') / (<'#' NUM>)
			CNUM			<- NUM (IDENT / '%')
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
		if (!ok)
			return {};

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


namespace std
{
	void split(const string_view& s, vector<string_view>& tokens, const string_view& delimiters = " ")
	{
		string::size_type lastPos = s.find_first_not_of(delimiters, 0);
		string::size_type pos = s.find_first_of(delimiters, lastPos);
		while (string::npos != pos || string::npos != lastPos)
		{
			auto substr = s.substr(lastPos, pos - lastPos);
			tokens.push_back(substr);//use emplace_back after C++11
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
		}
	}

	inline bool ends_with(std::string_view const& value, std::string_view const& ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}
namespace OGUI
{
	bool FromString(std::string_view str, float& value)
	{
		try
		{
			value = std::stof(std::string{str.begin(), str.end()});
			return true;
		}
		catch (std::exception)
		{
			return false;
		}
	}

	bool FromString(std::string_view str, Color4f& value)
	{
		auto grammar = R"(
			Color	<- RGBA / RGB/ HSLA / HSL / HEX / IDENT
			RGBA	<- 'rgba(' _ CNUM _ ',' _ CNUM _ ',' _ CNUM _ ',' _ CNUM _ ')'
			RGBAS	<- 'rgba(' _ CNUM __ CNUM __ CNUM _ '/' _ CNUM _ ')'
			HSLA	<- 'hsla(' _ ANUM _ ',' _ CNUM _ ',' _ CNUM _ ',' _ CNUM _ ')'
			HSLAS	<- 'hsl(' _ ANUM __ CNUM __ CNUM _ '/' _ CNUM _ ')'
			NUM		<- < ([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+) >
			CNUM	<- NUM '%'?
			ANUM	<- NUM ('deg' / 'rad' / 'grad' / 'turn')?
			HEX		<- '#' [0-9a-fA-F]
			IDENT	<- < [a-zA-Z] [a-zA-Z0-9-]* >
			_		<- [ ]*
			__		<- [ ]+
		)";

		using namespace peg;
		using namespace std;

		parser parser;

		parser.log = [](size_t line, size_t col, const string& msg)
		{
			cerr << line << ":" << col << ": " << msg << "\n";
		};

		auto ok = parser.load_grammar(grammar);
		if (!ok)
			return {};

		parser["IDENT"] = [](SemanticValues& vs)
		{
			
		};

		std::vector<std::string_view> tokens;
		std::split(str, tokens, ",");
		if (tokens.size() != 4)
			return false;
		float comp;
		Color4f res;
		for (int i = 0; i < 4; ++i)
		{
			if (FromString(tokens[i], comp))
				res.m_[i] = comp / 255;
			else
				return false;
		}
		value = res;
		return true;
	}
	bool FromString(std::string_view str, int& value)
	{
		try
		{
			value = std::stoi(std::string{str.begin(), str.end()});
			return true;
		}
		catch (std::exception)
		{
			return false;
		}
	}
	bool FromString(std::string_view str, YGValue& value)
	{
		if (str == "auto")
		{
			value = YGValueAuto;
			return true;
		}
		if (str == "none")
		{
			value = YGValueUndefined;
			return true;
		}
		//TODO: allow 0.0000...
		if (str == "0")
		{
			value = YGValueZero;
			return true;
		}
		float comp;
		std::string numstr;
		YGUnit unit;
		if (ends_with(str, "%"))
		{
			numstr = str.substr(0, str.length() - 1);
			unit = YGUnitPercent;
		}
		else if (ends_with(str, "px"))
		{
			numstr = str.substr(0, str.length() - 2);
			unit = YGUnitPoint;
		}
		else
			return false;

		if (FromString(numstr, comp))
		{
			value = YGValue{comp, unit};
			return true;
		}
		else
			return false;
	}

	bool FromString(std::string_view str, YGValue& left, YGValue& top, YGValue& right, YGValue& bottom)
	{
		std::vector<std::string_view> tokens;
		std::split(std::string{str.begin(), str.end()}, tokens, ",");
		if (tokens.size() == 1)
		{
			if (!FromString(tokens[0], left))
				return false;
			top = right = bottom = left;
			return true;
		}
		else if (tokens.size() == 2)
		{
			if (!FromString(tokens[0], top))
				return false;
			bottom = top;
			if (!FromString(tokens[1], left))
				return false;
			right = left;
			return true;
		}
		else if (tokens.size() == 3)
		{
			if (!FromString(tokens[0], top))
				return false;
			if (!FromString(tokens[1], left))
				return false;
			right = left;
			if (!FromString(tokens[2], bottom))
				return false;
			return true;
		}
		else if (tokens.size() == 4)
		{
			if (!FromString(tokens[0], top))
				return false;
			if (!FromString(tokens[1], left))
				return false;
			if (!FromString(tokens[2], right))
				return false;
			if (!FromString(tokens[3], bottom))
				return false;
			return true;
		}
		else
			return false;
	}

	bool FromString(std::string_view str, YGPositionType& value)
	{
		if (str == "static")
		{
			value = YGPositionTypeStatic;
			return true;
		}
		else if (str == "relative")
		{
			value = YGPositionTypeRelative;
			return true;
		}
		else if (str == "absolute")
		{
			value = YGPositionTypeAbsolute;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGOverflow& value)
	{
		if (str == "visible")
		{
			value = YGOverflowVisible;
			return true;
		}
		else if (str == "hidden")
		{
			value = YGOverflowHidden;
			return true;
		}
		else if (str == "scroll")
		{
			value = YGOverflowScroll;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGAlign& value)
	{
		if (str == "auto")
		{
			value = YGAlignAuto;
			return true;
		}
		else if (str == "flex-start")
		{
			value = YGAlignFlexStart;
			return true;
		}
		else if (str == "center")
		{
			value = YGAlignCenter;
			return true;
		}
		else if (str == "flex-end")
		{
			value = YGAlignFlexEnd;
			return true;
		}
		else if (str == "stretch")
		{
			value = YGAlignStretch;
			return true;
		}
		else if (str == "baseline")
		{
			value = YGAlignBaseline;
			return true;
		}
		else if (str == "space-between")
		{
			value = YGAlignSpaceBetween;
			return true;
		}
		else if (str == "space-around")
		{
			value = YGAlignSpaceAround;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGFlexDirection& value)
	{
		if (str == "column")
		{
			value = YGFlexDirectionColumn;
			return true;
		}
		else if (str == "column-reverse")
		{
			value = YGFlexDirectionColumnReverse;
			return true;
		}
		else if (str == "row")
		{
			value = YGFlexDirectionRow;
			return true;
		}
		else if (str == "row-reverse")
		{
			value = YGFlexDirectionRowReverse;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGJustify& value)
	{
		if (str == "flex-start")
		{
			value = YGJustifyFlexStart;
			return true;
		}
		else if (str == "center")
		{
			value = YGJustifyCenter;
			return true;
		}
		else if (str == "flex-end")
		{
			value = YGJustifyFlexEnd;
			return true;
		}
		else if (str == "space-between")
		{
			value = YGJustifySpaceBetween;
			return true;
		}
		else if (str == "space-around")
		{
			value = YGJustifySpaceAround;
			return true;
		}
		else if (str == "space-evenly")
		{
			value = YGJustifySpaceEvenly;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGWrap& value)
	{
		if (str == "no-wrap")
		{
			value = YGWrapNoWrap;
			return true;
		}
		else if (str == "wrap")
		{
			value = YGWrapWrap;
			return true;
		}
		else if (str == "wrap-reverse")
		{
			value = YGWrapWrapReverse;
			return true;
		}
		return false;
	}

	bool FromString(std::string_view str, YGDisplay& value)
	{
		if (str == "flex")
		{
			value = YGDisplayFlex;
			return true;
		}
		else if (str == "none")
		{
			value = YGDisplayNone;
			return true;
		}
		return false;
	}
}

namespace OGUI
{
	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, StyleProperty>
		AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const T& value)
	{
		auto handle = sheet.Push<T>(value);
		return {id, false, handle};
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, StyleProperty>
		AddPropertyImpl(StyleSheetStorage& sheet, StylePropertyId id, const T& value)
	{
		auto handle = sheet.Push<int>((int)value);
		return {id, false, handle};
	}
}

bool OGUI::ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view str, StyleRule& rule, const char*& errorMsg, ParseErrorType& errorType)
{
	auto id = PropertyNameToId(name);
	if (id == StylePropertyId::Num)
	{
		errorMsg = "custom style property is not support yet!";
		errorType = ParseErrorType::InvalidProperty;
		return false;
	}

	//shorthands
#define SHORTHAND(mm, mm2, smm) \
	if (name == smm) \
	{ \
		YGValue left, top, right, bottom; \
		if(!FromString(str, left, top, right, bottom)) \
		{ \
			errorMsg = "failed to parse style value!"; \
			errorType = ParseErrorType::InvalidValue; \
			return false; \
		} \
		rule.properties.push_back(StyleProperty{StylePropertyId::mm##Left##mm2, false, sheet.Push(left)}); \
		rule.properties.push_back(StyleProperty{StylePropertyId::mm##Top##mm2, false, sheet.Push(top)}); \
		rule.properties.push_back(StyleProperty{StylePropertyId::mm##Right##mm2, false, sheet.Push(right)}); \
		rule.properties.push_back(StyleProperty{StylePropertyId::mm##Bottom##mm2, false, sheet.Push(bottom)}); \
	}
	SHORTHAND(margin, , "margin")
	else SHORTHAND(border, Width, "border-width")
	else SHORTHAND(padding, , "padding")
	else if (name == "border-radius")
	{
	//TODO: implement this
	errorMsg = "border-radius is unimplemented!";
	errorType = ParseErrorType::InvalidProperty;
	return false;
	}

#define STYLEPROP(idd, index, inherit, type, ...) \
	if(id == StylePropertyId::idd) \
	{ \
		type value; \
		if (FromString(str, value)) \
		{ \
			rule.properties.push_back(AddPropertyImpl(sheet, id, value)); \
			return true; \
		} \
		else \
		{ \
			errorMsg = "failed to parse style value!"; \
			errorType = ParseErrorType::InvalidValue; \
			return false; \
		} \
	}
#include "OpenGUI/Style/StylePropertiesDef.h"
#undef STYLEPROP
	return true;
}
