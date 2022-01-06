#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/OName.h"
#include "OpenGUI/Bind/EventArg.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <optional>

namespace OGUI reflect
{
    using AttrSync = std::function<void(Meta::ValueRef ref)>;

    struct OGUI_API AttrSource
    {
        using AccessFunc = std::function<void(const AttrSync&)>;
        Name name;
        Meta::ValueRef ref;
        AccessFunc accessFun;
        std::vector<struct AttrBind*> binds;
        mutable bool _guard = false;

        void DataChange(bool force = false) const;
        void PushData(struct AttrBind* target) const;

        AttrSource(Name name, Meta::ValueRef ref);
        AttrSource(Name name, AccessFunc accessor);
        ~AttrSource();
    };

    struct OGUI_API AttrBind
    {
        using OnChange = std::function<void(Meta::ValueRef ref)>;
        using OnChangePost = std::function<void(bool /*是否应用成功（没找到AttrConverter就会false）*/)>;
        using AssignFunc = std::function<void(void* /*dst*/, const void* /*src*/)>;

        AttrBind(Name name, OnChange changeFun);
        AttrBind(Name name, Meta::ValueRef ref, OnChangePost changePostFun = {});
        ~AttrBind();

        void Sync(Meta::ValueRef ref, bool force = false);
        void Sync();

        Name name;
        Meta::ValueRef ref;
        OnChange changeFun;
        OnChangePost changePostFun;
        AttrSource* source = nullptr;
        AttrSource* bdBind = nullptr;
    };
    using EventHandlerType = std::function<bool(IEventArg&)>;
    using EventBag = std::unordered_map<Name, Name>;
    //TODO: Separate bind descriptor and bind instance, optimize for array
    struct OGUI_API reflect attr("rtti":true)
    Bindable
    {
        std::vector<AttrBind> binds;
        std::unordered_map<Name, AttrSource> sources;
        std::vector<Bindable*> bindingTo;
        std::vector<Bindable*> bindingBy;
        std::unordered_multimap<Name, EventHandlerType> eventHandlers;
        bool _builded = false;
        void AddBind(AttrBind bind);
        AttrSource* GetSource(Name name);
        void AddSource(AttrSource src);
        void Notify(Name name, bool force = false);
        attr("script":true)
        void Bind(Bindable& other);
        attr("script":true)
        void Unbind(Bindable& other);
        void AddEventBind(Name eventName, EventHandlerType fun);
        virtual bool HandleEvent(Name eventName, IEventArg& args);
        virtual ~Bindable();
    protected:
        void Build();
    };

    //向 bindable 发送消息
    template <typename ... Args>
    bool SendEventTo(Bindable& bindable, Name eventName, Args&&... args)
    {
        EventArgs eargs(args...);
        return bindable.HandleEvent(eventName, eargs);
    };

    //向绑定到 bindable 的对象发送消息
    template <typename ... Args>
    bool SendEvent(Bindable& bindable, Name eventName, Args&&... args)
    {
        bool handled = false;
        for(auto binding : bindable.bindingBy)
            handled |= SendEventTo(*binding, eventName, std::forward<Args>(args)...);
        return handled;
    };
}