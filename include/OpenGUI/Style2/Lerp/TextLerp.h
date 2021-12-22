#pragma once

#include "OpenGUI/Style2/Shadow.h"
#include <vector>

namespace OGUI
{
    OGUI_API TextShadow Lerp(const TextShadow& a, const TextShadow& b, float alpha);
    OGUI_API std::vector<TextShadow> Lerp(gsl::span<const TextShadow> a, gsl::span<const TextShadow> b, float alpha);
}