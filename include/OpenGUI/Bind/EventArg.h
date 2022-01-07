#pragma once
#include <any>
#include <string>
#include <functional>
#include <optional>
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/value.h"

namespace OGUI reflect
{
    template<class T>
    struct NamedEventArg    
    {
        ostr::string name;
        T data;
    };

    template<class T>
    bool TryGet(NamedEventArg<T>& v, ostr::string_view name, OGUI::Meta::ValueRef& a)
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
        virtual OGUI::Meta::ValueRef TryGet(ostr::string_view name) = 0;
        
        template<class T>
        std::optional<T> TryGet(ostr::string_view name)
        {
            auto r = TryGet(name);
            if(r && r.Convertible<T>())
                return r.Convert<T>();
            return {};
        }
    };
    
    struct OGUI_API EventArgs : public IEventArg
    {
        std::function<OGUI::Meta::ValueRef(ostr::string_view)> impl;
        template<class... Ts>
        EventArgs(Ts&&... args)
        {
            impl = [&](ostr::string_view name) mutable
            {
                OGUI::Meta::ValueRef r;
                (OGUI::TryGet(args, name, r)||...);
                return r;
            };
        }
        OGUI::Meta::ValueRef TryGet(ostr::string_view name) override
        {
            return impl(name);
        }
        template<class T>
        std::optional<T> TryGet(ostr::string_view name)
        {
            auto r = TryGet(name);
            if(r && r.Convertible<T>())
                return r.Convert<T>();
            return {};
        }
    };

    template<class T>
    auto MakeEventArg(ostr::string name, T value)
    {
        NamedEventArg<T> arg{std::move(name), value};
        return arg;
    }

    #define EARG(var) MakeEventArg(#var, var)
    #define EARG_REF(var) MakeEventArg(#var, std::ref(var))
}