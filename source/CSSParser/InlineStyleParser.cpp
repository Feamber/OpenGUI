#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Core/Utilities/any_move.hpp"
#include "peglib.h"

namespace OGUI
{
    std::optional<InlineStyle> ParseInlineStyle(std::string_view str)
	{
		auto grammar = R"(
			PropertyList		<- Property? (_ ';' _ Property)* _ ';'?
			Property			<- <IDENT> w ':' w <ValueList> _
			~ValueList			<- Value (Spliter Value)*
			~Spliter			<- (w ',' w) / [ ]+
			~Value				<- URL / CNUM / HEX / CALL / IDENT
			~URL				<- 'url' w '(' (!')' .)* ')'
			~IDENT				<- [a-zA-Z] [a-zA-Z0-9-]*
			~HEX				<- ('#' NUM) 
			~CNUM				<- NUM (IDENT / '%')?
			~NUM				<- ('+' / '-')? (([0-9]*"."([0-9]+ 'e')?[0-9]+) / ([0-9]+))
			~CALL				<- IDENT w '('  w CNUM ( w ',' w CNUM)* w  ')'
			~blockcomment		<- '/*' (!'*/' .)* '*/'
			~_					<- ([ \t\r\n] / blockcomment)*
			~w					<- [ ]*
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