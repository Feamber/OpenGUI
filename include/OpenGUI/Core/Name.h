#pragma once
#include <map>
#include <vector>
#include <unordered_map>
#include <OpenGUI/Configure.h>
#include <string_view>
#include "open_string.h"

namespace OGUI
{
	
	struct NamePool
	{
		// _entrytable:		entry -> [hash, index]			< entry map to a tuple with hash and index inside >
		// _hashtable:		hash -> {index, index ... }		< hash map to array of indices, prevent collision >

		std::vector<ostr::string> _data;
		std::map<uint32_t, std::vector<size_t>> _hashtable;

		// TODO: rwlock

		size_t Make(const ostr::string_view& str);
		ostr::string_view Read(size_t entry) const;

		static NamePool& Get();

	private:

		NamePool();

	};

#define ENAME \
	ENAME_USAGE(None),	\
	ENAME_USAGE(Test)

	enum class EName : uint8_t
	{
#define ENAME_USAGE(e) e
		ENAME
#undef ENAME_USAGE
	};

	class OGUI_API Name
	{
	public:
		Name();
		Name(EName e);
		Name(const ostr::string& str);
		Name(ostr::string_view sv);
		Name(const Name& other) = default;
		Name(Name&& other) = default;
		Name& operator=(const Name& other) = default;

		template<typename T, size_t N>
		Name(ostr::char_array<T, N> arr)
			: Name(ostr::string(arr))
		{}

		template<typename T>
		Name(const T* cstr)
			: Name(ostr::string(cstr))
		{}

		template<typename T>
		Name(std::basic_string<T> str)
			: Name(ostr::string(str))
		{}

		template<typename T>
		Name(std::basic_string_view<T> sv)
			: Name(ostr::string(sv))
		{}

		bool operator==(const Name& rhs) const;
		bool operator!=(const Name& rhs) const;
		bool operator<(const Name& rhs) const;

		ostr::string_view ToStringView() const;

		int Compare_Id(const Name& rhs) const;
		int Compare_Literal(const Name& rhs) const;

		bool IsNone() const;

		void Reset();

		size_t GetHash() const;

		size_t GetStringHash() const;

	private:
		size_t _entry;
	};

}

namespace std
{
	template<>
	struct hash<OGUI::Name>
	{
		size_t operator()(const OGUI::Name& n) const
		{
			return n.GetHash();
		}
	};
}
