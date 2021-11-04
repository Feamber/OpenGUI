#pragma once
#include "OpenGUI/Core/ostring/helpers.h"

// used for switch stl string
#define switchstr(s) switch(ostr::string __str = s; __str.get_hash())

// used for switch Name
#define switchname(n) switch(ostr::string_view __str = n.ToStringView(); n.GetStringHash())

#define casestr(s) case u ## s ##_o.get_hash(): if(__str != u ## s ##_o) break;

namespace OGUI
{
    inline constexpr size_t _FNV_offset_basis = 14695981039346656037ULL;
    inline constexpr size_t _FNV_prime        = 1099511628211ULL;

    _NODISCARD inline size_t _Fnv1a_append_bytes(size_t _Val, const unsigned char* const _First,
        const size_t _Count) noexcept { // accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
        for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
            _Val ^= static_cast<size_t>(_First[_Idx]);
            _Val *= _FNV_prime;
        }

        return _Val;
    }

    template <class _Ty>
    _NODISCARD size_t _Fnv1a_append_range(const size_t _Val, const _Ty* const _First,
        const _Ty* const _Last) noexcept { // accumulate range [_First, _Last) into partial FNV-1a hash _Val
        static_assert(std::is_trivial_v<_Ty>, "Only trivial types can be directly hashed.");
        const auto _Firstb = reinterpret_cast<const unsigned char*>(_First);
        const auto _Lastb  = reinterpret_cast<const unsigned char*>(_Last);
        return _Fnv1a_append_bytes(_Val, _Firstb, static_cast<size_t>(_Lastb - _Firstb));
    }

    template <class _Kty>
    _NODISCARD size_t _Fnv1a_append_value(
        const size_t _Val, const _Kty& _Keyval) noexcept { // accumulate _Keyval into partial FNV-1a hash _Val
        static_assert(std::is_trivial_v<_Kty>, "Only trivial types can be directly hashed.");
        return _Fnv1a_append_bytes(_Val, &reinterpret_cast<const unsigned char&>(_Keyval), sizeof(_Kty));
    }

    // FUNCTION TEMPLATE _Hash_representation
    template <class _Kty>
    _NODISCARD size_t _Hash_representation(const _Kty& _Keyval) noexcept { // bitwise hashes the representation of a key
        return _Fnv1a_append_value(_FNV_offset_basis, _Keyval);
    }

    // FUNCTION TEMPLATE _Hash_array_representation
    template <class _Kty>
    _NODISCARD size_t _Hash_array_representation(
        const _Kty* const _First, const size_t _Count) noexcept { // bitwise hashes the representation of an array
        static_assert(std::is_trivial_v<_Kty>, "Only trivial types can be directly hashed.");
        return _Fnv1a_append_bytes(
            _FNV_offset_basis, reinterpret_cast<const unsigned char*>(_First), _Count * sizeof(_Kty));
    }

    inline size_t hash(const std::string& v)
    {
        return _Hash_array_representation(v.c_str(), v.size());
    }

    inline size_t hash(const std::string_view& v)
    {
        return _Hash_array_representation(v.data(), v.size());
    }
}