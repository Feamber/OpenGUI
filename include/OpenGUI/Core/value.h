#pragma once
#include <variant>
#include <string>
#include <cstdint>
#include "OpenGUI/Configure.h"

namespace OGUI
{
    struct BoolType {};
    struct IntType {};
    struct FloatType {};
    struct StringType {};
    struct ObjectType 
    {
        const std::string_view name;
        const ObjectType* base;
        struct Field* fields;
    };
    struct ArrayType
    {
        uint32_t fixed : 1;
        uint32_t size : 31;
        struct Type* elementType;
    };
    struct Type
    {
        std::variant<BoolType, IntType, FloatType, StringType, ObjectType, ArrayType> v;
    };

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
            std::string _str;
            void* _obj;
            uint8_t _smallObj[40];
        };
    };

    template<class T>
    OGUI_API const Type* GetType();
}