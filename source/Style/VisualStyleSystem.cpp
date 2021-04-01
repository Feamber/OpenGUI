#include <algorithm>
#include <bitset>
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style/StyleSelector.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"

void OGUI::VisualStyleSystem::InvalidateCache()
{
	styleCache.clear(); 
	_cacheInvalidated = true;
}

void OGUI::VisualStyleSystem::Update(VisualElement* Tree)
{
	//TODO: lazy update
	Traverse(Tree);
	_cacheInvalidated = false;
	assert(matchingContext.styleSheetStack.size() == 0);
	matchingContext.styleSheetStack.clear();
}

namespace OGUI
{
	bool Match(VisualElement* element, StyleSelector& selector)
	{
		bool match = true;

		for(auto& part : selector.parts)
		{
			switch (part.type)
			{
				case StyleSelector::Wildcard:
					break;
				case StyleSelector::Class:
					match = element->ContainClass(part.value);
					break;
				case StyleSelector::Name:
					match = element->_name == part.value;
					break;
				case StyleSelector::Type:
					match = element->IsA(part.value);
					break;
				default:
					match = false;
					break;
			}
			if (!match)
				return false;
		}
		if (selector.pseudoMask != 0)
		{
			match = (selector.pseudoMask & element->_pseudoMask) == selector.pseudoMask;
			if (match)
				element->_dependencyPseudoMask |= selector.pseudoMask;
			else
				element->_triggerPseudoMask |= selector.pseudoMask;
		}
		if (selector.reversedPseudoMask != 0)
		{
			match &= (selector.pseudoMask & ~element->_pseudoMask) == selector.pseudoMask;
			if (match)
				element->_dependencyPseudoMask |= selector.pseudoMask;
			else
				element->_triggerPseudoMask |= selector.pseudoMask;
		}
		return match;
	}

	bool Match(VisualElement* current, StyleComplexSelector& complexSel)
	{
		//https://speakerdeck.com/constellation/css-jit-just-in-time-compiled-css-selectors-in-webkit
		auto& selectors = complexSel.selectors;
		int count = selectors.size();
		int index = count - 1;
		VisualElement* saved = nullptr;
		int savedIndex = -1;

		while (index >= 0)
		{
			if (current == nullptr)
				break;

			bool success = Match(current, selectors[index]);
			if (!success)
			{
				if (index < count - 1 && selectors[index + 1].relationship == StyleSelectorRelationship::Descendent)
				{
					current = current->GetParent();
					continue;
				}
				if (saved != nullptr)
				{
					current = saved;
					index = savedIndex;
					continue;
				}
				break;
			}
			if (index < count - 1 && selectors[index + 1].relationship == StyleSelectorRelationship::Descendent)
			{
				saved = current->GetParent();
				savedIndex = index;
			}
			if (--index < 0)
				return true;

			current = current->GetParent();
		}
		return false;
	}

	VisualElement* QueryFirst(VisualElement* root, std::string_view str)
	{
		auto selector = ParseSelector(str);
		if (!selector)
			return nullptr;
		return QueryFirst(root, selector.value());
	}
	void QueryAll(VisualElement* root, std::string_view str, std::vector<VisualElement*>& result)
	{
		auto selector = ParseSelector(str);
		if (!selector)
			return;
		return QueryAll(root, selector.value(), result);
	}

	VisualElement* QueryFirst(VisualElement* root, StyleComplexSelector& complexSel)
	{
		if (Match(root, complexSel))
			return root;
		for (auto& child : root->_children)
			if (auto elem = QueryFirst(child, complexSel))
				return elem;
		return nullptr;
	}

	void QueryAll(VisualElement* root, StyleComplexSelector& complexSel, std::vector<VisualElement*>& result)
	{
		if (Match(root, complexSel))
			result.push_back(root);
		for (auto& child : root->_children)
			QueryAll(child, complexSel, result);
	}

	void Lookup(
		VisualElement* current,
		std::vector<SelectorMatchRecord>& matchedSelectors,
		StyleSheet::SelectorMap& map,
		std::string_view input,
		SelectorMatchRecord& record)
	{
		auto range = map.equal_range(input);
		if (range.first != map.end())
		{
			for (auto iter = range.first; iter != range.second; ++iter)
			{
				auto& selector = record.sheet->styleSelectors[iter->second];
				if (Match(current, selector))
				{
					record.complexSelector = &selector;
					matchedSelectors.push_back(record);
				}
			}
		}
	}
}

void OGUI::VisualStyleSystem::FindMatches(StyleMatchingContext& context, std::vector<SelectorMatchRecord>& matchedSelectors)
{
	matchedSelectors.clear();
	//check(context.currentElement != nullptr);
	auto element = context.currentElement;
	auto sheetCount = context.styleSheetStack.size();
	for (int i = 0; i < sheetCount; ++i)
	{
		StyleSheet* sheet = context.styleSheetStack[i];
		SelectorMatchRecord record{sheet, i, nullptr};
		Lookup(context.currentElement, matchedSelectors, sheet->typeSelectors, element->GetTypeName(), record);
		Lookup(context.currentElement, matchedSelectors, sheet->typeSelectors, "*", record);
		if(!element->_name.empty())
			Lookup(context.currentElement, matchedSelectors, sheet->nameSelectors, element->_name, record);
		for(auto& cls : element->_styleClasses)
			Lookup(context.currentElement, matchedSelectors, sheet->classSelectors, cls, record);
	}
}


namespace OGUI
{
	//TODO: Utilitilize those function?
	template<class T>
	int cmp(const T& a, const T& b)
	{
		if (a > b)
			return 1;
		if (a < b)
			return -1;
		else
			return 0;
	}

	int cmp(const std::string& a, const std::string& b)
	{
		return a.compare(b);
	}

	int cmp(const SelectorMatchRecord& a, const SelectorMatchRecord& b)
	{
		int result = cmp((int)a.complexSelector->pseudoElem, (int)b.complexSelector->pseudoElem);
		if (result != 0)
			return result;
		result = cmp(a.complexSelector->specificity, b.complexSelector->specificity);
		if (result != 0)
			return result;
		result = cmp(a.sheetIndex, b.sheetIndex);
		if (result != 0)
			return result;
		result = cmp(a.complexSelector->priority, b.complexSelector->priority);
		return result;
	}

#if defined(_X32)
	static constexpr size_t _FNV_offset_basis = 2166136261U;
	static constexpr size_t _FNV_prime = 16777619U;
#else // defined(_X32)
	static constexpr size_t _FNV_offset_basis = 14695981039346656037ULL;
	static constexpr size_t _FNV_prime = 1099511628211ULL;
#endif // defined(_X32)

	size_t append_hash(size_t value, size_t append)
	{
		return (value * _FNV_prime) ^ append;
	}

	template<class T>
	size_t hash(const T& value)
	{
		return std::hash<T>{}(value);
	}

	size_t hash(const StyleRule& rule)
	{
		size_t value = _FNV_offset_basis;
		for (auto& prop : rule.properties)
			value = append_hash(value, (int)prop.id + prop.value.index);
		//for (auto& prop : rule.customProperties)
		//	append_hash(value, prop.value.index);
		return value;
	}

}

namespace OGUI
{
	void GetOverrideMask(const gsl::span<StyleProperty>& props, std::bitset<96>& ovr, std::bitset<96>& iht)
	{
		for (auto& prop : props)
		{
			if (prop.keyword)
			{
				ovr.set((int)prop.id, false);
				if (prop.value.index == (int)StyleKeyword::Inherit)
					iht.set((int)prop.id);
				if (prop.value.index == (int)StyleKeyword::Initial)
					iht.set((int)prop.id, false);
				continue;
			}
			else
			{
				ovr.set((int)prop.id, true);
				iht.set((int)prop.id, false);
			}
		}
	}

	template<class T>
	bool same(const T& a, const T& b) { return a == b; }

	bool same(float a, float b) { return (std::isnan(a) && std::isnan(b)) || a == b; }

	struct YogaStyle
	{
#define GEN(name, type, ...) \
		type name;
		YOGAPROP(GEN)
#undef GEN

		YogaStyle(const Style& style)
		{
#define GEN(name, ...) \
			name = style.name;
			YOGAPROP(GEN)
#undef GEN
		}
		bool operator==(const Style& style)
		{
#define GEN(name, ...) \
			if(!same(name, style.name)) \
				return false; 
			YOGAPROP(GEN)
#undef GEN
			return true;
		}
	};

	struct TRSStyle
	{
#define GEN(name, type, ...) \
		type name;
		TRSPROP(GEN)
#undef GEN

		TRSStyle(const Style& style)
		{
#define GEN(name, ...) \
			name = style.name;
			TRSPROP(GEN)
#undef GEN
		}
		bool operator==(const Style& style)
		{
#define GEN(name, ...) \
			if(!(name == style.name)) \
				return false;
			TRSPROP(GEN)
#undef GEN
			return true;
		}
	};
}

void OGUI::VisualStyleSystem::Traverse(VisualElement* element)
{
	const std::vector<StyleSheet*>& ess = element->GetStyleSheets();
	auto& sstack = matchingContext.styleSheetStack;
	int originStyleSheetCount = sstack.size();
	for (auto& ss : ess)
	{
		if (std::find(sstack.begin(), sstack.end(), ss) == sstack.end())
			sstack.push_back(ss);
	}
	if(element->_selectorDirty || _cacheInvalidated)
	{
		element->_selectorDirty = false;
		matchingContext.currentElement = element;
		std::vector<SelectorMatchRecord> result;
		FindMatches(matchingContext, result);
		if (result.size() > 0)
		{
			std::sort(result.begin(), result.end(), [](const SelectorMatchRecord& a, const SelectorMatchRecord& b) { return cmp(a, b) < 0; });
			auto begin = result.begin();
			auto end = std::find_if(result.begin(), result.end(), [](const SelectorMatchRecord& a) { return a.complexSelector->pseudoElem != PseudoElements::None; });
			if (begin != result.end())
				ApplyMatchedRules(element, gsl::span<SelectorMatchRecord>(&*begin, end - begin));
			begin = end;
			end = std::find_if(end, result.end(), [](const SelectorMatchRecord& a) { return a.complexSelector->pseudoElem != PseudoElements::Before; });
			if (begin != result.end())
				ApplyMatchedRules(element->GetBeforePseudoElement(), gsl::span<SelectorMatchRecord>(&*begin, end - begin));
			else
				element->ReleaseBeforePseudoElement();
			begin = end;
			end = std::find_if(end, result.end(), [](const SelectorMatchRecord& a) { return a.complexSelector->pseudoElem != PseudoElements::After; });
			if (begin != result.end())
				ApplyMatchedRules(element->GetAfterPseudoElement(), gsl::span<SelectorMatchRecord>(&*begin, end - begin));
			else
				element->ReleaseAfterPseudoElement();
		}
		matchingContext.currentElement = nullptr;
	}
	UpdateStyle(element);
	if(element->_beforeElement)
		UpdateStyle(element->_beforeElement);
	if(element->_afterElement)
		UpdateStyle(element->_afterElement);
	element->Traverse([this](VisualElement* element)
		{
			if(!element->_isPseudoElement)
				Traverse(element);
		});
	element->_styleDirty = false;
	int styleSheetCount = sstack.size();
	auto start = sstack.begin();
	if (styleSheetCount > originStyleSheetCount) //pop
		sstack.erase(start + originStyleSheetCount, start + styleSheetCount);
}

void OGUI::VisualStyleSystem::ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors)
{
	size_t matchHash = hash(element->GetFullTypeName());

	for (auto& record : matchedSelectors)
	{
		matchHash = append_hash(matchHash, (size_t)record.sheet);
		matchHash = append_hash(matchHash, (size_t)record.complexSelector->ruleIndex);
		matchHash = append_hash(matchHash, record.complexSelector->specificity);
	}
	CachedStyle* sharedStyle = nullptr;
	auto iter = styleCache.find(matchHash);
	if (iter != styleCache.end()) //todo: double check identity?
	{
		sharedStyle = iter->second.get();
	}
	else
	{
		std::bitset<96> overrideMask = {}, inheritMask = GetInheritMask();
		Style resolvedStyle = Style::Create(nullptr, true);
		std::vector<AnimationStyle> animStyles;
		for (auto& record : matchedSelectors)
		{
			auto& rule = record.sheet->styleRules[record.complexSelector->ruleIndex];
			GetOverrideMask(rule.properties, overrideMask, inheritMask);
			resolvedStyle.ApplyPropertiesFast(record.sheet->storage, rule.properties, nullptr);
			AnimationStyle::ApplyProperties(animStyles, record.sheet->storage, rule.properties);
		}
		CachedStyle cs{
			overrideMask,
			inheritMask,
			std::move(resolvedStyle),
			std::move(animStyles)
		};
		auto pair = styleCache.emplace(matchHash, std::make_unique<CachedStyle>(std::move(cs)));
		//check(pair.second);
		sharedStyle = pair.first->second.get();
	}
	{
		element->_sharedDirty = element->_sharedStyle != sharedStyle || _cacheInvalidated;
		element->_sharedStyle = sharedStyle;
	}
}

void OGUI::VisualStyleSystem::UpdateStyle(VisualElement* element)
{
	/*
	* anim style =
	*	 yielding + shared + inline/procedure
	* animated rule =
	*	anim style + anim context
	* final style =
	*	initial style +
	*	parent style * inherit mask +
	*	shared style * override mask +
	*	animated rule +
	*	inline/procedure rule +
	*
	* animated rule and inline rule could change frequently
	* so they are not cached
	*/
	VisualElement* parent = element->GetHierachyParent();
	auto parentStyle = parent ? &parent->_style : nullptr;
	bool sharedDirty = element->_sharedDirty;
	element->_sharedDirty = false;
	bool procedureDirty = element->_procedureStyleDirty;
	element->_procedureStyleDirty = false;
	bool parentDirty = parent ? parent->_styleDirty : false;
	if (procedureDirty || sharedDirty)
	{
		std::vector<AnimationStyle> anims;
		if (element->_sharedStyle)
			anims = element->_sharedStyle->animStyles;
		std::vector<AnimRunContext> ctxs;
		if (element->_inlineStyle)
			AnimationStyle::ApplyProperties(anims, element->_inlineStyle->storage, element->_inlineStyle->rule.properties);
		if (element->_procedureStyle)
			AnimationStyle::ApplyProperties(anims, element->_procedureStyle->storage, element->_procedureStyle->rule.properties);
		ctxs.resize(anims.size());
		//Inherit Context
		std::vector<bool> dynbitset;
		dynbitset.resize(element->_animContext.size());
		for (auto&& [i, anim] : ipair(anims))
		{
			bool founded = false;
			for (auto&& [j, oldAnim] : ipair(element->_animStyles))
			{
				if (oldAnim.animName == anim.animName)
				{
					anim.keyframes = oldAnim.keyframes;
					anim.sheet = oldAnim.sheet;
					ctxs[i] = element->_animContext[j];
					if (anim.animResumeMode == EAnimResumeMode::Reset && element->_animContext[j].yielding)
					{
						//Play from begining
						ctxs[i].time = 0;
					}
					else
					{	//Sync iteration
						auto& oldCtx = element->_animContext[j];
						float iter = (oldCtx.time - oldAnim.animDelay) / oldAnim.animDuration;
						if (((int)anim.animDirections % 2) != ((int)oldAnim.animDirections % 2))
							iter = iter + (1 - (iter - (int)iter));
						ctxs[i].time = iter * anim.animDuration + anim.animDelay;
					}
					ctxs[i].goingback = false;
					ctxs[i].yielding = false;
					founded = true;
					dynbitset[j] = true;
					break;
				}
			}
			if (!founded)
				anim.ResolveReference(matchingContext.styleSheetStack);
		}
		std::vector<AnimationStyle> yieldingAnims;
		std::vector<AnimRunContext> yieldingCtxs;
		if (_cacheInvalidated) //just remove if cache is invalidated
			goto noYield;
		for (int i = 0; i < dynbitset.size(); ++i)
		{
			if (!dynbitset[i] && element->_animStyles[i].animYieldMode != EAnimYieldMode::Stop)
			{
				yieldingAnims.emplace_back(std::move(element->_animStyles[i]));
				auto& ctx = yieldingCtxs.emplace_back(std::move(element->_animContext[i]));
				ctx.yielding = true;
				ctx.evaluating = true;
				ctx.goingback = element->_animStyles[i].animYieldMode == EAnimYieldMode::Goback;
			}
		}
		if (yieldingAnims.size() > 0)
		{
			yieldingAnims.reserve(yieldingAnims.size() + anims.size());
			std::move(anims.begin(), anims.end(), std::back_inserter(yieldingAnims));
			yieldingCtxs.reserve(yieldingCtxs.size() + ctxs.size());
			std::move(ctxs.begin(), ctxs.end(), std::back_inserter(yieldingCtxs));
			std::swap(element->_animContext, yieldingCtxs);
			std::swap(element->_animStyles, yieldingAnims);
		}
		else
		{
			noYield:
			std::swap(element->_animContext, ctxs);
			std::swap(element->_animStyles, anims);
		}
	}
	bool styleDirty = false;
	YogaStyle yogaStyle = element->_style;
	TRSStyle trsStyle = element->_style;
	if (sharedDirty || procedureDirty || parentDirty)
	{
		element->_preAnimatedStyle = Style::Create(nullptr, true);
		std::bitset<96> overrideMask = {}, inheritMask = GetInheritMask();
		if (element->_sharedStyle)
		{
			inheritMask = element->_sharedStyle->inheritMask;
			overrideMask = element->_sharedStyle->overrideMask;
		}
		if (parentStyle)
			element->_preAnimatedStyle.MergeStyle(*parentStyle, inheritMask);
		if (element->_sharedStyle)
			element->_preAnimatedStyle.MergeStyle(element->_sharedStyle->style, overrideMask);
		if (element->_inlineStyle)
			element->_preAnimatedStyle.ApplyPropertiesFast(element->_inlineStyle->storage, element->_inlineStyle->rule.properties, parentStyle);
		if (element->_procedureStyle)
			element->_preAnimatedStyle.ApplyPropertiesFast(element->_procedureStyle->storage, element->_procedureStyle->rule.properties, parentStyle);
		styleDirty = true;
	}
	bool animationEvaling = false;
	for (auto& ctx : element->_animContext)
		animationEvaling |= ctx.evaluating;
	if (element->_animContext.size() == 0 && element->_prevEvaluating)
		styleDirty = true;
	if (animationEvaling || styleDirty)
	{
		element->_style = element->_preAnimatedStyle;
		for (auto&& [i, anim] : ipair(element->_animStyles))
			element->_style.ApplyAnimation(anim, element->_animContext[i], parentStyle);
		styleDirty = true;
	}
	element->_prevEvaluating = animationEvaling;
	if (styleDirty)
	{
		element->_styleDirty = styleDirty; //todo: check identity?
		if (!(yogaStyle == element->_style))
			element->SyncYogaStyle();
		if (!(trsStyle == element->_style))
			element->_transformDirty = true;
		//todo: shape dirty?
	}
}
