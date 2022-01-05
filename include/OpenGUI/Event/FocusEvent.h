#pragma once
#include <string_view>
#include <vector>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    class VisualElement;

	enum class attr("script":true, "rtti":true) 
    FocusChangeCause
    {
        UserActions,
        FocusNavigation,
        ActivateWindow,
    };

    struct push_attr("event-data":true) 
    FocusDataBase : public EventBase
    {
        FocusChangeCause cause;
        ostr::string_view causeDescribe;
    };

    struct PreFocusData : public FocusDataBase
    {
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
    PreGotKeyboardFocusEvent : PreKeyboardFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API PreLostKeyboardFocusEvent : PreKeyboardFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API PreGotFocusEvent : PreFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API PreLostFocusEvent : PreFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API GotKeyboardFocusEvent : KeyboardFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API LostKeyboardFocusEvent : KeyboardFocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API GotFocusEvent : FocusData
    {
        static const Name& GetEventName();
    };

    struct OGUI_API LostFocusEvent : FocusData
    {
        static const Name& GetEventName();
    };

}