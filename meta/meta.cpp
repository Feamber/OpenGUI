#include "meta.h"
#include "llvm/Support/JSON.h"

///   json::OStream J(OS);
//    J.arrayBegin();
///   for (const Event &E : Events) {
///     J.objectBegin();
///     J.attribute("timestamp", int64_t(E.Time));
///     J.attributeBegin("participants");
///     for (const Participant &P : E.Participants)
///       J.value(P.toString());
///     J.attributeEnd();
///     J.objectEnd();
///   }
///   J.arrayEnd();
namespace meta {
    void serializeAttr(llvm::json::OStream& J, const std::string& attr)
    {
        J.attributeBegin("attrs");
        J.rawValue("{ " + attr + " }");
        J.attributeEnd();
    }

    void serialize(llvm::json::OStream& J, const Field& P)
    {
        J.objectBegin();
        J.attribute("name", P.name);
        J.attribute("type", P.type);
        serializeAttr(J, P.attrs);
        J.attribute("line", P.line);
        J.objectEnd();
    }

    void serialize(llvm::json::OStream& J, const Function& P)
    {
        J.objectBegin();
        J.attribute("isStatic", P.isStatic);
        J.attribute("name", P.name);
        serializeAttr(J, P.attrs);
        J.attributeBegin("parameters");
        J.arrayBegin();
        for(auto param : P.parameters)
            serialize(J, param);
        J.arrayEnd();
        J.attributeEnd();
        J.attribute("retType", P.retType);
        J.attribute("fileName", P.fileName);
        J.attribute("line", P.line);
        J.objectEnd();
    }
}

std::string meta::serialize(const Database& P)
{
    std::string str;
    llvm::raw_string_ostream output(str);
    llvm::json::OStream J(output);
    J.objectBegin();
    J.attributeBegin("record");
    J.arrayBegin();
    for(auto& record : P.records)
    {
        J.objectBegin();
        J.attribute("name", record.name);
        serializeAttr(J, record.attrs);
        J.attributeBegin("fields");
        J.arrayBegin();
        for(auto field : record.fields)
            serialize(J, field);
        J.arrayEnd();
        J.attributeEnd();
        J.attributeBegin("functions");
        J.arrayBegin();
        for(auto f : record.functions)
            serialize(J, f);
        J.arrayEnd();
        J.attributeEnd();
        J.attribute("fileName", record.fileName);
        J.attribute("line", record.line);
        J.objectEnd();
    }
    J.arrayEnd();
    J.attributeEnd();
    
    J.attributeBegin("functions");
    J.arrayBegin();
    for(auto& f : P.functions)
        serialize(J, f);
    J.arrayEnd();
    J.attributeEnd();

    
    J.attributeBegin("enums");
    J.arrayBegin();
    for(auto& e : P.enums)
    {
        J.objectBegin();
        J.attribute("name", e.name);
        serializeAttr(J, e.attrs);
        J.attributeBegin("values");
        J.arrayBegin();
        for(auto v : e.values)
        {
            J.objectBegin();
            J.attribute("name", v.name);
            serializeAttr(J, v.attrs);
            J.attribute("value", v.value);
            J.attribute("line", v.line);
            J.objectEnd();
        }
        J.arrayEnd();
        J.attributeEnd();
        J.attribute("fileName", e.fileName);
        J.attribute("line", e.line);
        J.objectEnd();
    }
    J.arrayEnd();
    J.attributeEnd();

    
    serializeAttr(J, P.attrs);
    J.objectEnd();
    return str;
}