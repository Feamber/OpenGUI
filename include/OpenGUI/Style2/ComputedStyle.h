#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Forward.h"
#include <gsl/span>
#include <memory>
#include <unordered_map>
namespace OGUI
{

    struct ComputedStyle;

    struct StyleDesc
    {
		ostr::string_view name;
		size_t hash;
		size_t entry;
		bool inherited;
        void (*ApplyProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent);
		RestyleDamage (*ApplyAnimatedProperties)(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override);
		RestyleDamage (*ApplyTransitionProperties)(ComputedStyle& style, const ComputedStyle& src, const ComputedStyle& dst, const gsl::span<TransitionProperty>& props, const StyleMasks& override);
		void (*Merge)(ComputedStyle& style, ComputedStyle& other, const StyleMasks& override);
		void (*MergeId)(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override);
		size_t (*GetProperty)(ostr::string_view name);
    };

	struct StyleRegistry
	{
		std::vector<StyleDesc> descriptions;
		static const StyleRegistry& Get();
	};

	OGUI_API void RegisterStyleStruct(const StyleDesc& Registry);
	OGUI_API size_t NewStyleStructEntry();
	
	//helper
	template<class T>
	StyleDesc RegisterStyleStruct()
	{
		StyleDesc desc;
		desc.hash = T::hash;
		desc.inherited = T::inherited;
		desc.name = T::name;
		desc.ApplyAnimatedProperties = &T::ApplyAnimatedProperties;
		desc.ApplyTransitionProperties = &T::ApplyTransitionProperties;
		desc.ApplyProperties = &T::ApplyProperties;
		desc.Merge = &T::Merge;
		desc.MergeId = &T::MergeId;
		desc.GetProperty = &T::GetProperty;
		desc.entry = NewStyleStructEntry();
        RegisterStyleStruct(desc);
		T::SetEntry(desc.entry);
        T::SetupParser();
		return desc;
	}
	
	OGUI_API void RegisterBuiltinStructs();

	struct ComputedStyle
	{
		struct ComputedStruct
		{
			std::shared_ptr<void> ptr;
			bool owned = true;
		};
		std::array<ComputedStruct, 8> structs;
		ComputedStyle() = default;
		ComputedStyle(const ComputedStyle& other);
		ComputedStyle(ComputedStyle&& other) = default;
		ComputedStyle& operator=(const ComputedStyle& other);
		ComputedStyle& operator=(ComputedStyle&& other) = default;
		static ComputedStyle Create(const ComputedStyle* parent);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const StyleMasks& override, const ComputedStyle* parent);
        RestyleDamage ApplyAnimatedProperties(const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const StyleMasks& override);
		RestyleDamage ApplyTransitionProperties(const ComputedStyle& src, const ComputedStyle& dst, const gsl::span<TransitionProperty>& props, const StyleMasks& override);
		void Merge(ComputedStyle& other, const StyleMasks& override);
		void MergeId(ComputedStyle& other, const gsl::span<size_t>& override);
		static size_t GetProperty(ostr::string_view name);
	};

}