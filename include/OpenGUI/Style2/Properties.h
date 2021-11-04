
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

	struct VariantHandle
	{
		//int type;
		int index : 31;
		bool dynamic : 1;
		VariantHandle() = default;
		VariantHandle(int index) : index(index), dynamic(false) {}
	};

    struct StyleProperty
	{
		size_t id;
		VariantHandle value;
		bool keyword : 1;
		int arrayIndex : 31;
		StyleProperty() = default;
		StyleProperty(size_t id, VariantHandle handle, int arrayIndex = -1)
			: id(id), value(handle), keyword(false), arrayIndex(arrayIndex) {}
		StyleProperty(size_t id, int keyword, int arrayIndex = -1)
			: id(id), value{keyword}, keyword(true), arrayIndex(arrayIndex)
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