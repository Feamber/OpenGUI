#include "OpenGUI/Style2/Transform.h"
#define DLL_IMPLEMENTATION
#include "OpenGUI/Style2/Lerp/Math.h"

namespace OGUI
{
    float OGUI::Lerp(float a, float b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    double OGUI::Lerp(double a, double b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    Vector2f OGUI::Lerp(Vector2f a, Vector2f b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    //https://developer.mozilla.org/en-US/docs/Web/CSS/color_value#interpolation
    Color4f OGUI::Lerp(Color4f a, Color4f b, float alpha)
    {
        return a * (1-alpha) + b * alpha;
    }
    
    ComputedTransform Lerp(ComputedTransform a, ComputedTransform b, float alpha)
    {
        Vector2f atr, asc, ask, btr, bsc, bsk;
        float aro, bro;
        a.decompose(atr, aro, ask, asc);
        b.decompose(btr, bro, bsk, bsc);
        return ComputedTransform::compose(
            Lerp(atr, btr, alpha), 
            Lerp(aro, bro, alpha), 
            Lerp(ask, bsk, alpha), 
            Lerp(asc, bsc, alpha));
    }
}