#pragma once
#include "Types.h"
#include "OpenGUI/Interface/Interfaces.h"
#include <optional>

namespace OGUI
{
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
        inline void ValidateAndBatch()
        {
            const size_t ic = indices.size();
            command_list.emplace_back(
                PrimDraw{0, 0,
                (uint32_t)ic,
                nullptr, nullptr}
            );
            const size_t i_aligned = (ic / 4) * 4 + 4;
            indices.resize(
                i_aligned
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
	
	namespace PrimitiveDraw
	{
		struct DrawContext
		{
			PrimDrawList prims;
			Vector2f     resolution = Vector2f(0.f, 0.f);
		}; 

		struct BoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			TextureInterface* texture;
			std::optional<float4x4> transform;

			static BoxParams MakeSolid(const Rect rect, const Color4f color);
			static BoxParams MakeSolid(const Rect rect, const Color4f color, const float4x4& transform);
			//Radius
			//SVG
			//Material
		};
		struct CheckBox0Params
		{
			Rect outter;
			Color4f outter_color;
			Rect inner;
			Color4f inner_color;
			bool checked;
		};
		struct CircleParams 
		{
			Vector2f pos;
			Rect uv;
			Color4f color;
			TextureInterface* texture;
			float radius;
		};
		struct FanParams
		{
			Vector2f pos;
			Rect uv;
			Color4f color;
			TextureInterface* texture;
			float radius;
			float degree;
			float beginDegree;
		};
		struct RoundBoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			TextureInterface* texture;
			float radius;
		};
		struct LineParams
		{
			std::vector<Vector2f> points;
			Rect uv;
			Color4f color;
			TextureInterface* texture;
			float thinkness;
		};

		// Call from DrawList.
		OGUI_API void DrawBox(PrimDrawList& list, const BoxParams& params);
		OGUI_API void DrawCircle(PrimDrawList& list, const CircleParams& params, int32_t sampleCount = 20);
		OGUI_API void DrawFan(PrimDrawList& list, const FanParams& params, int32_t sampleCount = 10);
		OGUI_API void DrawRoundBox(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount = 10);
		OGUI_API void DrawLines(PrimDrawList& list, const LineParams& params, bool bAnitAliasing = false, bool bClosed = false);
		
		OGUI_API void DrawCheckBox0(PrimDrawList& context, const CheckBox0Params& params);
	}
}
