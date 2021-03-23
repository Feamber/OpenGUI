#include "OpenGUI/CSSPhaser/CSSPhaser.h"

#include <iostream>

namespace OGUI
{

	std::string Lexer::EatAllSpace(const std::string& s)
	{
		std::string str = "";
		for (auto& element : s)
		{
			if (std::isspace(element))
				continue;
			str += element;
		}
		return str;
	}

	/// Example:
	//  selector : keyword {
	//      left: <length> | auto | test;
	//      top: <length> | auto;
	//      right : <length> | auto;
	//      bottom : <length> | auto;
	//  }
	//
	// value of <length> should be int or float ( 1 or 1.0 )

	Token Lexer::GetNextToken()
	{
		if (pos == 0) // get the selector
		{
			size_t colonPos = text.find_first_of(':', pos);
			size_t lBracketPos = text.find_first_of('{', pos);
			if (colonPos < lBracketPos) // selector have keyword
			{
				pos = colonPos + 1;
				isNextKeyword = true;
				return { TokenType::Selector, EatAllSpace(text.substr(0, colonPos)) };
			}
			else // selector have keyword
			{
				pos = lBracketPos + 1;
				return { TokenType::Selector, EatAllSpace(text.substr(0, lBracketPos)) };
			}
		}
		else // get the properties
		{
			size_t colonPos = text.find_first_of(':', pos);
			size_t orPos = text.find_first_of('|', pos);
			if (isNextValue) // value of the prop
			{
				size_t temp = pos;
				pos = orPos + 1;
				isNextValue = false;
				isNextComp = true;
				return { TokenType::Float, EatAllSpace(text.substr(temp, orPos - temp)) };
			}
			else if (isNextKeyword)
			{
				size_t lBracketPos = text.find_first_of('{', pos);
				size_t temp = pos;
				pos = lBracketPos + 1;
				isNextKeyword = false;
				return { TokenType::Keyword, EatAllSpace(text.substr(temp, lBracketPos - temp)) };
			}
			else if (isNextComp) // component of the prop
			{
				size_t semicolonPos = text.find_first_of(';', pos);
				size_t temp = pos;
				if (orPos < semicolonPos) // there are some props left
				{
					pos = orPos + 1;
					return { TokenType::Component, EatAllSpace(text.substr(temp, orPos - temp)) };
				}
				else
				{
					pos = semicolonPos + 1;
					isNextComp = false;
					return { TokenType::Component, EatAllSpace(text.substr(temp, semicolonPos - temp)) };
				}
			}
			else // name of the prop
			{
				if (colonPos == std::string::npos)
					return { TokenType::TEOF, "EOF" };
				
				size_t temp = pos;
				pos = colonPos + 1;
				isNextValue = true;
				return { TokenType::Property, EatAllSpace(text.substr(temp, colonPos - temp)) };
			}
		}
	}

}