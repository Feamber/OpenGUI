
#define DLL_IMPLEMENTATION
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Style2/Properties.h"
#include <algorithm>
#include <bitset>
#include "OpenGUI/Style2/VisualStyleSystem.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Context.h"

void OGUI::VisualStyleSystem::InvalidateCache()
{
	_cacheInvalidated = true;
}

void OGUI::VisualStyleSystem::Update(VisualElement* Tree)
{
	Traverse(Tree, false);
	_cacheInvalidated = false;
	OASSERT(matchingContext.styleSheetStack.size() == 0);
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
		if (selector.pseudoMask != PseudoStates::None)
		{
			match = (selector.pseudoMask & element->_pseudoMask) == selector.pseudoMask;
			if (match)
				element->_dependencyPseudoMask |= selector.pseudoMask;
			else
				element->_triggerPseudoMask |= selector.pseudoMask;
		}
		if (selector.reversedPseudoMask != PseudoStates::None)
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
					current = current->GetHierachyParent();
					if(current)
						current->_depended = true;
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
				saved = current->GetHierachyParent();
				savedIndex = index;
			}
			if (--index < 0)
				return true;

			current = current->GetHierachyParent();
			current->_depended = true;
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
		Lookup(element, matchedSelectors, sheet->typeSelectors, element->GetTypeName(), record);
		Lookup(element, matchedSelectors, sheet->typeSelectors, "*", record);
		if(!element->_name.empty())
			Lookup(element, matchedSelectors, sheet->nameSelectors, element->_name, record);
		for(auto& cls : element->_styleClasses)
			Lookup(element, matchedSelectors, sheet->classSelectors, cls, record);
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

	bool eq(const SelectorMatchRecord& a, const SelectorMatchRecord& b)
	{
		return a.complexSelector->ruleIndex == b.complexSelector->ruleIndex && 
		a.sheet == b.sheet;
	}

	size_t append_hash(size_t value, size_t append)
	{
		return (value * FNV_prime) ^ append;
	}

	template<class T>
	size_t hash(const T& value)
	{
		return std::hash<T>{}(value);
	}

	size_t hash(const StyleRule& rule)
	{
		size_t value = FNV_offset_basis;
		for (auto& prop : rule.properties)
			value = append_hash(value, (int)prop.id + prop.value.index);
		//for (auto& prop : rule.customProperties)
		//	append_hash(value, prop.value.index);
		return value;
	}
}

void OGUI::VisualStyleSystem::Traverse(VisualElement* element, bool force)
{
	const std::vector<StyleSheet*>& ess = element->GetStyleSheets();
	auto& sstack = matchingContext.styleSheetStack;
	int originStyleSheetCount = sstack.size();
	for (auto& ss : ess)
	{
		if (std::find(sstack.begin(), sstack.end(), ss) == sstack.end())
			sstack.push_back(ss);
	}
	if (_cacheInvalidated)
		element->ResetStyles();
	element->_selectorDirty |= force;
	if(element->_selectorDirty)
	{
		if(element->_depended)
			force = true;
		element->_depended = false;
		element->_dependencyPseudoMask = element->_triggerPseudoMask = PseudoStates::None;
		matchingContext.currentElement = element;
		std::vector<SelectorMatchRecord> result;
		//TODO: share match result?
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
	element->Traverse([&](VisualElement* element)
		{
			if(!element->_isPseudoElement)
				Traverse(element, force);
		});
	element->_selectorDirty = false;
	element->_styleDirty = false;
	int styleSheetCount = sstack.size();
	auto start = sstack.begin();
	if (styleSheetCount > originStyleSheetCount) //pop
		sstack.erase(start + originStyleSheetCount, start + styleSheetCount);
}



void OGUI::VisualStyleSystem::ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors)
{
	element->_styleDirty = true;
	auto parent = element->_logicalParent ? &element->_logicalParent->_style : nullptr;
	ComputedStyle resolvedStyle = ComputedStyle::Create(parent);
	std::vector<AnimStyle> anims;
	for (auto& record : matchedSelectors)
	{
		auto& rule = record.sheet->styleRules[record.complexSelector->ruleIndex];
		resolvedStyle.ApplyProperties(record.sheet->storage, rule.properties, parent);
		for(auto& properties : rule.animation)
		{
			AnimStyle* style = nullptr;
			for(auto& anim : anims)
			{
				if(anim.animationName == properties.name)
				{
					style = &anim;
					break;
				}
			}
			if(!style)
			{
				anims.push_back({});
				style = &anims.back();
				style->Initialize();
				style->animationName = properties.name;
			}
			style->ApplyProperties(record.sheet->storage, properties.properties);
		}
	}
	{
		element->_preAnimatedStyle = std::move(resolvedStyle);
		element->_style = element->_preAnimatedStyle;
	}
	{
		std::vector<bool> dynbitset;
		dynbitset.resize(anims.size());
		std::vector<ComputedAnim> newAnims;

		auto iter = std::remove_if(element->_anims.begin(), element->_anims.end(), [&](ComputedAnim& anim)
		{
			bool founded = false;
			for(int i=0; i<anims.size(); ++i)
			{
				auto& newStyle = anims[i];

				if(anim.style.animationName == newStyle.animationName)
				{
					if (anim.style.animationResumeMode == EAnimResumeMode::Reset && anim.yielding)
					{
						//Play from begining
						anim.time = 0;
					}
					else
					{	//Sync iteration
						float iter = (anim.time - anim.style.animationDelay) / anim.style.animationDuration;
						if (((int)newStyle.animationDirection % 2) != ((int)anim.style.animationDirection % 2))
							iter = iter + (1 - (iter - (int)iter));
						anim.time = iter * newStyle.animationDuration + newStyle.animationDelay;
					}
					anim.goingback = false;
					anim.yielding = false;
					anim.style = newStyle;
					founded = true;
					dynbitset[i] = true;
					break;
				}
			}
			if(_cacheInvalidated)
				if(!anim.Init(matchingContext.styleSheetStack))
					return true;
			if(!founded)
			{
				if(anim.style.animationYieldMode != EAnimYieldMode::Stop)
				{
					anim.yielding = true;
					anim.evaluating = true;
					anim.goingback = anim.style.animationYieldMode == EAnimYieldMode::GoBack;
				}
				else 
					return true;
			}
			return false;
		});
		if(iter != element->_anims.end())
			element->_anims.erase(iter, element->_anims.end());

		for (int i = 0; i < dynbitset.size(); ++i)
		{
			if(!dynbitset[i])
			{
				ComputedAnim newAnim;
				newAnim.style = anims[i];
				if(newAnim.Init(matchingContext.styleSheetStack))
					element->_anims.push_back(std::move(newAnim));
			}
		}
	}
}

namespace OGUI
{
	void UpdateAnimTime(std::vector<ComputedAnim>& anims)
	{
		auto& ctx = Context::Get();
		auto iter = std::remove_if(anims.begin(), anims.end(), [&](ComputedAnim& anim)
		{
			float maxTime = anim.style.animationDuration * anim.style.animationIterationCount + anim.style.animationDelay;
			bool paused = anim.style.animationPlayState == EAnimPlayState::Paused;
			bool infinite = anim.style.animationIterationCount <= 0.f;
			if (paused || (!infinite && anim.time >= maxTime))
				anim.evaluating = false;
			else
				anim.evaluating = true;
			if (!paused || anim.yielding)
			{
				if (anim.goingback)
					anim.time = std::max(anim.style.animationDelay, anim.time - ctx._deltaTime);
				else if (anim.style.animationIterationCount > 0.f)
					anim.time = std::min(ctx._deltaTime + anim.time, maxTime);
				else
					anim.time += ctx._deltaTime;
			}
			if (anim.yielding)
			{
				bool shouldStop = false;
				if (!anim.goingback && anim.style.animationIterationCount > 0 && anim.time >= maxTime)
					shouldStop = true;
				else if(anim.goingback && anim.time <= anim.style.animationDelay)
					shouldStop = true;
				if (shouldStop)
					return true;
			}
			return false;
		});
		if(iter != anims.end())
			anims.erase(iter, anims.end());
	}
}

OGUI::RestyleDamage OGUI::VisualStyleSystem::UpdateAnim(VisualElement* element)
{
	RestyleDamage damage = RestyleDamage::None;
	bool animationEvaling = false;
	for (auto& ctx : element->_anims)
		animationEvaling |= ctx.evaluating;
	for (auto& ctx : element->_procedureAnims)
		animationEvaling |= ctx.evaluating;
	if (element->_anims.empty() && element->_procedureAnims.empty() && element->_prevEvaluating)
	{
		element->_style = element->_preAnimatedStyle;
		element->SyncYogaStyle();
		element->MarkStyleTransformDirty();
	}
	if (animationEvaling || element->_styleDirty)
	{
		element->_style = element->_preAnimatedStyle;
		for (auto& anim : element->_anims)
			damage |= anim.Apply(element->_style);
		for (auto& anim : element->_procedureAnims)
			damage |= anim.Apply(element->_style);
	}
	element->_prevEvaluating = animationEvaling;
	
	UpdateAnimTime(element->_anims);
	UpdateAnimTime(element->_procedureAnims);
	return damage;
}

void OGUI::VisualStyleSystem::UpdateStyle(VisualElement* element)
{
	RestyleDamage damage = RestyleDamage::None;
	damage |= UpdateAnim(element);
	damage |= element->ApplyProcedureStyle();
	if((damage & RestyleDamage::Yoga) == RestyleDamage::Yoga || element->_selectorDirty)
		element->SyncYogaStyle();
	if((damage & RestyleDamage::Transform) == RestyleDamage::Transform || element->_selectorDirty)
		element->MarkStyleTransformDirty();
}