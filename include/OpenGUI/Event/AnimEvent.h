#pragma once
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    struct attr("event-data":true) 
    AnimEventData : public EventBase
    {
        ostr::string animName;
    };

    struct  push_attr("event":true) 
    AnimStartEvent : AnimEventData {};
    struct AnimEndEvent : AnimEventData {};
    struct AnimStopEvent : AnimEventData {};
}