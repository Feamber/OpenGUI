#pragma once
#include <any>
#include <string>
#include <functional>
#include <optional>
#include "OpenGUI/Configure.h"
#include "OpenGUI/Bind/any.h"

namespace OGUI reflect
{
    template<class T>
    struct NamedEventArg    
    {
        std::string name;
        T data;
    };

    template<class T>
    bool TryGet(NamedEventArg<T>& v, std::string_view name, OGUI::any& a)
    {
        if(v.name == name)
        {
            a = v.data;
            return true;
        }
        return false;
    }

    struct OGUI_API reflect attr("script":true) 
    IEventArg
    {
        virtual ~IEventArg() {}
        attr("script":true) 
        virtual OGUI::any TryGet(std::string_view name) = 0;
        
        template<class T>
        std::optional<T> TryGet(std::string_view name)
        {
            auto r = TryGet(name);
            if(r.type() == typeid(T))
                return OGUI::any_cast<T>(r);
            else
                return {};
        }
    };
    
    struct OGUI_API EventArgs : public IEventArg
    {
        std::function<OGUI::any(std::string_view)> impl;
        template<class... Ts>
        EventArgs(Ts&&... args)
        {
            impl = [&](std::string_view name) mutable
            {
                OGUI::any r;
                (OGUI::TryGet(args, name, r)||...);
                return r;
            };
        }
        OGUI::any TryGet(std::string_view name) override
        {
            return impl(name);
        }
        template<class T>
        std::optional<T> TryGet(std::string_view name)
        {
            auto r = TryGet(name);
            if(r.type() == typeid(T))
                return OGUI::any_cast<T>(r);
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