#pragma once
#include "OpenGUI/Configure.h"
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Style2/Transform.h"
#include <vector>

namespace OGUI
{
    OGUI_FORCEINLINE float Lerp(float a, float b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    OGUI_FORCEINLINE double Lerp(double a, double b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    OGUI_FORCEINLINE Vector2f Lerp(Vector2f a, Vector2f b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    //https://developer.mozilla.org/en-US/docs/Web/CSS/color_value#interpolation
    OGUI_FORCEINLINE Color4f Lerp(Color4f a, Color4f b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    
    //TODO: 换成 FORCE_INLINE?
    OGUI_API ComputedTransform Lerp(ComputedTransform a, ComputedTransform b, float alpha);
    OGUI_API std::vector<TransformFunction> Lerp(gsl::span<const TransformFunction> a, gsl::span<const TransformFunction> b, float alpha);
}