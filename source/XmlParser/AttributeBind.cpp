#define DLL_IMPLEMENTATION
#include <algorithm>
#include <typeindex>
#include <vector>
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Bind/AttributeBind.h"

namespace OGUI
{
    namespace AttributeBind 
    {
         static std::unordered_map<std::type_index, std::unordered_map<std::type_index, AttrConverterFun>> AllConverter;
    }
    using namespace AttributeBind;

    AttrSource::AttrSource(Name inName, std::type_index type, void* data)
        : name(inName), type(type), data(data)
    {
    }

    AttrSource::~AttrSource()
    {
    }

    void AttrSource::DataChange(bool force) const
    {
        if(_guard)
            return;
        _guard = true;
        for(auto bind : binds)
                bind->Sync(*this, force);
        _guard = false;
    }

    AttrBind::AttrBind(Name inName, OnChange changeFun)
        : name(inName), changeFun(changeFun), changePostFun(), type(typeid(nullptr)), data(nullptr)
    {
    }

    AttrBind::AttrBind(Name inName, std::type_index type, void* data, OnChangePost changePostFun, AssignFunc assignFunc)
        : name(inName), changeFun(), changePostFun(changePostFun), assignFunc(assignFunc), type(type), data(data)
    {
    }

    AttrBind::~AttrBind()
    {
    }

    void AttrBind::Sync(const AttrSource& inSource, bool force)
    {
        if(bdBind && bdBind->_guard && !force)
            return;
        if(data)
        {
            if(inSource.type == type)
            {
                assignFunc(data, inSource.data);
                changePostFun(true);
            }
            else
                changePostFun(AttrConverter(inSource.type, inSource.data, type, data));
        }
        else
            changeFun(inSource);
    }

    void AttrBind::Sync()
    {
        if(source)
            Sync(*source);
    }

    void AttrBag::AddBind(AttrBind bind)
    {
        if(_builded)
        {
            olog::Warn(u"已绑定的数据包不可添加内容"_o);
            return;
        }
        binds.emplace_back(std::move(bind));
    }

    void AttrBag::AddSource(AttrSource src)
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
            sources.try_emplace(src.name, std::move(src));
    }

    void AttrBag::Build()
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

    void AttrBag::Notify(Name name, bool force)
    {
        auto iter = sources.find(name);
        if(iter!=sources.end())
            iter->second.DataChange(force);
    }

    void AttrBag::Bind(AttrBag& other)
    {
        if(!_builded)
            Build();
        if(!other._builded)
            other.Build();
        for(auto& bind : binds)
        {
            auto iter = other.sources.find(bind.name);
            if(iter != other.sources.end())
            {
                iter->second.binds.emplace_back(&bind);
                bind.source = &iter->second;
                bind.Sync(iter->second);
            }
        }
        bindingTo.emplace_back(&other);
        other.bindingBy.emplace_back(this);
    }

    void AttrBag::Unbind(AttrBag& other)
    {
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

    AttrBag::~AttrBag()
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

    bool AttrConverter(std::type_index sourceType, void* source, std::type_index targetType, void* target)
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
        RegisterAttrConverter<int, int32>([](int& source, int32 out)
            {
                out =source;
                return true;
            });
        RegisterAttrConverter<int, float>([](int& source, float& out)
            {
                out =source;
                return true;
            });
        RegisterAttrConverter<int, ostr::string>([](int& source, ostr::string& out)
            {
                out = std::to_string(source);
                return true;
            });
        
        RegisterAttrConverter<int32, int>([](int32& source, int& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<int32, float>([](int32& source, float& out)
            {
                out = source;
                return true;
            });

        RegisterAttrConverter<float, int32>([](float& source, int32& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<float, int>([](float& source, int& out)
            {
                out = source;
                return true;
            });
        RegisterAttrConverter<float, ostr::string>([](float& source, ostr::string& out)
            {
                out = std::to_string(source);
                return true;
            });
        
        RegisterAttrConverter<ostr::string, std::string>([](ostr::string& source, std::string& out)
            {
                auto strv = source.to_sv();
                out = {strv.begin(), strv.end()};
                return true;
            });

        RegisterAttrConverter<std::string, ostr::string>([](std::string& source, ostr::string& out)
            {
                out = source;
                return true;
            });
    }
}