#pragma once
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Core/Math/Matrix.h"
#include "OpenGUI/Core/Math/Vector.h"

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
            Vector2f translate;
            float rotate;
            Vector2f skew;
            Vector2f scale;
            ComputedTransform matrix;
        };
        ComputedTransform to_transform();
    };

    FORCEINLINE ComputedTransform multiply(ComputedTransform a, ComputedTransform b);
}