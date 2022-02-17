//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.cpp.mako


#include <memory>
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/ComputedStyle.h"
#include "OpenGUI/VisualElement.h"
#include "position_shorthands.h"

size_t StylePositionEntry = 0;

size_t OGUI::StylePosition::GetEntry()
{
    return StylePositionEntry;
}

void OGUI::StylePosition::SetEntry(size_t e)
{
    StylePositionEntry = e;
}

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
    auto& s = style.structs[StylePositionEntry];
    return (StylePosition*)s.ptr.get();
}

OGUI::StylePosition& OGUI::StylePosition::GetOrAdd(ComputedStyle& style)
{
    auto& s = style.structs[StylePositionEntry];
    if(!s.ptr)
    {
        auto value = std::make_shared<OGUI::StylePosition>();
        value->Initialize();
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else if(!s.owned)
    {
        auto value = std::make_shared<OGUI::StylePosition>(*(OGUI::StylePosition*)s.ptr.get());
        s.ptr = std::static_pointer_cast<void>(value);
        s.owned = true;
        return *value.get();
    }
    else 
        return *(StylePosition*)s.ptr.get();
}

void OGUI::StylePosition::Dispose(ComputedStyle& style)
{
    style.structs[StylePositionEntry].ptr.reset();
}

void OGUI::StylePosition::Initialize()
{
    transform = {};
    flexGrow = 0.f;
    flexShrink = 1.f;
    flexBasis = YGValueAuto;
    top = YGValueAuto;
    right = YGValueAuto;
    bottom = YGValueAuto;
    left = YGValueAuto;
    marginTop = YGValueZero;
    marginRight = YGValueZero;
    marginBottom = YGValueZero;
    marginLeft = YGValueZero;
    paddingTop = YGValueZero;
    paddingRight = YGValueZero;
    paddingBottom = YGValueZero;
    paddingLeft = YGValueZero;
    width = YGValueAuto;
    height = YGValueAuto;
    position = YGPositionTypeRelative;
    overflow = EFlexOverflow::Visible;
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

void OGUI::StylePosition::ApplyProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props, const gsl::span<size_t>& override, const ComputedStyle* parent)
{
    auto pst = parent ? TryGet(*parent) : nullptr;
    OGUI::StylePosition* st = nullptr;
    auto& s = style.structs[StylePositionEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StylePosition*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StylePosition>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StylePosition>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    auto mask = override[StylePositionEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::transform: if(mask & (1ull<<0)) continue; break;
            case Ids::flexGrow: if(mask & (1ull<<1)) continue; break;
            case Ids::flexShrink: if(mask & (1ull<<2)) continue; break;
            case Ids::flexBasis: if(mask & (1ull<<3)) continue; break;
            case Ids::top: if(mask & (1ull<<4)) continue; break;
            case Ids::right: if(mask & (1ull<<5)) continue; break;
            case Ids::bottom: if(mask & (1ull<<6)) continue; break;
            case Ids::left: if(mask & (1ull<<7)) continue; break;
            case Ids::marginTop: if(mask & (1ull<<8)) continue; break;
            case Ids::marginRight: if(mask & (1ull<<9)) continue; break;
            case Ids::marginBottom: if(mask & (1ull<<10)) continue; break;
            case Ids::marginLeft: if(mask & (1ull<<11)) continue; break;
            case Ids::paddingTop: if(mask & (1ull<<12)) continue; break;
            case Ids::paddingRight: if(mask & (1ull<<13)) continue; break;
            case Ids::paddingBottom: if(mask & (1ull<<14)) continue; break;
            case Ids::paddingLeft: if(mask & (1ull<<15)) continue; break;
            case Ids::width: if(mask & (1ull<<16)) continue; break;
            case Ids::height: if(mask & (1ull<<17)) continue; break;
            case Ids::position: if(mask & (1ull<<18)) continue; break;
            case Ids::overflow: if(mask & (1ull<<19)) continue; break;
            case Ids::alignSelf: if(mask & (1ull<<20)) continue; break;
            case Ids::maxWidth: if(mask & (1ull<<21)) continue; break;
            case Ids::maxHeight: if(mask & (1ull<<22)) continue; break;
            case Ids::minWidth: if(mask & (1ull<<23)) continue; break;
            case Ids::minHeight: if(mask & (1ull<<24)) continue; break;
            case Ids::flexDirection: if(mask & (1ull<<25)) continue; break;
            case Ids::alignContent: if(mask & (1ull<<26)) continue; break;
            case Ids::alignItems: if(mask & (1ull<<27)) continue; break;
            case Ids::justifyContent: if(mask & (1ull<<28)) continue; break;
            case Ids::flexWrap: if(mask & (1ull<<29)) continue; break;
            case Ids::flexDisplay: if(mask & (1ull<<30)) continue; break;
            case Ids::verticalAlign: if(mask & (1ull<<31)) continue; break;
            case Ids::aspectRatio: if(mask & (1ull<<32)) continue; break;
        }
        if(prop.keyword)
        {
            if (prop.value.index == (int)StyleKeyword::Initial || !pst
            || prop.value.index == (int)StyleKeyword::Unset
             )
            {
                switch(prop.id)
                {
                case Ids::transform: {
                    auto v = fget();
                    v->transform = {};
                    break;
                    }
                case Ids::flexGrow: {
                    auto v = fget();
                    v->flexGrow = 0.f;
                    break;
                    }
                case Ids::flexShrink: {
                    auto v = fget();
                    v->flexShrink = 1.f;
                    break;
                    }
                case Ids::flexBasis: {
                    auto v = fget();
                    v->flexBasis = YGValueAuto;
                    break;
                    }
                case Ids::top: {
                    auto v = fget();
                    v->top = YGValueAuto;
                    break;
                    }
                case Ids::right: {
                    auto v = fget();
                    v->right = YGValueAuto;
                    break;
                    }
                case Ids::bottom: {
                    auto v = fget();
                    v->bottom = YGValueAuto;
                    break;
                    }
                case Ids::left: {
                    auto v = fget();
                    v->left = YGValueAuto;
                    break;
                    }
                case Ids::marginTop: {
                    auto v = fget();
                    v->marginTop = YGValueZero;
                    break;
                    }
                case Ids::marginRight: {
                    auto v = fget();
                    v->marginRight = YGValueZero;
                    break;
                    }
                case Ids::marginBottom: {
                    auto v = fget();
                    v->marginBottom = YGValueZero;
                    break;
                    }
                case Ids::marginLeft: {
                    auto v = fget();
                    v->marginLeft = YGValueZero;
                    break;
                    }
                case Ids::paddingTop: {
                    auto v = fget();
                    v->paddingTop = YGValueZero;
                    break;
                    }
                case Ids::paddingRight: {
                    auto v = fget();
                    v->paddingRight = YGValueZero;
                    break;
                    }
                case Ids::paddingBottom: {
                    auto v = fget();
                    v->paddingBottom = YGValueZero;
                    break;
                    }
                case Ids::paddingLeft: {
                    auto v = fget();
                    v->paddingLeft = YGValueZero;
                    break;
                    }
                case Ids::width: {
                    auto v = fget();
                    v->width = YGValueAuto;
                    break;
                    }
                case Ids::height: {
                    auto v = fget();
                    v->height = YGValueAuto;
                    break;
                    }
                case Ids::position: {
                    auto v = fget();
                    v->position = YGPositionTypeRelative;
                    break;
                    }
                case Ids::overflow: {
                    auto v = fget();
                    v->overflow = EFlexOverflow::Visible;
                    break;
                    }
                case Ids::alignSelf: {
                    auto v = fget();
                    v->alignSelf = YGAlignAuto;
                    break;
                    }
                case Ids::maxWidth: {
                    auto v = fget();
                    v->maxWidth = YGValueUndefined;
                    break;
                    }
                case Ids::maxHeight: {
                    auto v = fget();
                    v->maxHeight = YGValueUndefined;
                    break;
                    }
                case Ids::minWidth: {
                    auto v = fget();
                    v->minWidth = YGValueAuto;
                    break;
                    }
                case Ids::minHeight: {
                    auto v = fget();
                    v->minHeight = YGValueAuto;
                    break;
                    }
                case Ids::flexDirection: {
                    auto v = fget();
                    v->flexDirection = YGFlexDirectionRow;
                    break;
                    }
                case Ids::alignContent: {
                    auto v = fget();
                    v->alignContent = YGAlignFlexStart;
                    break;
                    }
                case Ids::alignItems: {
                    auto v = fget();
                    v->alignItems = YGAlignStretch;
                    break;
                    }
                case Ids::justifyContent: {
                    auto v = fget();
                    v->justifyContent = YGJustifyFlexStart;
                    break;
                    }
                case Ids::flexWrap: {
                    auto v = fget();
                    v->flexWrap = YGWrapNoWrap;
                    break;
                    }
                case Ids::flexDisplay: {
                    auto v = fget();
                    v->flexDisplay = YGDisplayFlex;
                    break;
                    }
                case Ids::verticalAlign: {
                    auto v = fget();
                    v->verticalAlign = EInlineAlign::Middle;
                    break;
                    }
                case Ids::aspectRatio: {
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
                case Ids::left:{
                    auto v = fget();
                    v->left = pst->left;
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
                case Ids::marginLeft:{
                    auto v = fget();
                    v->marginLeft = pst->marginLeft;
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
                case Ids::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = pst->paddingLeft;
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
                    v->transform = ToOwned(sheet.Get<const gsl::span<TransformFunction>>(prop.value));
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
                case Ids::left:{
                    auto v = fget();
                    v->left = sheet.Get<YGValue>(prop.value);
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
                case Ids::marginLeft:{
                    auto v = fget();
                    v->marginLeft = sheet.Get<YGValue>(prop.value);
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
                case Ids::paddingLeft:{
                    auto v = fget();
                    v->paddingLeft = sheet.Get<YGValue>(prop.value);
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
                    v->overflow = sheet.Get<EFlexOverflow>(prop.value);
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


OGUI::RestyleDamage OGUI::StylePosition::ApplyAnimatedProperties(ComputedStyle& style, const StyleSheetStorage& sheet, const gsl::span<AnimatedProperty>& props, const gsl::span<size_t>& override)
{
    OGUI::StylePosition* st = nullptr;
    RestyleDamage damage = RestyleDamage::None;
    auto& s = style.structs[StylePositionEntry];
    bool owned = false;
    if(s.ptr)
    {
        st = (StylePosition*)s.ptr.get();
        owned = s.owned;
    }
    auto fget = [&]
    {
        if(!st)
        {
            auto value = std::make_shared<OGUI::StylePosition>();
            value->Initialize();
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        else if(!owned)
        {
            auto value = std::make_shared<OGUI::StylePosition>(*st);
            s.ptr = std::static_pointer_cast<void>(value);
            s.owned = owned = true;
            st = value.get();
        }
        return st;
    };
    
    auto mask = override[StylePositionEntry];
    
    for(auto& prop : props)
    {
        switch(prop.id)
        {
            case Ids::transform: if(mask & (1ull<<0)) continue; break;
            case Ids::flexGrow: if(mask & (1ull<<1)) continue; break;
            case Ids::flexShrink: if(mask & (1ull<<2)) continue; break;
            case Ids::flexBasis: if(mask & (1ull<<3)) continue; break;
            case Ids::top: if(mask & (1ull<<4)) continue; break;
            case Ids::right: if(mask & (1ull<<5)) continue; break;
            case Ids::bottom: if(mask & (1ull<<6)) continue; break;
            case Ids::left: if(mask & (1ull<<7)) continue; break;
            case Ids::marginTop: if(mask & (1ull<<8)) continue; break;
            case Ids::marginRight: if(mask & (1ull<<9)) continue; break;
            case Ids::marginBottom: if(mask & (1ull<<10)) continue; break;
            case Ids::marginLeft: if(mask & (1ull<<11)) continue; break;
            case Ids::paddingTop: if(mask & (1ull<<12)) continue; break;
            case Ids::paddingRight: if(mask & (1ull<<13)) continue; break;
            case Ids::paddingBottom: if(mask & (1ull<<14)) continue; break;
            case Ids::paddingLeft: if(mask & (1ull<<15)) continue; break;
            case Ids::width: if(mask & (1ull<<16)) continue; break;
            case Ids::height: if(mask & (1ull<<17)) continue; break;
            case Ids::position: if(mask & (1ull<<18)) continue; break;
            case Ids::overflow: if(mask & (1ull<<19)) continue; break;
            case Ids::alignSelf: if(mask & (1ull<<20)) continue; break;
            case Ids::maxWidth: if(mask & (1ull<<21)) continue; break;
            case Ids::maxHeight: if(mask & (1ull<<22)) continue; break;
            case Ids::minWidth: if(mask & (1ull<<23)) continue; break;
            case Ids::minHeight: if(mask & (1ull<<24)) continue; break;
            case Ids::flexDirection: if(mask & (1ull<<25)) continue; break;
            case Ids::alignContent: if(mask & (1ull<<26)) continue; break;
            case Ids::alignItems: if(mask & (1ull<<27)) continue; break;
            case Ids::justifyContent: if(mask & (1ull<<28)) continue; break;
            case Ids::flexWrap: if(mask & (1ull<<29)) continue; break;
            case Ids::flexDisplay: if(mask & (1ull<<30)) continue; break;
            case Ids::verticalAlign: if(mask & (1ull<<31)) continue; break;
            case Ids::aspectRatio: if(mask & (1ull<<32)) continue; break;
        }
        switch(prop.id)
        {
            case Ids::transform:{
                auto v = fget();
                if(prop.alpha == 0.f && prop.from == prop.to)
                    break;
                if(prop.alpha == 0.f)
                    v->transform = ToOwned(sheet.Get<const gsl::span<TransformFunction>>(prop.from));
                else if(prop.alpha == 1.f)
                    v->transform = ToOwned(sheet.Get<const gsl::span<TransformFunction>>(prop.to));
                else if(prop.from == prop.to)
                    v->transform = OGUI::Lerp(v->transform, sheet.Get<const gsl::span<TransformFunction>>(prop.to), prop.alpha);
                else
                    v->transform = OGUI::Lerp(sheet.Get<const gsl::span<TransformFunction>>(prop.from), sheet.Get<const gsl::span<TransformFunction>>(prop.to), prop.alpha);
                
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
                    v->overflow = sheet.Get<EFlexOverflow>(prop.from);
                else if(prop.alpha == 1.f)
                    v->overflow = sheet.Get<EFlexOverflow>(prop.to);
                else if(prop.from == prop.to)
                    v->overflow = OGUI::Lerp(v->overflow, sheet.Get<EFlexOverflow>(prop.to), prop.alpha);
                else
                    v->overflow = OGUI::Lerp(sheet.Get<EFlexOverflow>(prop.from), sheet.Get<EFlexOverflow>(prop.to), prop.alpha);
                
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

void OGUI::StylePosition::Merge(ComputedStyle& style, ComputedStyle& other, const gsl::span<size_t>& override)
{
    auto po = TryGet(other);
    if(!po)
        return;
    auto mask = override[StylePositionEntry];
    if(!mask)
        return;
    auto& s = GetOrAdd(style);
    if(mask & (1ull << 0))
        s.transform = po->transform;
    if(mask & (1ull << 1))
        s.flexGrow = po->flexGrow;
    if(mask & (1ull << 2))
        s.flexShrink = po->flexShrink;
    if(mask & (1ull << 3))
        s.flexBasis = po->flexBasis;
    if(mask & (1ull << 4))
        s.top = po->top;
    if(mask & (1ull << 5))
        s.right = po->right;
    if(mask & (1ull << 6))
        s.bottom = po->bottom;
    if(mask & (1ull << 7))
        s.left = po->left;
    if(mask & (1ull << 8))
        s.marginTop = po->marginTop;
    if(mask & (1ull << 9))
        s.marginRight = po->marginRight;
    if(mask & (1ull << 10))
        s.marginBottom = po->marginBottom;
    if(mask & (1ull << 11))
        s.marginLeft = po->marginLeft;
    if(mask & (1ull << 12))
        s.paddingTop = po->paddingTop;
    if(mask & (1ull << 13))
        s.paddingRight = po->paddingRight;
    if(mask & (1ull << 14))
        s.paddingBottom = po->paddingBottom;
    if(mask & (1ull << 15))
        s.paddingLeft = po->paddingLeft;
    if(mask & (1ull << 16))
        s.width = po->width;
    if(mask & (1ull << 17))
        s.height = po->height;
    if(mask & (1ull << 18))
        s.position = po->position;
    if(mask & (1ull << 19))
        s.overflow = po->overflow;
    if(mask & (1ull << 20))
        s.alignSelf = po->alignSelf;
    if(mask & (1ull << 21))
        s.maxWidth = po->maxWidth;
    if(mask & (1ull << 22))
        s.maxHeight = po->maxHeight;
    if(mask & (1ull << 23))
        s.minWidth = po->minWidth;
    if(mask & (1ull << 24))
        s.minHeight = po->minHeight;
    if(mask & (1ull << 25))
        s.flexDirection = po->flexDirection;
    if(mask & (1ull << 26))
        s.alignContent = po->alignContent;
    if(mask & (1ull << 27))
        s.alignItems = po->alignItems;
    if(mask & (1ull << 28))
        s.justifyContent = po->justifyContent;
    if(mask & (1ull << 29))
        s.flexWrap = po->flexWrap;
    if(mask & (1ull << 30))
        s.flexDisplay = po->flexDisplay;
    if(mask & (1ull << 31))
        s.verticalAlign = po->verticalAlign;
    if(mask & (1ull << 32))
        s.aspectRatio = po->aspectRatio;
}

void OGUI::StylePosition::SetupParser()
{
    CSSParser::RegisterMargin();
    CSSParser::RegisterPadding();
	{
        using namespace CSSParser;
        static const auto grammar = "transformValue <- GlobalValue / (TransformFunction (_ ',' _ TransformFunction)*) \ntransform <- 'transform' _ ':' _ transformValue";
        RegisterProperty("transform");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::transform;
            parser["transformValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                {
                    std::vector<TransformFunction> value;
                    for(auto& e : vs)
                        value.emplace_back(any_move<TransformFunction>(e));
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<const gsl::span<TransformFunction>>(value)});
                }
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-growValue <- GlobalValue / Number \nflex-grow <- 'flex-grow' _ ':' _ flex-growValue";
        RegisterProperty("flex-grow");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexGrow;
            parser["flex-growValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-shrinkValue <- GlobalValue / Number \nflex-shrink <- 'flex-shrink' _ ':' _ flex-shrinkValue";
        RegisterProperty("flex-shrink");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexShrink;
            parser["flex-shrinkValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-basisValue <- GlobalValue / Width \nflex-basis <- 'flex-basis' _ ':' _ flex-basisValue";
        RegisterProperty("flex-basis");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexBasis;
            parser["flex-basisValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "topValue <- GlobalValue / LengthPercentage \ntop <- 'top' _ ':' _ topValue";
        RegisterProperty("top");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::top;
            parser["topValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "rightValue <- GlobalValue / LengthPercentage \nright <- 'right' _ ':' _ rightValue";
        RegisterProperty("right");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::right;
            parser["rightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "bottomValue <- GlobalValue / LengthPercentage \nbottom <- 'bottom' _ ':' _ bottomValue";
        RegisterProperty("bottom");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::bottom;
            parser["bottomValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "leftValue <- GlobalValue / LengthPercentage \nleft <- 'left' _ ':' _ leftValue";
        RegisterProperty("left");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::left;
            parser["leftValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "margin-topValue <- GlobalValue / LengthPercentage \nmargin-top <- 'margin-top' _ ':' _ margin-topValue";
        RegisterProperty("margin-top");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::marginTop;
            parser["margin-topValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "margin-rightValue <- GlobalValue / LengthPercentage \nmargin-right <- 'margin-right' _ ':' _ margin-rightValue";
        RegisterProperty("margin-right");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::marginRight;
            parser["margin-rightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "margin-bottomValue <- GlobalValue / LengthPercentage \nmargin-bottom <- 'margin-bottom' _ ':' _ margin-bottomValue";
        RegisterProperty("margin-bottom");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::marginBottom;
            parser["margin-bottomValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "margin-leftValue <- GlobalValue / LengthPercentage \nmargin-left <- 'margin-left' _ ':' _ margin-leftValue";
        RegisterProperty("margin-left");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::marginLeft;
            parser["margin-leftValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "padding-topValue <- GlobalValue / LengthPercentage \npadding-top <- 'padding-top' _ ':' _ padding-topValue";
        RegisterProperty("padding-top");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::paddingTop;
            parser["padding-topValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "padding-rightValue <- GlobalValue / LengthPercentage \npadding-right <- 'padding-right' _ ':' _ padding-rightValue";
        RegisterProperty("padding-right");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::paddingRight;
            parser["padding-rightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "padding-bottomValue <- GlobalValue / LengthPercentage \npadding-bottom <- 'padding-bottom' _ ':' _ padding-bottomValue";
        RegisterProperty("padding-bottom");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::paddingBottom;
            parser["padding-bottomValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "padding-leftValue <- GlobalValue / LengthPercentage \npadding-left <- 'padding-left' _ ':' _ padding-leftValue";
        RegisterProperty("padding-left");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::paddingLeft;
            parser["padding-leftValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "widthValue <- GlobalValue / Width \nwidth <- 'width' _ ':' _ widthValue";
        RegisterProperty("width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::width;
            parser["widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "heightValue <- GlobalValue / Width \nheight <- 'height' _ ':' _ heightValue";
        RegisterProperty("height");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::height;
            parser["heightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "positionValue <- GlobalValue / FlexPosition \nposition <- 'position' _ ':' _ positionValue";
        RegisterProperty("position");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::position;
            parser["positionValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGPositionType>(std::any_cast<YGPositionType&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "overflowValue <- GlobalValue / FlexOverflow \noverflow <- 'overflow' _ ':' _ overflowValue";
        RegisterProperty("overflow");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::overflow;
            parser["overflowValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<EFlexOverflow>(std::any_cast<EFlexOverflow&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "align-selfValue <- GlobalValue / FlexAlign \nalign-self <- 'align-self' _ ':' _ align-selfValue";
        RegisterProperty("align-self");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::alignSelf;
            parser["align-selfValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGAlign>(std::any_cast<YGAlign&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "max-widthValue <- GlobalValue / LengthPercentage \nmax-width <- 'max-width' _ ':' _ max-widthValue";
        RegisterProperty("max-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::maxWidth;
            parser["max-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "max-heightValue <- GlobalValue / LengthPercentage \nmax-height <- 'max-height' _ ':' _ max-heightValue";
        RegisterProperty("max-height");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::maxHeight;
            parser["max-heightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "min-widthValue <- GlobalValue / Width \nmin-width <- 'min-width' _ ':' _ min-widthValue";
        RegisterProperty("min-width");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::minWidth;
            parser["min-widthValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "min-heightValue <- GlobalValue / Width \nmin-height <- 'min-height' _ ':' _ min-heightValue";
        RegisterProperty("min-height");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::minHeight;
            parser["min-heightValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGValue>(std::any_cast<YGValue&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-directionValue <- GlobalValue / FlexDirection \nflex-direction <- 'flex-direction' _ ':' _ flex-directionValue";
        RegisterProperty("flex-direction");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexDirection;
            parser["flex-directionValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGFlexDirection>(std::any_cast<YGFlexDirection&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "align-contentValue <- GlobalValue / FlexAlign \nalign-content <- 'align-content' _ ':' _ align-contentValue";
        RegisterProperty("align-content");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::alignContent;
            parser["align-contentValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGAlign>(std::any_cast<YGAlign&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "align-itemsValue <- GlobalValue / FlexAlign \nalign-items <- 'align-items' _ ':' _ align-itemsValue";
        RegisterProperty("align-items");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::alignItems;
            parser["align-itemsValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGAlign>(std::any_cast<YGAlign&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "justify-contentValue <- GlobalValue / FlexJustify \njustify-content <- 'justify-content' _ ':' _ justify-contentValue";
        RegisterProperty("justify-content");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::justifyContent;
            parser["justify-contentValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGJustify>(std::any_cast<YGJustify&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-wrapValue <- GlobalValue / FlexWrap \nflex-wrap <- 'flex-wrap' _ ':' _ flex-wrapValue";
        RegisterProperty("flex-wrap");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexWrap;
            parser["flex-wrapValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGWrap>(std::any_cast<YGWrap&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "flex-displayValue <- GlobalValue / FlexDisplay \nflex-display <- 'flex-display' _ ':' _ flex-displayValue";
        RegisterProperty("flex-display");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::flexDisplay;
            parser["flex-displayValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<YGDisplay>(std::any_cast<YGDisplay&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "vertical-alignValue <- GlobalValue / InlineAlign \nvertical-align <- 'vertical-align' _ ':' _ vertical-alignValue";
        RegisterProperty("vertical-align");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::verticalAlign;
            parser["vertical-alignValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<EInlineAlign>(std::any_cast<EInlineAlign&>(vs[0]))});
            };
        });
    }
	{
        using namespace CSSParser;
        static const auto grammar = "aspect-ratioValue <- GlobalValue / AspectRatio \naspect-ratio <- 'aspect-ratio' _ ':' _ aspect-ratioValue";
        RegisterProperty("aspect-ratio");
        RegisterGrammar(grammar, [](peg::parser& parser)
        {
            static size_t hash = Ids::aspectRatio;
            parser["aspect-ratioValue"] = [](peg::SemanticValues& vs, std::any& dt){
                auto& ctx = GetContext<PropertyListContext>(dt);
                if(vs.choice() == 0)
                    ctx.rule->properties.push_back({hash, (int)std::any_cast<StyleKeyword>(vs[0])});
                else
                    ctx.rule->properties.push_back({hash, ctx.storage->Push<float>(std::any_cast<float&>(vs[0]))});
            };
        });
    }
}


attr("script": true)
void OGUI::SetStyleTransform(VisualElement* element, const gsl::span<TransformFunction>& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<0;
    StylePosition::GetOrAdd(element->_style).transform = ToOwned(value);
    RestyleDamage damage = RestyleDamage::Transform;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleTransform(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<0);
}
attr("script": true)
void OGUI::SetStyleFlexGrow(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<1;
    StylePosition::GetOrAdd(element->_style).flexGrow = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexGrow(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<1);
}
attr("script": true)
void OGUI::SetStyleFlexShrink(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<2;
    StylePosition::GetOrAdd(element->_style).flexShrink = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexShrink(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<2);
}
attr("script": true)
void OGUI::SetStyleFlexBasis(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<3;
    StylePosition::GetOrAdd(element->_style).flexBasis = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexBasis(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<3);
}
attr("script": true)
void OGUI::SetStyleTop(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<4;
    StylePosition::GetOrAdd(element->_style).top = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleTop(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<4);
}
attr("script": true)
void OGUI::SetStyleRight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<5;
    StylePosition::GetOrAdd(element->_style).right = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleRight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<5);
}
attr("script": true)
void OGUI::SetStyleBottom(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<6;
    StylePosition::GetOrAdd(element->_style).bottom = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleBottom(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<6);
}
attr("script": true)
void OGUI::SetStyleLeft(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<7;
    StylePosition::GetOrAdd(element->_style).left = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleLeft(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<7);
}
attr("script": true)
void OGUI::SetStyleMarginTop(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<8;
    StylePosition::GetOrAdd(element->_style).marginTop = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMarginTop(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<8);
}
attr("script": true)
void OGUI::SetStyleMarginRight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<9;
    StylePosition::GetOrAdd(element->_style).marginRight = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMarginRight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<9);
}
attr("script": true)
void OGUI::SetStyleMarginBottom(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<10;
    StylePosition::GetOrAdd(element->_style).marginBottom = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMarginBottom(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<10);
}
attr("script": true)
void OGUI::SetStyleMarginLeft(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<11;
    StylePosition::GetOrAdd(element->_style).marginLeft = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMarginLeft(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<11);
}
attr("script": true)
void OGUI::SetStylePaddingTop(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<12;
    StylePosition::GetOrAdd(element->_style).paddingTop = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStylePaddingTop(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<12);
}
attr("script": true)
void OGUI::SetStylePaddingRight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<13;
    StylePosition::GetOrAdd(element->_style).paddingRight = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStylePaddingRight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<13);
}
attr("script": true)
void OGUI::SetStylePaddingBottom(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<14;
    StylePosition::GetOrAdd(element->_style).paddingBottom = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStylePaddingBottom(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<14);
}
attr("script": true)
void OGUI::SetStylePaddingLeft(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<15;
    StylePosition::GetOrAdd(element->_style).paddingLeft = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStylePaddingLeft(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<15);
}
attr("script": true)
void OGUI::SetStyleWidth(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<16;
    StylePosition::GetOrAdd(element->_style).width = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleWidth(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<16);
}
attr("script": true)
void OGUI::SetStyleHeight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<17;
    StylePosition::GetOrAdd(element->_style).height = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleHeight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<17);
}
attr("script": true)
void OGUI::SetStylePosition(VisualElement* element, const YGPositionType& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<18;
    StylePosition::GetOrAdd(element->_style).position = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStylePosition(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<18);
}
attr("script": true)
void OGUI::SetStyleOverflow(VisualElement* element, const EFlexOverflow& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<19;
    StylePosition::GetOrAdd(element->_style).overflow = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleOverflow(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<19);
}
attr("script": true)
void OGUI::SetStyleAlignSelf(VisualElement* element, const YGAlign& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<20;
    StylePosition::GetOrAdd(element->_style).alignSelf = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleAlignSelf(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<20);
}
attr("script": true)
void OGUI::SetStyleMaxWidth(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<21;
    StylePosition::GetOrAdd(element->_style).maxWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMaxWidth(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<21);
}
attr("script": true)
void OGUI::SetStyleMaxHeight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<22;
    StylePosition::GetOrAdd(element->_style).maxHeight = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMaxHeight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<22);
}
attr("script": true)
void OGUI::SetStyleMinWidth(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<23;
    StylePosition::GetOrAdd(element->_style).minWidth = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMinWidth(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<23);
}
attr("script": true)
void OGUI::SetStyleMinHeight(VisualElement* element, const YGValue& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<24;
    StylePosition::GetOrAdd(element->_style).minHeight = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleMinHeight(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<24);
}
attr("script": true)
void OGUI::SetStyleFlexDirection(VisualElement* element, const YGFlexDirection& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<25;
    StylePosition::GetOrAdd(element->_style).flexDirection = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexDirection(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<25);
}
attr("script": true)
void OGUI::SetStyleAlignContent(VisualElement* element, const YGAlign& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<26;
    StylePosition::GetOrAdd(element->_style).alignContent = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleAlignContent(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<26);
}
attr("script": true)
void OGUI::SetStyleAlignItems(VisualElement* element, const YGAlign& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<27;
    StylePosition::GetOrAdd(element->_style).alignItems = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleAlignItems(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<27);
}
attr("script": true)
void OGUI::SetStyleJustifyContent(VisualElement* element, const YGJustify& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<28;
    StylePosition::GetOrAdd(element->_style).justifyContent = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleJustifyContent(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<28);
}
attr("script": true)
void OGUI::SetStyleFlexWrap(VisualElement* element, const YGWrap& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<29;
    StylePosition::GetOrAdd(element->_style).flexWrap = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexWrap(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<29);
}
attr("script": true)
void OGUI::SetStyleFlexDisplay(VisualElement* element, const YGDisplay& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<30;
    StylePosition::GetOrAdd(element->_style).flexDisplay = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleFlexDisplay(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<30);
}
attr("script": true)
void OGUI::SetStyleVerticalAlign(VisualElement* element, const EInlineAlign& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<31;
    StylePosition::GetOrAdd(element->_style).verticalAlign = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleVerticalAlign(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<31);
}
attr("script": true)
void OGUI::SetStyleAspectRatio(VisualElement* element, const float& value)
{
    element->_procedureOverrides[StylePositionEntry] |= 1ull<<32;
    StylePosition::GetOrAdd(element->_style).aspectRatio = value;
    RestyleDamage damage = RestyleDamage::Layout;
    element->UpdateStyle(damage);
}
attr("script": true)
void OGUI::ResetStyleAspectRatio(VisualElement* element)
{
    element->_procedureOverrides[StylePositionEntry] &= ~(1ull<<32);
}
