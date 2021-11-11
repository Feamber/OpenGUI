#pragma once
#include <vector>
#include <string>
#include <map>
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Core/olog.h"

namespace OGUI
{
    struct StyleSheetStorage
    {
        //TODO: handle alignment?
        std::vector<char> bulkData;
        std::vector<std::string> stringData;

        template<class T>
        const T& Get(VariantHandle handle) const
        {
            if constexpr(std::is_same_v<T, std::string>)
            {
                return stringData[handle.index];
            }
            else 
            {
                return *(T*)(bulkData.data() + handle.index);
            }
        }

        template<class T>
        VariantHandle Push(const T& value)
        {
            if constexpr(std::is_same_v<T, std::string>)
            {
                stringData.push_back(value);
                return {(int)stringData.size()-1};
            }
            else 
            {
                int offset = (int)bulkData.size();
				bulkData.resize(bulkData.size() + sizeof(T));
				std::memcpy(bulkData.data() + offset, &value, sizeof(T));
				return {offset};
            }
        }

        template<class T>
        VariantHandle PushArray(const T* value, size_t length)
        {
            int offset = (int)bulkData.size();
            bulkData.resize(bulkData.size() + sizeof(T)*length + sizeof(size_t));
            *(size_t*)(bulkData.data() + offset) = length;
            std::memcpy(bulkData.data() + offset + sizeof(size_t), value, sizeof(T)*length);
            return {offset};
        }

        template<class T>
        const T* GetArray(VariantHandle handle, size_t& length) const
        {
            length = *(size_t*)(bulkData.data() + handle.index);
            return (T*)(bulkData.data() + handle.index);
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

		OGUI_API void Initialize();
    };

	struct InlineStyle
	{
		StyleRule rule;
		StyleSheetStorage storage;
	};
}