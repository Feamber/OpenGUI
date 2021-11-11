#define DLL_IMPLEMENTATION
#include "OpenGUI/Style2/Lerp/Yoga.h"
#include "OpenGUI/Core/olog.h"
#include "YGValue.h"

YGValue OGUI::Lerp(YGValue a, YGValue b, float alpha)
{
    if(a.unit == YGUnitPercent || b.unit == YGUnitPercent)
    {
        OUNREACHABLE
        return a;
    }
    if(a == YGValueAuto || a == YGValueUndefined || b == YGValueAuto || b == YGValueUndefined)
    {
        return a;
    }
    return YGValue{a.value*(1-alpha) + b.value*alpha, a.unit};
}