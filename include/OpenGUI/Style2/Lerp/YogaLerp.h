#pragma once
#include "OpenGUI/Configure.h"
#include "YGValue.h"
#include "yoga/Yoga.h"

namespace OGUI
{
    OGUI_FORCEINLINE YGValue Lerp(YGValue a, YGValue b, float alpha)
    {
        //TODO: 是否需要支持不同单位之间的插值
        if(a.unit != b.unit)
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