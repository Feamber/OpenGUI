#include "OpenGUI/Core/value.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/ostring/format.h"
#include "OpenGUI/Core/ostring/helpers.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/Utilities/overload.hpp"
#include <memory>

namespace OGUI::Meta
{
    template<>
    const Type* TypeOf<bool>::Get()
    {
        static BoolType type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<int32_t>::Get()
    {
        static Int32Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<int64_t>::Get()
    {
        static Int64Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<uint32_t>::Get()
    {
        static UInt32Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<uint64_t>::Get()
    {
        static UInt64Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<float>::Get()
    {
        static Float32Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<double>::Get()
    {
        static Float64Type type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<ostr::string>::Get()
    {
        static StringType type;
        return &type;
    }
    
    template<>
    const Type* TypeOf<ostr::string_view>::Get()
    {
        static StringViewType type;
        return &type;
    }

    size_t Type::Size() const
    {
        using namespace EType;
        switch(type)
        {
            case _b : return sizeof(bool);
            case _i32: return sizeof(int32_t);
            case _i64: return sizeof(int64_t);
            case _u32: return sizeof(uint32_t);
            case _u64: return sizeof(uint64_t);
            case _f32: return sizeof(float);
            case _f64: return sizeof(double);
            case _s: return sizeof(ostr::string);
            case _sv: return sizeof(ostr::string_view);
            case _a:  return  ((ArrayType*)this)->size;
            case _da: return sizeof(std::vector<char>);
            case _av: return sizeof(gsl::span<char>);
            case _o: return ((RecordType*)this)->size;
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        return sizeof(void*);
                    case ReferenceType::Owned:
                        return sizeof(std::unique_ptr<void>);
                    case ReferenceType::Shared:
                        return sizeof(std::shared_ptr<void>);
                }
            }
        }
    }

    size_t Type::Align() const
    {
        using namespace EType;
        switch(type)
        {
            case _b : return alignof(bool);
            case _i32: return alignof(int32_t);
            case _i64: return alignof(int64_t);
            case _u32: return alignof(uint32_t);
            case _u64: return alignof(uint64_t);
            case _f32: return alignof(float);
            case _f64: return alignof(double);
            case _s: return alignof(ostr::string);
            case _sv: return alignof(ostr::string_view);
            case _a:  return  ((ArrayType*)this)->size;
            case _da: return alignof(std::vector<char>);
            case _av: return alignof(gsl::span<char>);
            case _o: return ((RecordType*)this)->size;
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        return alignof(void*);
                    case ReferenceType::Owned:
                        return alignof(std::unique_ptr<void>);
                    case ReferenceType::Shared:
                        return alignof(std::shared_ptr<void>);
                }
            }
        }
    }

    bool Type::Same(const Type* srcType) const
    {
        using namespace EType;
        if(type!=srcType->type)
            return false;
        switch(type)
        {
            case _b: case _i32: case _i64: case _u32: case _u64: 
            case _f32: case _f64: case _s: case _sv:
                return true;
            case _a:  
                return ((ArrayType*)this)->elementType->Same(((ArrayType*)srcType)->elementType)
                    && ((ArrayType*)this)->num == ((ArrayType*)srcType)->num;
            case _da: 
                return ((DynArrayType*)this)->elementType->Same(((DynArrayType*)srcType)->elementType);
            case _av: 
                return ((ArrayViewType*)this)->elementType->Same(((ArrayViewType*)srcType)->elementType);
            case _o: 
                return this == srcType; //对象类型直接比较地址
            case _r: 
            {
                return ((ReferenceType*)this)->ownership == ((ReferenceType*)srcType)->ownership
                    && ((ReferenceType*)this)->nullable == ((ReferenceType*)srcType)->nullable
                    && ((ReferenceType*)this)->pointee->Same(((ReferenceType*)srcType)->pointee);
            }
        }
    } 

    bool Type::Convertible(const Type *srcType, bool format) const
    {
        using namespace EType;
        auto stype = srcType->type;
        gsl::span<size_t> acceptIndices;
        switch(type)
        {
            case _b:
            {
                if(format)
                {
                    static size_t accept[] = {_b, _i32, _i64, _u32, _u64, _f32, _f64, _s, _sv, _r};
                    acceptIndices = accept;
                }
                else 
                {
                    static size_t accept[] = {_b, _i32, _i64, _u32, _u64, _f32, _f64, _r};
                    acceptIndices = accept;
                }
                break;
            }
            case _i32: case _i64: case _u32: case _u64: case _f32: case _f64:
            {
                if(format)
                {
                    static size_t accept[] = {_b, _i32, _i64, _u32, _u64, _f32, _f64, _s, _sv};
                    acceptIndices = accept;
                }
                else 
                {
                    static size_t accept[] = {_b, _i32, _i64, _u32, _u64, _f32, _f64};
                    acceptIndices = accept;
                }
                break;
            }
            case _sv:
            {
                static size_t accept[] = {_s, _sv};
                acceptIndices = accept;
                break;
            }
            case _s:
            {
                if(format)
                    return true;
                static size_t accept[] = {_s, _sv};
            }
            case _a:
            {
                if(stype == _a)
                {
                    auto& sarray = (const ArrayType&)(*srcType);
                    auto& array = (const ArrayType&)(*this);
                    if(array.num != sarray.num)
                        return false;
                    if(array.elementType->Convertible(sarray.elementType, format))
                        return true;
                }
                else
                    return false;
            }
            case _da:
            {
                auto& array = (const DynArrayType&)(*this);
                if(stype == _a)
                {
                    auto& sarray = (const ArrayType&)(*srcType);
                    if(array.elementType->Convertible(sarray.elementType, format))
                        return true;
                }
                else if(stype == _da)
                {
                    auto& sarray = (const DynArrayType&)(*srcType);
                    if(array.elementType->Convertible(sarray.elementType, format))
                        return true;
                }
                else if(stype == _av)
                {
                    auto& sarray = (const ArrayViewType&)(*srcType);
                    if(array.elementType->Convertible(sarray.elementType, format))
                        return true;
                }
                else
                    return false;
            }
            case _av:
            {
                static size_t accept[] = {_a, _da, _av};
                acceptIndices = accept;
                break;
            }
            case _o:
            {
                return false;
            }
            case _r:
            {
                auto& ptr = (const ReferenceType&)(*this);
                if(stype == _r)
                {
                    auto& sptr = (const ReferenceType&)(*srcType);
                    if((sptr.ownership != ptr.ownership ||  ptr.ownership == ReferenceType::Owned) && ptr.ownership != ReferenceType::Observed)
                        return false;
                    if(ptr.pointee->type == _o && sptr.pointee->type == _o)
                    {
                        auto& sobj = (const RecordType&)(*sptr.pointee);
                        auto& obj = (const RecordType&)(*ptr.pointee);
                        if(obj.IsBaseOf(sobj))
                            return true;
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else if(ptr.ownership == ReferenceType::Observed && ptr.pointee->Same(srcType))
                    return true;
                else
                    return false;
            }
        };
        if(std::find(acceptIndices.begin(), acceptIndices.end(), stype) != acceptIndices.end())
            return true;
        return false;
    }

    void Type::Convert(void *dst, const void *src, const Type *srcType, struct ValueSerializePolicy* policy) const
    {
        using namespace EType;
        if(!Convertible(srcType, policy != nullptr))
            return;
        
        #define BASE_CONVERT \
                case _b: \
                    dstV = (T)*(bool*)src; break; \
                case _i32: \
                    dstV = (T)*(int32_t*)src; break; \
                case _i64: \
                    dstV = (T)*(int64_t*)src; break; \
                case _u32: \
                    dstV = (T)*(uint32_t*)src; break; \
                case _u64: \
                    dstV = (T)*(uint64_t*)src; break; \
                case _f32: \
                    dstV = (T)*(float*)src; break; \
                case _f64: \
                    dstV = (T)*(double*)src; break; \
                case _s:  \
                    policy->parse(policy, ostr::string_view(*(ostr::string*)src), dst, this); \
                case _sv: \
                    policy->parse(policy, *(ostr::string_view*)src, dst, this);
                    
        switch(type)
        {
            case _b:
            {
                using T = bool;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    case _r:
                    {
                        auto& ref = (const ReferenceType&)type;
                        switch(ref.ownership)
                        {
                            case ReferenceType::Observed:
                            {
                                auto& srcV = *(void**)src;
                                dstV = (bool)srcV;
                                break;
                            }
                            case ReferenceType::Owned:
                            {
                                auto& srcV = *(std::unique_ptr<void>*)src;
                                dstV = (bool)srcV;
                                break;
                            }
                            case ReferenceType::Shared:
                            {
                                auto& srcV = *(std::shared_ptr<void>*)src;
                                dstV = (bool)srcV;
                                break;
                            }
                        }
                    }
                    default:
                        break;
                }
                break;
            }
            case _i32:
            {
                using T = int32_t;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _i64:
            {
                using T = int64_t;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _u32:
            {
                using T = uint32_t;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _u64:
            {
                using T = uint64_t;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _f32:
            {
                using T = float;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _f64:
            {
                using T = double;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _s:
            {
                auto& dstV = *(ostr::string*)dst;
                dstV = srcType->ToString(src);
                break;
            }
            case _sv:
            {
                auto& dstV = *(ostr::string_view*)dst;
                switch (srcType->type) {
                    case _s:
                        dstV = *(ostr::string*)src; break;
                    case _sv:
                        dstV = *(ostr::string_view*)src; break;
                    default: 
                        break;
                }
                break;
            }
            case _a:
            {
                auto& arr = (const ArrayType&)(*this);
                auto element = arr.elementType;
                auto data = (char*)dst;
                auto& sarr = (const ArrayType&)(*srcType);
                auto& selement = sarr.elementType;
                auto sdata = (char*)src;
                auto size = element->Size(), ssize = selement->Size();
                for(int i=0; i<arr.num; ++i)
                    element->Convert(data + i * size, sdata + i * ssize, selement);
                break;
            }
            case _da:
            {  
                auto& arr = (const DynArrayType&)(*this);
                auto element = arr.elementType;
                auto size = element->Size();
                auto num = arr.operations.size(dst);
                switch(srcType->type)
                {
                    case _da:
                    {
                        auto& sarr = (const DynArrayType&)(*srcType);
                        auto& selement = sarr.elementType;
                        auto ssize = selement->Size();
                        auto snum = sarr.operations.size((void*)src);
                        if(num != snum)
                            arr.operations.resize(dst, snum);
                        auto data = (char*)arr.operations.data(dst);
                        auto sdata = (char*)sarr.operations.data((void*)src);
                        for(int i=0; i<snum; ++i)
                            element->Convert(data + i * size, sdata + i * ssize, selement);
                        break;
                    }
                    case _a:
                    {
                        auto& sarr = (const ArrayType&)(*srcType);
                        auto& selement = sarr.elementType;
                        auto ssize = selement->Size();
                        if(num != sarr.num)
                            arr.operations.resize(dst, sarr.num);
                        auto data = (char*)arr.operations.data(dst);
                        auto sdata = (char*)src;
                        for(int i=0; i<sarr.num; ++i)
                            element->Convert(data + i * size, sdata + i * ssize, selement);
                        break;
                    }
                    case _av:
                    {
                        auto& sarr = (const ArrayViewType&)(*srcType);
                        auto& selement = sarr.elementType;
                        auto ssize = selement->Size();
                        auto sv = *(gsl::span<char>*)src;
                        auto snum = sv.size();
                        if(num != snum)
                            arr.operations.resize(dst, snum);
                        auto data = (char*)arr.operations.data(dst);
                        auto sdata = sv.data();
                        for(int i=0; i<snum; ++i)
                            element->Convert(data + i * size, sdata + i * ssize, selement);
                        break;
                    }
                    default:
                        break;
                }
                
                break;
            }
            case _av:
            {
                auto& dstV = *(gsl::span<char>*)dst;
                switch (srcType->type) {
                    case _a:
                        dstV = {(char*)src, ((const ArrayType&)(*srcType)).num}; break;
                    case _da:
                    {
                        auto& sarr = (const DynArrayType&)(*srcType);
                        auto snum = sarr.operations.size((void*)src);
                        auto sdata = (char*)sarr.operations.data((void*)src);
                        dstV = {sdata, snum}; break;
                    }
                    case _av:
                        dstV =  *(gsl::span<char>*)src; break;
                    default: 
                        break;
                }
                break;
            }
            case _r:
            {
                if(srcType->type == _r)
                {
                    auto& ptr = (const ReferenceType&)(*this);
                    auto& sptr = (const ReferenceType&)(*srcType);
                    if(sptr.ownership == ReferenceType::Observed)
                    {
                        auto& srcV = *(void**)src;
                        auto& dstV = *(void**)dst;
                        dstV = srcV;
                    }
                    else if(ptr.ownership == ReferenceType::Owned)
                    {
                        auto& srcV = *(std::unique_ptr<void>*)src;
                        auto& dstV = *(void**)dst;
                        dstV = srcV.get();
                    }
                    else 
                    {
                        auto& srcV = *(std::shared_ptr<void>*)src;
                        if(ptr.ownership == ReferenceType::Observed)
                        {
                            auto& dstV = *(void**)dst;
                            dstV = srcV.get();
                        }
                        else if(ptr.ownership == ReferenceType::Shared)
                        {
                            auto& dstV = *(std::shared_ptr<void>*)dst;
                            dstV = srcV;
                        }
                    }
                }
                else
                {
                    auto& dstV = *(void**)dst;
                    dstV = (void*)src;
                }
            }
            default:
                break;
        }
    }

    ostr::string Type::ToString(const void* dst, ValueSerializePolicy* policy) const
    {
        using namespace EType;
        return policy->format(policy, dst, this);
    }

    size_t HashAppend(size_t base, size_t hash)
    {
        base ^= hash;
        base *= FNV_prime;
        return base;
    }

    template<class T>
    size_t HashImpl(const void* dst, size_t base)
    {
        return HashAppend(base, std::hash<T>{}(*(T*)dst));
    }

    size_t Type::Hash(const void* dst, size_t base) const
    {
        using namespace EType;
        switch(type)
        {
            case _b : 
                return HashImpl<bool>(dst, base);
            case _i32: 
                return HashImpl<int32_t>(dst, base);
            case _i64: 
                return HashImpl<int64_t>(dst, base);
            case _u32: 
                return HashImpl<uint32_t>(dst, base);
            case _u64: 
                return HashImpl<uint64_t>(dst, base);
            case _f32: 
                return HashImpl<float>(dst, base);
            case _f64: 
                return HashImpl<double>(dst, base);
            case _s: 
                return (*(ostr::string*)dst).get_hash();
            case _sv: 
                return (*(ostr::string_view*)dst).get_hash();
             case _a:  
            {
                auto& arr = (const ArrayType&)(*this);
                auto element = arr.elementType;
                auto d = (char*)dst;
                auto size = element->Size();
                for(int i=0; i<arr.num; ++i)
                    base = element->Hash(d + i * size, base);
                return base;
            }
            case _da: 
            {
                auto& arr = (const DynArrayType&)(*this);
                auto element = arr.elementType;
                auto d = (char*)arr.operations.data(dst);
                auto n = arr.operations.size(dst);
                auto size = element->Size();
                for(int i=0; i<n; ++i)
                    base = element->Hash(d + i * size, base);
                return base;
            }
            case _av: 
            {
                auto& arr = (const ArrayViewType&)(*this);
                auto& element = arr.elementType;
                auto size = element->Size();
                auto v = *(gsl::span<char>*)dst;
                auto n = v.size();
                auto d = v.data();
                for(int i=0; i<n; ++i)
                    base = element->Hash(d + i * size, base);
                return base;
            }
            case _o: 
            {
                auto& obj = (const RecordType&)(*this);
                return obj.nativeMethods.Hash(dst, base);
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                        case ReferenceType::Observed:
                            return HashImpl<void*>(*(void**)dst, base); break;
                        case ReferenceType::Owned:
                            return HashImpl<void*>((*(std::unique_ptr<void>*)dst).get(), base); break;
                        case ReferenceType::Shared:
                            return HashImpl<void*>((*(std::shared_ptr<void>*)dst).get(), base); break;
                }
            }
        }
    }

    void Type::Destruct(void* address) const
    {
        using namespace EType;
         //destruct owned data
        switch(type)
        {
            case _o:
            {
                ((const RecordType&)(*this)).nativeMethods.dtor(address);
            }
            case _s:
            {
                ((ostr::string*)address)->~string();
                break;
            }
            case _da:
            { 
                auto& arr = (const DynArrayType&)(*this);
                arr.operations.dtor(address);
                break;
            }
            case _a:
            {
                auto& arr = (const ArrayType&)(*this);
                auto element = arr.elementType;
                auto data = (char*)address;
                auto size = element->Size();
                for(int i=0; i<arr.num; ++i)
                    element->Destruct(data + i * size);
                break;
            }
            case _r:
            {
                auto& ptr = (const ReferenceType&)(*this);
                if(ptr.ownership == ReferenceType::Owned)
                {
                    void* pointee = (void**)address;
                    ptr.pointee->Destruct(pointee);
                    free(pointee);
                }
                else if(ptr.ownership == ReferenceType::Shared)
                {
                    ((std::shared_ptr<void>*)address)->~shared_ptr();
                }
            }
            default:
                break;
        }
    }

    template<class T>
    void CopyImpl(void* dst, const void* src)
    {
        *(T*)dst = *(const T*)src;
    }

    void Type::Copy(void* dst, const void* src) const
    {
        using namespace EType;
        switch(type)
        {
            case _b : CopyImpl<bool>(dst, src); break;
            case _i32: CopyImpl<int32_t>(dst, src); break;
            case _i64: CopyImpl<int64_t>(dst, src); break;
            case _u32: CopyImpl<uint32_t>(dst, src); break;
            case _u64: CopyImpl<uint64_t>(dst, src); break;
            case _f32: CopyImpl<float>(dst, src); break;
            case _f64: CopyImpl<double>(dst, src); break;
            case _s: CopyImpl<ostr::string>(dst, src); break;
            case _sv: CopyImpl<ostr::string_view>(dst, src); break;
            case _a:  
            {
                auto& arr = (const ArrayType&)(*this);
                auto element = arr.elementType;
                auto data = (char*)dst;
                auto sdata = (char*)src;
                auto size = element->Size();
                for(int i=0; i<arr.num; ++i)
                    element->Copy(data + i * size, sdata + i * size);
                break;
            }
            case _da:
            {
                auto& arr = (const DynArrayType&)(*this);
                arr.operations.dtor(dst);
                arr.operations.copy(dst, src);
            }
            case _av: CopyImpl<gsl::span<char>>(dst, src); break;
            case _o: 
            {
                auto& obj = (const RecordType&)(*this);
                obj.nativeMethods.dtor(dst);
                obj.nativeMethods.copy(dst, src);
                break;
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        CopyImpl<void*>(dst, src);
                        break;
                    case ReferenceType::Owned:
                        //TODO throw
                        break;
                    case ReferenceType::Shared:
                        CopyImpl<std::shared_ptr<void>>(dst, src);
                        break;
                }
                break;
            }
        }
    }
    
    template<class T>
    void MoveImpl(void* dst, void* src)
    {
        *(T*)dst = std::move(*(T*)src);
    }

    void Type::Move(void* dst, void* src) const
    {
        using namespace EType;
        switch(type)
        {
            case _b : MoveImpl<bool>(dst, src); break;
            case _i32: MoveImpl<int32_t>(dst, src); break;
            case _i64: MoveImpl<int64_t>(dst, src); break;
            case _u32: MoveImpl<uint32_t>(dst, src); break;
            case _u64: MoveImpl<uint64_t>(dst, src); break;
            case _f32: MoveImpl<float>(dst, src); break;
            case _f64: MoveImpl<double>(dst, src); break;
            case _s: MoveImpl<ostr::string>(dst, src); break;
            case _sv: MoveImpl<ostr::string_view>(dst, src); break;
            case _a:  
            {
                auto& arr = (const ArrayType&)(*this);
                auto element = arr.elementType;
                auto data = (char*)dst;
                auto sdata = (char*)src;
                auto size = element->Size();
                for(int i=0; i<arr.num; ++i)
                    element->Move(data + i * size, sdata + i * size);
                break;
            }
            case _da:
            {
                auto& arr = (const DynArrayType&)(*this);
                arr.operations.copy(dst, src);
                break;
            }
            case _av: MoveImpl<gsl::span<char>>(dst, src); break;
            case _o: 
            {
                auto& obj = (const RecordType&)(*this);
                obj.nativeMethods.move(dst, src);
                break;
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        MoveImpl<void*>(dst, src);
                        break;
                    case ReferenceType::Owned:
                        MoveImpl<std::unique_ptr<void>>(dst, src);
                        break;
                    case ReferenceType::Shared:
                        MoveImpl<std::shared_ptr<void>>(dst, src);
                        break;
                }
                break;
            }
        }
    }

    Value::Value()
        :type(nullptr)
    {}

    Value::Value(const Value& other)
    {
        _Copy(other);
    }

    Value::Value(Value&& other)
    {
        _Move(std::move(other));
    }

    Value& Value::operator=(const Value& other)
    {
        Reset();
        _Copy(other);
        return *this;
    }

    Value& Value::operator=(Value&& other)
    {
        Reset();
        _Move(std::move(other));
        return *this;
    }

    void* Value::Ptr()
    {
        if(!type)
            return nullptr;
        if(type->Size() < smallSize)
            return &_smallObj[0];
        else
            return _ptr;
    }

    const void* Value::Ptr() const
    {
        if(!type)
            return nullptr;
        if(type->Size() < smallSize)
            return &_smallObj[0];
        else
            return _ptr;
    }

    void Value::Reset()
    {
        if(!type)
            return;
        void* ptr;
        if(type->Size() < smallSize)
            ptr = &_smallObj[0];
        else
            ptr = _ptr;
        type->Destruct(ptr);
        type = nullptr;
    }

    size_t Value::Hash() const{
      if (!type)
        return 0;
      return type->Hash(Ptr(), FNV_offset_basis);
    }

    ostr::string Value::ToString() const{
      if (!type)
        return {};
      return type->ToString(Ptr());
    }

    void* Value::_Alloc()
    {
        if(!type)
            return nullptr;
        auto size = type->Size();
        if(size < smallSize)
            return &_smallObj[0];
        else
            return _ptr = malloc(size);
    }

    void Value::_Copy(const Value& other)
    {
        type = other.type;
        auto ptr = _Alloc();
        type->Copy(ptr, other.Ptr());
    }
    
    void Value::_Move(Value&& other)
    {
        type = other.type;
        auto ptr = _Alloc();
        type->Move(ptr, other.Ptr());
        other.Reset();
    }
} // namespace OGUI::Meta