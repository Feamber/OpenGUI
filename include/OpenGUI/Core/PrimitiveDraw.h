#pragma once
#include "OpenGUI/Context.h"
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
		
        TextureHandle texture; // affect patching

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
	OGUI_API const Vector2f Transform(Vector2f p, const float4x4& transform);

    struct OGUI_API PrimDrawList
    {
        inline void ValidateAndBatch()
        {
            const size_t ic = indices.size();
			if(ic != __last_index)
			{
				command_list.emplace_back(
					PrimDraw{
						(uint32_t)0, 
						(uint32_t)__last_index,
						(uint32_t)(ic - __last_index),
						__last_tex, nullptr
					}
				); //封口
			}
            const size_t i_aligned = (ic / 4) * 4 + 4;
            indices.resize(
                i_aligned
            );
			__last_tex = nullptr;
            // batch not implemented now.
            //std::sort(command_list.begin(), command_list.end(),
            //    [](const PrimDraw& a, const PrimDraw& b){
            //        return a.texture < b.texture;
            //    });
        }
        VertexList vertices;
        IndexList  indices;
        std::vector<PrimDraw> command_list;
		std::vector<Matrix4x4> clipStack;
		
		int beginCount;
		uint32_t __last_index = 0;
		uint32_t __last_vertex = 0;
		TextureHandle __last_tex = nullptr;//!!!!!
    };

    struct OGUI_API PersistantPrimDrawList 
    {
        std::vector<PersistantPrimDraw> command_list;
    };
	
	namespace PrimitiveDraw
	{
		struct OGUI_API DrawContext
		{
			WindowContext& Window;
			PrimDrawList prims;
		}; 

		struct OGUI_API BoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;

			static BoxParams MakeSolid(const Rect rect, const Color4f color);
			//Radius
			//SVG
			//Material
		};
		struct OGUI_API CheckBox0Params
		{
			Rect outter;
			Color4f outter_color;
			Rect inner;
			Color4f inner_color;
			bool checked;
		};
		struct OGUI_API CircleParams 
		{
			Vector2f pos;
			Rect uv;
			Color4f color;
			float radius;
		};
		struct OGUI_API FanParams
		{
			Vector2f pos;
			Rect uv;
			Color4f color;
			float radius;
			float degree;
			float beginDegree;
		};
		struct OGUI_API RoundBoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			float radius[4];
		};
		struct OGUI_API LineParams
		{
			std::vector<Vector2f> points;
			Rect uv;
			Color4f color;
			float thinkness;
		};

		// Call from DrawList.
		OGUI_API void BeginDraw(PrimDrawList& list);
		OGUI_API void EndDraw(PrimDrawList& list, const float4x4& transform);
		OGUI_API void BoxShape(PrimDrawList& list, const BoxParams& params);
		OGUI_API void CircleShape(PrimDrawList& list, const CircleParams& params, int32_t sampleCount = 20);
		OGUI_API void FanShape(PrimDrawList& list, const FanParams& params, int32_t sampleCount = 10);
		OGUI_API void RoundBoxShape(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount = 10);
		OGUI_API void RoundBoxShape2(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount = 10);
		OGUI_API void LineShape(PrimDrawList& list, const LineParams& params, bool bAnitAliasing = false, bool bClosed = false);
		OGUI_API void CheckBox0Shape(PrimDrawList& list, const CheckBox0Params& params);

		template<auto ShapeF, typename... ShapeParams>
		inline void PrimitiveDraw(TextureInterface* texture,
			PrimDrawList& list, ShapeParams&&... params)
		{
			// Generate DrawCall if texture is changed.
			if(texture != list.__last_tex)
			{
				const auto vc = (uint32_t)list.vertices.size();
				const auto ic = (uint32_t)list.indices.size();
				if(vc != 0 && ic != 0 && ic != list.__last_index)
				{
					list.command_list.emplace_back(
						PrimDraw{
							(uint32_t)0, 
							(uint32_t)list.__last_index,
							(uint32_t)(ic - list.__last_index),
							list.__last_tex, nullptr
						}
					);
				}
				list.__last_vertex = vc;
				list.__last_index = ic;
				list.__last_tex = texture;
			}
			ShapeF(list, std::forward<ShapeParams>(params)...);
		}
	}
}
