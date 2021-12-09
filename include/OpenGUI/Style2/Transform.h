#pragma once
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Core/Math/Matrix.h"
#include "OpenGUI/Core/Math/Vector.h"
#include <vector>
#include <gsl/span>

namespace OGUI
{
    struct ComputedTransform
    {
        Matrix2x2 m;
        Vector2f trans;
        static ComputedTransform ident();
        static ComputedTransform translate(Vector2f t);
        static ComputedTransform rotate(float angle);
        static ComputedTransform skew(Vector2f s);
        static ComputedTransform scale(Vector2f s);

        Matrix4x4 to_3D() const;
        void decompose(Vector2f& trans, float& rot, Vector2f& sk, Vector2f& sc) const;
        static ComputedTransform compose(Vector2f trans, float rot, Vector2f sk, Vector2f sc);
    };

    struct TransformFunction
    {
        enum class Type
        {
            translate,
            rotate,
            skew,
            scale,
            matrix,
        } type;
        union
        {
            Vector2f _translate;
            float _rotate;
            Vector2f _skew;
            Vector2f _scale;
            ComputedTransform _matrix;
        };
        static TransformFunction translate(Vector2f t);
        static TransformFunction rotate(float angle);
        static TransformFunction skew(Vector2f s);
        static TransformFunction scale(Vector2f s);
        static TransformFunction matrix(float a, float b,float c,float d,float e,float f);
        static TransformFunction matrix(ComputedTransform t);
        static TransformFunction ident(Type type);
        ComputedTransform to_transform() const;
        TransformFunction();
    };
    ComputedTransform evaluate(gsl::span<const TransformFunction> transformList);
    FORCEINLINE ComputedTransform multiply(ComputedTransform a, ComputedTransform b)
    {
        return {math::multiply(a.m, b.m), math::multiply(b.m, a.trans) + b.trans};
    }
    FORCEINLINE Vector2f multiply(ComputedTransform a, Vector2f point)
    {
        return math::multiply(a.m, point) + a.trans;
    }
}