#pragma once
#include <vector>
#include <boost/hana.hpp>

namespace OGUI
{
	namespace hana = boost::hana;

	struct VariantHandle
	{
		//int type;
		int index;
	};

	template<class... Ts>
	struct VariantStorage
	{
		hana::tuple<std::vector<Ts>...> vectors;
		template<class T>
		constexpr static auto IndexOf()
		{
			return *hana::index_if(hana::tuple_t<Ts...>, hana::_ == hana::type_c<T>);
		}

		template<class T> 
		constexpr static int TypeOf() { return IndexOf<T>().value; }

		template<class T>
		T& Get(VariantHandle handle)
		{
			return hana::at(vectors, IndexOf<T>())[handle.index];
		}

		template<class T>
		VariantHandle Push(T const& value)
		{
			constexpr auto index = IndexOf<T>();
			auto& vector = hana::at(vectors, index);
			vector.push_back(value);
			return {vector.size() - 1};
			//return {index.value, vector.size() - 1};
		}

		template<class T>
		const T& Get(VariantHandle handle) const
		{
			return hana::at(vectors, IndexOf<T>())[handle.index];
		}
	};
}