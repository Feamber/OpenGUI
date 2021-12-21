//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


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
    overflow = StyleOverflow::Visible;
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
    verticalAlign = EInlineAlign::Middle;
    aspectRatio = YGUndefined;
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
                case Ids::transform:{
                    auto v = fget();
                    v->transform = {};
                    break;
                    }
                case Ids::flexGrow:{
                    auto v = fget();
                    v->flexGrow = 0.f;
                    break;
                    }
                case Ids::flexShrink:{
                    auto v = fget();
                    v->flexShrink = 1.f;
                    break;
                    }
                case Ids::flexBasis:{
                    auto v = fget();
                    v->flexBasis = YGValueAuto;
                    break;
                    }
                case Ids::left:{
                    auto v = fget();
                    v->left = YGValueAuto;
                    break;
                    }
                case Ids::top:{
                    auto v = fget();
                    v->top = YGValueAuto;
                    break;
                    }
                case Ids::right:{
                    auto v = fget();
                    v->right = YGValueAuto;
                    break;
                    }
                case Ids::bottom:{
                    auto v = fget();
                    v->bottom = YGValueAuto;
                    break;
                    }
                case Ids::marginLeft:{
                    auto v = fget();
                    v->marginLeft = YGValueZero;
                    break;
                    }
                case Ids::marginTop:{
                    auto v = fget();
                    v->marginTop = YGValueZero;
                    break;
                    }
                case Ids::marginRight:{
                    auto v = fget();
                    v->marginRight = YGValueZero;
                    break;
                    }
                case Ids::marginBottom:{
                    auto v = fget();
                    v->marginBottom = YGValueZero;
                    break;
                    }
                case Ids::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = YGValueZero;
                    break;
                    }
                case Ids::paddingTop:{
                    auto v = fget();
                    v->paddingTop = YGValueZero;
                    break;
                    }
                case Ids::paddingRight:{
                    auto v = fget();
                    v->paddingRight = YGValueZero;
                    break;
                    }
                case Ids::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = YGValueZero;
                    break;
                    }
                case Ids::width:{
                    auto v = fget();
                    v->width = YGValueAuto;
                    break;
                    }
                case Ids::height:{
                    auto v = fget();
                    v->height = YGValueAuto;
                    break;
                    }
                case Ids::position:{
                    auto v = fget();
                    v->position = YGPositionTypeRelative;
                    break;
                    }
                case Ids::overflow:{
                    auto v = fget();
                    v->overflow = StyleOverflow::Visible;
                    break;
                    }
                case Ids::alignSelf:{
                    auto v = fget();
                    v->alignSelf = YGAlignAuto;
                    break;
                    }
                case Ids::maxWidth:{
                    auto v = fget();
                    v->maxWidth = YGValueUndefined;
                    break;
                    }
                case Ids::maxHeight:{
                    auto v = fget();
                    v->maxHeight = YGValueUndefined;
                    break;
                    }
                case Ids::minWidth:{
                    auto v = fget();
                    v->minWidth = YGValueAuto;
                    break;
                    }
                case Ids::minHeight:{
                    auto v = fget();
                    v->minHeight = YGValueAuto;
                    break;
                    }
                case Ids::flexDirection:{
                    auto v = fget();
                    v->flexDirection = YGFlexDirectionRow;
                    break;
                    }
                case Ids::alignContent:{
                    auto v = fget();
                    v->alignContent = YGAlignFlexStart;
                    break;
                    }
                case Ids::alignItems:{
                    auto v = fget();
                    v->alignItems = YGAlignStretch;
                    break;
                    }
                case Ids::justifyContent:{
                    auto v = fget();
                    v->justifyContent = YGJustifyFlexStart;
                    break;
                    }
                case Ids::flexWrap:{
                    auto v = fget();
                    v->flexWrap = YGWrapNoWrap;
                    break;
                    }
                case Ids::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = YGDisplayFlex;
                    break;
                    }
                case Ids::verticalAlign:{
                    auto v = fget();
                    v->verticalAlign = EInlineAlign::Middle;
                    break;
                    }
                case Ids::aspectRatio:{
                    auto v = fget();
                    v->aspectRatio = YGUndefined;
                    break;
                    }
                default: break;
                }
            }
            else
            { 
                switch(prop.id)
                {
                case Ids::transform:{
                    auto v = fget();
                    v->transform = pst->transform;
                    break;
                    }
                case Ids::flexGrow:{
                    auto v = fget();
                    v->flexGrow = pst->flexGrow;
                    break;
                    }
                case Ids::flexShrink:{
                    auto v = fget();
                    v->flexShrink = pst->flexShrink;
                    break;
                    }
                case Ids::flexBasis:{
                    auto v = fget();
                    v->flexBasis = pst->flexBasis;
                    break;
                    }
                case Ids::left:{
                    auto v = fget();
                    v->left = pst->left;
                    break;
                    }
                case Ids::top:{
                    auto v = fget();
                    v->top = pst->top;
                    break;
                    }
                case Ids::right:{
                    auto v = fget();
                    v->right = pst->right;
                    break;
                    }
                case Ids::bottom:{
                    auto v = fget();
                    v->bottom = pst->bottom;
                    break;
                    }
                case Ids::marginLeft:{
                    auto v = fget();
                    v->marginLeft = pst->marginLeft;
                    break;
                    }
                case Ids::marginTop:{
                    auto v = fget();
                    v->marginTop = pst->marginTop;
                    break;
                    }
                case Ids::marginRight:{
                    auto v = fget();
                    v->marginRight = pst->marginRight;
                    break;
                    }
                case Ids::marginBottom:{
                    auto v = fget();
                    v->marginBottom = pst->marginBottom;
                    break;
                    }
                case Ids::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = pst->paddingLeft;
                    break;
                    }
                case Ids::paddingTop:{
                    auto v = fget();
                    v->paddingTop = pst->paddingTop;
                    break;
                    }
                case Ids::paddingRight:{
                    auto v = fget();
                    v->paddingRight = pst->paddingRight;
                    break;
                    }
                case Ids::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = pst->paddingBottom;
                    break;
                    }
                case Ids::width:{
                    auto v = fget();
                    v->width = pst->width;
                    break;
                    }
                case Ids::height:{
                    auto v = fget();
                    v->height = pst->height;
                    break;
                    }
                case Ids::position:{
                    auto v = fget();
                    v->position = pst->position;
                    break;
                    }
                case Ids::overflow:{
                    auto v = fget();
                    v->overflow = pst->overflow;
                    break;
                    }
                case Ids::alignSelf:{
                    auto v = fget();
                    v->alignSelf = pst->alignSelf;
                    break;
                    }
                case Ids::maxWidth:{
                    auto v = fget();
                    v->maxWidth = pst->maxWidth;
                    break;
                    }
                case Ids::maxHeight:{
                    auto v = fget();
                    v->maxHeight = pst->maxHeight;
                    break;
                    }
                case Ids::minWidth:{
                    auto v = fget();
                    v->minWidth = pst->minWidth;
                    break;
                    }
                case Ids::minHeight:{
                    auto v = fget();
                    v->minHeight = pst->minHeight;
                    break;
                    }
                case Ids::flexDirection:{
                    auto v = fget();
                    v->flexDirection = pst->flexDirection;
                    break;
                    }
                case Ids::alignContent:{
                    auto v = fget();
                    v->alignContent = pst->alignContent;
                    break;
                    }
                case Ids::alignItems:{
                    auto v = fget();
                    v->alignItems = pst->alignItems;
                    break;
                    }
                case Ids::justifyContent:{
                    auto v = fget();
                    v->justifyContent = pst->justifyContent;
                    break;
                    }
                case Ids::flexWrap:{
                    auto v = fget();
                    v->flexWrap = pst->flexWrap;
                    break;
                    }
                case Ids::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = pst->flexDisplay;
                    break;
                    }
                case Ids::verticalAlign:{
                    auto v = fget();
                    v->verticalAlign = pst->verticalAlign;
                    break;
                    }
                case Ids::aspectRatio:{
                    auto v = fget();
                    v->aspectRatio = pst->aspectRatio;
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
                case Ids::transform:{
                    auto v = fget();
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.value));
                    break;
                    }
                case Ids::flexGrow:{
                    auto v = fget();
                    v->flexGrow = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::flexShrink:{
                    auto v = fget();
                    v->flexShrink = sheet.Get<float>(prop.value);
                    break;
                    }
                case Ids::flexBasis:{
                    auto v = fget();
                    v->flexBasis = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::left:{
                    auto v = fget();
                    v->left = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::top:{
                    auto v = fget();
                    v->top = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::right:{
                    auto v = fget();
                    v->right = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::bottom:{
                    auto v = fget();
                    v->bottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::marginLeft:{
                    auto v = fget();
                    v->marginLeft = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::marginTop:{
                    auto v = fget();
                    v->marginTop = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::marginRight:{
                    auto v = fget();
                    v->marginRight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::marginBottom:{
                    auto v = fget();
                    v->marginBottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::paddingTop:{
                    auto v = fget();
                    v->paddingTop = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::paddingRight:{
                    auto v = fget();
                    v->paddingRight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::paddingBottom:{
                    auto v = fget();
                    v->paddingBottom = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::width:{
                    auto v = fget();
                    v->width = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::height:{
                    auto v = fget();
                    v->height = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::position:{
                    auto v = fget();
                    v->position = sheet.Get<YGPositionType>(prop.value);
                    break;
                    }
                case Ids::overflow:{
                    auto v = fget();
                    v->overflow = sheet.Get<StyleOverflow>(prop.value);
                    break;
                    }
                case Ids::alignSelf:{
                    auto v = fget();
                    v->alignSelf = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Ids::maxWidth:{
                    auto v = fget();
                    v->maxWidth = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::maxHeight:{
                    auto v = fget();
                    v->maxHeight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::minWidth:{
                    auto v = fget();
                    v->minWidth = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::minHeight:{
                    auto v = fget();
                    v->minHeight = sheet.Get<YGValue>(prop.value);
                    break;
                    }
                case Ids::flexDirection:{
                    auto v = fget();
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.value);
                    break;
                    }
                case Ids::alignContent:{
                    auto v = fget();
                    v->alignContent = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Ids::alignItems:{
                    auto v = fget();
                    v->alignItems = sheet.Get<YGAlign>(prop.value);
                    break;
                    }
                case Ids::justifyContent:{
                    auto v = fget();
                    v->justifyContent = sheet.Get<YGJustify>(prop.value);
                    break;
                    }
                case Ids::flexWrap:{
                    auto v = fget();
                    v->flexWrap = sheet.Get<YGWrap>(prop.value);
                    break;
                    }
                case Ids::flexDisplay:{
                    auto v = fget();
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.value);
                    break;
                    }
                case Ids::verticalAlign:{
                    auto v = fget();
                    v->verticalAlign = sheet.Get<EInlineAlign>(prop.value);
                    break;
                    }
                case Ids::aspectRatio:{
                    auto v = fget();
                    v->aspectRatio = sheet.Get<float>(prop.value);
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
            case Ids::transform:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.from));
                else if(prop.alpha == 1.f)
                    v->transform = ToOwned(sheet.Get<gsl::span<TransformFunction>>(prop.to));
                else if(prop.from == prop.to)
                    v->transform = OGUI::Lerp(v->transform, sheet.Get<gsl::span<TransformFunction>>(prop.to), prop.alpha);
                else
                    v->transform = OGUI::Lerp(sheet.Get<gsl::span<TransformFunction>>(prop.from), sheet.Get<gsl::span<TransformFunction>>(prop.to), prop.alpha);
                
                    damage |= RestyleDamage::Transform;
                break;
                }
            case Ids::flexGrow:{
                auto v = fget();
                auto prevValue = v->flexGrow;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexGrow = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexGrow = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->flexGrow = OGUI::Lerp(v->flexGrow, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->flexGrow = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->flexGrow)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::flexShrink:{
                auto v = fget();
                auto prevValue = v->flexShrink;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexShrink = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexShrink = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->flexShrink = OGUI::Lerp(v->flexShrink, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->flexShrink = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->flexShrink)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::flexBasis:{
                auto v = fget();
                auto prevValue = v->flexBasis;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexBasis = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexBasis = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->flexBasis = OGUI::Lerp(v->flexBasis, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->flexBasis = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->flexBasis)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::left:{
                auto v = fget();
                auto prevValue = v->left;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->left = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->left = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->left = OGUI::Lerp(v->left, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->left = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->left)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::top:{
                auto v = fget();
                auto prevValue = v->top;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->top = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->top = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->top = OGUI::Lerp(v->top, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->top = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->top)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::right:{
                auto v = fget();
                auto prevValue = v->right;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->right = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->right = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->right = OGUI::Lerp(v->right, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->right = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->right)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::bottom:{
                auto v = fget();
                auto prevValue = v->bottom;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->bottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->bottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->bottom = OGUI::Lerp(v->bottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->bottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->bottom)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::marginLeft:{
                auto v = fget();
                auto prevValue = v->marginLeft;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->marginLeft = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginLeft = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginLeft = OGUI::Lerp(v->marginLeft, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginLeft = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->marginLeft)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::marginTop:{
                auto v = fget();
                auto prevValue = v->marginTop;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->marginTop = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginTop = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginTop = OGUI::Lerp(v->marginTop, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginTop = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->marginTop)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::marginRight:{
                auto v = fget();
                auto prevValue = v->marginRight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->marginRight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginRight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginRight = OGUI::Lerp(v->marginRight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginRight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->marginRight)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::marginBottom:{
                auto v = fget();
                auto prevValue = v->marginBottom;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->marginBottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->marginBottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->marginBottom = OGUI::Lerp(v->marginBottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->marginBottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->marginBottom)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::paddingLeft:{
                auto v = fget();
                auto prevValue = v->paddingLeft;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->paddingLeft = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingLeft = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingLeft = OGUI::Lerp(v->paddingLeft, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingLeft = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->paddingLeft)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::paddingTop:{
                auto v = fget();
                auto prevValue = v->paddingTop;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->paddingTop = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingTop = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingTop = OGUI::Lerp(v->paddingTop, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingTop = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->paddingTop)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::paddingRight:{
                auto v = fget();
                auto prevValue = v->paddingRight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->paddingRight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingRight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingRight = OGUI::Lerp(v->paddingRight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingRight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->paddingRight)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::paddingBottom:{
                auto v = fget();
                auto prevValue = v->paddingBottom;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->paddingBottom = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->paddingBottom = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->paddingBottom = OGUI::Lerp(v->paddingBottom, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->paddingBottom = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->paddingBottom)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::width:{
                auto v = fget();
                auto prevValue = v->width;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->width = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->width = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->width = OGUI::Lerp(v->width, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->width = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->width)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::height:{
                auto v = fget();
                auto prevValue = v->height;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->height = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->height = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->height = OGUI::Lerp(v->height, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->height = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->height)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::position:{
                auto v = fget();
                auto prevValue = v->position;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->position = sheet.Get<YGPositionType>(prop.from);
                else if(prop.alpha == 1.f)
                    v->position = sheet.Get<YGPositionType>(prop.to);
                else if(prop.from == prop.to)
                    v->position = OGUI::Lerp(v->position, sheet.Get<YGPositionType>(prop.to), prop.alpha);
                else
                    v->position = OGUI::Lerp(sheet.Get<YGPositionType>(prop.from), sheet.Get<YGPositionType>(prop.to), prop.alpha);
                
                if(prevValue != v->position)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::overflow:{
                auto v = fget();
                auto prevValue = v->overflow;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->overflow = sheet.Get<StyleOverflow>(prop.from);
                else if(prop.alpha == 1.f)
                    v->overflow = sheet.Get<StyleOverflow>(prop.to);
                else if(prop.from == prop.to)
                    v->overflow = OGUI::Lerp(v->overflow, sheet.Get<StyleOverflow>(prop.to), prop.alpha);
                else
                    v->overflow = OGUI::Lerp(sheet.Get<StyleOverflow>(prop.from), sheet.Get<StyleOverflow>(prop.to), prop.alpha);
                
                if(prevValue != v->overflow)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::alignSelf:{
                auto v = fget();
                auto prevValue = v->alignSelf;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->alignSelf = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignSelf = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignSelf = OGUI::Lerp(v->alignSelf, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignSelf = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                
                if(prevValue != v->alignSelf)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::maxWidth:{
                auto v = fget();
                auto prevValue = v->maxWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->maxWidth = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->maxWidth = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->maxWidth = OGUI::Lerp(v->maxWidth, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->maxWidth = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->maxWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::maxHeight:{
                auto v = fget();
                auto prevValue = v->maxHeight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->maxHeight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->maxHeight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->maxHeight = OGUI::Lerp(v->maxHeight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->maxHeight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->maxHeight)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::minWidth:{
                auto v = fget();
                auto prevValue = v->minWidth;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->minWidth = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->minWidth = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->minWidth = OGUI::Lerp(v->minWidth, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->minWidth = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->minWidth)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::minHeight:{
                auto v = fget();
                auto prevValue = v->minHeight;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->minHeight = sheet.Get<YGValue>(prop.from);
                else if(prop.alpha == 1.f)
                    v->minHeight = sheet.Get<YGValue>(prop.to);
                else if(prop.from == prop.to)
                    v->minHeight = OGUI::Lerp(v->minHeight, sheet.Get<YGValue>(prop.to), prop.alpha);
                else
                    v->minHeight = OGUI::Lerp(sheet.Get<YGValue>(prop.from), sheet.Get<YGValue>(prop.to), prop.alpha);
                
                if(prevValue != v->minHeight)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::flexDirection:{
                auto v = fget();
                auto prevValue = v->flexDirection;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexDirection = sheet.Get<YGFlexDirection>(prop.to);
                else if(prop.from == prop.to)
                    v->flexDirection = OGUI::Lerp(v->flexDirection, sheet.Get<YGFlexDirection>(prop.to), prop.alpha);
                else
                    v->flexDirection = OGUI::Lerp(sheet.Get<YGFlexDirection>(prop.from), sheet.Get<YGFlexDirection>(prop.to), prop.alpha);
                
                if(prevValue != v->flexDirection)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::alignContent:{
                auto v = fget();
                auto prevValue = v->alignContent;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->alignContent = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignContent = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignContent = OGUI::Lerp(v->alignContent, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignContent = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                
                if(prevValue != v->alignContent)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::alignItems:{
                auto v = fget();
                auto prevValue = v->alignItems;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->alignItems = sheet.Get<YGAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->alignItems = sheet.Get<YGAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->alignItems = OGUI::Lerp(v->alignItems, sheet.Get<YGAlign>(prop.to), prop.alpha);
                else
                    v->alignItems = OGUI::Lerp(sheet.Get<YGAlign>(prop.from), sheet.Get<YGAlign>(prop.to), prop.alpha);
                
                if(prevValue != v->alignItems)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::justifyContent:{
                auto v = fget();
                auto prevValue = v->justifyContent;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->justifyContent = sheet.Get<YGJustify>(prop.from);
                else if(prop.alpha == 1.f)
                    v->justifyContent = sheet.Get<YGJustify>(prop.to);
                else if(prop.from == prop.to)
                    v->justifyContent = OGUI::Lerp(v->justifyContent, sheet.Get<YGJustify>(prop.to), prop.alpha);
                else
                    v->justifyContent = OGUI::Lerp(sheet.Get<YGJustify>(prop.from), sheet.Get<YGJustify>(prop.to), prop.alpha);
                
                if(prevValue != v->justifyContent)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::flexWrap:{
                auto v = fget();
                auto prevValue = v->flexWrap;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexWrap = sheet.Get<YGWrap>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexWrap = sheet.Get<YGWrap>(prop.to);
                else if(prop.from == prop.to)
                    v->flexWrap = OGUI::Lerp(v->flexWrap, sheet.Get<YGWrap>(prop.to), prop.alpha);
                else
                    v->flexWrap = OGUI::Lerp(sheet.Get<YGWrap>(prop.from), sheet.Get<YGWrap>(prop.to), prop.alpha);
                
                if(prevValue != v->flexWrap)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::flexDisplay:{
                auto v = fget();
                auto prevValue = v->flexDisplay;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.from);
                else if(prop.alpha == 1.f)
                    v->flexDisplay = sheet.Get<YGDisplay>(prop.to);
                else if(prop.from == prop.to)
                    v->flexDisplay = OGUI::Lerp(v->flexDisplay, sheet.Get<YGDisplay>(prop.to), prop.alpha);
                else
                    v->flexDisplay = OGUI::Lerp(sheet.Get<YGDisplay>(prop.from), sheet.Get<YGDisplay>(prop.to), prop.alpha);
                
                if(prevValue != v->flexDisplay)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::verticalAlign:{
                auto v = fget();
                auto prevValue = v->verticalAlign;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->verticalAlign = sheet.Get<EInlineAlign>(prop.from);
                else if(prop.alpha == 1.f)
                    v->verticalAlign = sheet.Get<EInlineAlign>(prop.to);
                else if(prop.from == prop.to)
                    v->verticalAlign = OGUI::Lerp(v->verticalAlign, sheet.Get<EInlineAlign>(prop.to), prop.alpha);
                else
                    v->verticalAlign = OGUI::Lerp(sheet.Get<EInlineAlign>(prop.from), sheet.Get<EInlineAlign>(prop.to), prop.alpha);
                
                if(prevValue != v->verticalAlign)
                    damage |= RestyleDamage::Layout;
                break;
                }
            case Ids::aspectRatio:{
                auto v = fget();
                auto prevValue = v->aspectRatio;
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->aspectRatio = sheet.Get<float>(prop.from);
                else if(prop.alpha == 1.f)
                    v->aspectRatio = sheet.Get<float>(prop.to);
                else if(prop.from == prop.to)
                    v->aspectRatio = OGUI::Lerp(v->aspectRatio, sheet.Get<float>(prop.to), prop.alpha);
                else
                    v->aspectRatio = OGUI::Lerp(sheet.Get<float>(prop.from), sheet.Get<float>(prop.to), prop.alpha);
                
                if(prevValue != v->aspectRatio)
                    damage |= RestyleDamage::Layout;
                break;
                }
            default: break;
        }
    }
    return damage;
}

bool OGUI::StylePosition::ParseProperties(StyleSheetStorage& sheet, std::string_view prop, std::string_view value, StyleRule& rule, std::string& errorMsg)
{
    size_t phash = OGUI::hash(prop);

    StyleKeyword keyword = StyleKeyword::None;
    ParseValue(value, keyword);
    if(keyword != StyleKeyword::None)
    {
        switch(phash)
        {
            case Ids::transform:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexGrow:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexShrink:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexBasis:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::left:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::top:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::right:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::bottom:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::marginLeft:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::marginTop:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::marginRight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::marginBottom:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::paddingLeft:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::paddingTop:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::paddingRight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::paddingBottom:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::width:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::height:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::position:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::overflow:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::alignSelf:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::maxWidth:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::maxHeight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::minWidth:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::minHeight:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexDirection:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::alignContent:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::alignItems:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::justifyContent:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexWrap:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::flexDisplay:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::verticalAlign:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::aspectRatio:
                rule.properties.push_back({phash,(int)keyword});
                return true;
            case Ids::margin:
                rule.properties.push_back({Ids::marginLeft,(int)keyword});
                rule.properties.push_back({Ids::marginTop,(int)keyword});
                rule.properties.push_back({Ids::marginRight,(int)keyword});
                rule.properties.push_back({Ids::marginBottom,(int)keyword});
            case Ids::padding:
                rule.properties.push_back({Ids::paddingLeft,(int)keyword});
                rule.properties.push_back({Ids::paddingTop,(int)keyword});
                rule.properties.push_back({Ids::paddingRight,(int)keyword});
                rule.properties.push_back({Ids::paddingBottom,(int)keyword});
            default: break;
        }
        return false;
    }
    //shorthands
    switch(phash)
    {
        case Ids::margin:
            return Parse::ParseMargin(sheet, prop, value, rule, errorMsg);
        case Ids::padding:
            return Parse::ParsePadding(sheet, prop, value, rule, errorMsg);
        default: break;
    }
    //longhands
    switch(phash)
    {
        case Ids::transform:{
            std::vector<TransformFunction> v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<gsl::span<TransformFunction>>(v)});
            else
            {
                errorMsg = "failed to parse transform value!";
                return false;
            }
            return true;
        }
        case Ids::flexGrow:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse flex-grow value!";
                return false;
            }
            return true;
        }
        case Ids::flexShrink:{
            float v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse flex-shrink value!";
                return false;
            }
            return true;
        }
        case Ids::flexBasis:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse flex-basis value!";
                return false;
            }
            return true;
        }
        case Ids::left:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse left value!";
                return false;
            }
            return true;
        }
        case Ids::top:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse top value!";
                return false;
            }
            return true;
        }
        case Ids::right:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse right value!";
                return false;
            }
            return true;
        }
        case Ids::bottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse bottom value!";
                return false;
            }
            return true;
        }
        case Ids::marginLeft:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-left value!";
                return false;
            }
            return true;
        }
        case Ids::marginTop:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-top value!";
                return false;
            }
            return true;
        }
        case Ids::marginRight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-right value!";
                return false;
            }
            return true;
        }
        case Ids::marginBottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse margin-bottom value!";
                return false;
            }
            return true;
        }
        case Ids::paddingLeft:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-left value!";
                return false;
            }
            return true;
        }
        case Ids::paddingTop:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-top value!";
                return false;
            }
            return true;
        }
        case Ids::paddingRight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-right value!";
                return false;
            }
            return true;
        }
        case Ids::paddingBottom:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse padding-bottom value!";
                return false;
            }
            return true;
        }
        case Ids::width:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse width value!";
                return false;
            }
            return true;
        }
        case Ids::height:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse height value!";
                return false;
            }
            return true;
        }
        case Ids::position:{
            YGPositionType v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGPositionType>(v)});
            else
            {
                errorMsg = "failed to parse position value!";
                return false;
            }
            return true;
        }
        case Ids::overflow:{
            StyleOverflow v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<StyleOverflow>(v)});
            else
            {
                errorMsg = "failed to parse overflow value!";
                return false;
            }
            return true;
        }
        case Ids::alignSelf:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-self value!";
                return false;
            }
            return true;
        }
        case Ids::maxWidth:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse max-width value!";
                return false;
            }
            return true;
        }
        case Ids::maxHeight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse max-height value!";
                return false;
            }
            return true;
        }
        case Ids::minWidth:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse min-width value!";
                return false;
            }
            return true;
        }
        case Ids::minHeight:{
            YGValue v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGValue>(v)});
            else
            {
                errorMsg = "failed to parse min-height value!";
                return false;
            }
            return true;
        }
        case Ids::flexDirection:{
            YGFlexDirection v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGFlexDirection>(v)});
            else
            {
                errorMsg = "failed to parse flex-direction value!";
                return false;
            }
            return true;
        }
        case Ids::alignContent:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-content value!";
                return false;
            }
            return true;
        }
        case Ids::alignItems:{
            YGAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGAlign>(v)});
            else
            {
                errorMsg = "failed to parse align-items value!";
                return false;
            }
            return true;
        }
        case Ids::justifyContent:{
            YGJustify v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGJustify>(v)});
            else
            {
                errorMsg = "failed to parse justify-content value!";
                return false;
            }
            return true;
        }
        case Ids::flexWrap:{
            YGWrap v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGWrap>(v)});
            else
            {
                errorMsg = "failed to parse flex-wrap value!";
                return false;
            }
            return true;
        }
        case Ids::flexDisplay:{
            YGDisplay v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<YGDisplay>(v)});
            else
            {
                errorMsg = "failed to parse flex-display value!";
                return false;
            }
            return true;
        }
        case Ids::verticalAlign:{
            EInlineAlign v;
            if(ParseValue(value, v))
                rule.properties.push_back({phash, sheet.Push<EInlineAlign>(v)});
            else
            {
                errorMsg = "failed to parse vertical-align value!";
                return false;
            }
            return true;
        }
        case Ids::aspectRatio:{
            float v;
            if(ParseRatio(value, v))
                rule.properties.push_back({phash, sheet.Push<float>(v)});
            else
            {
                errorMsg = "failed to parse aspect-ratio value!";
                return false;
            }
            return true;
        }
        default: break;
    }
    return false;
}