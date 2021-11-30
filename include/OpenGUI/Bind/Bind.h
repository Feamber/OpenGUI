#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Name.h"
#include "OpenGUI/Bind/EventArg.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <optional>

namespace OGUI reflect
{
    using AttrSync = std::function<void(std::type_index type, void*)>;

    struct OGUI_API AttrSource
    {
        using AccessFunc = std::function<void(const AttrSync&)>;
        Name name;
        std::type_index type;
        const void* data;
        AccessFunc accessFun;
        std::vector<struct AttrBind*> binds;
        mutable bool _guard = false;

        void DataChange(bool force = false) const;
        void PushData(struct AttrBind* target) const;

        AttrSource(Name name, std::type_index type, const void* data);
        AttrSource(Name name, AccessFunc accessor);
        template<typename T>
        AttrSource(Name name, const T* data) :AttrSource(name, std::type_index(typeid(T)), (void*)data) {};
        ~AttrSource();
    };

    struct OGUI_API AttrBind
    {
        using OnChange = std::function<void(const void*)>;
        using OnChangePost = std::function<void(bool /*是否应用成功（没找到AttrConverter就会false）*/)>;
        using AssignFunc = std::function<void(void* /*dst*/, const void* /*src*/)>;

        AttrBind(Name name, std::type_index type, size_t size, OnChange changeFun);
        AttrBind(Name name, std::type_index type, size_t size, void* data, OnChangePost changePostFun, AssignFunc assignFunc);
        template<typename T>
        AttrBind(Name name, T* data, OnChangePost changePostFun = {})
            : AttrBind(name, std::type_index(typeid(T)), sizeof(T), (void*)data, changePostFun, [](void* dst, const void* src){ *(T*)dst = *(const T*)src; })
        {
            
        };
        ~AttrBind();

        void Sync(std::type_index type, const void* data, bool force = false);
        void Sync();

        Name name;
        OnChange changeFun;
        OnChangePost changePostFun;
        AssignFunc assignFunc;
        std::type_index type;
        size_t size;
        void* data;
        AttrSource* source = nullptr;
        AttrSource* bdBind = nullptr;
    };
    using EventHandlerType = std::function<bool(EventArgs&)>;
    using EventBag = std::unordered_map<Name, Name>;
    //TODO: Separate bind descriptor and bind instance, optimize for array
    struct OGUI_API reflect Bindable
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
        ~Bindable();
    protected:
        void Build();
    };

    template <typename ... Args>
    bool Broadcast(Bindable& bindable, Name eventName, Args&&... args)
    {
        bool handled = false;
        bool binded = false;
        for(auto binding : bindable.bindingBy)
        {
            auto find = binding->eventHandlers.equal_range(eventName);
            EventArgs eargs(args...);
            if(find.first == find.second)
                continue;
            binded = true;
            for(auto i = find.first; i!=find.second; ++i)
                handled |= i->second(eargs);
            return handled;
        }
        return !binded || handled;
    };

    using AttrConverterFun = std::function<bool(const void* source, void* target)>;
    OGUI_API bool RegisterAttrConverter(std::type_index source, std::type_index target, AttrConverterFun converter);
    OGUI_API bool AttrConverter(std::type_index sourceType, const void* source, std::type_index targetType, void* target);
    OGUI_API void RegisterBaseAttrConverter();

    template<typename sourceType, typename targetType, class F>
    bool RegisterAttrConverter(F converter)
    {
        return RegisterAttrConverter(typeid(sourceType), typeid(targetType), [converter](const void* source, void* out)
        {
            return converter(*(const sourceType*)source, *(targetType*)out);
        });
    };

    template<typename sourceType, typename targetType>
    bool AttrConverter(const sourceType& source, targetType& target)
    {
        return AttrConverter(typeid(sourceType), (const void*)&source, typeid(targetType), (void*)&target);
    };
}