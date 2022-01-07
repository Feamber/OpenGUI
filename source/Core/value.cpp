#include "OpenGUI/Core/value.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Core/ostring/format.h"
#include "OpenGUI/Core/ostring/helpers.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/ostring/osv.h"
#include "OpenGUI/Core/Utilities/overload.hpp"
#include <memory>
#include <string>

namespace OGUI::Meta
{
    const Type* TypeOf<bool>::Get()
    {
        static BoolType type;
        return &type;
    }
    
    const Type* TypeOf<int32_t>::Get()
    {
        static Int32Type type;
        return &type;
    }
    
    const Type* TypeOf<int64_t>::Get()
    {
        static Int64Type type;
        return &type;
    }
    
    const Type* TypeOf<uint32_t>::Get()
    {
        static UInt32Type type;
        return &type;
    }
    
    const Type* TypeOf<uint64_t>::Get()
    {
        static UInt64Type type;
        return &type;
    }
    
    const Type* TypeOf<float>::Get()
    {
        static Float32Type type;
        return &type;
    }
    
    const Type* TypeOf<double>::Get()
    {
        static Float64Type type;
        return &type;
    }
    
    const Type* TypeOf<ostr::string>::Get()
    {
        static StringType type;
        return &type;
    }
    
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
            case _e: return ((EnumType*)this)->underlyingType->Size();
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        return sizeof(void*);
                    case ReferenceType::Shared:
                        return sizeof(std::shared_ptr<void>);
                }
            }
        }
        return 0;
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
            case _e: return ((EnumType*)this)->underlyingType->Align();
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        return alignof(void*);
                    case ReferenceType::Shared:
                        return alignof(std::shared_ptr<void>);
                }
            }
        }
        return 0;
    }

    ostr::string Type::Name() const
    {
        using namespace EType;
        using namespace ostr::literal;
        switch(type)
        {
            case _b : return u"bool";
            case _i32: return u"int32_t";
            case _i64: return u"int64_t";
            case _u32: return u"uint32_t";
            case _u64: return u"uint64_t";
            case _f32: return u"float";
            case _f64: return u"double";
            case _s: return u"ostr::string";
            case _sv: return u"ostr::string_view";
            case _a:  
            {
                auto& arr = (ArrayType&)(*this);
                return  u"{}[{}]"_o.format(arr.elementType->Name(), (int)arr.num);
            }
            case _da: 
            {
                auto& arr = (DynArrayType&)(*this);
                return  u"std::vector<{}>"_o.format(arr.elementType->Name());
            }
            case _av: 
            {
                auto& arr = (ArrayViewType&)(*this);
                return  u"gsl::span<{}>"_o.format(arr.elementType->Name());
            }
            case _o: return ((RecordType*)this)->name;
            case _e: return ((EnumType*)this)->name;
            case _r: 
            {
                auto& ref = (ReferenceType&)(*this);
                switch (ref.ownership) {
                    case ReferenceType::Observed:
                        return  u"std::shared_ptr<{}>"_o.format(ref.pointee->Name());
                    case ReferenceType::Shared:
                        return ref.pointee->Name() + u" *";
                }
            }
        }
        return u"";
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
            case _o: case _e:
                return this == srcType; //对象类型直接比较地址
            case _r: 
            {
                return ((ReferenceType*)this)->ownership == ((ReferenceType*)srcType)->ownership
                    && ((ReferenceType*)this)->nullable == ((ReferenceType*)srcType)->nullable
                    && ((ReferenceType*)this)->pointee->Same(((ReferenceType*)srcType)->pointee);
            }
        }
        return false;
    } 

    bool Type::Convertible(const Type *srcType, bool format) const
    {
        using namespace EType;
        auto stype = srcType->type;
        gsl::span<size_t> acceptIndices;
        if(Same(srcType))
            return true;
        if(srcType->type == _r)
        {
            auto& sptr = (const ReferenceType&)(*srcType);
            if(!sptr.nullable && Convertible(sptr.pointee))
            {
                return true;
            }
        }
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
            case _i32: case _i64: case _u32: case _u64: 
                if(srcType->type == _e)
                {
                    if(srcType->Size() > Size())
                        return false;
                    else
                        return true;
                }
            case _f32: case _f64:
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
                static size_t accept[] = {_b, _i32, _i64, _u32, _u64, _f32, _f64, _e, _s, _sv};
                acceptIndices = accept;
                break;
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
            case _e:
            {
                switch(srcType->type)
                {
                    case _i32: case _i64: case _u32: case _u64: 
                        return srcType->Size() >= Size();
                    case _s: case _sv:
                        return true;
                    default:
                        return false;
                }
            }
            case _r:
            {
                auto& ptr = (const ReferenceType&)(*this);
                if(stype == _r)
                {
                    auto& sptr = (const ReferenceType&)(*srcType);
                    if(sptr.nullable > ptr.nullable)
                        return false;
                    if(sptr.ownership != ptr.ownership && ptr.ownership != ReferenceType::Observed)
                        return false;
                    if(ptr.pointee->Same(sptr.pointee))
                    {
                        return true;
                    }
                    else if(ptr.pointee->type == _e || sptr.pointee->type == _e)
                    {
                        auto type1 = ptr.pointee->type == _e ? ((EnumType*)ptr.pointee)->underlyingType : ptr.pointee;
                        auto type2 = sptr.pointee->type == _e ? ((EnumType*)sptr.pointee)->underlyingType : sptr.pointee;
                        return type1->type == type2->type;
                    }
                    else if(ptr.pointee->type == _o && sptr.pointee->type == _o)
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
        if(Same(srcType))
        {
            Copy(dst, src);
            return;
        }
        if(!Convertible(srcType, policy != nullptr))
            return;

        if(srcType->type == _r)
        {
            auto& sptr = (const ReferenceType&)(*srcType);
            if(!sptr.nullable && Convertible(sptr.pointee)) 
                Convert(dst, *(void**)src, srcType, policy); //dereference and convert
        }
        
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
                    dstV = (T)*(double*)src; break; 
        #define STR_CONVERT \
                case _s: \
                    FromString(dst, ostr::string_view(*(ostr::string*)src), policy); \
                case _sv: \
                    FromString(dst, ostr::string_view(*(ostr::string_view*)src), policy);
        #define ENUM_CONVERT \
                case _e: \
                { \
                    auto& enm = (const EnumType&)(*this); \
                    switch(enm.underlyingType->type) \
                    { \
                        BASE_CONVERT \
                        default: \
                            break; \
                    } \
                }
        
                    
        switch(type)
        {
            case _b:
            {
                using T = bool;
                auto& dstV = *(T*)dst;
                switch(srcType->type)
                {
                    BASE_CONVERT
                    case _s:  
                        policy->parse(policy, ostr::string_view(*(ostr::string*)src), dst, this); 
                    case _sv: 
                        policy->parse(policy, *(ostr::string_view*)src, dst, this);
                    case _r:
                    {
                        auto& ref = (const ReferenceType&)(*srcType);
                        switch(ref.ownership)
                        {
                            case ReferenceType::Observed:
                            {
                                auto& srcV = *(void**)src;
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
                    STR_CONVERT
                    ENUM_CONVERT
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
                    STR_CONVERT
                    ENUM_CONVERT
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
                    STR_CONVERT
                    ENUM_CONVERT
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
                    STR_CONVERT
                    ENUM_CONVERT
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
                    STR_CONVERT
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
                    STR_CONVERT
                    default:
                        break;
                }
                break;
            }
            case _s:
            {
                auto& dstV = *(ostr::string*)dst;
                switch (srcType->type) {
                    case _sv:
                        dstV = *(ostr::string_view*)src; break;
                    default: 
                        dstV = srcType->ToString(src); break;
                }
                break;
            }
            case _sv:
            {
                auto& dstV = *(ostr::string_view*)dst;
                switch (srcType->type) {
                    case _s:
                        dstV = *(ostr::string*)src; break;
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
            case _e:
            {
                auto& enm = (const EnumType&)(*this);
                switch(enm.underlyingType->type)
                {
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
                    default:
                        break;
                }
                if(srcType->type == _s)
                    enm.FromString(dst, *(ostr::string*)src);
                else if(srcType->type == _sv)
                    enm.FromString(dst, *(ostr::string_view*)src);
                break;
            }
            case _r:
            {
                auto& ptr = (const ReferenceType&)(*this);
                if(srcType->type == _r)
                {
                    auto& sptr = (const ReferenceType&)(*srcType);
                    if(sptr.ownership == ReferenceType::Observed)
                    {
                        auto& srcV = *(void**)src;
                        auto& dstV = *(void**)dst;
                        dstV = srcV;
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
                else if(ptr.ownership == ReferenceType::Observed)
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
        if(policy)
            return policy->format(policy, dst, this);
        else
        { 
            std::u16string string;
            switch(type)
            {
                case _b : ostr::ofmt::to_string((int)*(bool*)dst, u"", string); return string;
                case _i32: ostr::ofmt::to_string((int)*(int32_t*)dst, u"", string); return string;
                case _i64: ostr::ofmt::to_string((int)*(int64_t*)dst, u"", string); return string;
                case _u32: ostr::ofmt::to_string((int)*(uint32_t*)dst, u"", string); return string;
                case _u64: ostr::ofmt::to_string((int)*(uint64_t*)dst, u"", string); return string;
                case _f32: ostr::ofmt::to_string(*(float*)dst, u"", string); return string;
                case _f64: ostr::ofmt::to_string((float)*(double*)dst, u"", string); return string;
                case _e: return ((const EnumType*)this)->ToString(dst);
                default:
                    break;
            }
            return u"";
        }
    }

    void Type::FromString(void *dst, ostr::string_view str, ValueSerializePolicy* policy) const
    {
        using namespace EType;
        if(policy)
            policy->parse(policy, str, dst, this);
        else
        {
            switch(type)
            {
                case _b : *(bool*)dst = str.to_bool();
                case _i32: *(int32_t*)dst = str.to_int();
                case _i64: *(int64_t*)dst = str.to_int();
                case _u32: *(uint32_t*)dst = str.to_int();
                case _u64: *(uint64_t*)dst = str.to_int();
                case _f32: *(float*)dst = str.to_float();
                case _f64: *(double*)dst = str.to_float();
                case _e: ((const EnumType*)this)->FromString(dst, str);
                default:
                    break;
            }
        }
    }

    size_t Hash(bool value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(int32_t value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(int64_t value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(uint32_t value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(uint64_t value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(float value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(double value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }
    size_t Hash(void* value, size_t base)
    {
        return Fnv1a_append_value(base, value);
    }

    template<class T>
    size_t HashImpl(const void* dst, size_t base)
    {
        return Hash(*(T*)dst, base);
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
            case _e:
            {
                auto& enm = (const EnumType&)(*this);
                switch(enm.underlyingType->type)
                {
                    case _i32: 
                        return HashImpl<int32_t>(dst, base);
                    case _i64: 
                        return HashImpl<int64_t>(dst, base);
                    case _u32: 
                        return HashImpl<uint32_t>(dst, base);
                    case _u64: 
                        return HashImpl<uint64_t>(dst, base);
                    default:
                        return 0;
                }
                return 0;
            }
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
                if(obj.nativeMethods.Hash)
                    return obj.nativeMethods.Hash(dst, base);
                else
                    return 0;
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                        case ReferenceType::Observed:
                            return HashImpl<void*>(*(void**)dst, base); break;
                        case ReferenceType::Shared:
                            return HashImpl<void*>((*(std::shared_ptr<void>*)dst).get(), base); break;
                }
            }
        }
        return 0;
    }

    void Type::Destruct(void* address) const
    {
        using namespace EType;
         //destruct owned data
        switch(type)
        {
            case _o:
            {
                auto& obj = ((const RecordType&)(*this));
                if(obj.nativeMethods.dtor)
                    obj.nativeMethods.dtor(address);
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
                if(ptr.ownership == ReferenceType::Shared)
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
            case _e:
            {
                auto& enm = (const EnumType&)(*this);
                switch(enm.underlyingType->type)
                {
                    case _i32: CopyImpl<int32_t>(dst, src); break;
                    case _i64: CopyImpl<int64_t>(dst, src); break;
                    case _u32: CopyImpl<uint32_t>(dst, src); break;
                    case _u64: CopyImpl<uint64_t>(dst, src); break;
                    default:
                        break;
                }
                break;
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        CopyImpl<void*>(dst, src);
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
            case _e:
            {
                auto& enm = (const EnumType&)(*this);
                switch(enm.underlyingType->type)
                {
                    case _i32: MoveImpl<int32_t>(dst, src); break;
                    case _i64: MoveImpl<int64_t>(dst, src); break;
                    case _u32: MoveImpl<uint32_t>(dst, src); break;
                    case _u64: MoveImpl<uint64_t>(dst, src); break;
                    default:
                        break;
                }
                break;
            }
            case _r: 
            {
                switch (((ReferenceType*)this)->ownership) {
                    case ReferenceType::Observed:
                        MoveImpl<void*>(dst, src);
                        break;
                    case ReferenceType::Shared:
                        MoveImpl<std::shared_ptr<void>>(dst, src);
                        break;
                }
                break;
            }
        }
    }

    void Type::Delete()
    {
        using namespace EType;
        switch(type)
        {
            case _b : delete (BoolType*)this;
            case _i32: delete (Int32Type*)this;
            case _i64: delete (Int64Type*)this;
            case _u32: delete (UInt32Type*)this;
            case _u64: delete (UInt64Type*)this;
            case _f32: delete (Float32Type*)this;
            case _f64: delete (Float64Type*)this;
            case _s: delete (StringType*)this;
            case _sv: delete (StringViewType*)this;
            case _a: delete (ArrayType*)this;
            case _da: delete (DynArrayType*)this;
            case _av: delete (ArrayViewType*)this;
            case _o: delete (RecordType*)this;
            case _e: delete (EnumType*)this;
            case _r: delete (ReferenceType*)this;
        }
    }

    bool RecordType::IsBaseOf(const RecordType &other) const
    {
        
        for(auto base = other.base;base;base = base->base)
            if(base == this)
                return true;
        return false;
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
        if(type->Size() < smallSize)
            type->Destruct(&_smallObj[0]);
        else
        {
            type->Destruct(_ptr);
            free(_ptr);
        }
        if(type->temp)
            ((Type*)type)->Delete();
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

    
    void ValueRef::Reset()
    {
        type = nullptr;
        ptr = nullptr;
    }

    size_t ValueRef::Hash() const{
      if (!type)
        return 0;
      return type->Hash(ptr, FNV_offset_basis);
    }

    ostr::string ValueRef::ToString() const{
      if (!type)
        return {};
      return type->ToString(ptr);
    }

    void EmptyDtor(void*){}
} // namespace OGUI::Meta