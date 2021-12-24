#pragma once

#include "meta.h"
#include "clang/AST/Attr.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/PrettyPrinter.h"
#include <string>
#include <unordered_set>
#include <algorithm>
#include "llvm/Support/JSON.h"

namespace meta
{
    struct Field
    {
        std::string name;
        std::string type;
        std::string rawType;
        std::string attrs;
        std::string comment;
        int line;
    };

    struct Function
    {
        bool isStatic;
        bool isConst;
        std::string name;
        std::string attrs;
        std::string comment;
        std::vector<Field> parameters;
        std::string retType;
        std::string rawRetType;
        std::string fileName;
        int line;
    };

    struct Record
    {
        std::vector<std::string> bases;
        std::string name;
        std::string attrs;
        std::string comment;
        std::vector<Field> fields;
        std::vector<Field> statics;
        std::vector<Function> methods;
        std::string fileName;
        int line;
    };
    
    struct Enumerator
    {
        std::string name;
        std::string attrs;
        uint64_t value;
        std::string comment;
        int line;
    };

    struct Enum
    {
        std::string name;
        std::string attrs;
        std::vector<Enumerator> values;
        std::string fileName;
        std::string comment;
        int line;
    };

    struct Identity
    {
        std::string fileName;
        int line;
        bool operator==(const Identity& other) const
        {
            return fileName == other.fileName && line == other.line;
        }
    };

    struct IdentityHash
    {
        size_t operator()(const Identity& id) const
        {
            return std::hash<std::string>{}(id.fileName) + id.line;
        }
    };

    struct Database
    {
        std::unordered_set<Identity, IdentityHash> parsed;
        std::vector<Record> records;
        std::vector<Function> functions;
        std::vector<Enum> enums;
        std::string attrs;
    };
    
    std::string serialize(const Database& P);
}