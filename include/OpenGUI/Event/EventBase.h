#pragma once
#include "OpenGUI/Core/OName.h"
#include "OpenGUI/Configure.h"

namespace OGUI full_reflect
{
    enum class attr("script":true, "rtti":true) 
    EventRoutePhase : int
    {
        None = 0,
        TrickleDown = 1,
        Reach = 2,
        Broadcast = 4,
        BubbleUp = 8,
        All = TrickleDown | Reach | Broadcast | BubbleUp,
        NoBroadcast = TrickleDown | Reach | BubbleUp,
    };

    struct attr("event-data":true) 
    EventBase
    {
        static constexpr EventRoutePhase PhaseMask = EventRoutePhase::NoBroadcast;
        EventRoutePhase currentPhase = EventRoutePhase::None;
    };
    
    template<class T>
    const OGUI::Name& GetEventName()
    {
        return GetEventName(type_t<std::remove_cv_t<T>>{});
    }
}