#pragma once
#include <string_view>
#include <vector>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    class VisualElement;

	enum class attr("script":true, "rtti":true) 
    EFocusChangeCause
    {
        UserActions,
        FocusNavigation,
        ActivateWindow,
    };

    struct push_attr("event-data":true) 
    FocusDataBase : public EventBase
    {
        EFocusChangeCause cause;
        ostr::string_view causeDescribe;
    };

    struct PreFocusData : public FocusDataBase
    {
        //TODO: 当前 lua 绑定不支持类型擦除的数组，考虑套一个结构体
        const std::vector<VisualElement*>* currentFocusedPath;
        const std::vector<VisualElement*>* newFocusedPath;
    };

    struct PreKeyboardFocusData : public FocusDataBase
    {
        VisualElement* currentFocused = nullptr;
        VisualElement* newFocused = nullptr;
    };

    struct FocusData : public FocusDataBase
    {
        const std::vector<VisualElement*>* oldFocusedPath;
        const std::vector<VisualElement*>* currentFocusedPath;
    };

    struct KeyboardFocusData : public FocusDataBase
    {
        VisualElement* oldFocused = nullptr;
        VisualElement* currentFocused = nullptr;
    };

    struct OGUI_API pop_attr() push_attr("event":true) 
    PreGotKeyboardFocusEvent : PreKeyboardFocusData{};
    struct OGUI_API PreLostKeyboardFocusEvent : PreKeyboardFocusData{};
    struct OGUI_API PreGotFocusEvent : PreFocusData{};
    struct OGUI_API PreLostFocusEvent : PreFocusData {};
    struct OGUI_API GotKeyboardFocusEvent : KeyboardFocusData{};
    struct OGUI_API LostKeyboardFocusEvent : KeyboardFocusData {};
    struct OGUI_API GotFocusEvent : FocusData {};
    struct OGUI_API LostFocusEvent : FocusData {};
}