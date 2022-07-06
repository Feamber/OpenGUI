#pragma once
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Bind/EventArgWrapper.h"
#include "OpenGUI/Bind/EventArg.h"
#include "OpenGUI/Event/EventName.h"
#include "OpenGUI/rtti.h"
namespace OGUI
{
    template<class T>
    bool SendElementEvent(VisualElement* element, VisualElement* target, T& event)
    {
        bool handled = false;
        static Name rawEvent("RawEvent");
        if(auto eventBind = element->GetEventBind(GetEventName<T>()))
            handled |= SendEvent(*element, *eventBind, event, MakeEventArg("element", element), MakeEventArg("target", target));
        handled |= SendEvent(*element, rawEvent, event, MakeEventArg("element", element), MakeEventArg("target", target));
        return handled;
    }

    template<class T>
    bool BroadcastEvent(VisualElement* element, T& event)
    {
        bool ghandled = false;
        element->Traverse([&](VisualElement* next) 
        { 
            if (next->_isPseudoElement)
                return;
            bool handled = false;
            handled |= SendElementEvent(next, element, event);
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
                handled |= SendElementEvent(element, target, event);
                handled |= element->_eventHandler.Handle(event);
                if(handled)
                    return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }
        if (currentPhase == EventRoutePhase::Reach)
        {
            bool handled = false;
            
            handled |= SendElementEvent(target, target, event);
            handled |= target->_eventHandler.Handle(event);
            if (target->_rerouteEvent)
                if (auto parent = target->GetParent())
                {
                    handled |= SendElementEvent(parent, target, event);
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
                handled |= SendElementEvent(parent, target, event);
                handled |= parent->_eventHandler.Handle(event);
                if(handled)
                        return true;
            }
            currentPhase = NextPhase(currentPhase, phaseMask);
        }

        return result;
    }

	template<class T>
	struct DeferredEvent
	{
		VisualElement* target;
		T event;
	};

	template<class... Ts>
	struct DeferredEvents
	{
		std::tuple<std::vector<DeferredEvent<Ts>>...> queue;
		template<class T>
		void AddEvent(VisualElement* target, T&& event)
		{
			std::get<std::vector<DeferredEvent<std::remove_reference_t<T>>>>(queue).push_back({target, std::move(event)});
		}

		template<class T>
		void Execute(std::vector<T>& queue)
		{
			for(auto& event : queue)
				RouteEvent(event.target, event.event);
			queue.clear();
		}

		template<size_t ...id>
		void Execute(std::index_sequence<id...>)
		{
			std::initializer_list<int> _ = {(Execute(std::get<id>(queue)), 0)...};
		}

		void Execute()
		{
			Execute(std::make_index_sequence<sizeof...(Ts)>());
		}
	};
}