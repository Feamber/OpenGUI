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
		using namespace std::string_view_literals;
		static const std::unordered_map<std::string_view, Color4f> NamedColor =
		{ 
{"AliceBlue"sv,Color4f({240.f / 255,248.f / 255,255.f / 255})},
{"AntiqueWhite"sv,Color4f({250.f / 255,235.f / 255,215.f / 255})},
{"Aqua"sv,Color4f({0.f / 255,255.f / 255,255.f / 255})},
{"Aquamarine"sv,Color4f({127.f / 255,255.f / 255,212.f / 255})},
{"Azure"sv,Color4f({240.f / 255,255.f / 255,255.f / 255})},
{"Beige"sv,Color4f({245.f / 255,245.f / 255,220.f / 255})},
{"Bisque"sv,Color4f({255.f / 255,228.f / 255,196.f / 255})},
{"Black"sv,Color4f({0.f / 255,0.f / 255,0.f / 255})},
{"BlanchedAlmond"sv,Color4f({255.f / 255,235.f / 255,205.f / 255})},
{"Blue"sv,Color4f({0.f / 255,0.f / 255,255.f / 255})},
{"BlueViolet"sv,Color4f({138.f / 255,43.f / 255,226.f / 255})},
{"Brown"sv,Color4f({165.f / 255,42.f / 255,42.f / 255})},
{"BurlyWood"sv,Color4f({222.f / 255,184.f / 255,135.f / 255})},
{"CadetBlue"sv,Color4f({95.f / 255,158.f / 255,160.f / 255})},
{"Chartreuse"sv,Color4f({127.f / 255,255.f / 255,0.f / 255})},
{"Chocolate"sv,Color4f({210.f / 255,105.f / 255,30.f / 255})},
{"Coral"sv,Color4f({255.f / 255,127.f / 255,80.f / 255})},
{"CornflowerBlue"sv,Color4f({100.f / 255,149.f / 255,237.f / 255})},
{"Cornsilk"sv,Color4f({255.f / 255,248.f / 255,220.f / 255})},
{"Crimson"sv,Color4f({220.f / 255,20.f / 255,60.f / 255})},
{"Cyan"sv,Color4f({0.f / 255,255.f / 255,255.f / 255})},
{"DarkBlue"sv,Color4f({0.f / 255,0.f / 255,139.f / 255})},
{"DarkCyan"sv,Color4f({0.f / 255,139.f / 255,139.f / 255})},
{"DarkGoldenRod"sv,Color4f({184.f / 255,134.f / 255,11.f / 255})},
{"DarkGray"sv,Color4f({169.f / 255,169.f / 255,169.f / 255})},
{"DarkGrey"sv,Color4f({169.f / 255,169.f / 255,169.f / 255})},
{"DarkGreen"sv,Color4f({0.f / 255,100.f / 255,0.f / 255})},
{"DarkKhaki"sv,Color4f({189.f / 255,183.f / 255,107.f / 255})},
{"DarkMagenta"sv,Color4f({139.f / 255,0.f / 255,139.f / 255})},
{"DarkOliveGreen"sv,Color4f({85.f / 255,107.f / 255,47.f / 255})},
{"DarkOrange"sv,Color4f({255.f / 255,140.f / 255,0.f / 255})},
{"DarkOrchid"sv,Color4f({153.f / 255,50.f / 255,204.f / 255})},
{"DarkRed"sv,Color4f({139.f / 255,0.f / 255,0.f / 255})},
{"DarkSalmon"sv,Color4f({233.f / 255,150.f / 255,122.f / 255})},
{"DarkSeaGreen"sv,Color4f({143.f / 255,188.f / 255,143.f / 255})},
{"DarkSlateBlue"sv,Color4f({72.f / 255,61.f / 255,139.f / 255})},
{"DarkSlateGray"sv,Color4f({47.f / 255,79.f / 255,79.f / 255})},
{"DarkSlateGrey"sv,Color4f({47.f / 255,79.f / 255,79.f / 255})},
{"DarkTurquoise"sv,Color4f({0.f / 255,206.f / 255,209.f / 255})},
{"DarkViolet"sv,Color4f({148.f / 255,0.f / 255,211.f / 255})},
{"DeepPink"sv,Color4f({255.f / 255,20.f / 255,147.f / 255})},
{"DeepSkyBlue"sv,Color4f({0.f / 255,191.f / 255,255.f / 255})},
{"DimGray"sv,Color4f({105.f / 255,105.f / 255,105.f / 255})},
{"DimGrey"sv,Color4f({105.f / 255,105.f / 255,105.f / 255})},
{"DodgerBlue"sv,Color4f({30.f / 255,144.f / 255,255.f / 255})},
{"FireBrick"sv,Color4f({178.f / 255,34.f / 255,34.f / 255})},
{"FloralWhite"sv,Color4f({255.f / 255,250.f / 255,240.f / 255})},
{"ForestGreen"sv,Color4f({34.f / 255,139.f / 255,34.f / 255})},
{"Fuchsia"sv,Color4f({255.f / 255,0.f / 255,255.f / 255})},
{"Gainsboro"sv,Color4f({220.f / 255,220.f / 255,220.f / 255})},
{"GhostWhite"sv,Color4f({248.f / 255,248.f / 255,255.f / 255})},
{"Gold"sv,Color4f({255.f / 255,215.f / 255,0.f / 255})},
{"GoldenRod"sv,Color4f({218.f / 255,165.f / 255,32.f / 255})},
{"Gray"sv,Color4f({128.f / 255,128.f / 255,128.f / 255})},
{"Grey"sv,Color4f({128.f / 255,128.f / 255,128.f / 255})},
{"Green"sv,Color4f({0.f / 255,128.f / 255,0.f / 255})},
{"GreenYellow"sv,Color4f({173.f / 255,255.f / 255,47.f / 255})},
{"HoneyDew"sv,Color4f({240.f / 255,255.f / 255,240.f / 255})},
{"HotPink"sv,Color4f({255.f / 255,105.f / 255,180.f / 255})},
{"IndianRed"sv,Color4f({205.f / 255,92.f / 255,92.f / 255})},
{"Indigo"sv,Color4f({75.f / 255,0.f / 255,130.f / 255})},
{"Ivory"sv,Color4f({255.f / 255,255.f / 255,240.f / 255})},
{"Khaki"sv,Color4f({240.f / 255,230.f / 255,140.f / 255})},
{"Lavender"sv,Color4f({230.f / 255,230.f / 255,250.f / 255})},
{"LavenderBlush"sv,Color4f({255.f / 255,240.f / 255,245.f / 255})},
{"LawnGreen"sv,Color4f({124.f / 255,252.f / 255,0.f / 255})},
{"LemonChiffon"sv,Color4f({255.f / 255,250.f / 255,205.f / 255})},
{"LightBlue"sv,Color4f({173.f / 255,216.f / 255,230.f / 255})},
{"LightCoral"sv,Color4f({240.f / 255,128.f / 255,128.f / 255})},
{"LightCyan"sv,Color4f({224.f / 255,255.f / 255,255.f / 255})},
{"LightGoldenRodYellow"sv,Color4f({250.f / 255,250.f / 255,210.f / 255})},
{"LightGray"sv,Color4f({211.f / 255,211.f / 255,211.f / 255})},
{"LightGrey"sv,Color4f({211.f / 255,211.f / 255,211.f / 255})},
{"LightGreen"sv,Color4f({144.f / 255,238.f / 255,144.f / 255})},
{"LightPink"sv,Color4f({255.f / 255,182.f / 255,193.f / 255})},
{"LightSalmon"sv,Color4f({255.f / 255,160.f / 255,122.f / 255})},
{"LightSeaGreen"sv,Color4f({32.f / 255,178.f / 255,170.f / 255})},
{"LightSkyBlue"sv,Color4f({135.f / 255,206.f / 255,250.f / 255})},
{"LightSlateGray"sv,Color4f({119.f / 255,136.f / 255,153.f / 255})},
{"LightSlateGrey"sv,Color4f({119.f / 255,136.f / 255,153.f / 255})},
{"LightSteelBlue"sv,Color4f({176.f / 255,196.f / 255,222.f / 255})},
{"LightYellow"sv,Color4f({255.f / 255,255.f / 255,224.f / 255})},
{"Lime"sv,Color4f({0.f / 255,255.f / 255,0.f / 255})},
{"LimeGreen"sv,Color4f({50.f / 255,205.f / 255,50.f / 255})},
{"Linen"sv,Color4f({250.f / 255,240.f / 255,230.f / 255})},
{"Magenta"sv,Color4f({255.f / 255,0.f / 255,255.f / 255})},
{"Maroon"sv,Color4f({128.f / 255,0.f / 255,0.f / 255})},
{"MediumAquaMarine"sv,Color4f({102.f / 255,205.f / 255,170.f / 255})},
{"MediumBlue"sv,Color4f({0.f / 255,0.f / 255,205.f / 255})},
{"MediumOrchid"sv,Color4f({186.f / 255,85.f / 255,211.f / 255})},
{"MediumPurple"sv,Color4f({147.f / 255,112.f / 255,219.f / 255})},
{"MediumSeaGreen"sv,Color4f({60.f / 255,179.f / 255,113.f / 255})},
{"MediumSlateBlue"sv,Color4f({123.f / 255,104.f / 255,238.f / 255})},
{"MediumSpringGreen"sv,Color4f({0.f / 255,250.f / 255,154.f / 255})},
{"MediumTurquoise"sv,Color4f({72.f / 255,209.f / 255,204.f / 255})},
{"MediumVioletRed"sv,Color4f({199.f / 255,21.f / 255,133.f / 255})},
{"MidnightBlue"sv,Color4f({25.f / 255,25.f / 255,112.f / 255})},
{"MintCream"sv,Color4f({245.f / 255,255.f / 255,250.f / 255})},
{"MistyRose"sv,Color4f({255.f / 255,228.f / 255,225.f / 255})},
{"Moccasin"sv,Color4f({255.f / 255,228.f / 255,181.f / 255})},
{"NavajoWhite"sv,Color4f({255.f / 255,222.f / 255,173.f / 255})},
{"Navy"sv,Color4f({0.f / 255,0.f / 255,128.f / 255})},
{"OldLace"sv,Color4f({253.f / 255,245.f / 255,230.f / 255})},
{"Olive"sv,Color4f({128.f / 255,128.f / 255,0.f / 255})},
{"OliveDrab"sv,Color4f({107.f / 255,142.f / 255,35.f / 255})},
{"Orange"sv,Color4f({255.f / 255,165.f / 255,0.f / 255})},
{"OrangeRed"sv,Color4f({255.f / 255,69.f / 255,0.f / 255})},
{"Orchid"sv,Color4f({218.f / 255,112.f / 255,214.f / 255})},
{"PaleGoldenRod"sv,Color4f({238.f / 255,232.f / 255,170.f / 255})},
{"PaleGreen"sv,Color4f({152.f / 255,251.f / 255,152.f / 255})},
{"PaleTurquoise"sv,Color4f({175.f / 255,238.f / 255,238.f / 255})},
{"PaleVioletRed"sv,Color4f({219.f / 255,112.f / 255,147.f / 255})},
{"PapayaWhip"sv,Color4f({255.f / 255,239.f / 255,213.f / 255})},
{"PeachPuff"sv,Color4f({255.f / 255,218.f / 255,185.f / 255})},
{"Peru"sv,Color4f({205.f / 255,133.f / 255,63.f / 255})},
{"Pink"sv,Color4f({255.f / 255,192.f / 255,203.f / 255})},
{"Plum"sv,Color4f({221.f / 255,160.f / 255,221.f / 255})},
{"PowderBlue"sv,Color4f({176.f / 255,224.f / 255,230.f / 255})},
{"Purple"sv,Color4f({128.f / 255,0.f / 255,128.f / 255})},
{"RebeccaPurple"sv,Color4f({102.f / 255,51.f / 255,153.f / 255})},
{"Red"sv,Color4f({255.f / 255,0.f / 255,0.f / 255})},
{"RosyBrown"sv,Color4f({188.f / 255,143.f / 255,143.f / 255})},
{"RoyalBlue"sv,Color4f({65.f / 255,105.f / 255,225.f / 255})},
{"SaddleBrown"sv,Color4f({139.f / 255,69.f / 255,19.f / 255})},
{"Salmon"sv,Color4f({250.f / 255,128.f / 255,114.f / 255})},
{"SandyBrown"sv,Color4f({244.f / 255,164.f / 255,96.f / 255})},
{"SeaGreen"sv,Color4f({46.f / 255,139.f / 255,87.f / 255})},
{"SeaShell"sv,Color4f({255.f / 255,245.f / 255,238.f / 255})},
{"Sienna"sv,Color4f({160.f / 255,82.f / 255,45.f / 255})},
{"Silver"sv,Color4f({192.f / 255,192.f / 255,192.f / 255})},
{"SkyBlue"sv,Color4f({135.f / 255,206.f / 255,235.f / 255})},
{"SlateBlue"sv,Color4f({106.f / 255,90.f / 255,205.f / 255})},
{"SlateGray"sv,Color4f({112.f / 255,128.f / 255,144.f / 255})},
{"SlateGrey"sv,Color4f({112.f / 255,128.f / 255,144.f / 255})},
{"Snow"sv,Color4f({255.f / 255,250.f / 255,250.f / 255})},
{"SpringGreen"sv,Color4f({0.f / 255,255.f / 255,127.f / 255})},
{"SteelBlue"sv,Color4f({70.f / 255,130.f / 255,180.f / 255})},
{"Tan"sv,Color4f({210.f / 255,180.f / 255,140.f / 255})},
{"Teal"sv,Color4f({0.f / 255,128.f / 255,128.f / 255})},
{"Thistle"sv,Color4f({216.f / 255,191.f / 255,216.f / 255})},
{"Tomato"sv,Color4f({255.f / 255,99.f / 255,71.f / 255})},
{"Turquoise"sv,Color4f({64.f / 255,224.f / 255,208.f / 255})},
{"Violet"sv,Color4f({238.f / 255,130.f / 255,238.f / 255})},
{"Wheat"sv,Color4f({245.f / 255,222.f / 255,179.f / 255})},
{"White"sv,Color4f({255.f / 255,255.f / 255,255.f / 255})},
{"WhiteSmoke"sv,Color4f({245.f / 255,245.f / 255,245.f / 255})},
{"Yellow"sv,Color4f({255.f / 255,255.f / 255,0.f / 255})},
{"YellowGreen"sv,Color4f({154.f / 255,205.f / 255,50.f / 255})}
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
