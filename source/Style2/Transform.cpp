
#define DLL_IMPLEMENTATION
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Core/Math.h"
#pragma warning( push )
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

void OGUI::ComputedTransform::decompose(Vector2f& inTrans, float& rot, Vector2f& sk, Vector2f& sc) const
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
    inTrans = this->trans;
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
#pragma warning(disable : 4582)
OGUI::TransformFunction::TransformFunction()
    :type(Type::translate), _translate(Vector2f::vector_zero())
{}


OGUI::TransformFunction OGUI::TransformFunction::translate(Vector2f t)
{
    OGUI::TransformFunction result;
    result.type = Type::translate;
    result._translate = t;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::rotate(float r)
{
    OGUI::TransformFunction result;
    result.type = Type::rotate;
    result._rotate = r;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::scale(Vector2f s)
{
    OGUI::TransformFunction result;
    result.type = Type::scale;
    result._scale = s;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::skew(Vector2f s)
{
    OGUI::TransformFunction result;
    result.type = Type::skew;
    result._skew = s;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::matrix(float a, float b,float c,float d,float e,float f)
{
    float data[] = {a, b, c, d};
    OGUI::ComputedTransform transform;
    transform.m = {data};
    transform.trans = {e, f};
    OGUI::TransformFunction result;
    result.type = Type::matrix;
    result._matrix = transform;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::matrix(ComputedTransform t)
{
    OGUI::TransformFunction result;
    result.type = Type::matrix;
    result._matrix = t;
    return result;
}
OGUI::TransformFunction OGUI::TransformFunction::ident(Type type)
{
    OGUI::TransformFunction result;
    result.type = type;
    switch (type) {
    case Type::translate:
        result._translate = Vector2f{0, 0};
        break;
    case Type::rotate:
        result._rotate = 0;
        break;
    case Type::skew:
        result._skew = Vector2f{0, 0};
        break;
    case Type::scale:
        result._scale = Vector2f{1, 1};
        break;
    case Type::matrix:
        result._matrix = ComputedTransform::ident();
        break;
    default:
        break;
    }
    return result;
}

OGUI::ComputedTransform OGUI::TransformFunction::to_transform() const
{
    switch (type) {
    case Type::translate:
        return ComputedTransform::translate(_translate);
    case Type::rotate:
        return ComputedTransform::rotate(_rotate);
    case Type::skew:
        return ComputedTransform::skew(_skew);
    case Type::scale:
        return ComputedTransform::scale(_scale);
    case Type::matrix:
        return _matrix;
    default:
        break;
    }
    return ComputedTransform::translate(_translate);
}


OGUI::ComputedTransform OGUI::evaluate(gsl::span<const TransformFunction> transformList)
{
    if(transformList.empty())
        return ComputedTransform::ident();
    OGUI::ComputedTransform result = transformList[0].to_transform();
    for(int i=1; i<transformList.size(); ++i)
        result = multiply(result, transformList[i].to_transform());
    return result;
}
#pragma warning( pop )