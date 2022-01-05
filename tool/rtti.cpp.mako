//DO NOT MODIFY THIS FILE
//generated from rtti.cpp.mako
#include "OpenGUI/rtti.generated.h"


%for record in db.records:
namespace OGUI::Meta
{
%if record.hashable:
    size_t Hash(const ${record.name}& value, size_t base)
    {

    }
%endif

    const Type* TypeOf<${record.name}>::Get()
    {
        static std::aligned_storage_t<sizeof(${record.name}), alignof(${record.name})> storage;
        static bool entry = false;
        if(entry)
            return std::launder(reinterpret_cast<const Type*>(&storage));
        entry = true;
        size_t size = sizeof(${record.name});
        size_t align = alignof(${record.name});
        ostr::string_view name = u"${record.name}";
    %if record.bases:
        auto base = (const RecordType*)TypeOf<${record.bases[0].name}>::Get();
    %else:
        auto base = (const RecordType*)nullptr;
    %endif
        ObjectMethodTable nativeMethods {
            +[](void* self) { ((${record.name}*)self)->~${record.short_name}(); }, //dtor
            +[](void* self, struct Value* param, size_t nparam) { /*TODO*/ }, //ctor
            GetCopyCtor<${record.name}>(),
            GetMoveCtor<${record.name}>(),
%if record.hashable:
            +[](const void* self, size_t base) { return Hash(*(const ${record.name}*)self, base); }, //hash
%else:
            nullptr, //hash
%endif
        };
    %if record.fields:
        static Field fields[] = {
        %for field in record.fields:
            {TypeOf<${field.type}>::Get(), u"${field.name}", ${field.offset}},
        %endfor
        };
    %else:
        static gsl::span<Field> fields;
    %endif
    %for method in record.methods.values():
    %if record.methods:
        static Field ${method.short_name}Params[] = {
        %for field in method.descs[0].fields:
            { u"${field.name}", TypeOf<${field.type}>::Get(), ${field.offset}},
        %endfor
        };
    %else:
        static gsl::span<Field> ${method.short_name}Params;
    %endif
    %endfor
    %if record.methods:
        static Method methods[] = {
        %for method in record.methods.values():
            {
                u"${method.short_name}", 
            %if method.descs[0].retType == "void":
                nullptr,
            %else:
                TypeOf<${method.descs[0].retType}>::Get(), 
            %endif
                ${method.short_name}Params,
                +[](void* self, struct Value* args, size_t nargs)
                {   
                    Value result = {};
                    if(nargs < ${len(method.descs[0].fields)})
                    {
                        olog::Error(u"not enough arguments provided, calling ${method.short_name}."_o);
                        return result;
                    }
                %for i, field in enumerate(method.descs[0].fields):
                    if(!args[${i}].type->Convertible(TypeOf<${field.type}>::Get()))
                    {
                        olog::Error(u"argument ${field.name} is not compatible, calling ${method.short_name}."_o);
                        return result;
                    }
                %endfor
            %if method.descs[0].retType != "void":
                    result.Emplace<${method.descs[0].retType}>(((${record.name}*)self)->${method.short_name}(${method.descs[0].getCall()}));
            %else:
                    ((${record.name}*)self)->${method.short_name}(${method.descs[0].getCall()});
            %endif
                    return result;
                }
            },
        %endfor
        };
    %else:
        static gsl::span<Method> methods;
    %endif
        new(&storage) RecordType(size, align, name, base, nativeMethods, fields, methods);
        return std::launder(reinterpret_cast<const Type*>(&storage));
    }
}
%endfor

%for enum in db.enums:
namespace OGUI::Meta
{
    const Type* TypeOf<${enum.name}>::Get()
    {
        static EnumType::Enumerator enumerators[] = 
        {
        %for enumerator in enum.enumerators:
            {u"${enumerator.short_name}", ${enumerator.value}},
        %endfor
        };
        static EnumType type{
            TypeOf<std::underlying_type_t<${enum.name}>>::Get(),
            u"${enum.name}", 
            +[](void* self, ostr::string_view str)
            {
                auto& This = *((${enum.name}*)self);
                switchstr(str)
                {
                %for enumerator in enum.enumerators:
                    casestr("${enumerator.short_name}") This = ${enumerator.name}; break;
                %endfor
                    default:
                        olog::Error(u"fail to parse ${enum.name}. input: {}"_o.format(str));
                        break;
                }
            },
            +[](const void* self)
            {
                auto& This = *((const ${enum.name}*)self);
                switch(This)
                {
                %for enumerator in enum.enumerators:
                    case ${enumerator.name}: return ostr::string(u"${enumerator.short_name}");
                %endfor
                }
                return ostr::string(u"${enum.name}::Unknown");
            },
            enumerators
        };
        return &type;
    }
}
%endfor