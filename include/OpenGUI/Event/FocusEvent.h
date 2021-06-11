#pragma once
#include <vector>
#include "OpenGUI/Event/EventBase.h"

namespace OGUI
{
    class VisualElement;

    struct PreFocusData
    {
        const std::vector<VisualElement*>* currentFocusedPath;
        const std::vector<VisualElement*>* newFocusedPath;
    };

    struct PreKeyboardFocusData
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

    struct FocusData
    {
        const std::vector<VisualElement*>* oldFocusedPath;
        const std::vector<VisualElement*>* currentFocusedPath;
    };

    struct KeyboardFocusData
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