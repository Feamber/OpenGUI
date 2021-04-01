#include "OpenGUI/Animation/AnimStyle.h"
#include "OpenGUI/Style/Style.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
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
#define GEN(name, type, id, def)\
			style.name = def;
			ANIMPROP(GEN)
#undef GEN
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

	for (auto& prop : props)
	{
		if ((int)prop.id < (int)StylePropertyId::NumStyle)
			continue;
		int i = prop.arrayIndex;
		if (i >= rules.size())
		{
			names.resize(i + 1);
			rules.resize(i + 1);
		}
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
#define GEN(name, ...)\
		if(propId == StylePropertyId::name) \
		{ \
			Assign(field, InitialStyle.name); \
		}
		ANIMPROP(GEN)
#undef GEN
	}

	template<class T>
	void GetGlobalProperty(T& field, const StyleProperty& prop)
	{
		if (prop.value.index == (int)StyleKeyword::Unset
			|| prop.value.index == (int)StyleKeyword::Initial)
		{
			GetInitialProperty<T>(field, prop.id);
			return;
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
#define GEN(name, type, ...)\
		if(prop.id == StylePropertyId::name) \
		{ \
			if(prop.keyword) \
				GetGlobalProperty<type>(name, prop); \
			else \
				GetProperty<type>(name, prop, sheet); \
			continue; \
		}
		ANIMPROP(GEN)
#undef GEN
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

bool OGUI::AnimationStyle::operator==(const AnimationStyle& other)
{
#define GEN(name, type, ...)\
	if(!(name == other.name)) return false;
	ANIMPROP(GEN)
#undef GEN
	return true;
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
		else if(direction == EAnimDirection::AlternateReverse && turn % 2 == 0)
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

namespace OGUI
{
	void GetTimingFunction(AnimTimingFunction& field, StyleSheetStorage& sheet, gsl::span<StyleProperty> props)
	{
		for(auto& prop : props)
			if (prop.id == StylePropertyId::animTimingFunction)
			{
				if (prop.keyword) 
					GetGlobalProperty<AnimTimingFunction>(field, prop);
				else 
					GetProperty<AnimTimingFunction>(field, prop, sheet);
				return;
			}
	}

	struct AppliedProperty
	{
		StylePropertyId id;
		float percentage;
		bool operator<(const AppliedProperty& other) { return (int)id < (int)other.id; }
	};
}



bool operator<(OGUI::StylePropertyId a, OGUI::StylePropertyId b)
{
	return (int)a < (int)b;
}
bool operator>(OGUI::StylePropertyId a, OGUI::StylePropertyId b)
{
	return (int)a > (int)b;
}

void OGUI::Style::ApplyAnimation(const AnimationStyle& anim, const AnimRunContext& ctx, const Style* parent)
{
	auto sheet = anim.sheet;
	auto keyframes = anim.keyframes;
	float time = ctx.time - anim.animDelay;
	auto applyToLast = [&]()
	{
		int count = keyframes->keys.size();
		for (int i = 0; i < count; ++i)
		{
			auto& key = keyframes->keys[i];
			if (i < 1 || keyframes->keys[i - 1].frameIndex != key.frameIndex) //diff frame
			{
				auto& fp = sheet->styleRules[key.frameIndex].properties;
				ApplyPropertiesFast(sheet->storage, fp, parent); //accelerate
			}
		}
	};
	auto applyFirst = [&]()
	{
		ApplyPropertiesFast(sheet->storage, sheet->styleRules[keyframes->keys[0].frameIndex].properties, parent);
	};
	if (time <= 0)
	{
		if (test(anim.animFillMode, EAnimFillMode::Backwards))
		{
			//apply first frame
			bool reversed = anim.animDirections == EAnimDirection::Reverse || anim.animDirections == EAnimDirection::AlternateReverse;
			if (reversed)
				applyToLast();
			else
				applyFirst();
		}
		return;
	}

	float iteration = 0.f;
	iteration = time / anim.animDuration;
	bool reversed = ShouldReverse(anim.animDirections, iteration);
	float percentage = 0.f;
	if (iteration > int(iteration))
		percentage = iteration - int(iteration);
	else
		percentage = iteration;
	percentage = reversed ? 1 - percentage : percentage;
	if (anim.animIterCount > 0)
	{
		if (iteration >= anim.animIterCount && !ctx.goingback)
		{
			if (test(anim.animFillMode, EAnimFillMode::Forwards))
			{
				//apply last frame
				auto& lastKey = keyframes->keys.back();
				if (reversed)
					applyFirst();
				else
					applyToLast();
			}
			return;
		}
		iteration = std::clamp(iteration, 0.f, anim.animIterCount);
	}

	std::vector<AppliedProperty> appliedProperties;
	std::vector<AppliedProperty> appliedProperties2;
	auto& ap = appliedProperties;
	auto& ap2 = appliedProperties2;
	int an = 0, an2 = 0;
	int i = 0, count = keyframes->keys.size();
	auto merge = [&](gsl::span<StyleProperty> fp, float p)
	{
		int l = 0, r = 0, ln = ap.size(), rn = fp.size();
		ap2.resize(ln + rn); an2 = 0;
		while (l < ln && r < rn)
		{
			if (ap[l].id < fp[r].id) //inherit
				ap2[an2++] = ap[l++];
			else if (ap[l].id > fp[r].id) //new
				ap2[an2++] = {fp[r++].id, p};
			else //override
				ap2[an2++] = {fp[(l++, r++)].id, p};
		}
		while (l < ln) //inherit
			ap2[an2++] = ap[l++];
		while (r < rn) //new
			ap2[an2++] = {fp[r++].id, p};
		std::swap(an2, an);
		std::swap(ap, ap2);
	};
	for (i = 0; i < count ; ++i)
	{
		auto& key = keyframes->keys[i];
		if (key.percentage > percentage)
			break;
		if (i < 1 || keyframes->keys[i - 1].frameIndex != key.frameIndex) //diff frame
		{
			auto& fp = sheet->styleRules[key.frameIndex].properties;
			ApplyPropertiesFast(sheet->storage, fp, parent); //accelerate
			merge(fp, key.percentage);
		}
	}
	std::bitset<96> lerped;
	for (;i < count; ++i)
	{
		auto& key = keyframes->keys[i];
		float alpha = percentage / key.percentage;
		if (i < 1 || keyframes->keys[i - 1].frameIndex != key.frameIndex) //diff frame
		{
			auto& fp = sheet->styleRules[key.frameIndex].properties;
			auto timeFunction = anim.animTimingFunction;
			GetTimingFunction(timeFunction, sheet->storage, fp);
			float p = key.percentage;
			int l = 0, r = 0, ln = ap.size(), rn = fp.size();
			ap2.resize(ln); an2 = 0;
			for(; r<rn; ++r)
			{
				if (lerped.test((int)fp[r].id))
					continue;
				while (l < an && ap[l].id < fp[r].id)
					l++;
				if(l < an && ap[l].id == fp[r].id)
				{
					float alpha = (percentage - ap[l].percentage) / (p - ap[l].percentage);
					alpha = ApplyTimingFunction(timeFunction, alpha);
					alpha = std::clamp(alpha, 0.f, 1.f);
					LerpPropertiesFast(sheet->storage, {&fp[r], 1}, parent, alpha);
				}
				else
				{
					float alpha = percentage / p;
					alpha = ApplyTimingFunction(timeFunction, alpha);
					alpha = std::clamp(alpha, 0.f, 1.f);
					LerpPropertiesFast(sheet->storage, {&fp[r], 1}, parent, alpha);
				}
				lerped.set((int)fp[r].id);
			}
		}
		else
		{
			auto& fp = sheet->styleRules[key.frameIndex].properties;
			for(auto& p : fp)
				lerped.set((int)p.id);
		}
	}
}