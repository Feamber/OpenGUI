#pragma once
#include <typeindex>
#include "OpenGUI/Core/Containers/vector.hpp"

namespace OGUI
{
    struct EventHandler
    {
        template<class T>
        bool Handle(T& event);

        template<class E, auto F>
        std::enable_if_t<!std::is_member_function_pointer_v<decltype(F)>, void>
        Register();

        template<class E, auto F, class T>
        std::enable_if_t<!std::is_member_function_pointer_v<decltype(F)>, void>
        Register(T& userData);

        template<class E, auto F, class O>
        std::enable_if_t<std::is_member_function_pointer_v<decltype(F)>, void>
        Register(O* owner);

        using Trampoline = bool(*)(void* owner, void* event);

        struct Callback
        {
            Trampoline trampoline = nullptr;
            void* payload = nullptr;
            std::type_index eventType = typeid(void);
        };

        std::vector<Callback> _callbacks;
    };
}

#include "EventHandler.inl"