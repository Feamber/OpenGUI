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

    void AttrSource::DataChange() const
    {
        for(auto bind : binds)
                bind->Sync(*this);
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

    void AttrBind::Sync(const AttrSource& source)
    {
        if(data)
        {
            if(source.type == type)
            {
                assignFunc(data, source.data);
                changePostFun(true);
            }
            else
                changePostFun(AttrConverter(source.type, source.data, type, data));
        }
        else
            changeFun(source);
    }

    void AttrBind::Sync()
    {
        if(source)
            Sync(*source);
    }

    void AttrBag::AddBind(AttrBind bind)
    {
        if(!bindingBy.empty())
        {
            olog::Warn(u"已绑定的数据包不可添加内容"_o);
            return;
        }
        binds.emplace_back(std::move(bind));
    }

    void AttrBag::AddSource(AttrSource src)
    {
        if(!bindingBy.empty())
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

    void AttrBag::Notify(Name name)
    {
         auto iter = sources.find(name);
        if(iter!=sources.end())
            iter->second.DataChange();
    }

    void AttrBag::Bind(AttrBag& other)
    {
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
            auto iter = other.sources.find(bind.name);
            if(iter != other.sources.end())
            {
                auto& ob = iter->second.binds;
                ob.erase(std::remove(ob.begin(), ob.end(), &bind), ob.end());
            }
            bind.source = nullptr;
        }
        bindingTo.erase(std::remove(bindingTo.begin(), bindingTo.end(), &other), bindingTo.end());
        other.bindingBy.erase(std::remove(other.bindingBy.begin(), other.bindingBy.end(), this), other.bindingBy.end());
    }

    void AttrBag::Clear()
    {
        for(auto& bind : binds)
            bind.source = nullptr;
        for(auto& source : sources)
            source.second.binds.clear();
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
            by->bindingTo.erase(std::remove(by->bindingTo.begin(), bindingTo.end(), this), by->bindingTo.end());
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
        RegisterAttrConverter<int, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((int*)source);
                return true;
            });
        RegisterAttrConverter<int, float>([](void* source, void* out)
            {
                *((float*)out) = *((int*)source);
                return true;
            });
        RegisterAttrConverter<int, ostr::string>([](void* source, void* out)
            {
                *((ostr::string*)out) = std::to_string(*((int*)source));
                return true;
            });
        
        RegisterAttrConverter<int32, int>([](void* source, void* out)
            {
                *((int*)out) = *((int32*)source);
                return true;
            });
        RegisterAttrConverter<int32, float>([](void* source, void* out)
            {
                *((float*)out) = *((int32*)source);
                return true;
            });

        RegisterAttrConverter<float, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((float*)source);
                return true;
            });
        RegisterAttrConverter<float, int>([](void* source, void* out)
            {
                *((int*)out) = *((float*)source);
                return true;
            });
        
        RegisterAttrConverter<ostr::string, std::string>([](void* source, void* out)
            {
                auto strv = (*(ostr::string*)source).to_sv();
                *((std::string*)out) = {strv.begin(), strv.end()};
                return true;
            });

        RegisterAttrConverter<std::string, ostr::string>([](void* source, void* out)
            {
                *((ostr::string*)out) = *((std::string*)source);
                return true;
            });
    }
}