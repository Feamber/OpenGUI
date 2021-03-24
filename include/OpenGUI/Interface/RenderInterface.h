#pragma once
#include "OpenGUI/Core/Primitive.h"

namespace OGUI
{
	struct RenderInterface
	{
		virtual ~RenderInterface();
		virtual PersistantPrimitiveHandle RegisterPrimitive(
            Vertex* vertices, uint32_t num_vertices,
            uint16_t* indices, uint32_t num_indices) = 0;
        virtual void ReleasePrimitive(PersistantPrimitiveHandle primitive) = 0;

        virtual void RenderPrimitives(const PrimDrawList&) = 0;
        virtual void RenderPrimitives(const PersistantPrimDrawList&) = 0;

        virtual TextureHandle RegisterTexture(const BitMap&) = 0;
        virtual void ReleaseTexture(TextureHandle) = 0;

        virtual void SetScissor(const Scissor scissor) = 0;
        virtual void ResetScissor() = 0;
	};
}