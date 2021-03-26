#include "OpenGUI/CSSParser/CSSParser.h"
#include "peglib.h"
#include <fstream>
#include <regex>

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
			PropertyList	<- Property? (';' _ Property)* _
			Property		<- IDENT w ':' w Value
			Value			<- SizeList / SIZE / COLOR / IDENT / NUM _
			SizeList		<- <SIZE (',' w SIZE){3}> _
			SIZE			<- (<NUM ('px' / '%')>) / <'0'>
			NUM				<- < ([0-9]*"."[0-9]+) / ([0-9]+) >
			COLOR			<- (< IDENT w '(' w CNUM ( w ',' w CNUM)* w ')' >) / (<'#' NUM>)
			CNUM			<- NUM (IDENT / '%')?
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

void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV)
{
	float fC = fV * fS; // Chroma
	float fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1)
	{
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2)
	{
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3)
	{
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4)
	{
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5)
	{
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6)
	{
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else
	{
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
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

	bool FromColorName(std::string_view str, Color4f& value)
	{
		using namespace std::string_view_literals;
		static const std::unordered_map<std::string_view, Color4f> NamedColor({
// pre-defined
{"aliceblue"sv,Color4f(240.f / 255,248.f / 255,255.f / 255, 1.f)},
{"antiquewhite"sv,Color4f(250.f / 255,235.f / 255,215.f / 255, 1.f)},
{"aqua"sv,Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f)},
{"aquamarine"sv,Color4f(127.f / 255,255.f / 255,212.f / 255, 1.f)},
{"azure"sv,Color4f(240.f / 255,255.f / 255,255.f / 255, 1.f)},
{"beige"sv,Color4f(245.f / 255,245.f / 255,220.f / 255, 1.f)},
{"bisque"sv,Color4f(255.f / 255,228.f / 255,196.f / 255, 1.f)},
{"black"sv,Color4f(0.f / 255,0.f / 255,0.f / 255, 1.f)},
{"blanchedalmond"sv,Color4f(255.f / 255,235.f / 255,205.f / 255, 1.f)},
{"blue"sv,Color4f(0.f / 255,0.f / 255,255.f / 255, 1.f)},
{"blueviolet"sv,Color4f(138.f / 255,43.f / 255,226.f / 255, 1.f)},
{"brown"sv,Color4f(165.f / 255,42.f / 255,42.f / 255, 1.f)},
{"burlywood"sv,Color4f(222.f / 255,184.f / 255,135.f / 255, 1.f)},
{"cadetblue"sv,Color4f(95.f / 255,158.f / 255,160.f / 255, 1.f)},
{"chartreuse"sv,Color4f(127.f / 255,255.f / 255,0.f / 255, 1.f)},
{"chocolate"sv,Color4f(210.f / 255,105.f / 255,30.f / 255, 1.f)},
{"coral"sv,Color4f(255.f / 255,127.f / 255,80.f / 255, 1.f)},
{"cornflowerblue"sv,Color4f(100.f / 255,149.f / 255,237.f / 255, 1.f)},
{"cornsilk"sv,Color4f(255.f / 255,248.f / 255,220.f / 255, 1.f)},
{"crimson"sv,Color4f(220.f / 255,20.f / 255,60.f / 255, 1.f)},
{"cyan"sv,Color4f(0.f / 255,255.f / 255,255.f / 255, 1.f)},
{"darkblue"sv,Color4f(0.f / 255,0.f / 255,139.f / 255, 1.f)},
{"darkcyan"sv,Color4f(0.f / 255,139.f / 255,139.f / 255, 1.f)},
{"darkgoldenrod"sv,Color4f(184.f / 255,134.f / 255,11.f / 255, 1.f)},
{"darkgray"sv,Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f)},
{"darkgrey"sv,Color4f(169.f / 255,169.f / 255,169.f / 255, 1.f)},
{"darkgreen"sv,Color4f(0.f / 255,100.f / 255,0.f / 255, 1.f)},
{"darkkhaki"sv,Color4f(189.f / 255,183.f / 255,107.f / 255, 1.f)},
{"darkmagenta"sv,Color4f(139.f / 255,0.f / 255,139.f / 255, 1.f)},
{"darkolivegreen"sv,Color4f(85.f / 255,107.f / 255,47.f / 255, 1.f)},
{"darkorange"sv,Color4f(255.f / 255,140.f / 255,0.f / 255, 1.f)},
{"darkorchid"sv,Color4f(153.f / 255,50.f / 255,204.f / 255, 1.f)},
{"darkred"sv,Color4f(139.f / 255,0.f / 255,0.f / 255, 1.f)},
{"darksalmon"sv,Color4f(233.f / 255,150.f / 255,122.f / 255, 1.f)},
{"darkseagreen"sv,Color4f(143.f / 255,188.f / 255,143.f / 255, 1.f)},
{"darkslateblue"sv,Color4f(72.f / 255,61.f / 255,139.f / 255, 1.f)},
{"darkslategray"sv,Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f)},
{"darkslategrey"sv,Color4f(47.f / 255,79.f / 255,79.f / 255, 1.f)},
{"darkturquoise"sv,Color4f(0.f / 255,206.f / 255,209.f / 255, 1.f)},
{"darkviolet"sv,Color4f(148.f / 255,0.f / 255,211.f / 255, 1.f)},
{"deeppink"sv,Color4f(255.f / 255,20.f / 255,147.f / 255, 1.f)},
{"deepskyblue"sv,Color4f(0.f / 255,191.f / 255,255.f / 255, 1.f)},
{"dimgray"sv,Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f)},
{"dimgrey"sv,Color4f(105.f / 255,105.f / 255,105.f / 255, 1.f)},
{"dodgerblue"sv,Color4f(30.f / 255,144.f / 255,255.f / 255, 1.f)},
{"firebrick"sv,Color4f(178.f / 255,34.f / 255,34.f / 255, 1.f)},
{"floralwhite"sv,Color4f(255.f / 255,250.f / 255,240.f / 255, 1.f)},
{"forestgreen"sv,Color4f(34.f / 255,139.f / 255,34.f / 255, 1.f)},
{"fuchsia"sv,Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f)},
{"gainsboro"sv,Color4f(220.f / 255,220.f / 255,220.f / 255, 1.f)},
{"ghostwhite"sv,Color4f(248.f / 255,248.f / 255,255.f / 255, 1.f)},
{"gold"sv,Color4f(255.f / 255,215.f / 255,0.f / 255, 1.f)},
{"goldenrod"sv,Color4f(218.f / 255,165.f / 255,32.f / 255, 1.f)},
{"gray"sv,Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f)},
{"grey"sv,Color4f(128.f / 255,128.f / 255,128.f / 255, 1.f)},
{"green"sv,Color4f(0.f / 255,128.f / 255,0.f / 255, 1.f)},
{"greenyellow"sv,Color4f(173.f / 255,255.f / 255,47.f / 255, 1.f)},
{"honeydew"sv,Color4f(240.f / 255,255.f / 255,240.f / 255, 1.f)},
{"hotpink"sv,Color4f(255.f / 255,105.f / 255,180.f / 255, 1.f)},
{"indianred"sv,Color4f(205.f / 255,92.f / 255,92.f / 255, 1.f)},
{"indigo"sv,Color4f(75.f / 255,0.f / 255,130.f / 255, 1.f)},
{"ivory"sv,Color4f(255.f / 255,255.f / 255,240.f / 255, 1.f)},
{"khaki"sv,Color4f(240.f / 255,230.f / 255,140.f / 255, 1.f)},
{"lavender"sv,Color4f(230.f / 255,230.f / 255,250.f / 255, 1.f)},
{"lavenderblush"sv,Color4f(255.f / 255,240.f / 255,245.f / 255, 1.f)},
{"lawngreen"sv,Color4f(124.f / 255,252.f / 255,0.f / 255, 1.f)},
{"lemonchiffon"sv,Color4f(255.f / 255,250.f / 255,205.f / 255, 1.f)},
{"lightblue"sv,Color4f(173.f / 255,216.f / 255,230.f / 255, 1.f)},
{"lightcoral"sv,Color4f(240.f / 255,128.f / 255,128.f / 255, 1.f)},
{"lightcyan"sv,Color4f(224.f / 255,255.f / 255,255.f / 255, 1.f)},
{"lightgoldenrodyellow"sv,Color4f(250.f / 255,250.f / 255,210.f / 255, 1.f)},
{"lightgray"sv,Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f)},
{"lightgrey"sv,Color4f(211.f / 255,211.f / 255,211.f / 255, 1.f)},
{"lightgreen"sv,Color4f(144.f / 255,238.f / 255,144.f / 255, 1.f)},
{"lightpink"sv,Color4f(255.f / 255,182.f / 255,193.f / 255, 1.f)},
{"lightsalmon"sv,Color4f(255.f / 255,160.f / 255,122.f / 255, 1.f)},
{"lightseagreen"sv,Color4f(32.f / 255,178.f / 255,170.f / 255, 1.f)},
{"lightskyblue"sv,Color4f(135.f / 255,206.f / 255,250.f / 255, 1.f)},
{"lightslategray"sv,Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f)},
{"lightslategrey"sv,Color4f(119.f / 255,136.f / 255,153.f / 255, 1.f)},
{"lightsteelblue"sv,Color4f(176.f / 255,196.f / 255,222.f / 255, 1.f)},
{"lightyellow"sv,Color4f(255.f / 255,255.f / 255,224.f / 255, 1.f)},
{"lime"sv,Color4f(0.f / 255,255.f / 255,0.f / 255, 1.f)},
{"limegreen"sv,Color4f(50.f / 255,205.f / 255,50.f / 255, 1.f)},
{"linen"sv,Color4f(250.f / 255,240.f / 255,230.f / 255, 1.f)},
{"magenta"sv,Color4f(255.f / 255,0.f / 255,255.f / 255, 1.f)},
{"maroon"sv,Color4f(128.f / 255,0.f / 255,0.f / 255, 1.f)},
{"mediumaquamarine"sv,Color4f(102.f / 255,205.f / 255,170.f / 255, 1.f)},
{"mediumblue"sv,Color4f(0.f / 255,0.f / 255,205.f / 255, 1.f)},
{"mediumorchid"sv,Color4f(186.f / 255,85.f / 255,211.f / 255, 1.f)},
{"mediumpurple"sv,Color4f(147.f / 255,112.f / 255,219.f / 255, 1.f)},
{"mediumseagreen"sv,Color4f(60.f / 255,179.f / 255,113.f / 255, 1.f)},
{"mediumslateblue"sv,Color4f(123.f / 255,104.f / 255,238.f / 255, 1.f)},
{"mediumspringgreen"sv,Color4f(0.f / 255,250.f / 255,154.f / 255, 1.f)},
{"mediumturquoise"sv,Color4f(72.f / 255,209.f / 255,204.f / 255, 1.f)},
{"mediumvioletred"sv,Color4f(199.f / 255,21.f / 255,133.f / 255, 1.f)},
{"midnightblue"sv,Color4f(25.f / 255,25.f / 255,112.f / 255, 1.f)},
{"mintcream"sv,Color4f(245.f / 255,255.f / 255,250.f / 255, 1.f)},
{"mistyrose"sv,Color4f(255.f / 255,228.f / 255,225.f / 255, 1.f)},
{"moccasin"sv,Color4f(255.f / 255,228.f / 255,181.f / 255, 1.f)},
{"navajowhite"sv,Color4f(255.f / 255,222.f / 255,173.f / 255, 1.f)},
{"navy"sv,Color4f(0.f / 255,0.f / 255,128.f / 255, 1.f)},
{"oldlace"sv,Color4f(253.f / 255,245.f / 255,230.f / 255, 1.f)},
{"olive"sv,Color4f(128.f / 255,128.f / 255,0.f / 255, 1.f)},
{"olivedrab"sv,Color4f(107.f / 255,142.f / 255,35.f / 255, 1.f)},
{"orange"sv,Color4f(255.f / 255,165.f / 255,0.f / 255, 1.f)},
{"orangered"sv,Color4f(255.f / 255,69.f / 255,0.f / 255, 1.f)},
{"orchid"sv,Color4f(218.f / 255,112.f / 255,214.f / 255, 1.f)},
{"palegoldenrod"sv,Color4f(238.f / 255,232.f / 255,170.f / 255, 1.f)},
{"palegreen"sv,Color4f(152.f / 255,251.f / 255,152.f / 255, 1.f)},
{"paleturquoise"sv,Color4f(175.f / 255,238.f / 255,238.f / 255, 1.f)},
{"palevioletred"sv,Color4f(219.f / 255,112.f / 255,147.f / 255, 1.f)},
{"papayawhip"sv,Color4f(255.f / 255,239.f / 255,213.f / 255, 1.f)},
{"peachpuff"sv,Color4f(255.f / 255,218.f / 255,185.f / 255, 1.f)},
{"peru"sv,Color4f(205.f / 255,133.f / 255,63.f / 255, 1.f)},
{"pink"sv,Color4f(255.f / 255,192.f / 255,203.f / 255, 1.f)},
{"plum"sv,Color4f(221.f / 255,160.f / 255,221.f / 255, 1.f)},
{"powderblue"sv,Color4f(176.f / 255,224.f / 255,230.f / 255, 1.f)},
{"purple"sv,Color4f(128.f / 255,0.f / 255,128.f / 255, 1.f)},
{"rebeccapurple"sv,Color4f(102.f / 255,51.f / 255,153.f / 255, 1.f)},
{"red"sv,Color4f(255.f / 255,0.f / 255,0.f / 255, 1.f)},
{"rosybrown"sv,Color4f(188.f / 255,143.f / 255,143.f / 255, 1.f)},
{"royalblue"sv,Color4f(65.f / 255,105.f / 255,225.f / 255, 1.f)},
{"saddlebrown"sv,Color4f(139.f / 255,69.f / 255,19.f / 255, 1.f)},
{"salmon"sv,Color4f(250.f / 255,128.f / 255,114.f / 255, 1.f)},
{"sandybrown"sv,Color4f(244.f / 255,164.f / 255,96.f / 255, 1.f)},
{"seagreen"sv,Color4f(46.f / 255,139.f / 255,87.f / 255, 1.f)},
{"seashell"sv,Color4f(255.f / 255,245.f / 255,238.f / 255, 1.f)},
{"sienna"sv,Color4f(160.f / 255,82.f / 255,45.f / 255, 1.f)},
{"silver"sv,Color4f(192.f / 255,192.f / 255,192.f / 255, 1.f)},
{"skyblue"sv,Color4f(135.f / 255,206.f / 255,235.f / 255, 1.f)},
{"slateblue"sv,Color4f(106.f / 255,90.f / 255,205.f / 255, 1.f)},
{"slategray"sv,Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f)},
{"slategrey"sv,Color4f(112.f / 255,128.f / 255,144.f / 255, 1.f)},
{"snow"sv,Color4f(255.f / 255,250.f / 255,250.f / 255, 1.f)},
{"springgreen"sv,Color4f(0.f / 255,255.f / 255,127.f / 255, 1.f)},
{"steelblue"sv,Color4f(70.f / 255,130.f / 255,180.f / 255, 1.f)},
{"tan"sv,Color4f(210.f / 255,180.f / 255,140.f / 255, 1.f)},
{"teal"sv,Color4f(0.f / 255,128.f / 255,128.f / 255, 1.f)},
{"thistle"sv,Color4f(216.f / 255,191.f / 255,216.f / 255, 1.f)},
{"tomato"sv,Color4f(255.f / 255,99.f / 255,71.f / 255, 1.f)},
{"turquoise"sv,Color4f(64.f / 255,224.f / 255,208.f / 255, 1.f)},
{"violet"sv,Color4f(238.f / 255,130.f / 255,238.f / 255, 1.f)},
{"wheat"sv,Color4f(245.f / 255,222.f / 255,179.f / 255, 1.f)},
{"white"sv,Color4f(255.f / 255,255.f / 255,255.f / 255, 1.f)},
{"whitesmoke"sv,Color4f(245.f / 255,245.f / 255,245.f / 255, 1.f)},
{"yellow"sv,Color4f(255.f / 255,255.f / 255,0.f / 255, 1.f)},
{"yellowgreen"sv,Color4f(154.f / 255,205.f / 255,50.f / 255, 1.f)},
// special
{ "transparent"sv, Color4f( 0.f, 0.f, 0.f, 0.f ) }
// customize (if required)
		});

		std::string lower(str);
		std::transform(str.cbegin(), str.cend(), lower.begin(),
			[](unsigned char c) { return std::tolower(c); });
		auto it = NamedColor.find(lower);
		if (it != NamedColor.cend())
		{
			value = it->second;
			return true;
		}
		return false;
	}
	bool FromString(std::string_view str, Color4f& value)
	{
		
		auto grammar = R"(
			Color	<- CALLS / CALL / HEX / IDENT
			CALL	<- FUNC '(' _ CNUM _ ',' _ CNUM _ ',' _ CNUM _ (',' _ CNUM _)? ')'
			CALLS	<- FUNC '(' _ CNUM __ CNUM __ CNUM _ ('/' _ CNUM _)? ')'
			FUNC	<- <'rgba' / 'rgb' / 'hsl' / 'hsla'>
			NUM		<- < ([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+) >
			CNUM	<- < NUM ('deg' / 'rad' / 'grad' / 'turn' / '%')? >
			HEX		<- '#' < [0-9a-fA-F] >
			IDENT	<- < [a-zA-Z] [a-zA-Z0-9-]* >
			~_		<- [ ]*
			~__		<- [ ]+
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
		auto token = [](SemanticValues& vs)
		{
			return vs.token(0);
		};
		parser["CNUM"] = token;

		parser["FUNC"] = [](SemanticValues& vs)
		{
			return vs.choice() / 2;
		};
		auto CALL = [](SemanticValues& vs)
		{
			size_t type = any_move<size_t>(vs[0]);
			Color4f color = Color4f::vector_one();
			auto ParseCNUM = [](string_view str, bool mustBePercent, bool normalize = true)
			{
				bool isPercent = false;
				auto valuestr = str;
				if (ends_with(str, "%"))
				{
					isPercent = true;
					valuestr = str.substr(0, str.length() - 1);
				}
				else if (mustBePercent)
					throw parse_error("invalid number, except percent.");
				float value;
				if (!FromString(valuestr, value))
					throw parse_error("invalid number.");
				if (isPercent && !mustBePercent)
					value = value / 100;
				else if (normalize)
					value = roundf(value) / 255;
				return value;
			};
			int size = vs.size();
			if (size < 4)
				throw parse_error("not enough parameter for color function.");
			if (size > 5)
				throw parse_error("too many parameter for color function.");
			auto first = any_move<string_view>(vs[1]);
			if (type == 1)
			{
				int angleType;
				string_view anglestr;
				if (ends_with(first, "deg"))
				{
					anglestr = first.substr(0, first.length() - 3);
					angleType = 0;
				}
				else if (ends_with(first, "rad"))
				{
					anglestr = first.substr(0, first.length() - 3);
					angleType = 1;
				}
				else if (ends_with(first, "grad"))
				{
					anglestr = first.substr(0, first.length() - 4);
					angleType = 2;
				}
				else if (ends_with(first, "turn"))
				{
					anglestr = first.substr(0, first.length() - 4);
					angleType = 3;
				}
				else if (ends_with(first, "%"))
				{
					throw parse_error("hue can not be percent value.");
				}
				else
				{
					anglestr = first.substr(0, first.length() - 3);
					angleType = 0;
				}
				float value;
				if(!FromString(anglestr, value))
					throw parse_error("invalid number.");
				if (angleType == 1)
					value = value / math::PI;// rad2deg(value);
				else if (angleType == 2)
					value = value * 0.9;// grad2deg(value);
				else if (angleType == 3)
					value = value * 360;// turn2deg(value);
				color.X = value;
			}
			else
				color.X = ParseCNUM(first, false);
			auto second = any_move<string_view>(vs[2]);
			color.Y = ParseCNUM(second, type == 1);
			auto third = any_move<string_view>(vs[3]);
			color.Z = ParseCNUM(third, type == 1);
			if (vs.size() == 5)
			{
				auto forth = any_move<string_view>(vs[4]);
				color.W = ParseCNUM(forth, false, false);
			}
			if (type == 1)
			{
				Color4f hsv = color;
				HSVtoRGB(hsv.X, hsv.Y, hsv.Z, color.X, color.Y, color.Z);
			}
			return color;
		};
		parser["CALLS"] = CALL;
		parser["CALL"] = CALL;

		parser["HEX"] = [](SemanticValues& vs)
		{
			Color4f color = Color4f::vector_one();
			auto str = vs.token();
			auto S2H = [](string_view hex)
			{
				string hexstr = {hex.begin(), hex.end()};
				try
				{
					return stoi(hexstr, nullptr, 16);
				}
				catch (std::exception)
				{
					throw parse_error("invalid hex value");
				}
			};
			auto length = str.length();
			string fullstr;
			if (length == 3 || length == 4)
			{
				fullstr.push_back(str[0]); fullstr.push_back(str[0]);
				fullstr.push_back(str[1]); fullstr.push_back(str[1]);
				fullstr.push_back(str[2]); fullstr.push_back(str[2]);
				if(length == 4)
				{
					fullstr.push_back(str[3]); fullstr.push_back(str[3]);
				}
				length = length * 2;
				str = fullstr;
			}
			if (length == 6 || length == 8)
			{
				color.X = S2H(str.substr(0, 2)) / 255.0;
				color.Y = S2H(str.substr(2, 2)) / 255.0;
				color.Z = S2H(str.substr(4, 2)) / 255.0;
				if (length == 8)
				{
					color.W = S2H(str.substr(6, 2)) / 255.0;
				}
			}
			else
			{
				throw parse_error("invalid hex value");
			}
			return color;
		};

		parser["IDENT"] = [](SemanticValues& vs)
		{
			Color4f res;
			if(!FromColorName(vs.token(), res))
				throw parse_error("invalid color name");
			return res;
		};
		auto forward = [](SemanticValues& vs)
		{
			return vs[0];
		};
		parser["Color"] = forward;

		return parser.parse(str, value);
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
		std::regex zero("0(\\.0+)?");
		if (std::regex_match(str.begin(), str.end(), zero))
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
