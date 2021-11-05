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
        bool isValid; // 有重名的就会注册失败
        Name fullName;
        std::type_index type;
        void* data;

        void DataChange() const;

        AttrSource(Name fullName, std::type_index type, void* data);
        ~AttrSource();
    };

    struct OGUI_API AttrBind : public std::enable_shared_from_this<AttrBind>
    {
        using OnChange = std::function<void(const AttrSource&)>;
        using OnChangePost = std::function<void(bool /*是否应用成功（没找到AttrConverter就会false）*/)>;

        AttrBind(Name fullName, OnChange changeFun);
        AttrBind(Name fullName, std::type_index type, void* data, OnChangePost changePostFun);
        ~AttrBind();

        Name fullName;
        OnChange changeFun;
        OnChangePost changePostFun;
        std::type_index type;
        void* data;
    };

    using AttrConverterFun = std::function<bool(void* source, void* target)>;
    OGUI_API bool RegisterAttrConverter(std::type_index source, std::type_index target, AttrConverterFun converter);
    OGUI_API bool AttrConverter(std::type_index sourceType, void* source, std::type_index targetType, void* target);
    OGUI_API void RegisterBaseAttrConverter();

    template<typename sourceType, typename targetType>
    bool RegisterAttrConverter(AttrConverterFun converter)
    {
        return RegisterAttrConverter(typeid(sourceType), typeid(targetType), converter);
    };

    template<typename sourceType, typename targetType>
    bool AttrConverter(sourceType& source, targetType& target)
    {
        return AttrConverter(typeid(sourceType), (void*)&source, typeid(targetType), (void*)&target);
    };
}