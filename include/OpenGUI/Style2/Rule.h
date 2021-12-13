#pragma once
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Core/olog.h"

namespace godot
{
    class Font;
    class FontData;
}

namespace OGUI
{
    template<class T>
    struct span_trait : std::false_type {};
    template<class T>
    struct span_trait<gsl::span<T>> : std::true_type { using type = T; };

    struct StyleSheetStorage
    {
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
                size_t length = *(size_t*)(bulkData.data() + handle.index - sizeof(size_t));
                auto begin = bulkData.data() +  handle.index;
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
                int offset = (int)bulkData.size() + sizeof(size_t);
                auto align = static_cast<size_t>(reinterpret_cast<uintptr_t>(bulkData.data() + offset) & (alignof(et) - 1));
                if(align != 0)
                    offset += alignof(et) - align;
                size_t length = value.size();
                bulkData.resize(offset + sizeof(et)*length);
                *(size_t*)(bulkData.data() + offset - sizeof(size_t)) = length;
                std::memcpy(bulkData.data() + offset, value.data(), sizeof(et)*length);
                return {offset};
            }
            else 
            {
                int offset = (int)bulkData.size();
                auto align = static_cast<size_t>(reinterpret_cast<uintptr_t>(bulkData.data() + offset) & (alignof(T) - 1));
                if(align != 0)
                    offset += alignof(T) - align;
				bulkData.resize(offset + sizeof(T));
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

    struct StyleFont
    {
        std::string fontFamily;
        std::vector<std::shared_ptr<godot::FontData>> datas;
    };

    struct StyleSheet
    {
        std::string path;
        StyleSheetStorage storage;
		std::vector<StyleRule> styleRules;
		std::vector<StyleComplexSelector> styleSelectors;
		std::vector<StyleKeyframes> styleKeyframes;
        std::vector<StyleFont> styleFonts;

        using FontMap = std::unordered_map<std::string_view, int>;
        FontMap namedStyleFamilies;

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