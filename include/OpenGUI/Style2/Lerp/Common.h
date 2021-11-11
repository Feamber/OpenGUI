#pragma once
#include <type_traits>
#include <string>
namespace OGUI
{
    template<class T>
    std::enable_if_t<std::is_enum_v<T>, T> Lerp(const T& a, const T& b, float alpha)
    {
        return a;
    }
    
    std::string Lerp(std::string a, std::string b, float alpha);
}