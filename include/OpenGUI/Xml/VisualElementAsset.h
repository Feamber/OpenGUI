#pragma once
#include <string>
#include <string_view>

namespace OGUI
{
	class VisualElementAsset
	{
	public:
		bool GetAttributeValue(std::string& out, const std::string_view attribute_name) const;
	};
}