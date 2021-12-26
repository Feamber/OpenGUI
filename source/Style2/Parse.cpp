#include <cstddef>
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Selector.h"
#include <functional>
#include <fstream>
#include <string_view>
#include "OpenGUI/Context.h"
#include "OpenGUI/Style2/peglib.h"
#include <sstream>
#include "../Text/godot/font.h"
#include "../Text/godot/config.h"
#include "OpenGUI/Context.h"


namespace OGUI::CSSParser
{
	struct StyleSheetContext
	{
		StyleSheet* styleSheet = nullptr;
	};
	struct FontfaceContext
	{
		StyleFont* font = nullptr;
	};
	template<class T>
	void PushContext(std::any& dt, T&& v)
	{
		std::any_cast<ParseContext&>(dt).emplace_back(std::forward<T>(v));
	}
	void PopContext(std::any& dt)
	{
		std::any_cast<ParseContext&>(dt).pop_back();
	}

	std::string_view GetUtilsGrammar()
	{
		static auto grammar = R"(	
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			Name				<- <IDENT>
			~blockcomment		<- '/*' (!'*/' .)* '*/'
			~_					<- ([ \t\r\n] / blockcomment)*
			~w					<- [ \t]*
			~PropertyList		<- Property? (_ ';' _ Property)* _ ';'?
			String				 <- '"'< (!'"' .)* >'"'
			Percentage		   <- Number%
			Number				<- ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+))
			URL					   <- 'url('< (!')' .)* >')'
			GlobalValue			<- inherit / initial  / unset
		)";
		return grammar;
	}


	void SetupUtilsActions(peg::parser& parser)
	{
		using namespace peg;
		parser["URL"] = [](SemanticValues& vs)
		{
			return vs.token();
		};
		parser["Name"] = [](SemanticValues& vs)
		{
			return vs.token();
		};
		parser["String"] = [](SemanticValues& vs)
		{
			return vs.token();
		};
		parser["Number"] = [](SemanticValues& vs)
		{
			return ParseNumber(vs.token());
		};
		parser["Percentage"] = [](SemanticValues& vs)
		{
			return std::any_cast<float>(vs[0]) / 100.f;
		};
		parser["GlobalValue"] = [](SemanticValues& vs)
		{
			return (StyleKeyword)vs.choice();
		};
	}
	

	std::string_view GetStyleRuleGrammar()
	{
		static auto grammar = R"(
			StyleRule			<- SelectorList _ '{' _ PropertyList _ '}' _
			SelectorList		<- ComplexSelector (',' _ ComplexSelector)* _
			ComplexSelector		<- Selector ComplexPart* ('::' PseudoElement)?
			PseudoElement		<- 'before'/'after'
			ComplexPart			<- (w Selector) / (w '>' w Selector)
			Selector			<- SelectorPart+
			SelectorPart		<- '*' / ('.' <IDENT>) / ('#' <IDENT>) / <IDENT>  / (':' <IDENT>)
		)";
		return grammar;
	}

	void SetupStyleRuleActions(peg::parser& parser)
	{
		using namespace peg;
		parser["SelectorPart"] = [](SemanticValues& vs)
		{
			std::string value;
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
		parser["ComplexPart"] = [](SemanticValues& vs)
		{
			StyleSelector selector = any_move<StyleSelector>(vs[0]);
			selector.relationship = vs.choice() == 0 ? StyleSelectorRelationship::Descendent : StyleSelectorRelationship::Child;
			return selector;
		};
		parser["ComplexSelector"] = [](SemanticValues& vs)
		{
			StyleComplexSelector complexSelector;
			if (vs.back().type() == typeid(PseudoElements))
				complexSelector.pseudoElem = std::any_cast<PseudoElements>(vs.back());
			vs.pop_back();
			for (auto& p : vs)
				complexSelector.selectors.push_back(any_move<StyleSelector>(p));
			//complexSelector.ruleIndex = vs.line_info().first;
			complexSelector.UpdateSpecificity();
			return complexSelector;
		};
		parser["PseudoElement"] = [](SemanticValues& vs)
		{
			return vs.choice() == 0 ? PseudoElements::Before : PseudoElements::After;
		};
		parser["SelectorList"] = [](SemanticValues& vs)
		{
			vector<StyleComplexSelector> selectorList;
			for (auto& p : vs)
				selectorList.push_back(any_move<StyleComplexSelector>(p));
			return selectorList;
		};
		parser["StyleRule"].enter = [](const char*, size_t, std::any& dt)
		{
			auto ctx = GetContext<StyleSheetContext&>(dt);
			PushContext(dt, PropertyListContext{&ctx.styleSheet->storage, &ctx.styleSheet->styleRules.emplace_back()});
		};
		parser["StyleRule"] = [](SemanticValues& vs,  std::any& dt)
		{
			PopContext(dt);
			auto ctx = GetContext<StyleSheetContext&>(dt);
			auto selectorList = any_move<vector<StyleComplexSelector>>(vs[0]);
			auto ruleIndex  = ctx.styleSheet->styleRules.size()-1;
			for(auto& selector : selectorList)
			{
				selector.ruleIndex = ruleIndex;
				ctx.styleSheet->styleSelectors.push_back(std::move(selector));
			}
		};
	}

	std::string_view GetKeyframeGrammar()
	{
		static auto grammar = R"(	
			Keyframes			<- '@keyframes' w <IDENT> _ '{' _ KeyframeBlock* _ '}' _
			KeyframeBlock		<- KeyframeSelectorList _ '{' _ PropertyList _ '}' _
			KeyframeSelectorList <- <KeyframeSelector> (w ',' w <KeyframeSelector> w)*
			KeyframeSelector	<- ( Number  '%') / 'from' / 'to'
		)";
		return grammar;
	}
	
	void SetupKeyFrameActions(peg::parser& parser)
	{
		using namespace peg;
		using Key = StyleKeyframes::Key;
		using AnimationCurve = std::vector<Key>;
		parser["KeyframeBlock"].enter = [](const char*, size_t, std::any& dt)
		{
			auto ctx = GetContext<StyleSheetContext&>(dt);
			PushContext(dt, PropertyListContext{&ctx.styleSheet->storage, &ctx.styleSheet->styleRules.emplace_back()});
		};
		parser["KeyframeBlock"] = [](SemanticValues& vs,  std::any& dt)
		{
			PopContext(dt);
			auto ctx = GetContext<StyleSheetContext&>(dt);
			auto ruleIndex  = ctx.styleSheet->styleRules.size()-1;
			return std::make_pair(any_move<AnimationCurve>(vs[0]), ruleIndex);
		};
		parser["KeyframeSelectorList"] = [](SemanticValues& vs)
		{
			AnimationCurve list;
			for(auto& k : vs)
				list.emplace_back(std::any_cast<float>(k));
			return list;
		};
		parser["KeyframeSelector"] = [](SemanticValues& vs)
		{
			if(vs.choice() == 0)
				return vs[0];
			if(vs.choice() == 1)
				return std::any(0.f);
			else
				return std::any(1.f);
		};
		parser["Keyframes"] = [](SemanticValues& vs,  std::any& dt)
		{
			auto ctx = GetContext<StyleSheetContext&>(dt);
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
			ctx.styleSheet->styleKeyframes.push_back(std::move(keyframes));
		};
	}


	std::string_view GetFontfaceGrammar()
	{
		static auto grammar = R"(
			Fontface			<- '@font-face' _ '{' _ FontPropertyList _ '}' _
			~FontPropertyList <- FontProperty? (_ ';' _ FontProperty)* _ ';'?
			FontProperty 	  <- FontPropFace / FontPropSrc
			FontPropFace	 <- 'font-family' _':' _ (Name / String)
			FontPropSrc		  <- 'src' _ ':' _ FontSrc (_ ',' _ FontSrc)
			FontSrc				 <- URL FontFormat?
			~FontFormat		 <- 'format('< (!')' .)* >')'
		)";
		return grammar;
	}
	
	void SetupFontfaceActions(peg::parser& parser)
	{
		using namespace peg;
		parser["Fontface"] .enter = [](const char*, size_t, std::any& dt)
		{
			auto ctx = GetContext<StyleSheetContext&>(dt);
			PushContext(dt, FontfaceContext{&ctx.styleSheet->styleFonts.emplace_back()});
		};
		parser["Fontface"] = [](SemanticValues& vs, std::any& dt)
		{
			{
				auto ctx = GetContext<FontfaceContext&>(dt);
				if(ctx.font->fontFamily.empty())
					throw peg::parse_error("Fontface family is not set!");
			}
			PopContext(dt);
		};
		parser["FontSrc"] = [](SemanticValues& vs, std::any& dt)
		{
			auto ctx = GetContext<FontfaceContext&>(dt);
			auto url = any_move<std::string_view>(vs[0]);
			std::string path = {url.begin(), url.end()};
			auto& file = Context::Get().fileImpl;
			auto f = file->Open(path.c_str());
			if(!f)
			{
				olog::Warn(u"load font file {} failed."_o.format(path));
				return;
			}
			//TODO: cache? async?
			auto length = file->Length(f);
			godot::PackedByteArray buffer;
			buffer.resize(length);
			file->Read(buffer.data(), length, f);
			file->Close(f);
			ctx.font->datas.emplace_back(new godot::FontData)->set_data(buffer);
		};
		parser["FontPropFace"] = [](SemanticValues& vs, std::any& dt)
		{
			auto ctx = GetContext<FontfaceContext&>(dt);
			ctx.font->fontFamily = std::any_cast<std::string_view>(vs[0]);
		};
	}


	struct CSSParser
	{
		struct Grammar
		{
			std::string_view grammar;
			ParserSetupFunction setupFunction;
		};
		std::vector<Grammar> grammars;
		std::vector<std::string> properties;

		static CSSParser& Get()
		{
			static CSSParser instance;
			return instance;
		}

		void RegisterGrammar(std::string_view grammar, ParserSetupFunction setupFunction)
		{
			grammars.emplace_back(grammar, setupFunction);
		}

		void RegisterProperty(std::string_view name)
		{
			properties.emplace_back(name);
		}

		std::string GetGrammar()
		{
			static auto grammar = R"(
				InlineStyle			<- PropertyList
				Stylesheet			<- _ (StyleRule / Keyframes / Fontface)*
			)";
			std::stringstream stream(grammar);
			stream << "\n" << GetUtilsGrammar();
			stream << "\n" << GetStyleRuleGrammar();
			stream << "\n" << GetKeyframeGrammar();
			stream << "\n" << GetFontfaceGrammar();
			for(auto& grammar : grammars)
				stream  << "\n" << grammar .grammar;
			bool first = true;
			stream << "\nProperty <- ";
			for(auto& property : properties)
			{
				if(!first)
					stream << " / ";
				stream << property;
			} 
			return stream.str();
		}

		void SetupParserActions(peg::parser& parser)
		{
			SetupUtilsActions(parser);
			SetupStyleRuleActions(parser);
			SetupKeyFrameActions(parser);
			SetupFontfaceActions(parser);
			for(auto& grammar : grammars)
				grammar.setupFunction(parser);
		}

		peg::parser& GetParser()
		{
			using namespace peg;
			struct ParserInitializer
			{
				parser parser;
				bool ok;
				ParserInitializer(CSSParser* ctx)
				{
					
					parser.log = [](size_t line, size_t col, const std::string& msg)
					{
						olog::Error(u"CSS解析失败，{} : {} [{}]"_o.format((int)line, (int)col, msg));
					};
					ok = parser.load_grammar(ctx->GetGrammar());
					if(!ok)
						return;
					parser.enable_packrat_parsing();
					ctx->SetupParserActions(parser);
				}
			};
			static ParserInitializer parserInitializer(this);
			return parserInitializer.parser;
		}

		std::optional<StyleSheet> Parse(std::string_view str)
		{
			auto& parser = GetParser();
			std::any dt{ParseContext{}};
			StyleSheet ss;
			PushContext(dt, StyleSheetContext{&ss});
			if(parser["Stylesheet"].parse(str.data(), str.size(), dt).ret)
				return ss;
			return {};
		}
		
		std::optional<StyleComplexSelector> ParseSelector(std::string_view str)
		{
			auto& parser = GetParser();
			StyleComplexSelector selector;
			if(parser["ComplexSelector"].parse_and_get_value(str.data(), str.size(), selector).ret);
				return selector;
			return {};
		}

		std::optional<InlineStyle> ParseInlineStyle(std::string_view str)
		{
			auto& parser = GetParser();
			std::any dt{ParseContext{}};
			InlineStyle is;
			PushContext(dt, PropertyListContext{&is.storage, &is.rule});
			if(parser["InlineStyle"].parse(str.data(), str.size(), dt).ret)
				return is;
			return {};
		}
	};

	void RegisterGrammar(std::string_view grammar, ParserSetupFunction initFunction)
	{
		CSSParser::Get().RegisterGrammar(grammar, initFunction);
	}

	void RegisterProperty(std::string_view name)
	{
		CSSParser::Get().RegisterProperty(name);
	}

	std::optional<StyleSheet> Parse(std::string_view str)
	{
		return CSSParser::Get().Parse(str);
	}

	std::optional<StyleComplexSelector> ParseSelector(std::string_view str)
	{
		return CSSParser::Get().ParseSelector(str);
	}

	
    std::optional<InlineStyle> ParseInlineStyle(std::string_view str)
	{
		return CSSParser::Get().ParseInlineStyle(str);
	}

	std::optional<StyleSheet> ParseFile(std::string path)
	{
		auto& ctx = Context::Get().fileImpl;
        auto f = ctx->Open(path.c_str());
        if(!f)
        {
            olog::Warn(u"加载文件失败，filePath：{}"_o, path);
            return std::optional<StyleSheet>();
        }
        auto length = ctx->Length(f);
        std::string data;
        data.resize(length);
        ctx->Read(data.data(), length, f);
        ctx->Close(f);
		auto styleSheet = Parse(data);
		if(styleSheet) styleSheet->path = path;
		return styleSheet;
	}
}
