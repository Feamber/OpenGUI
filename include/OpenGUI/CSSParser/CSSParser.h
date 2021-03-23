#pragma once

#include <string>

namespace OGUI
{

	/// Example:
	//  selector : keyword {
	//      left: <length> | auto
	//      top: <length> | auto
	//      right : <length> | auto
	//      bottom : <length> | auto
	//  }
	//
	// value of <length> should be int or float ( 1 or 1.0 )

	/// priority of selector
	//  name > class > type

//	static std::string PropToString(int index)
//	{
//#define STYLEPROP(name, index, inherit, type, ...)\
//		if(prop.id == StylePropertyId::name) \
//		{ \
//			ApplyProperty<type>(name, prop, sheet); \
//			continue; \
//		}
//#include "OpenGUI/Style/StylePropertiesDef.h"
//#undef	STYLEPROP
//	}

	enum class TokenType : uint32_t
	{
		// value type
		Integer,
		Float,
		// prop
		Selector,
		Keyword,
		Property,
		Component,
		// end of the file
		TEOF
	};

	struct Token
	{
		Token(TokenType t, const std::string& v)
			: type(t), value(v) {}

		TokenType type;
		std::string value;
	};

	class Lexer
	{
	public:
		Lexer(const std::string& s)
			: text(s) {}

		Token GetNextToken();
	private:
		std::string EatAllSpace(const std::string& s);
	private:
		size_t pos = 0;
		std::string text;
		bool isNextValue = false;
		bool isNextComp = false;
		bool isNextKeyword = false;
	};

	//class CSSParser
	//{
	//public:
	//	CSSParser(const std::string& s);

	//private:
	//	//CSSParserImpl* pImpl_;
	//};

}