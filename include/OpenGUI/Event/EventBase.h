#pragma once
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Configure.h"
namespace OGUI
{
    enum class EventRoutePhase : int
    {
        None = 0,
        TrickleDown = 1,
        Reach = 2,
        Broadcast = 4,
        BubbleUp = 8,
        All = TrickleDown | Reach | Broadcast | BubbleUp,
        NoBroadcast = TrickleDown | Reach | BubbleUp,
    };

    struct EventBase
    {
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
        EventRoutePhase currentPhase = EventRoutePhase::None;
    };
}