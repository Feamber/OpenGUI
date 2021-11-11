#pragma once
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Core/olog.h"

namespace OGUI
{
    template<class T>
    struct span_trait : std::false_type {};
    template<class T>
    struct span_trait<gsl::span<T>> : std::true_type { using type = T; };

    struct StyleSheetStorage
    {
        //TODO: handle alignment?
        std::vector<char> bulkData;
        std::vector<std::string> stringData;

        template<class T>
        T Get(VariantHandle handle) const
        {
            using st = span_trait<T>;
            if constexpr(std::is_same_v<T, std::string>)
            {
                return stringData[handle.index];
            }
            else if constexpr(st::value)
            {
                using et = typename st::type;
                size_t length = *(size_t*)(bulkData.data() + handle.index);
                auto begin = bulkData.data() +  handle.index + sizeof(size_t);
                auto end = begin + sizeof(et)*length;
                return {(et*)begin, (et*)end};
            }
            else 
            {
                return *(T*)(bulkData.data() + handle.index);
            }
        }

        template<class T>
        VariantHandle Push(const T& value)
        {
            using st = span_trait<T>;
            if constexpr(std::is_same_v<T, std::string>)
            {
                stringData.push_back(value);
                return {(int)stringData.size()-1};
            }
            else if constexpr(st::value)
            {
                using et = typename st::type;
                int offset = (int)bulkData.size();
                size_t length = value.size();
                bulkData.resize(bulkData.size() + sizeof(et)*length + sizeof(size_t));
                *(size_t*)(bulkData.data() + offset) = length;
                std::memcpy(bulkData.data() + offset + sizeof(size_t), value.data(), sizeof(et)*length);
                return {offset};
            }
            else 
            {
                int offset = (int)bulkData.size();
				bulkData.resize(bulkData.size() + sizeof(T));
				std::memcpy(bulkData.data() + offset, &value, sizeof(T));
				return {offset};
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

		OGUI_API void Initialize();
    };

	struct InlineStyle
	{
		StyleRule rule;
		StyleSheetStorage storage;
	};
}