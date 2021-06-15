#pragma once
#include <string_view>
#include <vector>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{
    class VisualElement;

    struct FocusDataBase
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

    struct PreGotKeyboardFocusEvent : PreKeyboardFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct PreLostKeyboardFocusEvent : PreKeyboardFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct PreGotFocusEvent : PreFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct PreLostFocusEvent : PreFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
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

    struct GotKeyboardFocusEvent : KeyboardFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct LostKeyboardFocusEvent : KeyboardFocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct GotFocusEvent : FocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

    struct LostFocusEvent : FocusData
    {
        EventRoutePhase currentPhase = EventRoutePhase::None;
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
    };

}