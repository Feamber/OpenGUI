#pragma once
#include <type_traits>
#include <variant>
#include <string>
#include <cstdint>
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/osv.h"

namespace OGUI
{
    struct BoolType {};
    struct IntType {};
    struct FloatType {};
    struct StringType {};
    struct StringViewType {};
    struct ObjectType 
    {
        const std::string_view name;
        const ObjectType* base;
        struct Field* fields;
        void (*dtor)(void*);
        void (*ctor)(void*);
    };
    struct ArrayType
    {
        uint32_t fixed : 1;
        uint32_t size : 31;
        struct Type* elementType;
    };
    struct ArrayViewType
    {
        uint32_t fixed : 1;
        uint32_t size : 31;
        struct Type* elementType;
    };
    struct Type
    {
        std::variant<BoolType, IntType, FloatType, StringType, StringViewType, ObjectType, ArrayType, ArrayViewType> v;
    };

    template<class T>
    OGUI_API const Type* GetType();

    struct Field
    {
        Type type;
        std::string_view name;
    };

    struct ValueRef
    {
        const Type* type = nullptr;
        void* address = nullptr;
    };

    struct Value
    {
        const Type* type = nullptr;
        union
        {
            bool _bool;
            int64_t _int;
            double _float;
            ostr::string _str;
            ostr::string_view _strV;
            void* _obj;
            uint8_t _smallObj[40];
        };

        operator bool() { return HasValue(); }

        bool HasValue() { return type != nullptr; }

        template<class T, class... Args>
        void Emplace(Args&&... args)
        {
            static_assert(!std::is_pointer_v<T> && !std::is_reference_v<T>, "use value ref instead.");
            if constexpr(std::is_same_v<T, bool>)
                EmplaceBool(std::forward<Args>(args)...);
            else if constexpr(std::is_floating_point_v<T>)
                EmplaceFloat(std::forward<Args>(args)...);
            else if constexpr(std::is_integral_v<T>)
                EmplaceInt(std::forward<Args>(args)...);
            else if constexpr(std::is_same_v<T, ostr::string>)
                EmplaceString({std::forward<Args>(args)...});
            else if constexpr(std::is_same_v<T, ostr::string_view>)
                EmplaceStringView({std::forward<Args>(args)...});
            else
                EmplaceObject<T>(std::forward<Args>(args)...);
        }

        void EmplaceBool(bool value);
        void EmplaceFloat(double value);
        void EmplaceInt(int64_t value);
        void EmplaceString(std::string value);
        void EmplaceStringView(std::string_view value);
        template<class T, class... Args>
        std::enable_if_t<std::is_constructible_v<T, Args...>, void>
        EmplaceObject(Args&&... args);

        template<class T>
        bool Is()
        {
            return GetType<Type>() == type;
        }

        template<class T>
        T& As()
        {
            return *(T*)Ptr();
        }

        void* Ptr();

        size_t Hash();
        ostr::string ToString();

        void Reset();

        ~Value() { Reset(); }
    };
}