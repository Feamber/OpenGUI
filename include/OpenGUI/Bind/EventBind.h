#pragma once
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Event/EventHandler.h"
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

        struct OGUI_API Handler : public std::enable_shared_from_this<Handler>
        {
            Name eventName;
            std::any fun;

            ~Handler();
        };

        template <typename ... Args>
        void Broadcast(Name eventName, Args... args)
        {
            auto find = GetAllEventHandler().find(eventName);
            if(find != GetAllEventHandler().end())
            {
                for(auto& handler : find->second)
                {
                    if(handler->fun.type() != typeid(std::function<void(Args...)>))
                    {
                        olog::Error(u"EventBind::Broadcast 有参数类型不一样的事件Handler! eventName:{} BroadcastType:{} HandlerType:{}"_o, eventName.ToStringView(), typeid(std::function<void(Args...)>).name(), handler->fun.type().name());
                        continue;;
                    }
                    std::any_cast<std::function<void(Args...)>>(handler->fun)(args...);
                }
            }
        };

        template <typename ... Args>
        std::shared_ptr<Handler> AddHandler(Name eventName, std::function<void(Args...)> fun)
        {
            auto result = GetAllEventHandler().try_emplace(eventName);
            auto& list = result.first->second;
            for(auto& handler : list)
            {
                if(handler->fun.type() != typeid(std::function<void(Args...)>))
                {
                    olog::Error(u"EventBind::AddHandler 有参数类型不一样的事件Handler! eventName:{} AddType:{} HandlerType:{}"_o, eventName.ToStringView(), typeid(std::function<void(Args...)>).name(), handler->fun.type().name());
                    return nullptr;
                }
            }
            std::shared_ptr<Handler> newHandler(new Handler());
            newHandler->eventName = eventName;
            newHandler->fun = fun;
            list.push_back(newHandler.get());
            return newHandler;
        };
    }
}