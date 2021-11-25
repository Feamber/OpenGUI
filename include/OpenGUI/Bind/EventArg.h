#pragma once
#include <any>
#include <string>
#include <functional>
#include <optional>
#include "OpenGUI/Configure.h"

namespace OGUI
{
    template<class T>
    struct NamedEventArg    
    {
        std::string name;
        T data;
    };

    template<class T>
    bool TryGet(NamedEventArg<T>& v, std::string_view name, std::any& a)
    {
        if(v.name == name)
        {
            a = v.data;
            return true;
        }
        return false;
    }

    /*
    int a, b;
    EventArgs args(EARG(a), EARG_REF(b))
    g = args.get<int>("a");
    */
    struct OGUI_API EventArgs
    {
        std::function<std::any(std::string_view)> impl;
        template<class... Ts>
        EventArgs(Ts&&... args)
        {
            impl = [&](std::string_view name) mutable
            {
                std::any r;
                (OGUI::TryGet(args, name, r)||...);
                return r;
            };
        }
        std::any TryGet(std::string_view name)
        {
            return impl(name);
        }
        template<class T>
        std::optional<T> TryGet(std::string_view name)
        {
            auto r = TryGet(name);
            if(r.type() == typeid(T))
                return std::any_cast<T>(r);
            else
                return {};
        }
    };

    template<class T>
    auto MakeEventArg(std::string name, T value)
    {
        NamedEventArg<T> arg{std::move(name), value};
        return arg;
    }

    #define EARG(var) MakeEventArg(#var, var)
    #define EARG_REF(var) MakeEventArg(#var, std::ref(var))
}