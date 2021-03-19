#pragma once
#include "OpenGUI/Configure.h"
#include "Primitive.h"

namespace OGUI 
{
    struct IRenderer 
    {
        virtual PersistantPrimitiveHandle register_primitive(
            Vertex* vertices, uint32_t num_vertices,
            uint16_t* indices, uint32_t num_indices) = 0;
        virtual void release_primitive(PersistantPrimitiveHandle primitive) = 0;

        virtual void render_primitives(const PrimDrawList&) = 0;
        virtual void render_primitives(const PersistantPrimDrawList&) = 0;

        virtual TextureHandle register_texture(const BitMap&) = 0;
        virtual void release_texture(TextureHandle) = 0;

        virtual void set_scissor(const Scissor scissor) = 0;
        virtual void reset_scissor() = 0;
    };
}
