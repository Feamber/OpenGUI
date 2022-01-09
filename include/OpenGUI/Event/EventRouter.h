#pragma once
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/Bind/EventArg.h"
#include "OpenGUI/Event/EventName.h"
#include "OpenGUI/rtti.h"
namespace OGUI
{
    template<class T>
    bool BroadcastEvent(VisualElement* element, T& event)
    {
        bool ghandled = false;
        element->Traverse([&](VisualElement* next) 
        { 
            if (next->_isPseudoElement)
                return;
            bool handled = false;
            if(auto eventBind = next->GetEventBind(GetEventName<T>()))
				handled |= SendEvent(*next, *eventBind, event, MakeEventArg("element", next));
            handled |= next->_eventHandler.Handle(event);
            ghandled |= handled;
            if(handled)
                return;
            BroadcastEvent<T>(next, event); 
        });
        return ghandled;
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

    //TODO: 换成非模板版本，让脚本也能发送Native Event？
    template<class T>
    bool RouteEvent(VisualElement* target, T& event)
    {
        int phaseMask = (int)event.PhaseMask;
        EventRoutePhase& currentPhase = event.currentPhase;
        std::vector<VisualElement*> routePath;
        bool result = false;
        if (currentPhase == EventRoutePhase::None)
            currentPhase = (EventRoutePhase)(phaseMask & -phaseMask);
        if (currentPhase == EventRoutePhase::TrickleDown)
        {
            BuildRoutePath(target, routePath);
            int count = routePath.size();
            for (int i = count-1; i >= 0; --i)
            {
                auto& element = routePath[i];
                bool handled = false;
                if(auto eventBind = element->GetEventBind(GetEventName<T>()))
					handled |= SendEvent(*element, *eventBind, event, MakeEventArg("element", element));
                handled |= element->_eventHandler.Handle(event);
                if(handled)
                    return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::Reach)
        {
            bool handled = false;
            if(auto eventBind = target->GetEventBind(GetEventName<T>()))
				handled |= SendEvent(*target, *eventBind, event, MakeEventArg("element", target));
            handled |= target->_eventHandler.Handle(event);
            if (target->_rerouteEvent)
                if (auto parent = target->GetParent())
                {
                    if(auto eventBind = parent->GetEventBind(GetEventName<T>()))
					    handled |= SendEvent(*parent, *eventBind, event, MakeEventArg("element", parent));
                    handled |= parent->_eventHandler.Handle(event);
                }
            if(handled)
                    return true;
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::Broadcast)
        {
            result = BroadcastEvent<T>(target, event);
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::BubbleUp)
        {
            if(routePath.size() == 0) BuildRoutePath(target, routePath);
            for(auto parent : routePath)
            {
                bool handled = false;
                if(auto eventBind = parent->GetEventBind(GetEventName<T>()))
					handled |= SendEvent(*parent, *eventBind, event, MakeEventArg("element", parent));
                handled |= parent->_eventHandler.Handle(event);
                if(handled)
                        return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }

        return result;
    }
}