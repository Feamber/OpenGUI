#pragma once
#include "Types.h"
#include "OpenGUI/Interface/Interfaces.h"
#include <optional>
#include "OpenGUI/Core/nanovg.h"

namespace OGUI
{
	struct PrimDrawResource
	{
		TextureHandle texture = nullptr;
		MaterialHandle material = nullptr;
		bool noGamma = true;
		NVGcompositeOperationState compositeOperation;
	};

	struct PrimDrawCommand
	{
		uint32_t index_offset;
		uint32_t element_count; // number of indices
		PrimDrawResource resource;
	};

	struct PersistantPrimDraw
	{
		PersistantPrimitiveHandle primitive;
		TextureHandle texture;
		void*    p_next;
	};

	using VertexList = std::vector<Vertex>;
	using IndexList = std::vector<uint16_t>;
	OGUI_API const Vector2f Transform(Vector2f p, const float4x4& transform);

	struct OGUI_API PrimDrawList
	{
		inline void ValidateAndBatch()
		{
			const size_t ic = indices.size();
			const size_t i_aligned = (ic / 4) * 4 + 4;
            indices.resize(
                i_aligned
            );
			// batch not implemented now.
			//std::sort(command_list.begin(), command_list.end(),
			//    [](const PrimDraw& a, const PrimDraw& b){
			//        return a.texture < b.texture;
			//    });
		}
		VertexList vertices;
		IndexList  indices;
		std::vector<PrimDrawCommand> command_list;
		std::vector<Matrix4x4> clipStack;
		PrimDrawCommand& GetCommand(const PrimDrawResource& resource);
		int beginCount;
	};

	struct OGUI_API PersistantPrimDrawList 
	{
		std::vector<PersistantPrimDraw> command_list;
	};

	struct OGUI_API PrimDrawContext
	{
		WindowContext& wctx;
		PrimDrawList prims;
		NVGcontext* nvg;
		PrimDrawContext(WindowContext& window);
		~PrimDrawContext();
	}; 

	// Call from DrawList.
	OGUI_API void BeginDraw(PrimDrawList& list);
	OGUI_API void EndDraw(PrimDrawList& list, const float4x4& transform);
}
