#pragma once
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Event/EventHandler.h"
#include "OpenGUI/Bind/EventArg.h"
#include <any>
#include <map>
#include <memory>
#include <vector>
#include <typeindex>
#include <functional>

namespace OGUI
{
    namespace EventBind
    {
        OGUI_API std::unordered_map<Name, std::vector<struct Handler*>>& GetAllEventHandler();
        using EventBag = std::unordered_map<Name, Name>;
        using EventHandlerType = std::function<bool(EventArgs&)>;

        struct OGUI_API Handler : public std::enable_shared_from_this<Handler>
        {
            Name eventName;
            EventHandlerType fun;

            ~Handler();
        };

        template <typename ... Args>
        bool Broadcast(Name eventName, Args&&... args)
        {
            auto find = GetAllEventHandler().find(eventName);
            EventArgs eargs(args...);
            if(find != GetAllEventHandler().end())
            {
                bool handled = false;
                for(auto& handler : find->second)
                    handled |= handler->fun(eargs);
                return handled;
            }
            return true;
        };

        OGUI_API std::shared_ptr<Handler> AddHandler(Name eventName, EventHandlerType fun);
    }
}