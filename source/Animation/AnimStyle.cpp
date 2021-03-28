#include "OpenGUI/Animation/AnimStyle.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Utilities/ipair.hpp"
#include <algorithm>

OGUI::AnimationStyle OGUI::AnimationStyle::Create(bool isShared)
{
	AnimationStyle style = GetInitialStyle();
	style.isShared = isShared;
	return style;
}

const OGUI::AnimationStyle& OGUI::AnimationStyle::GetInitialStyle()
{
	struct InitialStyle
	{
		InitialStyle()
		{
#define ANIMPROP(name, index, type, id, def)\
			style.name = def;
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
		}
		AnimationStyle style;
	};
	static InitialStyle initStyle;
	return initStyle.style;
}

void OGUI::AnimationStyle::ApplyProperties(std::vector<OGUI::AnimationStyle>& self, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
	std::vector<std::string> names;
	std::vector<std::vector<OGUI::StyleProperty>> rules;
	int counts[100] = {};
	auto Index = [&](StylePropertyId id)
	{
		int i = counts[(int)id]++;
		if (i >= rules.size())
		{
			names.resize(i + 1);
			rules.resize(i + 1);
		}
		return i;
	};

	for (auto& prop : props)
	{
		if ((int)prop.id < (int)StylePropertyId::NumStyle)
			continue;
		int i = Index(prop.id);
		std::vector<OGUI::StyleProperty>& rule = rules[i];
		rule.push_back(prop);
		if (prop.id == StylePropertyId::animName)
		{
			assert(!prop.keyword);
			names[i] =sheet.Get<std::string>(prop.value);
		}
	}
	auto& Initial = GetInitialStyle();
	for (auto&& [i, name] : ipair(names))
	{
		AnimationStyle* panim = nullptr;
		for (auto& anim : self)
		{
			if (anim.animName == name)
			{
				panim = &anim;
				break;
			}
		}
		if (panim == nullptr)
		{
			panim = &self.emplace_back();
			*panim = Initial;
		}
		panim->ApplyProperties(sheet, rules[i]);
	}
}

namespace OGUI
{
	template<class T>
	void Assign(T& field, const T& value)
	{
		field = value;
	}
	template<class T, class Y>
	void Assign(T& field, const Y& value)
	{	}

	template<class T>
	void GetInitialProperty(T& field, StylePropertyId propId)
	{
		auto& InitialStyle = AnimationStyle::GetInitialStyle();
#define ANIMPROP(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	}

	template<class T>
	void GetGlobalProperty(T& field, const StyleProperty& prop)
	{
		if (prop.value.index == (int)StyleKeyword::Unset
			|| prop.value.index == (int)StyleKeyword::Initial)
		{
			GetInitialProperty<T>(field, prop.id);
		}
		assert(false);
	}

	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, void>
		GetProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
	{
		field = sheet.Get<T>(prop.value);
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, void>
		GetProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet)
	{
		field = (T)sheet.Get<int>(prop.value);
	}

}

void OGUI::AnimationStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
	for (auto& prop : props)
	{
#define ANIMPROP(name, index, type, ...)\
		if(prop.id == StylePropertyId::name) \
		{ \
			if(prop.keyword) \
				GetGlobalProperty<type>(name, prop); \
			else \
				GetProperty<type>(name, prop, sheet); \
			continue; \
		}
#include "OpenGUI/Animation/AnimPropertiesDef.h"
#undef	ANIMPROP
	}
}

namespace OGUI
{

	template<class T>
	std::enable_if_t<!std::is_enum_v<T>, void>
		LerpProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet, float alpha)
	{
		field = sheet.Get<T>(prop.value);
	}

	template<class T>
	std::enable_if_t<std::is_enum_v<T>, void>
		LerpProperty(T& field, const StyleProperty& prop, const StyleSheetStorage& sheet, float alpha)
	{
		field = (T)sheet.Get<int>(prop.value);
	}

}

void OGUI::AnimationStyle::ResolveReference(const gsl::span<StyleSheet*>& sheets)
{
	for (auto& s : sheets)
	{
		auto iter = s->namedKeyframes.find(animName);
		if (iter != s->namedKeyframes.end())
		{
			sheet = s;
			//todo: this could cause crash if stylesheet is not immutable
			keyframes = &s->styleKeyframes[iter->second];
			return;
		}
	}
	assert(false);
}

namespace OGUI
{
	float ApplyTimingFunction(AnimTimingFunction function, float percentage)
	{
		//todo
		return percentage;
	}
	bool ShouldReverse(EAnimDirection direction, float iteration)
	{
		int turn = int(iteration);

		if (direction == EAnimDirection::Reverse)
			return true;
		else if(direction == EAnimDirection::Alternate && turn % 2 == 1)
			return true;
		else if(direction == EAnimDirection::Alternate && turn % 2 == 0)
			return true;
		else 
			return false;
	}
}

template<class T>
static bool equal(T f1, T f2)
{
	return (std::fabs(f1 - f2) <= std::numeric_limits<T>::epsilon() * std::fmax(std::fabs(f1), std::fabs(f2)));
}

template<class T>
bool test(T a, T b)
{
	return ((int)a & (int)b) == (int)b;
}

void OGUI::Style::ApplyAnimation(const AnimationStyle& anim, const AnimRunContext& ctx, const Style* parent)
{
	auto sheet = anim.sheet;
	auto keyframes = anim.keyframes;
	int keyId = anim.keyframes->curve.keys.size() - 1;
	float time = ctx.time - anim.animDelay;
	if (time < 0)
	{
		if (test(anim.animFillMode, EAnimFillMode::Backwards))
		{
			//apply first frame
			auto& nextKey = keyframes->curve.keys[0];
			ApplyProperties(sheet->storage, keyframes->frames[nextKey.frameIndex].properties, parent);
		}
		return;
	}

	float iteration = 0.f;
	iteration = ctx.time / anim.animDuration;
	bool reversed = ShouldReverse(anim.animDirections, iteration);
	float percentage = iteration - int(iteration);
	percentage = reversed ? 1 - percentage : percentage;
	if (anim.animIterCount > 0)
	{
		if (iteration >= anim.animIterCount)
		{
			if (test(anim.animFillMode, EAnimFillMode::Forwards))
			{
				//apply last frame
				auto& nextKey = keyframes->curve.keys[keyId];
				ApplyProperties(sheet->storage, keyframes->frames[reversed ? 0 : nextKey.frameIndex].properties, parent);
			}
			return;
		}
		iteration = std::clamp(iteration, 0.f, anim.animIterCount);
	}
	percentage = ApplyTimingFunction(anim.animTimingFunction, percentage);
	percentage = std::clamp(percentage, 0.f, 1.f);
	for (auto&& [i, key] : ipair(keyframes->curve.keys))
	{
		if (key.percentage >= percentage)
		{
			keyId = i;
			break;
		}
	}
	auto& nextKey = keyframes->curve.keys[keyId];
	percentage = std::clamp(percentage, 0.f, nextKey.percentage);
	if (equal(percentage, nextKey.percentage))
		ApplyProperties(sheet->storage, keyframes->frames[nextKey.frameIndex].properties, parent);
	else if (keyId > 1)
	{
		float alpha = percentage / nextKey.percentage;
		auto& key = keyframes->curve.keys[keyId - 1]; 
		ApplyProperties(sheet->storage, keyframes->frames[key.frameIndex].properties, parent);
		if (nextKey.frameIndex != key.frameIndex)
			LerpProperties(sheet->storage, keyframes->frames[nextKey.frameIndex].properties, parent, alpha);
	}
	else
	{
		float alpha = percentage / nextKey.percentage;
		LerpProperties(sheet->storage, keyframes->frames[nextKey.frameIndex].properties, parent, alpha);
	}
}