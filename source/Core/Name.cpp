#define DLL_IMPLEMENTATION
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{

	static bool bNamePoolInitialized;
	alignas(NamePool) static uint8_t NamePoolData[sizeof(NamePool)];

	size_t NamePool::Make(const ostr::string_view& str)
	{
		auto it = _table.find(str);
		if (it != _table.cend())
		{
			return it->second;
		}
		size_t index = _data.size();
		_data.push_back(str);
		_table[str] = index;
		return index;
	}
	
	NamePool& NamePool::Get()
	{
		if (bNamePoolInitialized)
		{
			return *(NamePool*)NamePoolData;
		}

		NamePool* singleton = new (NamePoolData) NamePool;
		bNamePoolInitialized = true;
		return *singleton;
	}

	NamePool::NamePool()
		: _data()
		, _table()
	{
		using namespace ostr::literal;
		for (const auto& s : {
#define ENAME_USAGE(e) u## #e ##_o
		ENAME
#undef ENAME_USAGE
			})
		{
			Make(s);
		}
	}
	
	Name::Name()
		: _entry(0)
	{
	}

	Name::Name(EName e)
		: _entry(std::underlying_type_t<EName>(e))
	{
	}

	Name::Name(ostr::string_view sv)
	{
		using namespace ostr::literal;
		if (sv.is_empty()) sv = u"None"_o;
		_entry = NamePool::Get().Make(sv);
	}

	Name::Name(const ostr::string& str)
		: Name(str.to_sv())
	{}

	Name::Name(const Name& other)
		: _entry(other._entry)
	{}

	Name::Name(Name&& other)
		: _entry(other._entry)
	{}

	bool Name::operator==(const Name& rhs) const
	{
		return _entry == rhs._entry;
	}

	bool Name::operator!=(const Name& rhs) const
	{
		return !operator==(rhs);
	}
	
	ostr::string_view Name::ToStringView() const
	{
		return NamePool::Get()._data[_entry];
	}

	int Name::Compare_Id(const Name& rhs) const
	{
		return (_entry == rhs._entry) ? 0 : ((_entry < rhs._entry) ? -1 : 1);
	}

	int Name::Compare_Literal(const Name& rhs) const
	{
		return ToStringView().compare(rhs.ToStringView());
	}
	
	bool Name::IsNone() const
	{
		return _entry == 0;
	}
}