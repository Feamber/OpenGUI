#pragma once
#include "configure.h"
#include <vector>

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
    template<typename T> struct Vector<T, 2> { union { struct { T x, y; }; struct { T r, g; }; struct { T data[2]; }; }; };
    template<typename T> struct Vector<T, 3> { union { struct { T x, y, z; }; struct { T r, g, b; }; struct { T data[3]; }; }; };
    template<typename T> struct Vector<T, 4> { union { struct { T x, y, z, w; }; struct { T r, g, b, a; }; struct { T data[4]; }; }; };
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
        Color4u  color; 
        Vector2f texcoord;
    };

    enum PixelFormat
    {
        PF_R8G8B8A8, PF_R16G16B16A16,
        PF_R8G8B8, PF_R16G16B16,
        PF_R8, PF_R16, PF_R32,
        PF_Count
    };

    struct BitMap
    {
        uint8_t*     bytes;
        uint32_t    bytes_size;
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
    struct OGUI_API PrimDrawList
    {
        
        VertexList vertices;
        IndexList  indices;
        std::vector<PrimDraw> command_list;
    };

    struct OGUI_API PersistantPrimDrawList 
    {

        std::vector<PersistantPrimDraw> command_list;
    };
}
