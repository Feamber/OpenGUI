#define DLL_IMPLEMENTATION
#include <algorithm>
#include <memory>
#include "OpenGUI/DataBind/PropertyManager.h"
#include "OpenGUI/Core/ostring/ostr.h"

namespace OGUI
{
    PropertyManager::PropertyManager()
    {
        if constexpr (!std::is_same<int, int32>::value)
        {
            SetTypeConv<int, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((int*)source);
            });

            SetTypeConv<int32, int>([](void* source, void* out)
            {
                *((int*)out) = *((int32*)source);
            });

            SetTypeConv<float, int32>([](void* source, void* out)
            {
                *((int32*)out) = *((float*)source);
            });

            SetTypeConv<int32, float>([](void* source, void* out)
            {
                *((float*)out) = *((int32*)source);
            });
        }
        

        SetTypeConv<int, float>([](void* source, void* out)
        {
            *((float*)out) = *((int*)source);
        });

        SetTypeConv<float, int>([](void* source, void* out)
        {
            *((int*)out) = *((float*)source);
        });

        SetTypeConv<ostr::string, std::string>([](void* source, void* out)
        {
            auto strv = (*(ostr::string*)source).to_sv();
            *((std::string*)out) = {strv.begin(), strv.end()};
        });

        SetTypeConv<std::string, ostr::string>([](void* source, void* out)
        {
            *((ostr::string*)out) = *((std::string*)source);
        });
    };
    
    bool PropertyManager::UnRegisterProperty(PropertyPtr property)
    {
        if(property.expired()) return false;
        auto prope = property.lock();

        PropertyArray* propes;
        if(prope->owner.expired())
            propes = &_propes;
        else 
            propes = &prope->owner.lock()->_propes;
        
        for (auto it = propes->begin(); it != propes->end(); ++it)
        {
            if(*it == prope)
            {
                CleanListenProperty((*it)->GetPropertyPath(), PropeChangeCause::UnRegisterProperty);
                propes->erase(it);
                return true;
            }
        }
        return false;
    };

    bool PropertyManager::UnRegisterProperty(PropertyPath path)
    {
        return UnRegisterProperty(FindProperty(path));
    };

    void PropertyManager::PropertyChange(PropertyPtr property, PropeChangeCause Cause)
    {
        if(!property.expired())
        {
            auto prope = property.lock().get();
            auto find = _listenPropes.find(prope->GetPropertyPath().ToName());
            if(find != _listenPropes.end())
            {
                auto& listenList =  find->second;
                for(auto& listen : listenList)
                {
                    listen.event(Cause, *prope);
                }
            }
        }
    };

     void PropertyManager::PropertyChange(PropertyPath path, PropeChangeCause Cause)
     {
         PropertyChange(FindProperty(path), Cause);
     };

    PropertyPtr PropertyManager::FindProperty(PropertyPath path)
    {
        PropertyArray* propes = &_propes;
        for(int i = 0; i < path.path.size(); ++i)
        {
            auto name = path.path[i];
            auto result = std::find_if(propes->begin(), propes->end(), [&name](std::shared_ptr<Property>& prope)
            {
                return name == prope->name;
            });
            if(result == propes->end()) break;
            if(i == path.path.size() - 1) return *result;

            propes = &(*result)->_propes;
        }
        

        return PropertyPtr();
    };

    ListenHandle PropertyManager::ListenProperty(PropertyPath listenTarget, OnPropChangeEvent event)
    {
        auto& listenList = _listenPropes[listenTarget.ToName()];
        listenList.emplace_back(ListenInfo{_newId, move(event)});
        return  ListenHandle(new int(_newId), [this](int* id)
        {
            UnListenProperty(*id);
        });
        ++_newId;
    };

    void PropertyManager::UnListenProperty(int handleId)
    {
        for(auto& it : _listenPropes)
        {
            auto& listenList = it.second;
            for (auto it2 = listenList.begin(); it2 != listenList.end(); ++it2)
            {
                if(it2->id == handleId)
                {
                    auto find = FindProperty(PropertyPath::Make(it.first.ToStringView()));
                    if(!find.expired())
                        it2->event(PropeChangeCause::UnListenProperty, *find.lock());
                    listenList.erase(it2);
                    return;
                }
            }
        }
    };

    void PropertyManager::CleanListenProperty(PropertyPath path, PropeChangeCause Cause)
    {
        auto find = FindProperty(path);
        if(!find.expired())
        {
            auto& listenList = _listenPropes[path.ToName()];
            for(auto& listen : listenList)
                listen.event(Cause, *find.lock());
        }
    };

    void PropertyManager::CleanListenProperty(PropertyPtr property, PropeChangeCause Cause)
    {
        if(!property.expired())
            CleanListenProperty(property.lock()->GetPropertyPath(), Cause);
    };

    PropertyPath Property::GetPropertyPath() const
    {
        PropertyPath out;
        auto current = this;
        while (current) 
        {
            out << current->name;
            if(current->owner.expired()) 
                break;
            current = current->owner.lock().get();
        }
        return out;
    };

    Name PropertyPath::ToName()
    {
        return ToString();
    };

    ostr::string PropertyPath::ToString()
    {
        ostr::string out;
        for(const auto& name : path)
        {
            if(out.length() > 0)
                out += ".";
            out += name.ToStringView();
        }
        return out;
    };

    PropertyPath PropertyPath::Make(ostr::string str)
    {
        std::vector<ostr::string_view> nameList;
        str.replace_origin(u"$"_o, u""_o);
        str.split(u"."_o, nameList);

        PropertyPath out;
        out.path.reserve(nameList.size());
        for(const auto& name : nameList)
            out << name;
        return out;
    };
};