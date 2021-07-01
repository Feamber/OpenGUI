#pragma once
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Interface/Interfaces.h"
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vcruntime_typeinfo.h>
#include <vector>

namespace OGUI
{
    enum class PropeChangeCause
    {
        RegisterProperty,
        UnRegisterProperty,
        PropeChange,
        UnListenProperty,
    };

    struct Property;
    struct PropertyPath;
    using PropertyName = Name;
    using PropertyArray = std::vector<std::shared_ptr<Property>>;
    using PropertyPtr = std::weak_ptr<Property>;
    using OnPropChangeEvent = std::function<void(const PropeChangeCause, Property& /*listenTarget*/)>;
    using TypeConvFun = std::function<void (void*, void*)>;
    using ListenHandle = std::shared_ptr<int>;

    class OGUI_API PropertyManager
    {
    public:
        struct OGUI_API TypeConvInfo
        {
            std::string_view source;
            std::string_view target;
            TypeConvFun Fun;

            TypeConvInfo(std::string_view source, std::string_view target, TypeConvFun& Fun)
                : source(source), target(target), Fun(move(Fun)) {};
        };

        struct OGUI_API ListenInfo
        {
            int id;
            OnPropChangeEvent event;
        };

        PropertyManager();

        template<typename T>
        PropertyPtr RegisterProperty(PropertyPtr owner, T* property, const PropertyName& namee);
        bool UnRegisterProperty(PropertyPtr property);
        bool UnRegisterProperty(PropertyPath path);
        void PropertyChange(PropertyPtr property, PropeChangeCause Cause = PropeChangeCause::PropeChange);
        void PropertyChange(PropertyPath path, PropeChangeCause Cause = PropeChangeCause::PropeChange);

        PropertyPtr FindProperty(PropertyPath path);

        ListenHandle ListenProperty(PropertyPath listenTarget, OnPropChangeEvent event);
        void UnListenProperty(int handleId);
        void CleanListenProperty(PropertyPath path, PropeChangeCause Cause = PropeChangeCause::UnListenProperty);
        void CleanListenProperty(PropertyPtr property, PropeChangeCause Cause = PropeChangeCause::UnListenProperty);

        template<typename Source, typename Target>
        void SetTypeConv(TypeConvFun conversionFun);

        PropertyArray _propes;
        std::unordered_map<Name, std::vector<ListenInfo>> _listenPropes;
        std::vector<TypeConvInfo> _allTypeConversion;
        int _newId = 1;
    };

    struct OGUI_API Property
    {
        PropertyPath GetPropertyPath() const;

        const PropertyPtr owner;
        const PropertyName name;
        std::string_view type;
        void* data;

        PropertyManager* manager;
        PropertyArray _propes;

        Property(const PropertyPtr owner, const PropertyName name, std::string_view type, void* data, PropertyManager* manager)
            : owner(owner), name(name), type(type), data(data), manager(manager) {};

        template<typename T>
        T* TryGetValue();

        template<typename T>
        bool TryTypeConv(T* out);
    };

    struct OGUI_API PropertyPath
    {
        std::vector<PropertyName> path;

        // PropertyName.PropertyName.PropertyName.PropertyName
        Name ToName();
        ostr::string ToString();
        static PropertyPath Make(ostr::string str);

        bool operator==(const PropertyPath& other)
        {
            return path == other.path;
        }

        PropertyPath operator+(const PropertyPath& other)
        {
            PropertyPath newPath;
            newPath.path.reserve(path.size() + other.path.size());
            newPath.path.insert(newPath.path.end(), path.begin(), path.end());
            newPath.path.insert(newPath.path.end(), other.path.begin(), other.path.end());
            return newPath;
        };

        PropertyPath& operator<<(const PropertyPath& other)
        {
            path.reserve(path.size() + other.path.size());
            path.insert(path.end(), other.path.begin(), other.path.end());
            return *this;
        }

        PropertyPath& operator<<(const PropertyName& other)
        {
            path.push_back(other);
            return *this;
        }
    };

    template<typename T>
    PropertyPtr PropertyManager::RegisterProperty(PropertyPtr owner, T* property, const PropertyName& name)
    {
        PropertyArray* propes;
        if(owner.expired())
            propes = &_propes;
        else 
            propes = &owner.lock()->_propes;

        for(auto& prope : *propes)
            if(prope->name == name) return PropertyPtr();
        
        auto newProp = std::make_shared<Property>(owner, name, typeid(T).name(), property, this);
        propes->push_back(newProp);
        PropertyChange(newProp->GetPropertyPath(), PropeChangeCause::RegisterProperty);
        return newProp;
    };

    template<typename Source, typename Target>
    void PropertyManager::SetTypeConv(std::function<void (void* /*Source*/, void* /*out*/)> conversionFun)
    {
        static_assert(!std::is_same<Source, Target>::value);
        for(auto& info : _allTypeConversion)
        {
            if(info.source == typeid(Source).name() && info.target == typeid(Target).name())
            {
                info.Fun = move(conversionFun);
                return;
            }
        }
        _allTypeConversion.emplace_back(typeid(Source).name(), typeid(Target).name(), conversionFun);
    };

    template<typename T>
    T* Property::TryGetValue()
    {
        if(type == typeid(T).name())
            return (T*)data;
        return nullptr;
    };

    template<typename T>
    bool Property::TryTypeConv(T* out)
    {
        T* value = TryGetValue<T>();
        if(value)
        {
            *out = *value;
            return true;
        }

        for(auto& info : manager->_allTypeConversion)
        {
            if(info.source == type && info.target == typeid(T).name())
            {
                info.Fun(data, (void*)out);
                return true;
            }
        }
        return false;
    };
}