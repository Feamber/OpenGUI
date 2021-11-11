

#define DLL_IMPLEMENTATION
#include <cstddef>
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "Parse/peglib.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Style2/Selector.h"
#include <functional>
#include <fstream>


namespace OGUI
{
	std::optional<StyleSheet> ParseCSS(std::string_view str)
	{
		static auto grammar = R"(
			Stylesheet			<- _ (StyleRule / Keyframes)*
			StyleRule			<- SelectorList _ '{' _ PropertyList _ '}' _
			Keyframes			<- '@keyframes' w <IDENT> _ '{' _ KeyframeBlock* _ '}' _
			KeyframeBlock		<- <KeyframeSelector> (w ',' w <KeyframeSelector> w)* _ '{' _ PropertyList _ '}' _
			SelectorList		<- ComplexSelector (',' w ComplexSelector)* _
			ComplexSelector		<- Selector ComplexPart* ('::' <pseudoElement>)?
			ComplexPart			<- ([ ]+ Selector) / (w '>' w Selector)
			Selector			<- SelectorPart+
			SelectorPart		<- "*" / ('.' <IDENT>) / ('#' <IDENT>) / <IDENT>  / (':' <IDENT>)
			PropertyList		<- Property? (_ ';' _ Property)* _ ';'?
			Property			<- <IDENT> w ':' w <(!(';'/'}') .)*> _
			~pseudoElement		<- 'before'/'after'
			~KeyframeSelector	<- ( NUM  '%') / 'from' / 'to'
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			~NUM				<- ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+))
			~blockcomment		<- '/*' (!'*/' .)* '*/'
			~_					<- ([ \t\r\n] / blockcomment)*
			~w					<- [ ]*
		)";
		using namespace peg;
		using namespace std;
		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				parser.log = [](size_t line, size_t col, const string& msg)
				{
					cerr << line << ":" << col << ": " << msg << "\n";
				};
				ok = parser.load_grammar(grammar);
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return {};

		StyleSheet sheet;
		parser["Property"] = [](SemanticValues& vs)
		{
			auto name = vs.token();
			auto value = vs.token(1);
			return make_pair(name, value);
		};
        using PropertyList = std::vector<std::pair<std::string_view, std::string_view>>;
		parser["PropertyList"] = [&](SemanticValues& vs)
		{
			PropertyList pairs;
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
			{
				auto part = any_move<StyleSelector::Part>(p);
				if(part.type == StyleSelector::PseudoClass)
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
			if (vs.tokens.size() > 0)
				complexSelector.SetPseudoElement(vs.token(0));
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
			auto list = any_move<PropertyList>(vs[1]);
			int animCount = -1;
			for (auto& pair : list)
			{
				if (pair.first == "animation-name")
                {
					animCount = std::count(pair.second.begin(), pair.second.end(), ',') + 1;
                    break;
                }
            }
			if(animCount > 0)
				rule.animation.resize(animCount);
			for (auto& pair : list)
			{
				std::string errorMsg;
				if (!ParseProperty(sheet.storage, pair.first, pair.second, rule, errorMsg, animCount))
					throw parse_error(errorMsg.c_str());
			}
			int ruleIndex = sheet.styleRules.size();
			sheet.styleRules.push_back(std::move(rule));
			auto selectorList = any_move<vector<StyleComplexSelector>>(vs[0]);
			for (auto& sel : selectorList)
			{
				sel.ruleIndex = ruleIndex;
				sheet.styleSelectors.push_back(sel);
			}
		};
		using Key = StyleKeyframes::Key;
		using AnimationCurve = std::vector<Key>;
		parser["KeyframeBlock"] = [&](SemanticValues& vs)
		{
			
			AnimationCurve curve;
			for (auto& key : vs.tokens)
			{
				Key k;
				if (key == "from")
					k.percentage = 0.f;
				else if (key == "to")
					k.percentage = 1.f;
				else if (std::ends_with(key, "%"))
				{
					if (!ParseValue(key.substr(0, key.length()), k.percentage))
						throw parse_error("invalid number");
					k.percentage /= 100;
				}
				else
					throw parse_error("invalid keyframe selector");

				curve.push_back(k);
			}
			StyleRule frame;
			auto list = any_move<PropertyList>(vs[0]);
			for (auto& pair : list)
			{
				std::string errorMsg;
				if (!ParseProperty(sheet.storage, pair.first, pair.second, frame, errorMsg))
				{
					throw parse_error(errorMsg.c_str());
				}
			}
			std::sort(frame.properties.begin(), frame.properties.end(), [](const StyleProperty& a, const StyleProperty& b)
				{
					return (int)a.id < (int)b.id;
				});
			size_t ruleIndex = sheet.styleRules.size();
			sheet.styleRules.push_back(std::move(frame));
			return std::make_pair(curve, ruleIndex);
		};
		parser["Keyframes"] = [&](SemanticValues& vs)
		{
			StyleKeyframes keyframes;
			auto name = vs.token();
			keyframes.name = {name.begin(), name.end()};
			for (auto& p : vs)
			{
				auto pair = any_move<std::pair<AnimationCurve, size_t>>(p);
				for(auto& k : pair.first)
				{
					k.frameIndex = pair.second;
					keyframes.keys.push_back(k);
				}
			}
			sheet.styleKeyframes.push_back(std::move(keyframes));
		};
		parser.enable_packrat_parsing();
		if (parser.parse(str))
			return sheet;
		return {};
	}

	std::optional<StyleComplexSelector> ParseSelector(std::string_view str)
	{
		static auto grammar = R"(
			ComplexSelector		<- Selector ComplexPart*
			ComplexPart			<- ([ ]+ Selector) / (w '>' w Selector)
			Selector			<- SelectorPart+
			SelectorPart		<- "*" / ('.' <IDENT>) / ('#' <IDENT>) / <IDENT> / (':' <IDENT>)
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			~w					<- [ ]*
		)"; 
		using namespace peg;
		using namespace std;

		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				parser.log = [](size_t line, size_t col, const string& msg)
				{
					cerr << line << ":" << col << ": " << msg << "\n";
				};
				ok = parser.load_grammar(grammar);
				if (ok)
				{
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
				}
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return {};
		StyleComplexSelector value;
		parser.enable_packrat_parsing();
		if (parser.parse(str, value))
			return value;
		return {};
	}

	
    std::optional<InlineStyle> ParseInlineStyle(std::string_view str)
	{
		static auto grammar = R"(
			PropertyList		<- Property? (_ ';' _ Property)* _ ';'?
			Property			<- <IDENT> w ':' w <(!(';') .)*> _
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			~blockcomment		<- '/*' (!'*/' .)* '*/'
			~_					<- ([ \t\r\n] / blockcomment)*
			~w					<- [ ]*
		)";
		using namespace peg;
		using namespace std;
		struct ParserInitializer
		{
			parser parser;
			bool ok;
			ParserInitializer()
			{
				parser.log = [](size_t line, size_t col, const string& msg)
				{
					cerr << line << ":" << col << ": " << msg << "\n";
				};
				ok = parser.load_grammar(grammar);
			}
		};
		static ParserInitializer parserInitializer; //do once
		auto& parser = parserInitializer.parser;

		if (!parserInitializer.ok)
			return {};

		InlineStyle sheet;
		parser["Property"] = [](SemanticValues& vs)
		{
			auto name = vs.token();
			auto value = vs.token(1);
			return make_pair(name, value);
		};
        using PropertyList = std::vector<std::pair<std::string_view, std::string_view>>;
		parser["PropertyList"] = [&](SemanticValues& vs)
		{
			PropertyList list;
			for (auto& p : vs)
				list.push_back(any_move<pair<string_view, string_view>>(p));

			int animCount = -1;
			for (auto& pair : list)
			{
				if (pair.first == "animation-name")
                {
					animCount = std::count(pair.second.begin(), pair.second.end(), ',') + 1;
                    break;
                }
            }
			if(animCount > 0)
				sheet.rule.animation.resize(animCount);
			for (auto& pair : list)
			{
				std::string errorMsg;
				if (!ParseProperty(sheet.storage, pair.first, pair.second, sheet.rule, errorMsg, animCount))
					throw parse_error(errorMsg.c_str());
			}
		};

		parser.enable_packrat_parsing();
		if (parser.parse(str))
			return sheet;
		return {};
	}

	std::optional<StyleSheet> ParseCSSFile(std::string path)
	{
		std::ifstream ifs(path);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		auto styleSheet = ParseCSS(content);
		if(styleSheet) styleSheet->path = path;
		return styleSheet;
	}

	
    bool ParseProperty(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg, int animCount)
	{
		auto& registry = StyleRegistry::Get();
		for(auto& desc : registry.descriptions)
		{
			bool valid = desc.ParseProperties(sheet, name, value, rule, errorMsg);
			if(!errorMsg.empty())
				return false;
			if(valid)
				return true;
		}
		bool valid = AnimStyle::ParseProperties(sheet, name, value, rule, errorMsg, animCount);
		if(!errorMsg.empty())
			return false;
		if(valid)
			return true;
		errorMsg = "unknown property.";
		return false;
	}
}
