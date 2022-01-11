#pragma once
#include <memory>
#include <string_view>
#include <type_traits>
#include <string>
#include <cstdint>
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/value.h"
#include <gsl/span>

namespace OGUI::Meta
{
    struct DynArrayMethodTable
    {
        void (*dtor)(void* self);
        void (*ctor)(void* self);
        void (*copy)(void* self, const void* other);
        void (*move)(void* self, void* other);
        void (*push)(void* self, const void* data);
        void (*insert)(void* self, const void* data, size_t index);
        void (*erase)(void* self, size_t index);
        void (*resize)(void* self, size_t size);
        size_t (*size)(const void* self);
        void* (*get)(const void* self, size_t index);
        void* (*data)(const void* self);
    };
    struct ObjectMethodTable
    {
        void (*dtor)(void* self);
        void (*ctor)(void* self, struct Value* param, size_t nparam);
        void (*copy)(void* self, const void* other);
        void (*move)(void* self, void* other);
        size_t (*Hash)(const void* self, size_t base);
    };
    
    template<class T>
    auto GetCopyCtor()
    {
        void(*copy)(void* self, const void* other) = nullptr;
        if constexpr(std::is_copy_constructible_v<T>)
            copy = +[](void* self, const void* other) { new(self) T(*(const T*)other); };
        return copy;
    }

    template<class T>
    auto GetMoveCtor()
    {
        void (*move)(void* self, void* other) = nullptr;
        if constexpr(std::is_move_constructible_v<T>)
            move = +[](void* self, void* other) { new(self) T(std::move(*(T*)other)); };
        return move;
    }

    namespace EType
    {
        enum TypeEnum : uint8_t {_b, _i32, _i64, _u32, _u64, _f32, _f64, _s, _sv, _a, _da, _av, _o, _e, _r};
    };

    OGUI_API size_t Hash(bool value, size_t base);
    OGUI_API size_t Hash(int32_t value, size_t base);
    OGUI_API size_t Hash(int64_t value, size_t base);
    OGUI_API size_t Hash(uint32_t value, size_t base);
    OGUI_API size_t Hash(uint64_t value, size_t base);
    OGUI_API size_t Hash(float value, size_t base);
    OGUI_API size_t Hash(double value, size_t base);
    OGUI_API size_t Hash(void* value, size_t base);

    struct OGUI_API Type
    {
        EType::TypeEnum type : 7;
        uint8_t temp : 1; //TODO: wtf
        size_t Size() const;
        size_t Align()  const;
        ostr::string Name() const;
        bool Same(const Type* srcType) const;
        bool Convertible(const Type* srcType, bool format = false) const;
        void Convert(void* dst, const void* src, const Type* srcType, struct ValueSerializePolicy* policy = nullptr) const;
        ostr::string ToString(const void* dst, struct ValueSerializePolicy* policy = nullptr) const;
        void FromString(void* dst, ostr::string_view str, struct ValueSerializePolicy* policy = nullptr) const;
        size_t Hash(const void* dst, size_t base) const;
        //lifetime operator
        void Destruct(void* dst) const;
        void Construct(void* dst, struct Value* args, size_t nargs) const;
        void Copy(void* dst, const void* src) const;
        void Move(void* dst, void* src) const;
        Type(EType::TypeEnum type)
            :type(type), temp(false) {}
        void Delete();
    };
    // bool
    struct BoolType : Type 
    {
        BoolType()
            : Type{EType::_b} {}
    };
    // int32_t
    struct Int32Type : Type 
    {
        Int32Type()
            : Type{EType::_i32} {}
    };
    // int64_t
    struct Int64Type : Type 
    {
        Int64Type()
            : Type{EType::_i64} {}
    };
    // uint32_t
    struct UInt32Type : Type 
    {
        UInt32Type()
            : Type{EType::_u32} {}
    };
    // uint64_t
    struct UInt64Type : Type 
    {
        UInt64Type()
            : Type{EType::_u64} {}
    };
    // float
    struct Float32Type : Type 
    {
        Float32Type()
            : Type{EType::_f32} {}
    };
    // double
    struct Float64Type : Type 
    {
        Float64Type()
            : Type{EType::_f64} {}
    };
    // ostr::string
    struct StringType : Type 
    {
        StringType()
            : Type{EType::_s} {}
    };
    // ostr::string_view
    struct StringViewType : Type 
    {
        StringViewType()
            : Type{EType::_sv} {}
    };
    // T[]
    struct ArrayType : Type
    {
        const struct Type* elementType;
        size_t num;
        size_t size;
        ArrayType(const struct Type* elementType, size_t num, size_t size)
            : Type{EType::_a}, elementType(elementType), num(num), size(size) {}
    }; 
    // std::vector<T>
    struct DynArrayType : Type
    {
        const struct Type* elementType;
        DynArrayMethodTable operations;
        DynArrayType(const Type* elementType, DynArrayMethodTable operations)
            : Type{EType::_da}, elementType(elementType), operations(operations) {}
    };
    // gsl::span<T>
    struct ArrayViewType : Type
    {
        const struct Type* elementType;
        ArrayViewType(const Type* elementType)
            : Type{EType::_av}, elementType(elementType) {}
    };
    // struct/class T
    struct RecordType : Type
    {
        size_t size;
        size_t align;
        const ostr::string_view name;
        const RecordType* base;
        ObjectMethodTable nativeMethods;
        const gsl::span<struct Field> fields;
        const gsl::span<struct Method> methods; 
        bool IsBaseOf(const RecordType& other) const;
        static const RecordType* FromName(ostr::string_view name);
        static void Register(const RecordType* type);
        RecordType(size_t size, size_t align, ostr::string_view name, const RecordType* base, ObjectMethodTable nativeMethods, 
            const gsl::span<struct Field> fields, const gsl::span<struct Method> methods)
            : Type{EType::_o}, size(size), align(align), name(name), base(base), nativeMethods(nativeMethods), 
                fields(fields), methods(methods)
        {}
    };
    // enum T
    struct EnumType : Type
    {
        const Type* underlyingType;
        const ostr::string_view name;
        void (*FromString)(void* self, ostr::string_view str);
        ostr::string (*ToString)(const void* self);
        struct Enumerator
        {
            const ostr::string_view name;
            int64_t value;
        };
        const gsl::span<Enumerator> enumerators;
        static const EnumType* FromName(ostr::string_view name);
        static void Register(const EnumType* type);
        EnumType(const Type* underlyingType, const ostr::string_view name, void (*FromString)(void* self, ostr::string_view str),
            ostr::string (*ToString)(const void* self), const gsl::span<Enumerator> enumerators)
            : Type{EType::_e}, underlyingType(underlyingType), name(name), FromString(FromString), ToString(ToString), enumerators(enumerators) {}
    };
    // T*, T&, std::unique_ptr<T>, std::shared_ptr<T>
    struct ReferenceType  : Type
    {
        enum Ownership{
            Observed, Shared
        } ownership;
        bool nullable;
        const struct Type* pointee;
        ReferenceType(Ownership ownership, bool nullable, const Type* pointee)
            : Type{EType::_r}, ownership(ownership), nullable(nullable), pointee(pointee) 
            {}
    };

    template<class T>
    struct TypeOf;

    template<>
    struct OGUI_API TypeOf<bool> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<int32_t> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<int64_t> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<uint32_t> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<uint64_t> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<float> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<double> { static const Type* Get(); };

    template<>
    struct OGUI_API TypeOf<ostr::string> { static const Type* Get(); };
    
    template<>
    struct OGUI_API TypeOf<ostr::string_view> { static const Type* Get(); };

    template<class T>
    struct TypeOf<const T>
    {
        static const Type* Get()
        {
            return TypeOf<T>::Get();
        }
    };

    template<class T>
    struct TypeOf<volatile T>
    {
        static const Type* Get()
        {
            return TypeOf<T>::Get();
        }
    };

    template<class T>
    struct TypeOf<T*>
    {
        static const Type* Get() 
        { 
            static ReferenceType type{ 
                ReferenceType::Observed, 
                true, 
                TypeOf<T>::Get()
            };
            return &type;
        }
    };

    template<class T>
    struct TypeOf<T&>
    {
        static const Type* Get() 
        { 
            static ReferenceType type{
                ReferenceType::Observed, 
                false, 
                TypeOf<T>::Get()
            };
            return &type;
        }
    };

    template<class T>
    struct TypeOf<std::shared_ptr<T>>
    {
        static const Type* Get() 
        { 
            static ReferenceType type{
                ReferenceType::Shared, 
                true, 
                TypeOf<T>::Get()
            };
            return &type;
        }
    };

    template<class T>
    struct TypeOf<std::vector<T>>
    {
        using V = std::vector<T>;
        static const Type* Get() 
        { 
            static DynArrayType type{
                TypeOf<T>::Get(),
                DynArrayMethodTable{
                    +[](void* self) { ((V*)(self))->~vector(); }, //dtor
                    +[](void* self) { new(self) V(); }, //ctor
                    +[](void* self, const void* other) { new(self) V(*((const V*)(other))); }, //copy
                    +[](void* self, void* other) { new(self) V(std::move(*(V*)(other))); }, //move
                    +[](void* self, const void* data) { ((V*)(self))->push_back(*(const T*)data); }, //push
                    +[](void* self, const void* data, size_t index) { ((V*)(self))->insert(((V*)(self))->begin() + index, *(const T*)data); }, //insert
                    +[](void* self, size_t index) { ((V*)(self))->erase(((V*)(self))->begin() + index); }, //erase
                    +[](void* self, size_t size) { ((V*)(self))->resize(size); }, //resize
                    +[](const void* self) {  return ((V*)(self))->size(); }, //size
                    +[](const void* self, size_t index) { return (void*)&((V*)(self))[index]; }, //get
                    +[](const void* self) { return (void*)((V*)(self))->data(); }, //data
                }
            };
            return &type;
        }
    };

    template<class T, size_t num>
    struct TypeOf<T[num]>
    {
        static const Type* Get() 
        { 
            static ArrayType type{ 
                TypeOf<T>::Get(),
                num,
                sizeof(T[num])
            };
            return &type;
        }
    };
    
    template<class T, size_t size>
    struct TypeOf<gsl::span<T, size>>
    {
        static const Type* Get() 
        { 
            static_assert(size == -1, "only dynamic extent is supported.");
            static ArrayViewType type{ 
                TypeOf<T>::Get()
            };
            return &type;
        }
    };

    template<class T>
    struct RecordRegister { RecordRegister(){ RecordType::Register((const RecordType *)TypeOf<T>::Get()); } };
    template<class T>
    struct EnumRegister { EnumRegister(){ EnumType::Register((const EnumType *)TypeOf<T>::Get()); } };
    
    struct OGUI_API alignas(16) Value
    {
        const Type* type;
        union
        {
            void* _ptr;
            uint8_t _smallObj[24];
        };

        static constexpr size_t smallSize = sizeof(_smallObj);

        Value();
        Value(Value&& other);
        Value(const Value& other);

        Value& operator=(Value&& other);
        Value& operator=(const Value& other);

        operator bool() const { return HasValue(); }

        bool HasValue() const { return type != nullptr; }

        template<class T, class... Args>
        std::enable_if_t<!std::is_reference_v<T> && std::is_constructible_v<T, Args...>, void>
        Emplace(Args&&... args)
        {
            Reset();
            type = TypeOf<T>::Get();
            void* ptr = _Alloc();
            new (ptr) T(std::forward<Args>(args)...);
        }

        template<class T, class V>
        std::enable_if_t<std::is_reference_v<T>, void>
        Emplace(const V& v)
        {
            Reset();
            type = TypeOf<T>::Get();
            void* ptr = _Alloc();
            *(std::remove_reference_t<T>**)ptr = &(V&)v;
        }

        template<class T>
        bool Is() const
        {
            return TypeOf<T>::Get() == type;
        }

        template<class T>
        T& As()
        {
            return *(T*)Ptr();
        }

        template<class T>
        bool Convertible() const
        {
            if(!type)
                return false;
            return TypeOf<T>::Get()->Convertible(type);
        }

        template<class T>
        T Convert()
        {
            std::aligned_storage_t<sizeof(T), alignof(T)> storage;
            TypeOf<T>::Get()->Convert(&storage, Ptr(), type);
            return std::move(*std::launder(reinterpret_cast<T*>(&storage)));
        }

        void* Ptr();
        const void* Ptr() const;

        size_t Hash() const;
        ostr::string ToString() const;

        void Reset();

        ~Value() { Reset(); }

    private:
        void* _Alloc();
        void _Copy(const Value& other);
        void _Move(Value&& other);
    };

    struct OGUI_API ValueRef
    {
        void* ptr = nullptr;
        const Type* type = nullptr;
        ValueRef() = default;
        template<class T>
        ValueRef(T& t)
            :ptr(&t), type(TypeOf<T>::Get())
        {}
        ValueRef(Value& v)
            :ptr(v.Ptr()), type(v.type) 
        {}
        ValueRef(ValueRef&& other) = default;
        ValueRef(ValueRef& other) 
            :ptr(other.ptr), type(other.type) {}
        ValueRef(const ValueRef& other) 
            :ptr(other.ptr), type(other.type) {}
        ValueRef& operator=(const ValueRef& other)
        {
            ptr = other.ptr; type = other.type;
            return *this;
        }
        ValueRef& operator=(ValueRef& other)
        {
            ptr = other.ptr; type = other.type;
            return *this;
        }
        ValueRef& operator=(ValueRef&& other) = default;
        template<class T>
        ValueRef& operator=(T& t) 
        { 
            ptr = (void*)&t; type = TypeOf<T>::Get();
            return *this; 
        }
        bool operator==(const ValueRef& other)
        {
            return ptr == other.ptr && type == other.type;
        }
        bool operator!=(const ValueRef& other)
        {
            return !((*this) == other);
        }
        operator bool() const { return HasValue(); }
        bool HasValue() const { return type != nullptr; }
        template<class T>
        bool Is() const
        {
            return TypeOf<T>::Get() == type;
        }
        template<class T>
        T& As()
        {
            return *(T*)ptr;
        }
        template<class T>
        bool Convertible() const
        {
            if(!type)
                return false;
            return TypeOf<T>::Get()->Convertible(type);
        }

        template<class T>
        T Convert()
        {
            std::aligned_storage_t<sizeof(T), alignof(T)> storage;
            TypeOf<T>::Get()->Convert(&storage, ptr, type);
            return std::move(*std::launder(reinterpret_cast<T*>(&storage)));
        }

        size_t Hash() const;
        ostr::string ToString() const;
        void Reset();
        ~ValueRef() { Reset(); }
    };

    struct Field
    {
        ostr::string_view name;
        const Type* type;
        size_t offset;
    };

    struct Method
    {
        ostr::string_view name;
        const Type* retType;
        const Field* parameters;
        Value (*execute)(void* self, Value* args, size_t nargs);
    };

    struct ValueSerializePolicy
    {
        ostr::string (*format)(void* self, const void* data, const struct Type* type);
        void (*parse)(void* self, ostr::string_view str, void* data, const struct Type* type);
    };


    /*
    struct Serializer
    {
        void BeginSerialize();
        ostr::string EndSerialize();
        void BeginDeserialize(ostr::string_view str);
        void EndDeserialize();
        void Raw(ostr::string_view);
        void Bool(bool&);
        void Int32(int32_t&);
        ....
        bool Defined(const RecordType*);
        void Object(void* data, const RecordType*);
        void BeginObject(const RecordType* type); 
        void EndObject();
        void BeginField(const Field* field); 
        void EndField();
        void BeginArray(); void EndArray();
    };
    */
    template<class Serializer>
    struct TValueSerializePolicy : ValueSerializePolicy
    {
        Serializer s;
        TValueSerializePolicy(Serializer s)
            :s(s)
        {
            format = +[](void* self, const void* data, const struct Type* type)
            {
                auto& s = ((TValueSerializePolicy*)self)->s;
                s.BeginSerialize();
                serializeImpl(&s, data, type);
                return s.EndSerialize();
            };
            parse = +[](void* self, ostr::string_view str, void* data, const struct Type* type)
            {
                auto& s = ((TValueSerializePolicy*)self)->s;
                s.BeginDeserialize(str);
                serializeImpl(&s, data, type);
                s.EndSerialize();
            };
        }

        static void serializeImpl(void* self, void* data, const struct Type* type)
        {
            using namespace EType;
            auto& ctx = *(Serializer*)self;
            switch(type->type)
            {
                case _b : 
                    ctx.Bool(*(bool*)data); break;
                case _i32: 
                    ctx.Int32(*(int32_t*)data); break;
                case _i64: 
                    ctx.Int64(*(int64_t*)data); break;
                case _u32: 
                    ctx.UInt32(*(uint32_t*)data); break;
                case _u64: 
                    ctx.UInt64(*(uint64_t*)data); break;
                case _f32: 
                    ctx.Float32(*(float*)data); break;
                case _f64: 
                    ctx.Float64(*(double*)data); break;
                case _s: 
                    ctx.String(*(ostr::string*)data); break;
                case _sv: 
                    ctx.StringView(*(ostr::string_view*)data); break;
                case _a:  
                {
                    ctx.BeginArray();
                    auto& arr = (const ArrayType&)(*type);
                    auto element = arr.elementType;
                    auto d = (char*)data;
                    auto size = element->Size();
                    for(int i=0; i<arr.num; ++i)
                        formatImpl(&ctx, d + i * size, element);
                    ctx.EndArray();
                    break;
                }
                case _da: 
                {
                    ctx.BeginArray();
                    auto& arr = (const DynArrayType&)(*type);
                    auto element = arr.elementType;
                    auto d = (char*)arr.operations.data(data);
                    auto n = arr.operations.size(data);
                    auto size = element->Size();
                    for(int i=0; i<n; ++i)
                        formatImpl(&ctx, d + i * size, element);
                    ctx.EndArray();
                    break;
                }
                case _av: 
                {
                    ctx.BeginArray();
                    auto& arr = (const ArrayViewType&)(*type);
                    auto& element = arr.elementType;
                    auto size = element->Size();
                    auto v = *(gsl::span<char>*)data;
                    auto n = v.size();
                    auto d = v.data();
                    for(int i=0; i<n; ++i)
                        formatImpl(&ctx, d + i * size, element);
                    ctx.EndArray();
                    break;
                }
                case _o: 
                {
                    auto obj = (const RecordType*)type;
                    if(ctx.Defined(obj)) //object can be predefined
                    {
                        ctx.Object((const RecordType*)type, data);
                    }
                    else
                    {
                        ctx.BeginObject(obj);
                        auto d = (char*)data;
                        for(const auto& field : obj->fields)
                        {
                            ctx.BeginField(field);
                            formatImpl(&ctx, d + field.offset, field.type);
                            ctx.EndField();
                        }
                        ctx.EndObject();
                    }
                }
                case _e:
                {
                    auto enm = (const EnumType*)type;
                    ctx.Raw(enm->ToString(data));
                }
                case _r: 
                {
                    void* address;
                    switch (((const ReferenceType*)type)->ownership) {
                        case ReferenceType::Observed:
                            address = *(void**)data; break;
                        case ReferenceType::Shared:
                            address = (*(std::shared_ptr<void>*)data).get(); break;
                    }
                    //TODO: 可选是否递归序列化
                    ctx.Reference(address);
                    break;
                }
            }
        }
    };

    template<class T, class Serializer>
    void Serialize(const std::vector<T>& value, Serializer& s)
    {
        s.BeginArray();
        for(auto& e : value)
            Serialize(e, s);
        s.EndArray();
    }

    template<class T, size_t N, class Serializer>
    void Serialize(T value[N], Serializer& s)
    {
        s.BeginArray();
        for(int i=0; i<N; ++i)
            Serialize(value[i], s);
        s.EndArray();
    }

    template<class T, class Serializer>
    void Serialize(const gsl::span<T>& value, Serializer& s)
    {
        s.BeginArray();
        for(auto& e : value)
            Serialize(e, s);
        s.EndArray();
    }

    template<class T, class Serializer>
    void Serialize(T* value, Serializer& s)
    {
        s.Reference(value);
    }
    
    #define SerializeField(type, field) \
        { \
            Field f{ \
                u###field, \
                TypeOf<decltype(type::field)>::Get(), \
                offsetof(type, field) \
            }; \
            s.BeginField(&f); \
            Serialize(value.field); \
            s.EndField(); \
        }
}