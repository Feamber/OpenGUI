#pragma once
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/Bind/EventArg.h"
namespace OGUI
{
    template<class T>
    void BroadcastEvent(VisualElement* element, T& event)
    {
        element->Traverse([&](VisualElement* next) 
        { 
            if (next->_isPseudoElement)
                return;
            if(auto eventBind = next->GetEventBind(event.GetEventName()))
				Broadcast(*next, *eventBind, event, MakeEventArg("element", next));
            next->_eventHandler.Handle(event);
            BroadcastEvent<T>(next, event); 
        });
    }

    inline void BuildRoutePath(VisualElement* target, std::vector<VisualElement*>& path)
    {
        for (auto parent = target->GetHierachyParent(); parent != nullptr; parent = parent->GetHierachyParent())
            path.push_back(parent);
    }

    inline EventRoutePhase NextPhase(EventRoutePhase current, int mask)
    {
        int value = (int)current;
        mask &= ~(value | (value - 1));
        mask &= -mask;
        return (EventRoutePhase)mask;
    }

    template<class T>
    bool RouteEvent(VisualElement* target, T& event)
    {
        int phaseMask = (int)event.PhaseMask;
        EventRoutePhase& currentPhase = event.currentPhase;
        std::vector<VisualElement*> routePath;
        if (currentPhase == EventRoutePhase::None)
            currentPhase = (EventRoutePhase)(phaseMask & -phaseMask);
        if (currentPhase == EventRoutePhase::TrickleDown)
        {
            BuildRoutePath(target, routePath);
            int count = routePath.size();
            for (int i = count-1; i >= 0; --i)
            {
                auto& element = routePath[i];
                if(auto eventBind = element->GetEventBind(event.GetEventName()))
					Broadcast(*element, *eventBind, event, MakeEventArg("element", element));
                if(element->_eventHandler.Handle(event))
                    return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::Reach)
        {
            if(auto eventBind = target->GetEventBind(event.GetEventName()))
				Broadcast(*target, *eventBind, event, MakeEventArg("element", target));
            if(target->_eventHandler.Handle(event))
                return true;
            if (target->_rerouteEvent)
                if (auto parent = target->GetParent())
                {
                    if(auto eventBind = parent->GetEventBind(event.GetEventName()))
					    Broadcast(*parent, *eventBind, event, MakeEventArg("element", parent));
                    if(parent->_eventHandler.Handle(event))
                        return true;
                }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::Broadcast)
        {
            BroadcastEvent<T>(target, event);
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::BubbleUp)
        {
            if(routePath.size() == 0) BuildRoutePath(target, routePath);
            for(auto parent : routePath)
            {
                if(auto eventBind = parent->GetEventBind(event.GetEventName()))
					Broadcast(*parent, *eventBind, event, MakeEventArg("element", parent));
                if(parent->_eventHandler.Handle(event))
                    return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }

        return false;
    }
}