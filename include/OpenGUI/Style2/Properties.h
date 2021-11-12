
#pragma once
#include "OpenGUI/Core/ostring/ostr.h"
#include <gsl/span>
#include <unordered_map>
namespace OGUI
{

	enum class StyleKeyword : int
	{
		Inherit,
		Initial,
		Auto,
		Unset,
		True,
		False,
		None
	};

	#define ENUM_CLASS_FLAGS(Enum) \
	inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; } \
	inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }

	enum class RestyleDamage
	{
		None = 0,
		Yoga = 0x1,
		Transform = 0x2,
	};
	ENUM_CLASS_FLAGS(RestyleDamage)

	struct VariantHandle
	{
		//int type;
		int index : 31;
		bool dynamic : 1;
		VariantHandle() = default;
		VariantHandle(int index) : index(index), dynamic(false) {}
		bool operator==(const VariantHandle& other) { return index == other.index && dynamic == other.dynamic; }
	};

    struct StyleProperty
	{
		size_t id;
		VariantHandle value;
		bool keyword;
		StyleProperty() = default;
		StyleProperty(size_t id, VariantHandle handle)
			: id(id), value(handle), keyword(false) {}
		StyleProperty(size_t id, int keyword)
			: id(id), value{keyword}, keyword(true)
		{}
	};

	struct AnimatedProperty
	{
		size_t id;
		VariantHandle from;
		VariantHandle to;
		float alpha;
	};
}