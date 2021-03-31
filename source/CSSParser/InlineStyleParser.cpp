#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "peglib.h"

namespace OGUI
{
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
		parser["PropertyList"] = [&](SemanticValues& vs)
		{
			property_list_t list;
			for (auto& p : vs)
				list.push_back(any_move<pair<string_view, string_view>>(p));

			sort(list);
			int animIndexBegin = 0;
			int animIndexEnd = 0;
			for (auto& pair : list)
			{
				const char* errorMsg;
				ParseErrorType errorType;
				if (pair.first == "animation-name")
				{
					animIndexBegin = animIndexEnd;
					animIndexEnd = std::count(str.begin(), str.end(), ',') + 1;
				}
				if (!ParseProperty(sheet.storage, pair.first, pair.second, sheet.rule, errorMsg, errorType, animIndexBegin, animIndexEnd))
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
}