#pragma once
#include <string_view>
#include <vector>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{
    class VisualElement;

    struct FocusDataBase : public EventBase
    {
        FocusChangeCause cause;
        std::string_view causeDescribe;
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

    struct OGUI_API PreGotKeyboardFocusEvent : PreKeyboardFocusData
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