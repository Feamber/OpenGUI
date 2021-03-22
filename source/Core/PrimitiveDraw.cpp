#define DLL_IMPLEMENTATION
#include "OpenGUI/Core/PrimitiveDraw.h"

namespace OGUI
{
    void PrimitiveDraw::DrawBox(PrimDrawList& list, const BoxParams& params)
    {
        const uint32_t vcount = list.vertices.size();
        const uint32_t icount = list.indices.size();
        
        const Vector2f RU = params.rect.max; 
        const Vector2f RUUV = params.uv.max;
        const Vector2f RB = Vector2f(params.rect.max.X, params.rect.min.Y);
        const Vector2f RBUV = Vector2f(params.uv.max.X, params.uv.min.Y);
        const Vector2f LU = Vector2f(params.rect.min.X, params.rect.max.Y);
        const Vector2f LUUV = Vector2f(params.uv.min.X, params.uv.max.Y);
        const Vector2f LB = params.rect.min;
        const Vector2f LBUV = params.uv.min;

        list.vertices.emplace_back(Vertex{RU, RUUV, params.color}); // vcount + 0
        list.vertices.emplace_back(Vertex{LU, LUUV, params.color}); // vcount + 1
        list.vertices.emplace_back(Vertex{RB, RBUV, params.color}); // vcount + 2
        list.vertices.emplace_back(Vertex{LB, LBUV, params.color}); // vcount + 3

        list.indices.emplace_back(vcount + 0u);
        list.indices.emplace_back(vcount + 1u);
        list.indices.emplace_back(vcount + 2u);
        list.indices.emplace_back(vcount + 2u);
        list.indices.emplace_back(vcount + 1u);
        list.indices.emplace_back(vcount + 3u);
    }

    void PrimitiveDraw::DrawCircle(PrimDrawList& list, const CircleParams& params, int32_t sampleCount)
	{
		const uint32_t vcount = list.vertices.size();
		const uint32_t icount = list.indices.size();

        Vector2f uvCenter = (params.uv.min + params.uv.max) / 2;
        Vector2f uvSizeHalf = (params.uv.max - params.uv.min) / 2;

        // center 
        list.vertices.emplace_back(Vertex{ params.pos, uvCenter, params.color });

        // add border 
        double DeltaDegree = math::PI * 2 / sampleCount;
        for (int i = 0; i < sampleCount; ++i)
        {
			// add vertex 
			Vector2f deltaPos;
			math::sincos(deltaPos.Y, deltaPos.X, DeltaDegree * i);
            list.vertices.emplace_back(Vertex{ params.pos + deltaPos * params.radius, uvCenter + deltaPos * uvSizeHalf, params.color });

			// add index 
			list.indices.emplace_back(vcount);
            list.indices.emplace_back(vcount + i + 1);
            list.indices.emplace_back(vcount + (i + 1) % sampleCount + 1);
        }
    }

    void PrimitiveDraw::DrawFan(PrimDrawList& list, const FanParams& params, int32_t sampleCount /* = 10 */)
	{
		const uint32_t vcount = list.vertices.size();
		const uint32_t icount = list.indices.size();

		Vector2f uvCenter = (params.uv.min + params.uv.max) / 2;
		Vector2f uvSizeHalf = (params.uv.max - params.uv.min) / 2;

		// center 
		list.vertices.emplace_back(Vertex{ params.pos, uvCenter, params.color });

		// add border 
		double DeltaDegree = params.degree / sampleCount;
		for (int i = 0; i <= sampleCount; ++i) 
        {
			// add vertex 
			Vector2f deltaPos;
			math::sincos(deltaPos.Y, deltaPos.X, params.beginDegree + DeltaDegree * i);
			list.vertices.emplace_back(Vertex{ params.pos + deltaPos * params.radius, uvCenter + deltaPos * uvSizeHalf, params.color });

			// add index 
            if (i == 0) continue;
			list.indices.emplace_back(vcount);
			list.indices.emplace_back(vcount + i);
			list.indices.emplace_back(vcount + i + 1);
		}
    }
    
    void PrimitiveDraw::DrawRoundBox(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount /* = 10 */)
	{
		const uint32_t vcount = list.vertices.size();
		const uint32_t icount = list.indices.size();

        // calculate info 
        Vector2f uvSize = params.uv.max - params.uv.min;
        Vector2f boxSize = params.rect.max - params.rect.min;
        Vector2f uvBorder = uvSize * params.radius / boxSize;
        Rect centerRect{ params.rect.min + params.radius, params.rect.max - params.radius };
        Rect centerRectUV{ params.uv.min + uvBorder, params.uv.max - uvBorder };
        
        // draw order 
        //  4 |  1  | 5
        // ----  1  -----
        //  2 |  1  | 3
		// ----  1  -----
		//  6 |  1  | 7 
        
        // draw middle rect(1) 
        DrawBox(list, {
			Rect{Vector2f(centerRect.min.X, params.rect.min.Y), Vector2f(centerRect.max.X, params.rect.max.Y)},
			Rect{Vector2f(centerRectUV.min.X, params.rect.min.Y), Vector2f(centerRectUV.max.X, params.rect.max.Y)},
            params.color,
            params.texture });

        // draw border rect(2->3) 
		DrawBox(list, {
			Rect{Vector2f(params.rect.min.X, centerRect.min.Y), Vector2f(centerRect.min.X, centerRect.max.Y)},
			Rect{Vector2f(params.uv.min.X, centerRectUV.min.Y),Vector2f(centerRectUV.min.X, centerRectUV.max.Y)},
			params.color,
			params.texture });
		DrawBox(list, {
			Rect{Vector2f(centerRect.max.X, centerRect.min.Y), Vector2f(params.rect.max.X, centerRect.max.Y)},
			Rect{Vector2f(centerRectUV.max.X, centerRectUV.min.Y), Vector2f(params.rect.max.X, centerRectUV.max.Y)},
			params.color,
			params.texture });

        // draw corner fan(4->7) 
        DrawFan(list, {
            Vector2f(centerRect.min.X, centerRect.max.Y),
            params.radius,
            math::PI / 2,
            math::PI / 2,
            Rect{Vector2f(params.uv.min.X, centerRectUV.max.Y), Vector2f(centerRectUV.min.X, params.uv.max.Y)},
            params.color,
            params.texture });
		DrawFan(list, {
			Vector2f(centerRect.max.X, centerRect.max.Y),
			params.radius,
			math::PI / 2,
			0,
			Rect{Vector2f(centerRectUV.max.X, centerRectUV.max.Y), Vector2f(centerRectUV.max.X, centerRectUV.max.Y)},
			params.color,
			params.texture });
		DrawFan(list, {
			Vector2f(centerRect.min.X, centerRect.min.Y),
			params.radius,
			math::PI / 2,
			-math::PI,
			Rect{Vector2f(params.uv.min.X, params.uv.min.Y), Vector2f(centerRectUV.min.X, centerRectUV.min.Y)},
			params.color,
			params.texture });
		DrawFan(list, {
			Vector2f(centerRect.max.X, centerRect.min.Y),
			params.radius,
			math::PI / 2,
			-math::PI / 2,
			Rect{Vector2f(centerRectUV.min.X, params.uv.min.Y), Vector2f(params.uv.max.X, centerRectUV.min.Y)},
			params.color,
			params.texture });
    }
}