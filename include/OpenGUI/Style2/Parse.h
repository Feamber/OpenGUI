#pragma once
#include <string>
#include <optional>
#include <string_view>
#include <sstream>
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Rule.h"
#include "AnimTypes.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Style2/ParseUtils.hpp"
#include "peglib.h"
// #include "fmt/format.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/Utilities/any_move.hpp"

namespace OGUI
{
	namespace CSSParser
	{
		using PropertyParseAction = void(*)(peg::SemanticValues& vs, std::any& dt);
		using ParserSetupFunction = void(*)(peg::parser& parser);
		OGUI_API void RegisterProperty(std::string_view name);
		OGUI_API void RegisterGrammar(std::string_view grammar, ParserSetupFunction setupFunction);
		OGUI_API StyleSheet* Parse(std::string_view str);
		OGUI_API std::optional<InlineStyle> ParseInlineStyle(std::string_view str);
		OGUI_API StyleSheet* ParseFile(std::string path);
		OGUI_API std::optional<StyleComplexSelector> ParseSelector(std::string_view str);

		using  ParseContext = std::vector<std::any>;
		struct PropertyListContext
		{
			StyleSheetStorage* storage = nullptr;
			StyleRule* rule = nullptr;
		};
		template<class T>
		T& GetContext(std::any& dt)
		{
			return std::any_cast<T&>(std::any_cast<ParseContext&>(dt).back());
		}
	}
}