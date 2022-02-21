

#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Event/AnimEvent.h"
#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Style2/Forward.h"
#include "OpenGUI/Style2/Properties.h"
#include <algorithm>
#include <bitset>
#include "OpenGUI/Style2/VisualStyleSystem.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/generated/transition.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Context.h"

void OGUI::VisualStyleSystem::Update(VisualElement* Tree, bool refresh)
{
	Traverse(Tree, false, refresh);
	OASSERT(matchingContext.styleSheetStack.size() == 0);
	matchingContext.styleSheetStack.clear();
	deferredEvents.Execute();
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

	VisualElement* QueryFirst(VisualElement* root, ostr::string_view str)
	{
		auto selector = CSSParser::ParseSelector(str.encode_to_utf8());
		if (!selector)
			return nullptr;
		return QueryFirst(root, selector.value());
	}

	void QueryAll(VisualElement* root, ostr::string_view str, std::vector<VisualElement*>& result)
	{
		auto selector = CSSParser::ParseSelector(str.encode_to_utf8());
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
		ostr::string_view input,
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
		Lookup(element, matchedSelectors, sheet->typeSelectors, u"*", record);
		if(!element->_name.is_empty())
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

void OGUI::VisualStyleSystem::Traverse(VisualElement* element, bool force, bool refresh)
{
	const std::vector<StyleSheet*>& ess = element->GetStyleSheets();
	auto& sstack = matchingContext.styleSheetStack;
	int originStyleSheetCount = sstack.size();
	for (auto& ss : ess)
	{
		if (std::find(sstack.begin(), sstack.end(), ss) == sstack.end())
			sstack.push_back(ss);
	}
	element->_selectorDirty |= force;
	element->_selectorDirty |= refresh;
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
				ApplyMatchedRules(element, gsl::span<SelectorMatchRecord>(&*begin, end - begin), refresh);
			begin = end;
			end = std::find_if(end, result.end(), [](const SelectorMatchRecord& a) { return a.complexSelector->pseudoElem != PseudoElements::Before; });
			if (begin != result.end())
				ApplyMatchedRules(element->GetBeforePseudoElement(), gsl::span<SelectorMatchRecord>(&*begin, end - begin), refresh);
			else
				element->ReleaseBeforePseudoElement();
			begin = end;
			end = std::find_if(end, result.end(), [](const SelectorMatchRecord& a) { return a.complexSelector->pseudoElem != PseudoElements::After; });
			if (begin != result.end())
				ApplyMatchedRules(element->GetAfterPseudoElement(), gsl::span<SelectorMatchRecord>(&*begin, end - begin), refresh);
			else
				element->ReleaseAfterPseudoElement();
		}
		else if(element->_inlineStyle)
			ApplyMatchedRules(element, gsl::span<SelectorMatchRecord>{}, refresh);
		else
		{
			auto newStyle = ComputedStyle::Create(element->_physicalParent ? &element->_physicalParent->_style : nullptr);
			newStyle.Merge(element->_style, element->_procedureOverrides);
			element->_style = std::move(newStyle);
			element->_anims.clear();
		}
		matchingContext.currentElement = nullptr;
	}
	UpdateStyle(element, sstack);
	if(element->_beforeElement)
		UpdateStyle(element->_beforeElement, sstack);
	if(element->_afterElement)
		UpdateStyle(element->_afterElement, sstack);
	element->Traverse([&](VisualElement* element)
		{
			if(!element->_isPseudoElement)
				Traverse(element, force, refresh);
		});
	element->_selectorDirty = false;
	element->_styleDirty = false;
	int styleSheetCount = sstack.size();
	auto start = sstack.begin();
	if (styleSheetCount > originStyleSheetCount) //pop
		sstack.erase(start + originStyleSheetCount, start + styleSheetCount);
}

void OGUI::VisualStyleSystem::ApplyMatchedRules(VisualElement* element, gsl::span<SelectorMatchRecord> matchedSelectors, bool refresh)
{
	element->_styleDirty = true;
	auto parent = element->_physicalParent ? &element->_physicalParent->_style : nullptr;
	ComputedStyle resolvedStyle = ComputedStyle::Create(parent);
	resolvedStyle.Merge(element->_style, element->_procedureOverrides);
	std::vector<AnimStyle> anims;
	std::vector<TransitionStyle> trans;
	auto ApplyProperties = [&](StyleRule& rule, StyleSheetStorage& storage)
	{
		resolvedStyle.ApplyProperties(storage, rule.properties, element->_procedureOverrides, parent);
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
			style->ApplyProperties(storage, properties.properties);
		}
		for(auto& properties : rule.transition)
		{
			TransitionStyle* style = nullptr;
			for(auto& tran : trans)
			{
				if(tran.transitionProperty == properties.property)
				{
					style = &tran;
					break;
				}
			}
			if(!style)
			{
				trans.push_back({});
				style = &trans.back();
				style->Initialize();
				style->transitionProperty = properties.property;
			}
			style->ApplyProperties(storage, properties.properties);
		}
	};
	for (auto& record : matchedSelectors)
	{
		auto& rule = record.sheet->styleRules[record.complexSelector->ruleIndex];
		ApplyProperties(rule, record.sheet->storage);
	}
	if(element->_inlineStyle)
		ApplyProperties(element->_inlineStyle->rule, element->_inlineStyle->storage);
	element->_transitionStyle = ComputedStyle();
	if(!trans.empty())
	{
		std::vector<size_t> props;
		for(auto tran : trans)
			props.push_back(tran.transitionProperty);
		element->_transitionStyle.MergeId(resolvedStyle, props);
		resolvedStyle.MergeId(element->_preAnimatedStyle, props);
		element->_trans.resize(trans.size());
		for(int i=0; i<trans.size(); ++i)
		{
			element->_trans[i].style = trans[i];
			element->_trans[i].time = 0.f;
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
						anim.time = (newStyle.animationDirection == EAnimDirection::Normal || newStyle.animationDirection == EAnimDirection::Alternate) ? 0.f : 1.f;
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
			if(refresh)
				if(!anim.Init(matchingContext.styleSheetStack))
					return true;
			if(!founded)
			{
				if(anim.style.animationYieldMode != EAnimYieldMode::Stop)
				{
					anim.yielding = true;
					anim.evaluating = true;
					float t = (anim.time - anim.style.animationDelay) / anim.style.animationDuration;
					anim.time = (t - int(t)) * anim.style.animationDuration + anim.style.animationDelay;
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
				{
					auto& anim = element->_anims.emplace_back(std::move(newAnim));
					AnimStartEvent event;
					event.animName = anim.style.animationName;
					deferredEvents.AddEvent(element, event);
				}
			}
		}
	}
}

void OGUI::VisualStyleSystem::UpdateAnimTime(std::vector<ComputedAnim>& anims, VisualElement* element)
{
	auto& ctx = Context::Get();
	auto iter = std::remove_if(anims.begin(), anims.end(), [&](ComputedAnim& anim)
	{
		float maxTime = anim.style.animationDuration * anim.style.animationIterationCount + anim.style.animationDelay;
		bool paused = anim.style.animationPlayState == EAnimPlayState::Paused;
		bool infinite = anim.style.animationIterationCount <= 0.f;
		if (paused || (!infinite && anim.time >= maxTime))
		{
			if(anim.evaluating)
			{
				AnimEndEvent event;
				event.animName = anim.style.animationName;
				deferredEvents.AddEvent(element, event);
			}
			anim.evaluating = false;
		}
		else
		{
			if(!anim.evaluating)
			{
				AnimStartEvent event;
				event.animName = anim.style.animationName;
				deferredEvents.AddEvent(element, event);
			}
			anim.evaluating = true;
		}
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
			{
				AnimStopEvent event;
				event.animName = anim.style.animationName;
				deferredEvents.AddEvent(element, event);
				return true;
			}
		}
		return false;
	});
	if(iter != anims.end())
		anims.erase(iter, anims.end());
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
			damage |= anim.Apply(element->_style, element->_procedureOverrides);
		for (auto& anim : element->_procedureAnims)
			damage |= anim.Apply(element->_style, element->_procedureOverrides);
	}
	element->_prevEvaluating = animationEvaling;
	
	UpdateAnimTime(element->_anims, element);
	UpdateAnimTime(element->_procedureAnims, element);
	return damage;
}

OGUI::RestyleDamage OGUI::VisualStyleSystem::UpdateTransition(VisualElement* element)
{
	auto& ctx = Context::Get();
	auto& trans = element->_trans;
	std::vector<TransitionProperty> props;
	auto iter = std::remove_if(trans.begin(), trans.end(), [&](ComputedTransition& tran)
	{
		tran.time += ctx._deltaTime;
		float alpha = (tran.time - tran.style.transitionDelay) / tran.style.transitionDuration;
		if(alpha < 0)
			return false;
		if(alpha >= 1.f)
			alpha = 1.f;
		props.push_back({tran.style.transitionProperty, ApplyTimingFunction(tran.style.transitionTimingFunction, alpha)});
		return alpha == 1.f;
	});
	if(iter != trans.end())
		trans.erase(iter, trans.end());
	return element->_preAnimatedStyle.ApplyTransitionProperties(element->_transitionStyle, props, element->_procedureOverrides);
}

void OGUI::VisualStyleSystem::UpdateStyle(VisualElement* element, const std::vector<StyleSheet*>& ss)
{
	RestyleDamage damage = element->_selectorDirty ? RestyleDamage::All : RestyleDamage::None;
	damage |= UpdateTransition(element);
	damage |= UpdateAnim(element);
	element->UpdateStyle(damage, ss);
}