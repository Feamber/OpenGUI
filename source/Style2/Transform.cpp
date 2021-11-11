#include "OpenGUI/Core/Math/Vector.h"
#define DLL_IMPLEMENTATION
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Core/Math.h"

OGUI::ComputedTransform OGUI::multiply(ComputedTransform a, ComputedTransform b)
{
    return {math::multiply(a.m, b.m), math::multiply(b.m, a.trans) + b.trans};
}

OGUI::ComputedTransform OGUI::ComputedTransform::ident()
{
    ComputedTransform result;
    result.m.M[0][0] = 1; result.m.M[0][1] = 0; 
    result.m.M[1][0] = 0; result.m.M[1][1] = 1; 
    result.trans  = Vector2f::vector_zero();
    return result;
}

//TODO: move code to math namespace?
OGUI::ComputedTransform OGUI::ComputedTransform::translate(Vector2f t)
{
    ComputedTransform result;
    result.trans = t;
    return result;
}
OGUI::ComputedTransform OGUI::ComputedTransform::rotate(float angle)
{
    ComputedTransform result;
    float cos = math::cos(angle), sin = math::sin(angle);
    result.m.M[0][0] = cos; result.m.M[0][1] = sin; 
    result.m.M[1][0] = -sin; result.m.M[1][1] = cos; 
    return result;
}
OGUI::ComputedTransform OGUI::ComputedTransform::skew(Vector2f s)
{
    ComputedTransform result;
    
    float shearX = s.X == 0 ? 0 : ( 1.0f / math::tan(s.X) );
    float shearY = s.Y == 0 ? 0 : ( 1.0f / math::tan(s.Y) );
    result.m.M[0][0] = 1; result.m.M[0][1] = shearY; 
    result.m.M[1][0] = shearX; result.m.M[1][1] = 1; 
    return result;
}
OGUI::ComputedTransform OGUI::ComputedTransform::scale(Vector2f s)
{
    ComputedTransform result;
    result.m.M[0][0] = s.X; result.m.M[0][1] = 0; 
    result.m.M[1][0] = 0; result.m.M[1][1] = s.Y; 
    return result;
}

void OGUI::ComputedTransform::decompose(Vector2f& trans, float& rot, Vector2f& sk, Vector2f& sc) const
{
    auto v = m.data_view();
    float a = v[0], b = v[1], c = v[2], d = v[3];
    auto delta = a * d - b * c;
    // Apply the QR-like decomposition.
    if (a != 0 || b != 0) {
        auto r = math::sqrt(a * a + b * b);
        rot = b > 0 ? math::acos(a / r) : -math::acos(a / r);
        sc = {r, delta / r};
        sk = {math::atan((a * c + b * d) / (r * r)), 0};
    } else if (c != 0 || d != 0) {
        auto s = math::sqrt(c * c + d * d);
        rot =
            math::PI_ / 2 - (d > 0 ? math::acos(-c / s) : -math::acos(c / s));
        sc = {delta / s, s};
        sk = {0, math::atan((a * c + b * d) / (s * s))};
    } else {
        rot = 0;
        sk = {0, 0};
        sc = {0, 0};
    }
    trans = this->trans;
}

OGUI::Matrix4x4 OGUI::ComputedTransform::to_3D() const
{
    float data[] = {
        m.M[0][0], m.M[0][1], 0, 0,
        m.M[1][0], m.M[1][1], 0, 0,
        0, 0, 1, 0,
        trans.x, trans.y, 0, 1
    };
    return {data};
}

OGUI::ComputedTransform OGUI::ComputedTransform::compose(Vector2f trans, float rot, Vector2f sk, Vector2f sc)
{
    return multiply(ComputedTransform::scale(sc), multiply(ComputedTransform::skew(sk), multiply(ComputedTransform::rotate(rot), ComputedTransform::translate(trans))));
}