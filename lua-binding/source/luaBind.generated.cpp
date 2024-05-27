//DO NOT MODIFY THIS FILE
//generated from luaBind.cpp.mako
#include "OpenGUI/Event/FocusEvent.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/generated/position.h"
#include "OpenGUI/Style2/generated/effects.h"
#include "OpenGUI/Bind/EventArg.h"
#include "OpenGUI/Style2/generated/border.h"
#include "OpenGUI/Style2/generated/text.h"
#include "OpenGUI/Style2/generated/background.h"
#include "OpenGUI/Style2/Selector.h"
#include "luaBind.hpp"
void BindLua_generated(lua_State* L)
{
    sol::state_view lua(L);
    sol::table OGUI = lua["OGUI"];
    {
        sol::usertype<OGUI::IEventArg> type = OGUI.new_usertype<OGUI::IEventArg>("IEventArg");
        type["TryGet"] = +[](OGUI::IEventArg* self, ostr::string _0) { return self->TryGet(_0); };
    }
    {
        sol::usertype<OGUI::Bindable> type = OGUI.new_usertype<OGUI::Bindable>("Bindable");
        type["Bind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::Bindable::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    {
        sol::usertype<OGUI::VisualElement> type = 
            OGUI.new_usertype<OGUI::VisualElement>("VisualElement", sol::base_classes, sol::bases<OGUI::Bindable>());
        type["GetParent"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetParent;
        type["GetHierachyParent"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetHierachyParent;
        type["Visible"] = (bool(OGUI::VisualElement::*)()const)&OGUI::VisualElement::Visible;
        type["GetName"] = (const ostr::string &(OGUI::VisualElement::*)())&OGUI::VisualElement::GetName;
        type["SetName"] = (void(OGUI::VisualElement::*)(ostr::string))&OGUI::VisualElement::SetName;
        type["DestoryTree"] = (void(*)(OGUI::VisualElement *))&OGUI::VisualElement::DestoryTree;
        type["SetVisibility"] = (void(OGUI::VisualElement::*)(bool))&OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = (bool(OGUI::VisualElement::*)())&OGUI::VisualElement::IsClippingChildren;
        type["IsPick"] = (bool(OGUI::VisualElement::*)())&OGUI::VisualElement::IsPick;
        type["SetIsPick"] = (void(OGUI::VisualElement::*)(bool))&OGUI::VisualElement::SetIsPick;
        type["PushChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::PushChild;
        type["InsertChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *, int))&OGUI::VisualElement::InsertChild;
        type["InsertChildAfter"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *, OGUI::VisualElement *))&OGUI::VisualElement::InsertChildAfter;
        type["RemoveChild"] = (void(OGUI::VisualElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::RemoveChild;
        type["ClearChildren"] = (void(OGUI::VisualElement::*)())&OGUI::VisualElement::ClearChildren;
        type["GetRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetLayoutRoot;
        type["GetSize"] = (OGUI::Vector<float, 2>(OGUI::VisualElement::*)()const)&OGUI::VisualElement::GetSize;
        type["SetTranslationYPixel"] = (void(OGUI::VisualElement::*)(float))&OGUI::VisualElement::SetTranslationYPixel;
        type["AddStyleClass"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->AddStyleClass(_0); };
        type["RemoveStyleClass"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->RemoveStyleClass(_0); };
        type["SetPseudoClass"] = (void(OGUI::VisualElement::*)(OGUI::PseudoStates, bool))&OGUI::VisualElement::SetPseudoClass;
        type["CalculateLayout"] = (void(OGUI::VisualElement::*)(float, float))&OGUI::VisualElement::CalculateLayout;
        type["ContainClass"] = +[](OGUI::VisualElement* self, ostr::string _0) { return self->ContainClass(_0); };
        type["SetFocusable"] = (void(OGUI::VisualElement::*)(bool))&OGUI::VisualElement::SetFocusable;
        type["GetFocusScopeFocused"] = (OGUI::VisualElement *(OGUI::VisualElement::*)())&OGUI::VisualElement::GetFocusScopeFocused;
        type["AddScroll"] = (void(OGUI::VisualElement::*)(OGUI::Vector<float, 2>))&OGUI::VisualElement::AddScroll;
        type["SetScroll"] = (void(OGUI::VisualElement::*)(OGUI::Vector<float, 2>))&OGUI::VisualElement::SetScroll;
        type["SetXmlFilter"] = (void(OGUI::VisualElement::*)(const char *, const char *))&OGUI::VisualElement::SetXmlFilter;
        type["CleanXmlFilter"] = (void(OGUI::VisualElement::*)(const char *))&OGUI::VisualElement::CleanXmlFilter;
        type["Bind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::VisualElement::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    {
        sol::usertype<OGUI::Context> type = OGUI.new_usertype<OGUI::Context>("Context");
        type["IsElementValid"] = (bool(OGUI::Context::*)(OGUI::VisualElement *)const)&OGUI::Context::IsElementValid;
        type["ActivateWindow"] = (bool(OGUI::Context::*)(OGUI::VisualWindow *))&OGUI::Context::ActivateWindow;
        type["SetFocus"] = (bool(OGUI::Context::*)(OGUI::VisualElement *, OGUI::EFocusChangeCause, ostr::string))&OGUI::Context::SetFocus;
        type["SetXmlFilter_Global"] = (void(OGUI::Context::*)(const char *, const char *))&OGUI::Context::SetXmlFilter_Global;
        type["CleanXmlFilter_Global"] = (void(OGUI::Context::*)(const char *))&OGUI::Context::CleanXmlFilter_Global;
        type["ResetXmlFilter_Global"] = (void(OGUI::Context::*)())&OGUI::Context::ResetXmlFilter_Global;
        type["UpdataXmlFilterCache_Global"] = (void(OGUI::Context::*)())&OGUI::Context::UpdataXmlFilterCache_Global;
        type["HasFilterTag_Global"] = (bool(OGUI::Context::*)(const char *)const)&OGUI::Context::HasFilterTag_Global;
        type["ForceRefreshXmlFilter"] = (void(OGUI::Context::*)())&OGUI::Context::ForceRefreshXmlFilter;
        type["Get"] = (OGUI::Context &(*)())&OGUI::Context::Get;
    }
    {
        sol::usertype<OGUI::TextElement> type = 
            OGUI.new_usertype<OGUI::TextElement>("TextElement", sol::base_classes, sol::bases<OGUI::VisualElement>());
        type["AddInlineText"] = (void(OGUI::TextElement::*)(OGUI::TextElement *))&OGUI::TextElement::AddInlineText;
        type["AddText"] = (void(OGUI::TextElement::*)(ostr::string))&OGUI::TextElement::AddText;
        type["ClearText"] = (void(OGUI::TextElement::*)())&OGUI::TextElement::ClearText;
        type["GetParent"] = (OGUI::VisualElement *(OGUI::TextElement::*)())&OGUI::VisualElement::GetParent;
        type["GetHierachyParent"] = (OGUI::VisualElement *(OGUI::TextElement::*)())&OGUI::VisualElement::GetHierachyParent;
        type["Visible"] = (bool(OGUI::TextElement::*)()const)&OGUI::VisualElement::Visible;
        type["GetName"] = (const ostr::string &(OGUI::TextElement::*)())&OGUI::VisualElement::GetName;
        type["SetName"] = (void(OGUI::TextElement::*)(ostr::string))&OGUI::VisualElement::SetName;
        type["DestoryTree"] = (void(*)(OGUI::VisualElement *))&OGUI::VisualElement::DestoryTree;
        type["SetVisibility"] = (void(OGUI::TextElement::*)(bool))&OGUI::VisualElement::SetVisibility;
        type["IsClippingChildren"] = (bool(OGUI::TextElement::*)())&OGUI::VisualElement::IsClippingChildren;
        type["IsPick"] = (bool(OGUI::TextElement::*)())&OGUI::VisualElement::IsPick;
        type["SetIsPick"] = (void(OGUI::TextElement::*)(bool))&OGUI::VisualElement::SetIsPick;
        type["PushChild"] = (void(OGUI::TextElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::PushChild;
        type["InsertChild"] = (void(OGUI::TextElement::*)(OGUI::VisualElement *, int))&OGUI::VisualElement::InsertChild;
        type["InsertChildAfter"] = (void(OGUI::TextElement::*)(OGUI::VisualElement *, OGUI::VisualElement *))&OGUI::VisualElement::InsertChildAfter;
        type["RemoveChild"] = (void(OGUI::TextElement::*)(OGUI::VisualElement *))&OGUI::VisualElement::RemoveChild;
        type["ClearChildren"] = (void(OGUI::TextElement::*)())&OGUI::VisualElement::ClearChildren;
        type["GetRoot"] = (OGUI::VisualElement *(OGUI::TextElement::*)())&OGUI::VisualElement::GetRoot;
        type["GetLayoutRoot"] = (OGUI::VisualElement *(OGUI::TextElement::*)())&OGUI::VisualElement::GetLayoutRoot;
        type["GetSize"] = (OGUI::Vector<float, 2>(OGUI::TextElement::*)()const)&OGUI::VisualElement::GetSize;
        type["SetTranslationYPixel"] = (void(OGUI::TextElement::*)(float))&OGUI::VisualElement::SetTranslationYPixel;
        type["AddStyleClass"] = +[](OGUI::TextElement* self, ostr::string _0) { return self->AddStyleClass(_0); };
        type["RemoveStyleClass"] = +[](OGUI::TextElement* self, ostr::string _0) { return self->RemoveStyleClass(_0); };
        type["SetPseudoClass"] = (void(OGUI::TextElement::*)(OGUI::PseudoStates, bool))&OGUI::VisualElement::SetPseudoClass;
        type["CalculateLayout"] = (void(OGUI::TextElement::*)(float, float))&OGUI::VisualElement::CalculateLayout;
        type["ContainClass"] = +[](OGUI::TextElement* self, ostr::string _0) { return self->ContainClass(_0); };
        type["SetFocusable"] = (void(OGUI::TextElement::*)(bool))&OGUI::VisualElement::SetFocusable;
        type["GetFocusScopeFocused"] = (OGUI::VisualElement *(OGUI::TextElement::*)())&OGUI::VisualElement::GetFocusScopeFocused;
        type["AddScroll"] = (void(OGUI::TextElement::*)(OGUI::Vector<float, 2>))&OGUI::VisualElement::AddScroll;
        type["SetScroll"] = (void(OGUI::TextElement::*)(OGUI::Vector<float, 2>))&OGUI::VisualElement::SetScroll;
        type["SetXmlFilter"] = (void(OGUI::TextElement::*)(const char *, const char *))&OGUI::VisualElement::SetXmlFilter;
        type["CleanXmlFilter"] = (void(OGUI::TextElement::*)(const char *))&OGUI::VisualElement::CleanXmlFilter;
        type["Bind"] = (void(OGUI::TextElement::*)(OGUI::Bindable &))&OGUI::Bindable::Bind;
        type["Unbind"] = (void(OGUI::TextElement::*)(OGUI::Bindable &))&OGUI::Bindable::Unbind;
    }
    OGUI["QueryFirst"] = +[](OGUI::VisualElement * _0, ostr::string _1) { return OGUI::QueryFirst(_0, _1); };
    OGUI["QueryAll"] = +[](OGUI::VisualElement * _0, ostr::string _1, std::vector<OGUI::VisualElement *> & _2) { return OGUI::QueryAll(_0, _1, _2); };
    OGUI["BindTree"] = (void(*)(OGUI::VisualElement *, OGUI::Bindable &))&OGUI::BindTree;
    OGUI["UnBindTree"] = (void(*)(OGUI::VisualElement *, OGUI::Bindable &))&OGUI::UnBindTree;
    OGUI["SetStyleBackgroundColor"] = (void(*)(OGUI::VisualElement *, const OGUI::Vector<float, 4> &))&OGUI::SetStyleBackgroundColor;
    OGUI["ResetStyleBackgroundColor"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBackgroundColor;
    OGUI["SetStyleBackgroundImage"] = (void(*)(OGUI::VisualElement *, const ostr::string_view &))&OGUI::SetStyleBackgroundImage;
    OGUI["ResetStyleBackgroundImage"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBackgroundImage;
    OGUI["SetStyleBackgroundMaterial"] = (void(*)(OGUI::VisualElement *, const ostr::string_view &))&OGUI::SetStyleBackgroundMaterial;
    OGUI["ResetStyleBackgroundMaterial"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBackgroundMaterial;
    OGUI["SetStyleBackgroundGamma"] = (void(*)(OGUI::VisualElement *, const bool &))&OGUI::SetStyleBackgroundGamma;
    OGUI["ResetStyleBackgroundGamma"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBackgroundGamma;
    OGUI["SetStyleBorderTopWidth"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleBorderTopWidth;
    OGUI["ResetStyleBorderTopWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderTopWidth;
    OGUI["SetStyleBorderRightWidth"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleBorderRightWidth;
    OGUI["ResetStyleBorderRightWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderRightWidth;
    OGUI["SetStyleBorderBottomWidth"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleBorderBottomWidth;
    OGUI["ResetStyleBorderBottomWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderBottomWidth;
    OGUI["SetStyleBorderLeftWidth"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleBorderLeftWidth;
    OGUI["ResetStyleBorderLeftWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderLeftWidth;
    OGUI["SetStyleBorderTopLeftRadius"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleBorderTopLeftRadius;
    OGUI["SetStyleBorderTopLeftRadiusPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderTopLeftRadiusPixel;
    OGUI["SetStyleBorderTopLeftRadiusPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderTopLeftRadiusPercentage;
    OGUI["ResetStyleBorderTopLeftRadius"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderTopLeftRadius;
    OGUI["SetStyleBorderTopRightRadius"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleBorderTopRightRadius;
    OGUI["SetStyleBorderTopRightRadiusPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderTopRightRadiusPixel;
    OGUI["SetStyleBorderTopRightRadiusPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderTopRightRadiusPercentage;
    OGUI["ResetStyleBorderTopRightRadius"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderTopRightRadius;
    OGUI["SetStyleBorderBottomRightRadius"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleBorderBottomRightRadius;
    OGUI["SetStyleBorderBottomRightRadiusPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderBottomRightRadiusPixel;
    OGUI["SetStyleBorderBottomRightRadiusPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderBottomRightRadiusPercentage;
    OGUI["ResetStyleBorderBottomRightRadius"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderBottomRightRadius;
    OGUI["SetStyleBorderBottomLeftRadius"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleBorderBottomLeftRadius;
    OGUI["SetStyleBorderBottomLeftRadiusPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderBottomLeftRadiusPixel;
    OGUI["SetStyleBorderBottomLeftRadiusPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBorderBottomLeftRadiusPercentage;
    OGUI["ResetStyleBorderBottomLeftRadius"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBorderBottomLeftRadius;
    OGUI["SetStyleOpacity"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleOpacity;
    OGUI["ResetStyleOpacity"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleOpacity;
    OGUI["ResetStyleTransform"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTransform;
    OGUI["SetStyleFlexGrow"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleFlexGrow;
    OGUI["ResetStyleFlexGrow"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexGrow;
    OGUI["SetStyleFlexShrink"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleFlexShrink;
    OGUI["ResetStyleFlexShrink"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexShrink;
    OGUI["SetStyleFlexBasis"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleFlexBasis;
    OGUI["SetStyleFlexBasisPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleFlexBasisPixel;
    OGUI["SetStyleFlexBasisPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleFlexBasisPercentage;
    OGUI["SetStyleFlexBasisAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleFlexBasisAuto;
    OGUI["ResetStyleFlexBasis"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexBasis;
    OGUI["SetStyleTop"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleTop;
    OGUI["SetStyleTopPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleTopPixel;
    OGUI["SetStyleTopPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleTopPercentage;
    OGUI["ResetStyleTop"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTop;
    OGUI["SetStyleRight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleRight;
    OGUI["SetStyleRightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleRightPixel;
    OGUI["SetStyleRightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleRightPercentage;
    OGUI["ResetStyleRight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleRight;
    OGUI["SetStyleBottom"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleBottom;
    OGUI["SetStyleBottomPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBottomPixel;
    OGUI["SetStyleBottomPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleBottomPercentage;
    OGUI["ResetStyleBottom"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleBottom;
    OGUI["SetStyleLeft"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleLeft;
    OGUI["SetStyleLeftPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleLeftPixel;
    OGUI["SetStyleLeftPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleLeftPercentage;
    OGUI["ResetStyleLeft"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleLeft;
    OGUI["SetStyleMarginTop"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMarginTop;
    OGUI["SetStyleMarginTopPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginTopPixel;
    OGUI["SetStyleMarginTopPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginTopPercentage;
    OGUI["ResetStyleMarginTop"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMarginTop;
    OGUI["SetStyleMarginRight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMarginRight;
    OGUI["SetStyleMarginRightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginRightPixel;
    OGUI["SetStyleMarginRightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginRightPercentage;
    OGUI["ResetStyleMarginRight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMarginRight;
    OGUI["SetStyleMarginBottom"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMarginBottom;
    OGUI["SetStyleMarginBottomPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginBottomPixel;
    OGUI["SetStyleMarginBottomPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginBottomPercentage;
    OGUI["ResetStyleMarginBottom"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMarginBottom;
    OGUI["SetStyleMarginLeft"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMarginLeft;
    OGUI["SetStyleMarginLeftPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginLeftPixel;
    OGUI["SetStyleMarginLeftPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMarginLeftPercentage;
    OGUI["ResetStyleMarginLeft"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMarginLeft;
    OGUI["SetStylePaddingTop"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStylePaddingTop;
    OGUI["SetStylePaddingTopPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingTopPixel;
    OGUI["SetStylePaddingTopPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingTopPercentage;
    OGUI["ResetStylePaddingTop"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStylePaddingTop;
    OGUI["SetStylePaddingRight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStylePaddingRight;
    OGUI["SetStylePaddingRightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingRightPixel;
    OGUI["SetStylePaddingRightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingRightPercentage;
    OGUI["ResetStylePaddingRight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStylePaddingRight;
    OGUI["SetStylePaddingBottom"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStylePaddingBottom;
    OGUI["SetStylePaddingBottomPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingBottomPixel;
    OGUI["SetStylePaddingBottomPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingBottomPercentage;
    OGUI["ResetStylePaddingBottom"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStylePaddingBottom;
    OGUI["SetStylePaddingLeft"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStylePaddingLeft;
    OGUI["SetStylePaddingLeftPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingLeftPixel;
    OGUI["SetStylePaddingLeftPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStylePaddingLeftPercentage;
    OGUI["ResetStylePaddingLeft"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStylePaddingLeft;
    OGUI["SetStyleWidth"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleWidth;
    OGUI["SetStyleWidthPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleWidthPixel;
    OGUI["SetStyleWidthPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleWidthPercentage;
    OGUI["SetStyleWidthAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleWidthAuto;
    OGUI["ResetStyleWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleWidth;
    OGUI["SetStyleHeight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleHeight;
    OGUI["SetStyleHeightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleHeightPixel;
    OGUI["SetStyleHeightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleHeightPercentage;
    OGUI["SetStyleHeightAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleHeightAuto;
    OGUI["ResetStyleHeight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleHeight;
    OGUI["SetStylePosition"] = (void(*)(OGUI::VisualElement *, const YGPositionType &))&OGUI::SetStylePosition;
    OGUI["ResetStylePosition"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStylePosition;
    OGUI["SetStyleOverflow"] = (void(*)(OGUI::VisualElement *, const OGUI::EFlexOverflow &))&OGUI::SetStyleOverflow;
    OGUI["ResetStyleOverflow"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleOverflow;
    OGUI["SetStyleAlignSelf"] = (void(*)(OGUI::VisualElement *, const YGAlign &))&OGUI::SetStyleAlignSelf;
    OGUI["ResetStyleAlignSelf"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleAlignSelf;
    OGUI["SetStyleMaxWidth"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMaxWidth;
    OGUI["SetStyleMaxWidthPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMaxWidthPixel;
    OGUI["SetStyleMaxWidthPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMaxWidthPercentage;
    OGUI["ResetStyleMaxWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMaxWidth;
    OGUI["SetStyleMaxHeight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMaxHeight;
    OGUI["SetStyleMaxHeightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMaxHeightPixel;
    OGUI["SetStyleMaxHeightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMaxHeightPercentage;
    OGUI["ResetStyleMaxHeight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMaxHeight;
    OGUI["SetStyleMinWidth"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMinWidth;
    OGUI["SetStyleMinWidthPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMinWidthPixel;
    OGUI["SetStyleMinWidthPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMinWidthPercentage;
    OGUI["SetStyleMinWidthAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleMinWidthAuto;
    OGUI["ResetStyleMinWidth"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMinWidth;
    OGUI["SetStyleMinHeight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleMinHeight;
    OGUI["SetStyleMinHeightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMinHeightPixel;
    OGUI["SetStyleMinHeightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleMinHeightPercentage;
    OGUI["SetStyleMinHeightAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleMinHeightAuto;
    OGUI["ResetStyleMinHeight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleMinHeight;
    OGUI["SetStyleFlexDirection"] = (void(*)(OGUI::VisualElement *, const YGFlexDirection &))&OGUI::SetStyleFlexDirection;
    OGUI["ResetStyleFlexDirection"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexDirection;
    OGUI["SetStyleAlignContent"] = (void(*)(OGUI::VisualElement *, const YGAlign &))&OGUI::SetStyleAlignContent;
    OGUI["ResetStyleAlignContent"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleAlignContent;
    OGUI["SetStyleAlignItems"] = (void(*)(OGUI::VisualElement *, const YGAlign &))&OGUI::SetStyleAlignItems;
    OGUI["ResetStyleAlignItems"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleAlignItems;
    OGUI["SetStyleJustifyContent"] = (void(*)(OGUI::VisualElement *, const YGJustify &))&OGUI::SetStyleJustifyContent;
    OGUI["ResetStyleJustifyContent"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleJustifyContent;
    OGUI["SetStyleFlexWrap"] = (void(*)(OGUI::VisualElement *, const YGWrap &))&OGUI::SetStyleFlexWrap;
    OGUI["ResetStyleFlexWrap"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexWrap;
    OGUI["SetStyleFlexDisplay"] = (void(*)(OGUI::VisualElement *, const YGDisplay &))&OGUI::SetStyleFlexDisplay;
    OGUI["ResetStyleFlexDisplay"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFlexDisplay;
    OGUI["SetStyleVerticalAlign"] = (void(*)(OGUI::VisualElement *, const OGUI::EInlineAlign &))&OGUI::SetStyleVerticalAlign;
    OGUI["ResetStyleVerticalAlign"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleVerticalAlign;
    OGUI["SetStyleAspectRatio"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleAspectRatio;
    OGUI["ResetStyleAspectRatio"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleAspectRatio;
    OGUI["SetStyleZOrderBias"] = (void(*)(OGUI::VisualElement *, const int &))&OGUI::SetStyleZOrderBias;
    OGUI["ResetStyleZOrderBias"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleZOrderBias;
    OGUI["SetStyleFontSize"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleFontSize;
    OGUI["ResetStyleFontSize"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFontSize;
    OGUI["SetStyleColor"] = (void(*)(OGUI::VisualElement *, const OGUI::Vector<float, 4> &))&OGUI::SetStyleColor;
    OGUI["ResetStyleColor"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleColor;
    OGUI["ResetStyleFontFamily"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFontFamily;
    OGUI["SetStyleFontStyle"] = (void(*)(OGUI::VisualElement *, const OGUI::ETextStyle &))&OGUI::SetStyleFontStyle;
    OGUI["ResetStyleFontStyle"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFontStyle;
    OGUI["SetStyleFontWeight"] = (void(*)(OGUI::VisualElement *, const int &))&OGUI::SetStyleFontWeight;
    OGUI["ResetStyleFontWeight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleFontWeight;
    OGUI["SetStyleLineHeight"] = (void(*)(OGUI::VisualElement *, const YGValue &))&OGUI::SetStyleLineHeight;
    OGUI["SetStyleLineHeightPixel"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleLineHeightPixel;
    OGUI["SetStyleLineHeightPercentage"] = (void(*)(OGUI::VisualElement *, float))&OGUI::SetStyleLineHeightPercentage;
    OGUI["SetStyleLineHeightAuto"] = (void(*)(OGUI::VisualElement *))&OGUI::SetStyleLineHeightAuto;
    OGUI["ResetStyleLineHeight"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleLineHeight;
    OGUI["SetStyleTextAlign"] = (void(*)(OGUI::VisualElement *, const OGUI::ETextAlign &))&OGUI::SetStyleTextAlign;
    OGUI["ResetStyleTextAlign"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextAlign;
    OGUI["SetStyleLetterSpacing"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleLetterSpacing;
    OGUI["ResetStyleLetterSpacing"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleLetterSpacing;
    OGUI["SetStyleWordSpacing"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleWordSpacing;
    OGUI["ResetStyleWordSpacing"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleWordSpacing;
    OGUI["ResetStyleTextShadow"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextShadow;
    OGUI["SetStyleTextDecorationColor"] = (void(*)(OGUI::VisualElement *, const OGUI::Vector<float, 4> &))&OGUI::SetStyleTextDecorationColor;
    OGUI["ResetStyleTextDecorationColor"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextDecorationColor;
    OGUI["SetStyleTextDecorationLine"] = (void(*)(OGUI::VisualElement *, const OGUI::ETextDecorationLine &))&OGUI::SetStyleTextDecorationLine;
    OGUI["ResetStyleTextDecorationLine"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextDecorationLine;
    OGUI["SetStyleTextDecorationThickness"] = (void(*)(OGUI::VisualElement *, const float &))&OGUI::SetStyleTextDecorationThickness;
    OGUI["ResetStyleTextDecorationThickness"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextDecorationThickness;
    OGUI["SetStyleTextJustify"] = (void(*)(OGUI::VisualElement *, const OGUI::ETextJustify &))&OGUI::SetStyleTextJustify;
    OGUI["ResetStyleTextJustify"] = (void(*)(OGUI::VisualElement *))&OGUI::ResetStyleTextJustify;
    OGUI.new_enum<int>("ENavMode", {
        {"None", 0},
        {"Horizontal", 1},
        {"Vertical", 2},
        {"Automatic", 3}
    });
    OGUI.new_enum<int>("ENavCycleMode", {
        {"None", 0},
        {"Horizontal", 1},
        {"Vertical", 2},
        {"Automatic", 3}
    });
    OGUI.new_enum<int>("ENavDirection", {
        {"Up", 0},
        {"Down", 1},
        {"Left", 2},
        {"Right", 3}
    });
    OGUI.new_enum<int>("EKeyCode", {
        {"Invalid", 0},
        {"Backspace", 8},
        {"Tab", 9},
        {"Clear", 12},
        {"Enter", 13},
        {"Shift", 16},
        {"Ctrl", 17},
        {"Alt", 18},
        {"Pause", 19},
        {"CapsLock", 20},
        {"Esc", 27},
        {"SpaceBar", 32},
        {"PageUp", 33},
        {"PageDown", 34},
        {"End", 35},
        {"Home", 36},
        {"Left", 37},
        {"Up", 38},
        {"Right", 39},
        {"Down", 40},
        {"Select", 41},
        {"Print", 42},
        {"Execute", 43},
        {"Print_screen", 44},
        {"Insert", 45},
        {"Del", 46},
        {"Help", 47},
        {"Zero", 48},
        {"One", 49},
        {"Two", 50},
        {"Three", 51},
        {"Four", 52},
        {"Five", 53},
        {"Six", 54},
        {"Seven", 55},
        {"Eight", 56},
        {"Nine", 57},
        {"A", 65},
        {"B", 66},
        {"C", 67},
        {"D", 68},
        {"E", 69},
        {"F", 70},
        {"G", 71},
        {"H", 72},
        {"I", 73},
        {"J", 74},
        {"K", 75},
        {"L", 76},
        {"M", 77},
        {"N", 78},
        {"O", 79},
        {"P", 80},
        {"Q", 81},
        {"R", 82},
        {"S", 83},
        {"T", 84},
        {"U", 85},
        {"V", 86},
        {"W", 87},
        {"X", 88},
        {"Y", 89},
        {"Z", 90},
        {"LSystem", 91},
        {"RSystem", 92},
        {"App", 93},
        {"Sleep", 95},
        {"Numpad0", 96},
        {"Numpad1", 97},
        {"Numpad2", 98},
        {"Numpad3", 99},
        {"Numpad4", 100},
        {"Numpad5", 101},
        {"Numpad6", 102},
        {"Numpad7", 103},
        {"Numpad8", 104},
        {"Numpad9", 105},
        {"Multiply", 106},
        {"Add", 107},
        {"Separator", 108},
        {"Subtract", 109},
        {"Decimal", 110},
        {"Divide", 111},
        {"F1", 112},
        {"F2", 113},
        {"F3", 114},
        {"F4", 115},
        {"F5", 116},
        {"F6", 117},
        {"F7", 118},
        {"F8", 119},
        {"F9", 120},
        {"F10", 121},
        {"F11", 122},
        {"F12", 123},
        {"F13", 124},
        {"F14", 125},
        {"F15", 126},
        {"F16", 127},
        {"F17", 128},
        {"F18", 129},
        {"F19", 130},
        {"F20", 131},
        {"F21", 132},
        {"F22", 133},
        {"F23", 134},
        {"F24", 135},
        {"Num_lock", 144},
        {"Scroll_lock", 145},
        {"LShift", 160},
        {"RShift", 161},
        {"LCtrl", 162},
        {"RCtrl", 163},
        {"LAlt", 164},
        {"RAlt", 165},
        {"Semicolon", 186},
        {"Plus", 187},
        {"Comma", 188},
        {"Minus", 189},
        {"Dot", 190},
        {"Slash", 191},
        {"Wave", 192},
        {"LBranket", 219},
        {"Backslash", 220},
        {"RBranket", 221},
        {"Quote", 222},
        {"Gamepad_Left2D", 223},
        {"Gamepad_LeftX", 224},
        {"Gamepad_LeftY", 225},
        {"Gamepad_Right2D", 226},
        {"Gamepad_RightX", 227},
        {"Gamepad_RightY", 228},
        {"Gamepad_LeftTriggerAxis", 229},
        {"Gamepad_RightTriggerAxis", 230},
        {"Gamepad_LeftThumbstick", 231},
        {"Gamepad_RightThumbstick", 232},
        {"Gamepad_Special_Left", 233},
        {"Gamepad_Special_Left_X", 234},
        {"Gamepad_Special_Left_Y", 235},
        {"Gamepad_Special_Right", 236},
        {"Gamepad_FaceButton_Bottom", 237},
        {"Gamepad_FaceButton_Right", 238},
        {"Gamepad_FaceButton_Left", 239},
        {"Gamepad_FaceButton_Top", 240},
        {"Gamepad_LeftShoulder", 241},
        {"Gamepad_RightShoulder", 242},
        {"Gamepad_LeftTrigger", 243},
        {"Gamepad_RightTrigger", 244},
        {"Gamepad_DPad_Up", 245},
        {"Gamepad_DPad_Down", 246},
        {"Gamepad_DPad_Right", 247},
        {"Gamepad_DPad_Left", 248},
        {"Gamepad_LeftStick_Up", 249},
        {"Gamepad_LeftStick_Down", 250},
        {"Gamepad_LeftStick_Right", 251},
        {"Gamepad_LeftStick_Left", 252},
        {"Gamepad_RightStick_Up", 253},
        {"Gamepad_RightStick_Down", 254},
        {"Gamepad_RightStick_Right", 255},
        {"Gamepad_RightStick_Left", 256}
    });
    OGUI.new_enum<int>("EInlineAlign", {
        {"Baseline", 0},
        {"Top", 1},
        {"Middle", 2}
    });
    OGUI.new_enum<int>("EMouseKey", {
        {"None", 0},
        {"LB", 1},
        {"RB", 2},
        {"MB", 4},
        {"X1B", 8},
        {"X2B", 16}
    });
    OGUI.new_enum<int>("EControllerAxis", {
        {"LX", 0},
        {"LY", 1},
        {"RX", 2},
        {"RY", 3},
        {"LT", 4},
        {"RT", 5}
    });
    OGUI.new_enum<int>("EControllerButton", {
        {"LThumb", 0},
        {"RThumb", 1},
        {"LUp", 2},
        {"LDown", 3},
        {"LLeft", 4},
        {"LRight", 5},
        {"RUp", 6},
        {"RDown", 7},
        {"RLeft", 8},
        {"RRight", 9},
        {"LB", 10},
        {"RB", 11},
        {"LT", 12},
        {"RT", 13},
        {"LSpecial", 14},
        {"RSpecial", 15}
    });
    OGUI.new_enum<int>("EControllerVibrationMotor", {
        {"Left", 1},
        {"Right", 2}
    });
    OGUI.new_enum<int>("EMouseCursor", {
        {"Hide", 0},
        {"Arrow", 1},
        {"TextInput", 2},
        {"ResizeAll", 3},
        {"ResizeEW", 4},
        {"ResizeNS", 5},
        {"ResizeNESW", 6},
        {"ResizeNWSE", 7},
        {"Hand", 8},
        {"NotAllowed", 9}
    });
    OGUI.new_enum<int>("EGestureEvent", {
        {"None", 0},
        {"Scroll", 1},
        {"Magnify", 2},
        {"Swipe", 3},
        {"Rotate", 4},
        {"LongPress", 5}
    });
    OGUI.new_enum<int>("EventRoutePhase", {
        {"None", 0},
        {"TrickleDown", 1},
        {"Reach", 2},
        {"Broadcast", 4},
        {"BubbleUp", 8},
        {"All", 15},
        {"NoBroadcast", 11}
    });
    OGUI.new_enum<int>("PseudoStates", {
        {"None", 0},
        {"Active", 1},
        {"Hover", 2},
        {"Checked", 4},
        {"Disabled", 8},
        {"Focus", 16},
        {"KeyboardFocus", 32},
        {"Scroll", 64},
        {"Root", 128}
    });
    OGUI.new_enum<int>("EFocusChangeCause", {
        {"UserActions", 0},
        {"FocusNavigation", 1},
        {"ActivateWindow", 2}
    });
}
