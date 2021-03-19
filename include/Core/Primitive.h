#pragma once
#include "configure.h"
#include "Core/Math.h"
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
    
    struct Scissor
    {
        int x, y;
        int width, height;
    };

    struct Vertex
    {
        Vector2f position;
        Vector2f texcoord;
        // Alignment
        Color4f  color; 
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
            // batch not implemented now.
            //std::sort(command_list.begin(), command_list.end(),
            //    [](const PrimDraw& a, const PrimDraw& b){
            //        return a.texture.value < b.texture.value;
            //    });
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
