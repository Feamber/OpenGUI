
#include "OpenGUI/Core/OName.h"
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{

	static bool bNamePoolInitialized;
	alignas(NamePool) static uint8_t NamePoolData[sizeof(NamePool)];

	size_t NamePool::Make(const ostr::string_view& str)
	{
		uint32_t hash = str.get_hash();
		auto& arr = _hashtable[hash];

		for(const auto& i : arr)
			if(_data[i] == str) return i;

		size_t index = _data.size();
		_data.push_back(str);

		_hashtable[hash].push_back(index);

		return index;
	}

	size_t NamePool::MakeLiteral(const char16_t* literal)
	{
		auto it = _literaltable.find(literal);
		if (it != _literaltable.cend())
			return it->second;
		size_t entry = Make(literal);
		_literaltable[literal] = entry;
		return entry;
	}
	
	ostr::string_view NamePool::Read(size_t entry) const
	{
		return _data[entry];
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
		, _hashtable()
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
	{}

	Name::Name(SpecialName e)
		: _entry(std::underlying_type_t<SpecialName>(e))
	{}

	Name::Name(ostr::string_view sv)
	{
		if (sv.is_empty()) 
		{ 
			_entry = 0; 
			return;
		}
		_entry = NamePool::Get().Make(sv);
	}

	Name::Name(const ostr::string& str)
		: Name(str.to_sv())
	{}

	bool Name::operator==(const Name& rhs) const
	{
		return _entry == rhs._entry;
	}

	bool Name::operator!=(const Name& rhs) const
	{
		return !operator==(rhs);
	}

	bool Name::operator<(const Name& rhs) const
	{
		return Compare_Id(rhs) < 0;
	}
	
	ostr::string_view Name::ToStringView() const
	{
		return NamePool::Get().Read(_entry);
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
	
	void Name::Reset()
	{
		_entry = 0;
	}
	
	size_t Name::GetHash() const
	{
		return std::hash<size_t>()(_entry);
	}
	
	size_t Name::GetStringHash() const
	{
		return ToStringView().get_hash();
	}
}