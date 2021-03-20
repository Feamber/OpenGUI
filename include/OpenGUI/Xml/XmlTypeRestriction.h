#pragma one
#include <string>
#include <vector>

struct XmlTypeRestriction
{
public:
	enum class Type
	{
		Null,
		RegexMatch,
		Bounds,
		Enumeration,
	};
	
	Type type = Type::Null;

	// RegexMatch
	std::string regex;

	// Bounds
	std::string min;
	std::string max;

	// Enumeration
	std::vector<std::string> enums;
};