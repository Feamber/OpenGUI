#pragma once

#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/AttachEvent.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    class VisualElement;

    struct push_attr("event":true) 
    PreDetachEvent : EventBase
    {
        VisualElement* prevParent;
        static const Name& GetEventName();
    };

    struct PreAttachEvent : EventBase
    {
        VisualElement* prevParent;
        VisualElement* nextParent;
        static const Name& GetEventName();
    };

    struct PostDetachEvent : EventBase
    {
        VisualElement* prevParent;
        static const Name& GetEventName();
    };

    struct PostAttachEvent : EventBase
    {
        VisualElement* prevParent;
        VisualElement* nextParent;
        static const Name& GetEventName();
    };
}