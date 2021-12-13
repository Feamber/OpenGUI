
#pragma once
#include "OpenGUI/Configure.h"
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

	enum class StyleOverflow
	{
		Visible,
		Hidden,
		Clip,
		Scroll,
		Auto
	};

	enum class RestyleDamage
	{
		None = 0,
		Yoga = 0x1,
		Transform = 0x2,
		Text = 0x4,
		TextLayout = 0x8,
		Font = 0x10,
		All = 0x20 - 1,
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