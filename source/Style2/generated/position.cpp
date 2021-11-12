//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp
#define DLL_IMPLEMENTATION
#include <memory>
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "position_shorthands.h"

const OGUI::StylePosition& OGUI::StylePosition::GetDefault()
{
    struct Helper
    {
        OGUI::StylePosition value;
        Helper()
        {
            value.Initialize();
        }
    };
    static Helper helper;
    return helper.value;
}

const OGUI::StylePosition& OGUI::StylePosition::Get(const ComputedStyle& style)
{
    auto value = TryGet(style);
    if(!value)
    {
        return GetDefault();
    }
    return *value;
}

OGUI::StylePosition* OGUI::StylePosition::TryGet(const ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        return nullptr;
    }
    else 
    {
        return (OGUI::StylePosition*)iter->second.ptr.get();
    }
}

OGUI::StylePosition& OGUI::StylePosition::GetOrAdd(ComputedStyle& style)
{
    auto iter = style.structs.find(hash);
    if(iter == style.structs.end())
    {
        auto value = std::make_shared<OGUI::StylePosition>();
        value->Initialize();
        style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
        return *value.get();
    }
    else 
    {
        return *(OGUI::StylePosition*)iter->second.ptr.get();
    }

}

void OGUI::StylePosition::Dispose(ComputedStyle& style)
{
    style.structs.erase(hash);
}

void OGUI::StylePosition::Initialize()
{
    transform = {};
    flexGrow = 0.f;
    flexShrink = 1.f;
    flexBasis = YGValueAuto;
    left = YGValueAuto;
    top = YGValueAuto;
    right = YGValueAuto;
    bottom = YGValueAuto;
    marginLeft = YGValueZero;
    marginTop = YGValueZero;
    marginRight = YGValueZero;
    marginBottom = YGValueZero;
    paddingLeft = YGValueZero;
    paddingTop = YGValueZero;
    paddingRight = YGValueZero;
    paddingBottom = YGValueZero;
    width = YGValueAuto;
    height = YGValueAuto;
    position = YGPositionTypeRelative;
    overflow = YGOverflowVisible;
    alignSelf = YGAlignAuto;
    maxWidth = YGValueUndefined;
    maxHeight = YGValueUndefined;
    minWidth = YGValueAuto;
    minHeight = YGValueAuto;
    flexDirection = YGFlexDirectionRow;
    alignContent = YGAlignFlexStart;
    alignItems = YGAlignStretch;
    justifyContent = YGJustifyFlexStart;
    flexWrap = YGWrapNoWrap;
    flexDisplay = YGDisplayFlex;
}

template<class T>
std::vector<T> ToOwned(gsl::span<T> s)
{
    return {s.begin(), s.end()};
}

void OGUI::StylePosition::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StylePosition* st = nullptr;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StylePosition*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StylePosition>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StylePosition>(*st);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Id::transform:{
                    auto v = fget();
                    v->transform = {};
                    break;
                    }
                case Id::flexGrow:{
                    auto v = fget();
                    v->flexGrow = 0.f;
                    break;
                    }
                case Id::flexShrink:{
                    auto v = fget();
                    v->flexShrink = 1.f;
                    break;
                    }
                case Id::flexBasis:{
                    auto v = fget();
                    v->flexBasis = YGValueAuto;
                    break;
                    }
                case Id::left:{
                    auto v = fget();
                    v->left = YGValueAuto;
                    break;
                    }
                case Id::top:{
                    auto v = fget();
                    v->top = YGValueAuto;
                    break;
                    }
                case Id::right:{
                    auto v = fget();
                    v->right = YGValueAuto;
                    break;
                    }
                case Id::bottom:{
                    auto v = fget();
                    v->bottom = YGValueAuto;
                    break;
                    }
                case Id::marginLeft:{
                    auto v = fget();
                    v->marginLeft = YGValueZero;
                    break;
                    }
                case Id::marginTop:{
                    auto v = fget();
                    v->marginTop = YGValueZero;
                    break;
                    }
                case Id::marginRight:{
                    auto v = fget();
                    v->marginRight = YGValueZero;
                    break;
                    }
                case Id::marginBottom:{
                    auto v = fget();
                    v->marginBottom = YGValueZero;
                    break;
                    }
                case Id::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = YGValueZero;
                    break;
                    }
                case Id::paddingTop:{
                    auto v = fget();
                    v->paddingTop = YGValueZero;
                    break;
                    }
                case Id::paddingRight:{
                    auto v = fget();
                    v->paddingRight = YGValueZero;
                    break;
                    }
                case Id::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = YGValueZero;
                    break;
                    }
                case Id::width:{
                    auto v = fget();
                    v->width = YGValueAuto;
                    break;
                    }
                case Id::height:{
                    auto v = fget();
                    v->height = YGValueAuto;
                    break;
                    }
                case Id::position:{
                    auto v = fget();
                    v->position = YGPositionTypeRelative;
                    break;
                    }
                case Id::overflow:{
                    auto v = fget();
                    v->overflow = YGOverflowVisible;
                    break;
                    }
                case Id::alignSelf:{
                    auto v = fget();
                    v->alignSelf = YGAlignAuto;
                    break;
                    }
                case Id::maxWidth:{
                    auto v = fget();
                    v->maxWidth = YGValueUndefined;
                    break;
                    }
                case Id::maxHeight:{
                    auto v = fget();
                    v->maxHeight = YGValueUndefined;
                    break;
                    }
                case Id::minWidth:{
                    auto v = fget();
                    v->minWidth = YGValueAuto;
                    break;
                    }
                case Id::minHeight:{
                    auto v = fget();
                    v->minHeight = YGValueAuto;
                    break;
                    }
                case Id::flexDirection:{
                    auto v = fget();
                    v->flexDirection = YGFlexDirectionRow;
                    break;
                    }
                case Id::alignContent:{
                    auto v = fget();
                    v->alignContent = YGAlignFlexStart;
                    break;
                    }
                case Id::alignItems:{
                    auto v = fget();
                    v->alignItems = YGAlignStretch;
                    break;
                    }
                case Id::justifyContent:{
                    auto v = fget();
                    v->justifyContent = YGJustifyFlexStart;
                    break;
                    }
                case Id::flexWrap:{
                    auto v = fget();
                    v->flexWrap = YGWrapNoWrap;
                    break;
                    }
                case Id::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = YGDisplayFlex;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Id::transform:{
                    auto v = fget();
                    v->transform = pst->transform;
                    break;
                    }
                case Id::flexGrow:{
                    auto v = fget();
                    v->flexGrow = pst->flexGrow;
                    break;
                    }
                case Id::flexShrink:{
                    auto v = fget();
                    v->flexShrink = pst->flexShrink;
                    break;
                    }
                case Id::flexBasis:{
                    auto v = fget();
                    v->flexBasis = pst->flexBasis;
                    break;
                    }
                case Id::left:{
                    auto v = fget();
                    v->left = pst->left;
                    break;
                    }
                case Id::top:{
                    auto v = fget();
                    v->top = pst->top;
                    break;
                    }
                case Id::right:{
                    auto v = fget();
                    v->right = pst->right;
                    break;
                    }
                case Id::bottom:{
                    auto v = fget();
                    v->bottom = pst->bottom;
                    break;
                    }
                case Id::marginLeft:{
                    auto v = fget();
                    v->marginLeft = pst->marginLeft;
                    break;
                    }
                case Id::marginTop:{
                    auto v = fget();
                    v->marginTop = pst->marginTop;
                    break;
                    }
                case Id::marginRight:{
                    auto v = fget();
                    v->marginRight = pst->marginRight;
                    break;
                    }
                case Id::marginBottom:{
                    auto v = fget();
                    v->marginBottom = pst->marginBottom;
                    break;
                    }
                case Id::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = pst->paddingLeft;
                    break;
                    }
                case Id::paddingTop:{
                    auto v = fget();
                    v->paddingTop = pst->paddingTop;
                    break;
                    }
                case Id::paddingRight:{
                    auto v = fget();
                    v->paddingRight = pst->paddingRight;
                    break;
                    }
                case Id::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = pst->paddingBottom;
                    break;
                    }
                case Id::width:{
                    auto v = fget();
                    v->width = pst->width;
                    break;
                    }
                case Id::height:{
                    auto v = fget();
                    v->height = pst->height;
                    break;
                    }
                case Id::position:{
                    auto v = fget();
                    v->position = pst->position;
                    break;
                    }
                case Id::overflow:{
                    auto v = fget();
                    v->overflow = pst->overflow;
                    break;
                    }
                case Id::alignSelf:{
                    auto v = fget();
                    v->alignSelf = pst->alignSelf;
                    break;
                    }
                case Id::maxWidth:{
                    auto v = fget();
                    v->maxWidth = pst->maxWidth;
                    break;
                    }
                case Id::maxHeight:{
                    auto v = fget();
                    v->maxHeight = pst->maxHeight;
                    break;
                    }
                case Id::minWidth:{
                    auto v = fget();
                    v->minWidth = pst->minWidth;
                    break;
                    }
                case Id::minHeight:{
                    auto v = fget();
                    v->minHeight = pst->minHeight;
                    break;
                    }
                case Id::flexDirection:{
                    auto v = fget();
                    v->flexDirection = pst->flexDirection;
                    break;
                    }
                case Id::alignContent:{
                    auto v = fget();
                    v->alignContent = pst->alignContent;
                    break;
                    }
                case Id::alignItems:{
                    auto v = fget();
                    v->alignItems = pst->alignItems;
                    break;
                    }
                case Id::justifyContent:{
                    auto v = fget();
                    v->justifyContent = pst->justifyContent;
                    break;
                    }
                case Id::flexWrap:{
                    auto v = fget();
                    v->flexWrap = pst->flexWrap;
                    break;
                    }
                case Id::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = pst->flexDisplay;
                    break;
                    }
                default: break;
                }
            }
        }
        else
        {
            switch(prop.id)
            {
                case Id::transform:{
                    auto v = fget();
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.value));
                    break;
                    }
                case Id::flexGrow:{
                    auto v = fget();
                    v->flexGrow = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::flexShrink:{
                    auto v = fget();
                    v->flexShrink = sheet.Get<float>(prop.value);
                    break;
                    }
                case Id::flexBasis:{
                    auto v = fget();
                    v->flexBasis = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::left:{
                    auto v = fget();
                    v->left = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::top:{
                    auto v = fget();
                    v->top = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::right:{
                    auto v = fget();
                    v->right = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::bottom:{
                    auto v = fget();
                    v->bottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::marginLeft:{
                    auto v = fget();
                    v->marginLeft = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::marginTop:{
                    auto v = fget();
                    v->marginTop = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::marginRight:{
                    auto v = fget();
                    v->marginRight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::marginBottom:{
                    auto v = fget();
                    v->marginBottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::paddingTop:{
                    auto v = fget();
                    v->paddingTop = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::paddingRight:{
                    auto v = fget();
                    v->paddingRight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::width:{
                    auto v = fget();
                    v->width = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::height:{
                    auto v = fget();
                    v->height = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::position:{
                    auto v = fget();
                    v->position = sheet.Get<YGPositionType>(prop.value);
                    break;
                    }
                case Id::overflow:{
                    auto v = fget();
                    v->overflow = sheet.Get<YGOverflow>(prop.value);
                    break;
                    }
                case Id::alignSelf:{
                    auto v = fget();
                    v->alignSelf = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Id::maxWidth:{
                    auto v = fget();
                    v->maxWidth = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::maxHeight:{
                    auto v = fget();
                    v->maxHeight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::minWidth:{
                    auto v = fget();
                    v->minWidth = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::minHeight:{
                    auto v = fget();
                    v->minHeight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Id::flexDirection:{
                    auto v = fget();
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.value);
                    break;
                    }
                case Id::alignContent:{
                    auto v = fget();
                    v->alignContent = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Id::alignItems:{
                    auto v = fget();
                    v->alignItems = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Id::justifyContent:{
                    auto v = fget();
                    v->justifyContent = sheet.Get<YGJustify>(prop.value);
                    break;
                    }
                case Id::flexWrap:{
                    auto v = fget();
                    v->flexWrap = sheet.Get<YGWrap>(prop.value);
                    break;
                    }
                case Id::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.value);
                    break;
                    }
                default: break;
            }
        }
    }
}


OGUI::RestyleDamage OGUI::StylePosition::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props)
{
    OGUI::StylePosition* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto iter = style.structs.find(hash);
    bool owned = false;
    if(iter != style.structs.end())
    {
        auto value = iter->second;
        st = (OGUI::StylePosition*)value.ptr.get();
        owned = value.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StylePosition>();
            value->Initialize();
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StylePosition>(*st);
            style.structs.erase(iter);
            style.structs.insert({hash, {std::static_pointer_cast<void>(value)}});
            owned = true;
            st = value.get();
        }
        return st;
    };
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Id::transform:{
                auto v = fget();
                    damage |= RestyleDamage::Transform;
                if(prop.alpha == 0.f)
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.from));
                else if(prop.alpha == 1.f)
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.to));
                else if(prop.from == prop.to)
                    v->transform = OGUI::Lerp(v->transform, sheet.Get<gsl::span<TransformFunction>>(prop.to), prop.alpha);
                else
                    v->transform = OGUI::Lerp(sheet.Get<gsl::span<TransformFunction>>(prop.from), sheet.Get<gsl::span<TransformFunction>>(prop.to), prop.alpha);
                break;
                }
            case Id::flexGrow:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexGrow = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexGrow = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->flexGrow = OGUI::Lerp(v->flexGrow, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->flexGrow = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::flexShrink:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexShrink = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexShrink = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->flexShrink = OGUI::Lerp(v->flexShrink, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->flexShrink = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                break;
                }
            case Id::flexBasis:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexBasis = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexBasis = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->flexBasis = OGUI::Lerp(v->flexBasis, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->flexBasis = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::left:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->left = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->left = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->left = OGUI::Lerp(v->left, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->left = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::top:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->top = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->top = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->top = OGUI::Lerp(v->top, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->top = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::right:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->right = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->right = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->right = OGUI::Lerp(v->right, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->right = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::bottom:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->bottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->bottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->bottom = OGUI::Lerp(v->bottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->bottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::marginLeft:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->marginLeft = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginLeft = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginLeft = OGUI::Lerp(v->marginLeft, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginLeft = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::marginTop:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->marginTop = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginTop = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginTop = OGUI::Lerp(v->marginTop, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginTop = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::marginRight:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->marginRight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginRight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginRight = OGUI::Lerp(v->marginRight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginRight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::marginBottom:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->marginBottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginBottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginBottom = OGUI::Lerp(v->marginBottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginBottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::paddingLeft:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->paddingLeft = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingLeft = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingLeft = OGUI::Lerp(v->paddingLeft, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingLeft = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::paddingTop:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->paddingTop = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingTop = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingTop = OGUI::Lerp(v->paddingTop, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingTop = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::paddingRight:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->paddingRight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingRight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingRight = OGUI::Lerp(v->paddingRight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingRight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::paddingBottom:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->paddingBottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingBottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingBottom = OGUI::Lerp(v->paddingBottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingBottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::width:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->width = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->width = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->width = OGUI::Lerp(v->width, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->width = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::height:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->height = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->height = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->height = OGUI::Lerp(v->height, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->height = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::position:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->position = sheet.Get<YGPositionType>(prop.from);
                else if(prop.alpha == 1.f)
                    v->position = sheet.Get<YGPositionType>(prop.to);
                else if(prop.from == prop.to)
                    v->position = OGUI::Lerp(v->position, sheet.Get<YGPositionType>(prop.to), prop.alpha);
                else
                    v->position = OGUI::Lerp(sheet.Get<YGPositionType>(prop.from), sheet.Get<YGPositionType>(prop.to), prop.alpha);
                break;
                }
            case Id::overflow:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->overflow = sheet.Get<YGOverflow>(prop.from);
                else if(prop.alpha == 1.f)
                    v->overflow = sheet.Get<YGOverflow>(prop.to);
                else if(prop.from == prop.to)
                    v->overflow = OGUI::Lerp(v->overflow, sheet.Get<YGOverflow>(prop.to), prop.alpha);
                else
                    v->overflow = OGUI::Lerp(sheet.Get<YGOverflow>(prop.from), sheet.Get<YGOverflow>(prop.to), prop.alpha);
                break;
                }
            case Id::alignSelf:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->alignSelf = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignSelf = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignSelf = OGUI::Lerp(v->alignSelf, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignSelf = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                break;
                }
            case Id::maxWidth:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->maxWidth = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->maxWidth = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->maxWidth = OGUI::Lerp(v->maxWidth, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->maxWidth = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::maxHeight:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->maxHeight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->maxHeight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->maxHeight = OGUI::Lerp(v->maxHeight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->maxHeight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::minWidth:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->minWidth = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->minWidth = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->minWidth = OGUI::Lerp(v->minWidth, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->minWidth = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::minHeight:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->minHeight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->minHeight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->minHeight = OGUI::Lerp(v->minHeight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->minHeight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                break;
                }
            case Id::flexDirection:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.to);
                else if(prop.from == prop.to)
                    v->flexDirection = OGUI::Lerp(v->flexDirection, sheet.Get<YGFlexDirection>(prop.to), prop.alpha);
                else
                    v->flexDirection = OGUI::Lerp(sheet.Get<YGFlexDirection>(prop.from), sheet.Get<YGFlexDirection>(prop.to), prop.alpha);
                break;
                }
            case Id::alignContent:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->alignContent = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignContent = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignContent = OGUI::Lerp(v->alignContent, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignContent = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                break;
                }
            case Id::alignItems:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->alignItems = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignItems = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignItems = OGUI::Lerp(v->alignItems, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignItems = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                break;
                }
            case Id::justifyContent:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->justifyContent = sheet.Get<YGJustify>(prop.from);
                else if(prop.alpha == 1.f)
                    v->justifyContent = sheet.Get<YGJustify>(prop.to);
                else if(prop.from == prop.to)
                    v->justifyContent = OGUI::Lerp(v->justifyContent, sheet.Get<YGJustify>(prop.to), prop.alpha);
                else
                    v->justifyContent = OGUI::Lerp(sheet.Get<YGJustify>(prop.from), sheet.Get<YGJustify>(prop.to), prop.alpha);
                break;
                }
            case Id::flexWrap:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexWrap = sheet.Get<YGWrap>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexWrap = sheet.Get<YGWrap>(prop.to);
                else if(prop.from == prop.to)
                    v->flexWrap = OGUI::Lerp(v->flexWrap, sheet.Get<YGWrap>(prop.to), prop.alpha);
                else
                    v->flexWrap = OGUI::Lerp(sheet.Get<YGWrap>(prop.from), sheet.Get<YGWrap>(prop.to), prop.alpha);
                break;
                }
            case Id::flexDisplay:{
                auto v = fget();
                    damage |= RestyleDamage::Yoga;
                if(prop.alpha == 0.f)
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.to);
                else if(prop.from == prop.to)
                    v->flexDisplay = OGUI::Lerp(v->flexDisplay, sheet.Get<YGDisplay>(prop.to), prop.alpha);
                else
                    v->flexDisplay = OGUI::Lerp(sheet.Get<YGDisplay>(prop.from), sheet.Get<YGDisplay>(prop.to), prop.alpha);
                break;
                }
            default: break;
        }
    }
    return damage;
}

bool OGUI::StylePosition::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, std::string& errorMsg)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
        case Id::margin:
            return Parse::ParseMargin(sheet, name, value, rule, errorMsg);
        default: break;
    }
    StyleKeyword keyword = StyleKeyword::None;
    ParseValue(value, keyword);
    if(keyword != StyleKeyword::None)
    {
        switch(hash)
        {
            case Id::transform:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexGrow:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexShrink:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexBasis:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::left:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::top:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::right:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::bottom:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::marginLeft:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::marginTop:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::marginRight:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::marginBottom:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::paddingLeft:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::paddingTop:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::paddingRight:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::paddingBottom:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::width:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::height:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::position:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::overflow:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::alignSelf:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::maxWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::maxHeight:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::minWidth:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::minHeight:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexDirection:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::alignContent:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::alignItems:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::justifyContent:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexWrap:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            case Id::flexDisplay:
                rule.properties.push_back({hash,(int)keyword});
                return true;
            default: break;
        }
        return false;
    }
    //longhands
    switch(hash)
    {
        case Id::transform:{
            std::vector<TransformFunction> v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<gsl::span<TransformFunction>>(v)});
            else
            {
                errorMsg = "failed to parse transform value!";
                return false;
            }
            return true;
        }
        case Id::flexGrow:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse flex-grow value!";
                return false;
            }
            return true;
        }
        case Id::flexShrink:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse flex-shrink value!";
                return false;
            }
            return true;
        }
        case Id::flexBasis:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse flex-basis value!";
                return false;
            }
            return true;
        }
        case Id::left:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse left value!";
                return false;
            }
            return true;
        }
        case Id::top:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse top value!";
                return false;
            }
            return true;
        }
        case Id::right:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse right value!";
                return false;
            }
            return true;
        }
        case Id::bottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse bottom value!";
                return false;
            }
            return true;
        }
        case Id::marginLeft:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-left value!";
                return false;
            }
            return true;
        }
        case Id::marginTop:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-top value!";
                return false;
            }
            return true;
        }
        case Id::marginRight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-right value!";
                return false;
            }
            return true;
        }
        case Id::marginBottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-bottom value!";
                return false;
            }
            return true;
        }
        case Id::paddingLeft:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-left value!";
                return false;
            }
            return true;
        }
        case Id::paddingTop:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-top value!";
                return false;
            }
            return true;
        }
        case Id::paddingRight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-right value!";
                return false;
            }
            return true;
        }
        case Id::paddingBottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-bottom value!";
                return false;
            }
            return true;
        }
        case Id::width:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse width value!";
                return false;
            }
            return true;
        }
        case Id::height:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse height value!";
                return false;
            }
            return true;
        }
        case Id::position:{
            YGPositionType v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGPositionType>(v)});
            else
            {
                errorMsg = "failed to parse position value!";
                return false;
            }
            return true;
        }
        case Id::overflow:{
            YGOverflow v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGOverflow>(v)});
            else
            {
                errorMsg = "failed to parse overflow value!";
                return false;
            }
            return true;
        }
        case Id::alignSelf:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-self value!";
                return false;
            }
            return true;
        }
        case Id::maxWidth:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse max-width value!";
                return false;
            }
            return true;
        }
        case Id::maxHeight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse max-height value!";
                return false;
            }
            return true;
        }
        case Id::minWidth:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse min-width value!";
                return false;
            }
            return true;
        }
        case Id::minHeight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse min-height value!";
                return false;
            }
            return true;
        }
        case Id::flexDirection:{
            YGFlexDirection v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGFlexDirection>(v)});
            else
            {
                errorMsg = "failed to parse flex-direction value!";
                return false;
            }
            return true;
        }
        case Id::alignContent:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-content value!";
                return false;
            }
            return true;
        }
        case Id::alignItems:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-items value!";
                return false;
            }
            return true;
        }
        case Id::justifyContent:{
            YGJustify v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGJustify>(v)});
            else
            {
                errorMsg = "failed to parse justify-content value!";
                return false;
            }
            return true;
        }
        case Id::flexWrap:{
            YGWrap v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGWrap>(v)});
            else
            {
                errorMsg = "failed to parse flex-wrap value!";
                return false;
            }
            return true;
        }
        case Id::flexDisplay:{
            YGDisplay v;
            if(ParseValue(value, v))
                rule.properties.push_back({hash, sheet.Push<YGDisplay>(v)});
            else
            {
                errorMsg = "failed to parse flex-display value!";
                return false;
            }
            return true;
        }
        default: break;
    }
    return false;
}