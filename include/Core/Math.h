#include <stdint.h>

namespace OGUI
{

    template<typename T, uint32_t Dimension>
    struct Vector
    {
        T data[Dimension];
    };
    template<typename T> struct Vector<T, 2>
    {
        Vector(T _0, T _1) { x = _0; y = _1; }
        union { struct { T x, y; }; struct { T r, g; }; struct { T data[2]; }; };
    };
    template<typename T> struct Vector<T, 3>
    {
        Vector(T _0, T _1, T _2) { x = _0; y = _1; z = _2; }
        union { struct { T x, y, z; }; struct { T r, g, b; }; struct { T data[3]; }; };
    };
    template<typename T> struct Vector<T, 4>
    {
        Vector(T _0, T _1, T _2, T _3) { x = _0; y = _1; z = _2; w = _3; }
        union { struct { T x, y, z, w; }; struct { T r, g, b, a; }; struct { T data[4]; }; };
    };
    using Vector2f = Vector<float, 2u>;
    using Vector3f = Vector<float, 3u>;
    using Vector4f = Vector<float, 4u>;
    using Color4u = Vector<uint8_t, 4u>;
    using Color4f = Vector<float, 4u>;

    struct Rect
    {
        Vector2f Min;
        Vector2f Max;
    };
}