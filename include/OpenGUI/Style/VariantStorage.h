#pragma once
#include "OpenGUI/Core/Containers/vector.hpp"
#include <boost/hana.hpp>

namespace boost::hana
{
	template <typename Iterable, typename T>
	constexpr auto index_of(Iterable const& iterable, T const& element)
	{
		auto size = decltype(hana::size(iterable)){};
		auto dropped = decltype(hana::size(
			hana::drop_while(iterable, hana::not_equal.to(element))
		)){};
		return size - dropped;
	}
}

namespace OGUI
{
	namespace hana = boost::hana;

	struct VariantHandle
	{
		//int type;
		int index : 31;
		bool dynamic : 1;
		VariantHandle() = default;
		VariantHandle(int index) : index(index), dynamic(false) {}
		VariantHandle(int index, bool dynamic) : index(index), dynamic(dynamic) {}
	};

	template<class T>
	struct not_pod : std::negation<std::is_pod<T>> {};
	template<class... T>
	using tuple_vector = hana::tuple<std::vector<T>...>;

	template<class... Ts>
	struct VariantStorage
	{
		constexpr static auto vectorValues = hana::filter(hana::tuple_t<Ts...>, hana::trait<not_pod>);
		using Tuple = typename decltype(hana::unpack(vectorValues, hana::template_<tuple_vector>))::type;
		Tuple vectors;
		std::vector<char> compressedData;
		template<class T>
		constexpr static auto IndexOf()
		{
			return hana::index_of(vectorValues, hana::type_c<T>);
		}

		template<class T> 
		constexpr static int TypeOf() { return IndexOf<T>().value; }

		template<class T>
		T& Get(VariantHandle handle)
		{
			if constexpr (hana::contains(vectorValues, hana::type_c<T>))
				return hana::at(vectors, IndexOf<T>())[handle.index];
			else
				return *(T*)(compressedData.data() + handle.index);
		}

		template<class T>
		VariantHandle Push(T const& value)
		{
			if constexpr (hana::contains(vectorValues, hana::type_c<T>))
			{
				constexpr auto index = IndexOf<T>();
				auto& vector = hana::at(vectors, index);
				vector.push_back(value);
				return {(int)(vector.size() - 1), false};
			}
			else
			{
				int offset = (int)compressedData.size();
				compressedData.resize(compressedData.size() + sizeof(T));
				std::memcpy(compressedData.data() + offset, &value, sizeof(T));
				return {offset, false};
			}
		}

		template<class T>
		const T& Get(VariantHandle handle) const
		{
			if constexpr (hana::contains(vectorValues, hana::type_c<T>))
				return hana::at(vectors, IndexOf<T>())[handle.index];
			else
				return *(T*)(compressedData.data() + handle.index);
		}

		//TODO: AddDynamic, RemoveDynamic, GetDynamic
	};


}