#pragma once
#include <map>
#include <vector>
#include <unordered_map>
#include "OpenGUI/Configure.h"
#include "open_string.h"

namespace OGUI
{

	struct NamePool
	{
		std::vector<ostr::string> _data;
		std::unordered_map<ostr::string_view, size_t, ostr::sv_hash> _table;
		// TODO: rwlock

		size_t Make(const ostr::string_view& str);

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
		Name(const Name& other);
		Name(Name&& other);

		template<typename T, size_t N>
		Name(ostr::char_array<T, N> arr)
			: Name(ostr::string(arr))
		{}

		template<typename T>
		Name(const T* cstr)
			: Name(ostr::string(cstr))
		{}

		bool operator==(const Name& rhs) const;
		bool operator!=(const Name& rhs) const;

		ostr::string_view ToStringView() const;

		int Compare_Id(const Name& rhs) const;
		int Compare_Literal(const Name& rhs) const;

		bool IsNone() const;

	private:
		size_t _entry;
	};

}