#pragma once
#include "configure.h"
#include "Primitive.h"

namespace OGUI 
{
    struct OGUI_API IRenderer 
    {
        virtual PersistantPrimitiveHandle register_primitive(
            Vertex* vertices, uint32_t num_vertices,
            uint32_t* indices, uint32_t num_indices) = 0;
        virtual void release_primitive(PersistantPrimitiveHandle primitive) = 0;

        virtual void render_primitive(
            Vertex* vertices, uint32_t num_vertices,
            uint32_t* indices, uint32_t num_indices,
            TextureHandle texture, const Vector2f& translation) = 0;
        virtual void render_primitive(
            PersistantPrimitiveHandle primitive,
            TextureHandle texture, const Vector2f& translation) = 0;

        virtual TextureHandle register_texture(const BitMap&) = 0;
        virtual void release_texture(TextureHandle) = 0;

        virtual void set_scissor(const Scissor scissor) = 0;
        virtual void reset_scissor() = 0;
    };
}
