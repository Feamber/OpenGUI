
#include <algorithm>
#include <typeindex>
#include <vector>
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Bind/Bind.h"

namespace OGUI
{

    AttrSource::AttrSource(Name inName, Meta::ValueRef ref)
        : name(inName), ref(ref)
    {
    }

    
    AttrSource::AttrSource(Name inName, AccessFunc accessor)
        : name(inName), accessFun(accessor)
    {
    }

    AttrSource::~AttrSource()
    {
    }

    void AttrSource::DataChange(bool force) const
    {
        if(_guard && !force)
            return;
        _guard = true;
        if(ref)
        {
            for(auto bind : binds)
                bind->Sync(ref, force);
        }
        else 
        {
            accessFun([&](Meta::ValueRef ref)
            {
                for(auto bind : binds)
                    bind->Sync(ref, force);
            });
        }
        _guard = false;
    }

    
    void AttrSource::PushData(struct AttrBind* bind) const
    {
        if(_guard)
            return;
        _guard = true;
        if(ref)
        {
            bind->Sync(ref, false);
        }
        else 
        {
            accessFun([&](Meta::ValueRef ref)
            {
                bind->Sync(ref, false);
            });
        }
        _guard = false;
    }

    AttrBind::AttrBind(Name inName, OnChange changeFun)
        : name(inName), changeFun(changeFun)
    {
    }

    AttrBind::AttrBind(Name inName, Meta::ValueRef ref, OnChangePost changePostFun)
        : name(inName), ref(ref), changePostFun(changePostFun)
    {
    }

    AttrBind::~AttrBind()
    {
    }

    void AttrBind::Sync(Meta::ValueRef inRef, bool force)
    {
        if(bdBind && bdBind->_guard && !force)
            return;
        if(ref)
        {
            if(ref.type->Convertible(inRef.type))
            {
                ref.type->Destruct(ref.ptr);
                ref.type->Convert(ref.ptr, inRef.ptr, inRef.type);
                changePostFun(true);
            }
            else
                changePostFun(false);
        }
        else
        {
            changeFun(inRef);
        }
        if(bdBind)
            bdBind->DataChange(false);
    }

    void Bindable::AddBind(AttrBind bind)
    {
        if(_builded)
        {
            olog::Warn(u"已绑定的数据包不可添加内容"_o);
            return;
        }
        binds.emplace_back(std::move(bind));
    }

    
    AttrSource* Bindable::GetSource(Name name)
    {
        auto iter = sources.find(name);
        if(iter!=sources.end())
            return &iter->second;
        return nullptr;
    }

    void Bindable::AddSource(AttrSource src)
    {
        if(_builded)
        {
            olog::Warn(u"已绑定的数据包不可添加内容"_o);
            return;
        }
        auto iter = sources.find(src.name);
        if(iter!=sources.end())
            olog::Warn(u"忽略重复注册的AttrSource name:{}"_o, src.name.ToStringView());
        else
        {
            sources.emplace(src.name, std::move(src));
        }
        return;
    }

    void Bindable::Build()
    {
        if(_builded)
            return;
        for(auto& bind : binds)
        {
            auto iter = sources.find(bind.name);
            if(iter != sources.end())
                bind.bdBind = &iter->second;
        }
        _builded = true;
    }

    void Bindable::Notify(Name name, bool force)
    {
        auto iter = sources.find(name);
        if(iter!=sources.end())
            iter->second.DataChange(force);
    }

    void Bindable::Bind(Bindable& other)
    {
        if(!_builded)
            Build();
        if(!other._builded)
            other.Build();
        for(auto& bind : binds)
        {
            auto ptr = other.GetSource(bind.name);
            if(ptr != nullptr)
            {
                ptr->binds.emplace_back(&bind);
                bind.source = ptr;
                ptr->PushData(&bind);
            }
        }
        bindingTo.emplace_back(&other);
        other.bindingBy.emplace_back(this);
    }

    void Bindable::Unbind(Bindable& other)
    {
        if(!_builded)
            return;
        auto iter = std::find(bindingTo.begin(), bindingTo.end(), &other);
        if(iter == bindingTo.end())
            return;
        for(auto& bind : binds)
        {
            auto iter2 = other.sources.find(bind.name);
            if(iter2 != other.sources.end())
            {
                auto& ob = iter2->second.binds;
                ob.erase(std::remove(ob.begin(), ob.end(), &bind), ob.end());
            }
            bind.source = nullptr;
        }
        bindingTo.erase(std::remove(bindingTo.begin(), bindingTo.end(), &other), bindingTo.end());
        other.bindingBy.erase(std::remove(other.bindingBy.begin(), other.bindingBy.end(), this), other.bindingBy.end());
    }

    void Bindable::AddEventBind(Name eventName, EventHandlerType fun)
    {
        eventHandlers.emplace(eventName, fun);
    }

    bool Bindable::HandleEvent(Name eventName, IEventArg& args)
    {
        bool handled = false;
        auto find = eventHandlers.equal_range(eventName);
        if(find.first == find.second)
            return false;
        for(auto i = find.first; i!=find.second; ++i)
            handled |= i->second(args);
        return handled;
    }

    Bindable::~Bindable()
    {
        for(auto to : bindingTo)
        {
            for(auto& bind : binds)
            {
                auto iter = to->sources.find(bind.name);
                if(iter != to->sources.end())
                {
                    auto& ob = iter->second.binds;
                    ob.erase(std::remove(ob.begin(), ob.end(), &bind), ob.end());
                }
            }
            to->bindingBy.erase(std::remove(to->bindingBy.begin(), to->bindingBy.end(), this), to->bindingBy.end());
        }
        for(auto by : bindingBy)
        {
            for(auto& bind : by->binds)
                bind.source = nullptr;
            by->bindingTo.erase(std::remove(by->bindingTo.begin(), by->bindingTo.end(), this), by->bindingTo.end());
        }
    }
}