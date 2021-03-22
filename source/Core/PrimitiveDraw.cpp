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
		for (int i = 0; i < sampleCount; ++i) 
        {
			// add vertex 
			Vector2f deltaPos;
			math::sincos(deltaPos.Y, deltaPos.X, params.beginDegree + DeltaDegree * i);
			list.vertices.emplace_back(Vertex{ params.pos + deltaPos * params.radius, uvCenter + deltaPos * uvSizeHalf, params.color });

			// add index 
			list.indices.emplace_back(vcount);
			list.indices.emplace_back(vcount + i + 1);
			list.indices.emplace_back(vcount + (i + 1) % sampleCount + 1);
		}
    }
}