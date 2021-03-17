#pragma once
#include "configure.h"
#include <vector>
#include <algorithm>

namespace OGUI 
{
    template<typename T>
    struct Handle 
    {
        inline Handle(T* v) : value(v) {}
        T* value;
    };

    template<typename T, uint32_t Dimension>
    struct Vector
    {
        T data[Dimension];
    };
    template<typename T> struct Vector<T, 2> { 
        Vector(T _0, T _1) {x = _0; y = _1;}
        union { struct { T x, y; }; struct { T r, g; }; struct { T data[2]; }; }; 
    };
    template<typename T> struct Vector<T, 3> { 
        Vector(T _0, T _1, T _2) {x = _0; y = _1; z = _2;}
        union { struct { T x, y, z; }; struct { T r, g, b; }; struct { T data[3]; }; };
    };
    template<typename T> struct Vector<T, 4> { 
        Vector(T _0, T _1, T _2, T _3) {x = _0; y = _1; z = _2; w = _3;}
        union { struct { T x, y, z, w; }; struct { T r, g, b, a; }; struct { T data[4]; }; }; 
    };
    using Vector2f = Vector<float, 2u>;
    using Vector3f = Vector<float, 3u>;
    using Vector4f = Vector<float, 4u>;
    using Color4u = Vector<uint8_t, 4u>;
    using Color4f = Vector<float, 4u>;
    
    struct Scissor
    {
        int x, y;
        int width, height;
    };

    struct Vertex
    {
        Vector2f position;
        Color4f  color; 
        Vector2f texcoord;
    };

    enum PixelFormat
    {
        PF_R8G8B8A8, PF_R16G16B16A16,
        PF_R8G8B8A8_SRGB, 
        PF_R8Uint, PF_R16Uint, PF_R32Uint,
        PF_Count
    };

    struct BitMap
    {
        uint8_t*    bytes;
        uint32_t    bytes_size;
        uint32_t    width, height;
        PixelFormat format;
    };

    struct ITexture {};
    struct IPersistantPrimitive {};
    using TextureHandle = Handle<ITexture>;
    using PersistantPrimitiveHandle = Handle<IPersistantPrimitive>;

    struct PrimDraw
    {
        uint32_t vertex_offset;
        uint32_t index_offset;
        uint32_t element_count; // number of indices
        TextureHandle texture;
        void*    p_next;
    };

    struct PersistantPrimDraw
    {
        PersistantPrimitiveHandle primitive;
        TextureHandle texture;
        void*    p_next;
    };

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<uint16_t>;
    struct BatchedPrimDrawSpan {
        PrimDraw* start;
        uint64_t count;
    };
    struct OGUI_API PrimDrawList
    {
        inline void validate_and_batch()
        {
            auto ic = indices.size();
            indices.reserve(
                (ic / 4) * 4 + 4
            );
            std::sort(command_list.begin(), command_list.end(),
                [](const PrimDraw& a, const PrimDraw& b){
                    return a.texture.value < b.texture.value;
                });
        }

        VertexList vertices;
        IndexList  indices;
        std::vector<PrimDraw> command_list;
    };

    struct OGUI_API PersistantPrimDrawList 
    {

        std::vector<PersistantPrimDraw> command_list;
    };
}
