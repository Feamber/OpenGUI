#pragma once
#include "OpenGUI/Configure.h"
#include "YGValue.h"
#include "yoga/Yoga.h"

namespace OGUI
{
    OGUI_FORCEINLINE YGValue Lerp(YGValue a, YGValue b, float alpha)
    {
        if(a.unit == YGUnitPercent || b.unit == YGUnitPercent)
        {
            return a;
        }
        if(a == YGValueAuto || a == YGValueUndefined || b == YGValueAuto || b == YGValueUndefined)
        {
            return a;
        }
        return YGValue{a.value*(1-alpha) + b.value*alpha, a.unit};
    }
}