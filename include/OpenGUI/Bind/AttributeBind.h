#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Name.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace OGUI
{
    struct OGUI_API AttrSource : public std::enable_shared_from_this<AttrSource>
    {
        Name name;
        std::type_index type;
        void* data;
        std::vector<struct AttrBind*> binds;
        mutable bool _guard = false;

        void DataChange(bool force = false) const;

        AttrSource(Name name, std::type_index type, void* data);
        template<typename T>
        AttrSource(Name name, T* data) :AttrSource(name, std::type_index(typeid(T)), (void*)data) {};
        ~AttrSource();
    };

    struct OGUI_API AttrBind : public std::enable_shared_from_this<AttrBind>
    {
        using OnChange = std::function<void(const AttrSource&)>;
        using OnChangePost = std::function<void(bool /*是否应用成功（没找到AttrConverter就会false）*/)>;
        using AssignFunc = std::function<void(void* /*dst*/, void* /*src*/)>;

        AttrBind(Name name, OnChange changeFun);
        AttrBind(Name name, std::type_index type, void* data, OnChangePost changePostFun, AssignFunc assignFunc);
        template<typename T>
        AttrBind(Name name, T* data, OnChangePost changePostFun = {})
            : AttrBind(name, std::type_index(typeid(T)), (void*)data, changePostFun, [](void* dst, void* src){ *(T*)dst = *(T*)src; })
        {
            
        };
        ~AttrBind();

        void Sync(const AttrSource& source, bool force = false);
        void Sync();

        Name name;
        OnChange changeFun;
        OnChangePost changePostFun;
        AssignFunc assignFunc;
        std::type_index type;
        void* data;
        AttrSource* source = nullptr;
        AttrSource* bdBind = nullptr;
    };

    struct OGUI_API AttrBag
    {
        std::vector<AttrBind> binds;
        std::unordered_map<Name, AttrSource> sources;
        std::vector<AttrBag*> bindingTo;
        std::vector<AttrBag*> bindingBy;
        void AddBind(AttrBind bind);
        void AddSource(AttrSource src);
        void Notify(Name name, bool force = false);
        void Bind(AttrBag& other);
        void Unbind(AttrBag& other);
        void BuildBD();
        ~AttrBag();
    protected:
        void Clear();
    };

    using AttrConverterFun = std::function<bool(void* source, void* target)>;
    OGUI_API bool RegisterAttrConverter(std::type_index source, std::type_index target, AttrConverterFun converter);
    OGUI_API bool AttrConverter(std::type_index sourceType, void* source, std::type_index targetType, void* target);
    OGUI_API void RegisterBaseAttrConverter();

    template<typename sourceType, typename targetType, class F>
    bool RegisterAttrConverter(F converter)
    {
        return RegisterAttrConverter(typeid(sourceType), typeid(targetType), [converter](void* source, void* out)
        {
            return converter(*(sourceType*)source, *(targetType*)out);
        });
    };

    template<typename sourceType, typename targetType>
    bool AttrConverter(sourceType& source, targetType& target)
    {
        return AttrConverter(typeid(sourceType), (void*)&source, typeid(targetType), (void*)&target);
    };
}