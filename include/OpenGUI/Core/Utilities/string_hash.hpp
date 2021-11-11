#pragma once
#include "OpenGUI/Core/ostring/helpers.h"

// used for switch stl string
#define switchstr(s) switch(ostr::string __str = s; __str.get_hash())

// used for switch Name
#define switchname(n) switch(ostr::string_view __str = n.ToStringView(); n.GetStringHash())

#define casestr(s) case u ## s ##_o.get_hash(): if(__str != u ## s ##_o) break;

namespace OGUI
{
    inline constexpr size_t FNV_offset_basis = 14695981039346656037ULL;
    inline constexpr size_t FNV_prime        = 1099511628211ULL;


    inline constexpr size_t hash(const std::string& v)
    {
        return ostr::helper::hash::hash_crc32(std::string_view{v});
    }

    inline constexpr size_t hash(const std::string_view& v)
    {
        return ostr::helper::hash::hash_crc32(v);
    }
}