#pragma once
#include <unordered_map>
#include <string>
#include <OpenGUI/Style/VariantStorage.h>
namespace OGUI
{
	struct Style
	{
		bool isShared;
		struct YGNode* yogaNode;
		std::unordered_map<std::string, VariantHandle> customProps;
		
		int GetCustomPropCount() { return customProps.size(); }
	};
}