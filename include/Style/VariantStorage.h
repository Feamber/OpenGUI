#pragma once
#include <vector>
namespace OGUI
{
	namespace hana = boost::hana;

	struct VariantHandle
	{
		int Type;
		int Index;
	};

	template<class... Ts>
	struct VariantStorage
	{
		hana::tuple<std::vector<Ts>...> vectors;
		template<class T>
		constexpr auto IndexOf()
		{
			return *hana::index_if(hana::tuple_t<Ts...>, hana::_ == hana::type_c<T>);
		}

		template<class T> TypeOf() { return IndexOf<T>().value; }
		template<class T>
		T& Get(int index)
		{
			return hana::at(vectors, IndexOf<T>())[index];
		}

		template<class T>
		const T& Get(int index) const
		{
			return hana::at(vectors, IndexOf<T>())[index];
		}
	};
}