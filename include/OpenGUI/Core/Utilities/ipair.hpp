#pragma once
#include <tuple>
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