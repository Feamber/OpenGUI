#pragma once
#include <vector>
#include <string>
#include <map>
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Selector.h"

namespace OGUI
{
    struct StyleSheetStorage
    {
        std::vector<char> bulkData;
        std::vector<std::string> stringData;

        template<class T>
        const T& Get(VariantHandle handle) const
        {
            if constexpr(std::is_pod_v<T>)
            {
                return *(T*)(bulkData.data() + handle.index);
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                return stringData[handle.index];
            }
        }

        template<class T>
        VariantHandle Push(const T& value)
        {
            if constexpr(std::is_pod_v<T>)
            {
                int offset = (int)bulkData.size();
				bulkData.resize(bulkData.size() + sizeof(T));
				std::memcpy(bulkData.data() + offset, &value, sizeof(T));
				return {offset};
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                stringData.push_back(value);
                return {(int)stringData.size()-1};
            }
        }
    };

    struct StyleRule
	{
		std::vector<StyleProperty> properties;
        struct AnimRule
        {
            std::string name;
            std::vector<StyleProperty> properties;
        };
        std::vector<AnimRule> animation;
	};

    struct StyleKeyframes
    {
		struct Key
		{
			float percentage;
			int frameIndex;
		};
		std::string name;
		std::vector<Key> keys;
    };

    struct StyleSheet
    {
        std::string path;
        StyleSheetStorage storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;
		std::vector<StyleKeyframes> styleKeyframes;

		using SelectorMap = std::multimap<std::string_view, int>;
		SelectorMap classSelectors;
		SelectorMap nameSelectors;
		SelectorMap typeSelectors;

		using KeyframesMap = std::map<std::string_view, int>;
		KeyframesMap namedKeyframes;
    };

	struct InlineStyle
	{
		StyleRule rule;
		StyleSheetStorage storage;
	};
}