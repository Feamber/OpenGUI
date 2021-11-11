#pragma once
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Style2/Transform.h"
#include <vector>

namespace OGUI
{
    float Lerp(float a, float b, float alpha);
    double Lerp(double a, double b, float alpha);
    Vector2f Lerp(Vector2f a, Vector2f b, float alpha);
    Color4f Lerp(Color4f a, Color4f b, float alpha);
    ComputedTransform Lerp(ComputedTransform a, ComputedTransform b, float alpha);
    std::vector<TransformFunction> Lerp(const std::vector<TransformFunction>& a, const std::vector<TransformFunction>& b, float alpha);

}