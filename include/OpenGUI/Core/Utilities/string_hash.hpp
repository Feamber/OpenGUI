#pragma once
#include <cinttypes>
#include <string_view>
namespace OGUI
{
    [[nodiscard]] inline constexpr uint64_t hash_(std::string_view str) noexcept
    {
        uint64_t hash = 5381;
        for (auto c : str)
            hash = hash * 33 + c;
        return hash;
    }
#define casestr(s) case hash_(s): if(str != s) break;
}