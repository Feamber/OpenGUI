
#include <algorithm>
#include <typeindex>
#include <vector>
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Bind/Bind.h"

namespace OGUI
{
    namespace AttributeBind 
    {
         static std::unordered_map<std::type_index, std::unordered_map<std::type_index, AttrConverterFun>> AllConverter;
    }
    using namespace AttributeBind;

    AttrSource::AttrSource(Name inName, std::type_index type, const void* data)
        : name(inName), type(type), data(data)
    {
    }

    
    AttrSource::AttrSource(Name inName, AccessFunc accessor)
        : name(inName), type(typeid(nullptr_t)), accessFun(accessor), data(nullptr)
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
        if(data)
        {
            for(auto bind : binds)
                bind->Sync(type, data, force);
        }
        else 
        {
            accessFun([&](std::type_index type, void* d)
            {
                for(auto bind : binds)
                    bind->Sync(type, d, force);
            });
        }
        _guard = false;
    }

    
    void AttrSource::PushData(struct AttrBind* bind) const
    {
        if(_guard)
            return;
        _guard = true;
        if(data)
        {
            bind->Sync(type, data, false);
        }
        else 
        {
            accessFun([&](std::type_index type, void* d)
            {
                bind->Sync(type, d, false);
            });
        }
        _guard = false;
    }

    AttrBind::AttrBind(Name inName, std::type_index type, size_t size, OnChange changeFun)
        : name(inName), changeFun(changeFun), changePostFun(), type(type), size(size), data(nullptr)
    {
    }

    AttrBind::AttrBind(Name inName, std::type_index type, size_t size, void* data, OnChangePost changePostFun, AssignFunc assignFunc)
        : name(inName), changeFun(), changePostFun(changePostFun), assignFunc(assignFunc), type(type), size(size), data(data)
    {
    }

    AttrBind::~AttrBind()
    {
    }

    void AttrBind::Sync(std::type_index inType, const void* inData, bool force)
    {
        if(bdBind && bdBind->_guard && !force)
            return;
        if(data)
        {
            if(inType == type)
            {
                assignFunc(data, inData);
                changePostFun(true);
            }
            else
                changePostFun(AttrConverter(inType, inData, type, data));
        }
        else
        {
            if(inType == type)
            {
                changeFun(inData);
            }
            else 
            {
                char* buffer;
                char staticBuffer[128];
                if(size > sizeof(staticBuffer))
                    buffer = new char[size];
                else
                    buffer = staticBuffer;
                bool succeed = AttrConverter(inType, inData, type, buffer);
                if(succeed)
                    changeFun(buffer);
                if(size > sizeof(staticBuffer))
                    delete buffer;
            }
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

    bool RegisterAttrConverter(std::type_index source, std::type_index target, AttrConverterFun converter)
    {
        auto result = AllConverter.try_emplace(source);
        auto& targetMap = result.first->second;
        auto result2 = targetMap.try_emplace(target, converter);
        return result2.second;
    }

    bool AttrConverter(std::type_index sourceType, const void* source, std::type_index targetType, void* target)
    {
        auto findSource = AllConverter.find(sourceType);
        if(findSource == AllConverter.end())
        {
            olog::Warn(u"AttrConverter error: {} to {}", sourceType.name(), targetType.name());
            return false;
        }
        
        auto& targetMap = findSource->second;
        auto findTarget = targetMap.find(targetType);
        if(findTarget == targetMap.end())
        {
            olog::Warn(u"AttrConverter error: {} to {}", sourceType.name(), targetType.name());
            return false;
        }
        
        return findTarget->second(source, target);
    }

    void RegisterBaseAttrConverter()
    {
        RegisterAttrConverter<int, int32>([](const int& source, int32 out)
            {
                out =source;
                return true;
            });
        RegisterAttrConverter<int, float>([](const int& source, float& out)
            {
                out =source;
                return true;
            });
        RegisterAttrConverter<int, ostr::string>([](const int& source, ostr::string& out)
            {
                out = std::to_string(source);
                return true;
            });
        
        RegisterAttrConverter<int32, int>([](const int32& source, int& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<int32, float>([](const int32& source, float& out)
            {
                out = source;
                return true;
            });

        RegisterAttrConverter<float, int32>([](const float& source, int32& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<float, int>([](const float& source, int& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<double, float>([](const double& source, float& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<float, double>([](const float& source, double& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<double, ostr::string>([](const double& source, ostr::string& out)
            {
                out = std::to_string(source);
                return true;
            });

        RegisterAttrConverter<float, ostr::string>([](const float& source, ostr::string& out)
            {
                out = std::to_string(source);
                return true;
            });
        
        RegisterAttrConverter<ostr::string, std::string>([](const ostr::string& source, std::string& out)
            {
                auto strv = source.to_sv();
                out = {strv.begin(), strv.end()};
                return true;
            });

        RegisterAttrConverter<std::string, ostr::string>([](const std::string& source, ostr::string& out)
            {
                out = source;
                return true;
            });
    }
}