#pragma once
#include "OpenGUI/Configure.h"
#include <type_traits>
#include <string>
#include <vector>
#include <gsl/span>

namespace OGUI
{
    template<class T>
    std::enable_if_t<std::is_enum_v<T>, T> Lerp(const T& a, const T& b, float alpha)
    {
        return a;
    }

    template<class T>
    std::vector<T> LerpArray(gsl::span<const T> a, gsl::span<const T> b, float alpha)
    {
        std::vector<T> result;
        int as = a.size(), bs = b.size();
        int ms = std::max(as, bs);
        T de{};
        for(int i = 0; i<ms; ++i)
        {
            const T* ae;
            if(i < as)
                ae = &a[i];
            else
                ae = &de;
            const T* be;
            if(i < bs)
                be = &b[i];
            else
                be = &de;
            auto resulte = Lerp(*ae, *be, alpha);
            result.push_back(resulte);
        }
        return result;
    }
    
    OGUI_API std::string Lerp(std::string a, std::string b, float alpha);
}