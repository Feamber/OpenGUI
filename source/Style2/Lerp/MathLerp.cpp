
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"

namespace OGUI
{
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

    //https://drafts.csswg.org/css-transforms-1/#interpolation-of-transforms
    std::vector<TransformFunction> Lerp(gsl::span<const TransformFunction> a, gsl::span<const TransformFunction> b, float alpha)
    {
        std::vector<TransformFunction> result;
        int i=0;
        for(; i<a.size() || i<b.size(); ++i)
        {
            TransformFunction ta;
            TransformFunction tb;
            if(i >= b.size())
                tb = TransformFunction::ident(ta.type);
            else
                tb = b[i];
            if(i >= a.size())
                ta = TransformFunction::ident(tb.type);
            else
                ta = a[i];  
            if(ta.type != tb.type)
                break;
            switch (ta.type) {
                case TransformFunction::Type::translate:
                    result.push_back(TransformFunction::translate(Lerp(ta._translate, tb._translate, alpha)));
                    break;
                case TransformFunction::Type::rotate:
                    result.push_back(TransformFunction::rotate(Lerp(ta._rotate, tb._rotate, alpha)));
                    break;
                case TransformFunction::Type::skew:
                    result.push_back(TransformFunction::skew(Lerp(ta._skew, tb._skew, alpha)));
                    break;
                case TransformFunction::Type::scale:
                    result.push_back(TransformFunction::scale(Lerp(ta._scale, tb._scale, alpha)));
                    break;
                case TransformFunction::Type::matrix:
                    result.push_back(TransformFunction::matrix(Lerp(ta._matrix, tb._matrix, alpha)));
                    break;
                default:
                    break;
            }
        }
        if(i<a.size() || i<b.size())
        {
            ComputedTransform ta = evaluate(a.subspan(i));
            ComputedTransform tb = evaluate(b.subspan(i));
            result.push_back(TransformFunction::matrix(Lerp(ta, tb, alpha)));
        }
        return result;
    }
}