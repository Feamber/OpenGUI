#pragma once

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
}