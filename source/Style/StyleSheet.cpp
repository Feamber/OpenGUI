#include "OpenGUI/Style/StyleSheet.h"

template<class T>
class IndexedWrapper
{
public:
	T& Container;

	struct iterator
	{
		int index;
		typename T::iterator i;
		iterator& operator++()
		{
			index++;
			++i;
			return *this;
		}
		bool operator!=(const iterator& other)
		{
			return i != other.i;
		}
		auto operator*()
		{
			return std::tie(index, *i);
		}
	};

	iterator begin()
	{
		return {0, Container.begin()};
	}

	iterator end()
	{
		return {0, Container.end()};
	}
};

template<class T>
IndexedWrapper<T> ipair(T& xxx)
{
	return {xxx};
}

void OGUI::StyleSheet::Initialize()
{
	for (auto&& [i, complexSel] : ipair(styleSelectors))
	{
		complexSel.priority = i;
		auto& lastSel = complexSel.selectors.back();
		SelectorMap* mapPtr = nullptr;
		std::string_view key;
		if (lastSel.parts.size() == 0)
		{
			key = "*";
			mapPtr = &typeSelectors;
		}
		else
		{
			key = lastSel.parts.back().value;
			switch (lastSel.parts.back().type)
			{
				case StyleSelector::Class:
					mapPtr = &classSelectors;
					break;
				case StyleSelector::Name:
					mapPtr = &nameSelectors;
					break;
				case StyleSelector::Type:
				case StyleSelector::Wildcard:
					if (lastSel.parts.back().value.empty())
						key = "*";
					mapPtr = &typeSelectors;
					break;
				default:
					assert(false);
					break;
			}
		}
		
		mapPtr->insert(std::make_pair(key, i));
	}
	for (auto&& [i, keyframes] : ipair(styleKeyframes))
		namedKeyframes.insert({keyframes.name, i});
}