

#include "OpenGUI/Core/ostring/ostr.h"
#include <gsl/span>
#include <unordered_map>
namespace OGUI
{
	struct AnimationStyle;
	struct AnimRunContext;
	struct StyleSheetStorage;

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

    struct Style;

    struct StyleDesc
    {
		void (*Dispose)(Style& style);
        void (*Merge)(Style& style, const Style& other, gsl::span<StyleProperty>& props);
        void (*Initialize)(Style& style);
        void (*ApplySS)(Style& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style& parent);
        void (*LerpSS)(Style& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
			const Style* parent, float alpha);
    };

	struct StyleRegistry
	{
		std::vector<StyleDesc> descriptions;
	};

	void RegisterStyleStruct(StyleDesc* Registry);

	struct Style
	{
		std::unordered_map<size_t, std::shared_ptr<void*>> structs;
		
		static const Style& GetInitialStyle();
		static Style Create();
		void MergeStyle(const Style& other, gsl::span<StyleProperty>& props);
		void ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const Style* parent);
		void ApplyAnimation(const AnimationStyle& anim, const AnimRunContext& ctx, const Style* parent);
		void LerpProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props,
			const Style* parent, float alpha);
	};
}