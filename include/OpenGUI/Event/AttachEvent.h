#pragma once

#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventBase.h"

namespace OGUI full_reflect
{
    class VisualElement;

    struct push_attr("event":true) 
    PreDetachEvent : EventBase
    {
        VisualElement* prevParent;
    };

    struct PreAttachEvent : EventBase
    {
        VisualElement* prevParent;
        VisualElement* nextParent;
    };

    struct PostDetachEvent : EventBase
    {
        VisualElement* prevParent;
    };

    struct PostAttachEvent : EventBase
    {
        VisualElement* prevParent;
        VisualElement* nextParent;
    };
}