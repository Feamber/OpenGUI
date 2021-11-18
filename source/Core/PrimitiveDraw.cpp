#include <limits>
#define DLL_IMPLEMENTATION
#include "OpenGUI/Core/PrimitiveDraw.h"

namespace OGUI
{
    uint32_t ToColor32ABGR(Vector4f color)
    {
        color.x = std::clamp(color.x, 0.f, 1.f);   
        color.y = std::clamp(color.y, 0.f, 1.f);   
        color.z = std::clamp(color.z, 0.f, 1.f);   
        color.w = std::clamp(color.w, 0.f, 1.f);   
        uint8_t r = color.x * 255;
        uint8_t g = color.y * 255;
        uint8_t b = color.z * 255;
        uint8_t a = color.w * 255;
        return ((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)g << 8) + (uint32_t)r;
    }

    Vertex MakeVertex(Vector2f position, Vector2f UV, Color4f color)
    {
        return Vertex{ position, UV, ToColor32ABGR(color) };
    }

    const Vector2f Transform(Vector2f p, const float4x4& transform)
    {
        const Vector4f dummy = Vector4f(p.X, p.Y, 0.f, 1.f);
        const Vector4f result = math::multiply(dummy, transform);
        p.X = result.X;
        p.Y = result.Y;
        return p;
    }

    OGUI_API void PrimitiveDraw::BeginDraw(PrimDrawList& list)
    {
        list.beginCount = list.vertices.size();
    }

    OGUI_API void PrimitiveDraw::EndDraw(PrimDrawList& list,
        const float4x4& transform)
    {
        int count = list.vertices.size();
        for (int i = list.beginCount; i < count; ++i)
        {
            auto& vertex = list.vertices[i];
            vertex.position = Transform(vertex.position, transform);
            if(!list.clipStack.empty())
            {
                auto& back = list.clipStack.back();
                auto clipPos = Transform(vertex.position, back.invTransform);
                auto size = back.rect.max - back.rect.min;
                auto center = (back.rect.max + back.rect.min) / 2;
                vertex.clipUV = (clipPos - center) * 2 / size;
            }
            else 
            {
                vertex.clipUV = Vector2f::vector_zero();
            }
        }
    }

    void PrimitiveDraw::BoxShape(PrimDrawList& list, const BoxParams& params)
    {
        Vector2f side_len = params.rect.max - params.rect.min;
        if (!side_len.X || !side_len.Y)
            return;

        const uint32_t vcount = list.vertices.size();
        //const uint32_t icount = list.indices.size();
        
        const Vector2f RU = params.rect.max;
        const Vector2f RB = Vector2f(params.rect.max.X, params.rect.min.Y);
        const Vector2f LU = Vector2f(params.rect.min.X, params.rect.max.Y);
        const Vector2f LB = params.rect.min;

        const Vector2f RUUV = params.uv.max;
        const Vector2f RBUV = Vector2f(params.uv.max.X, params.uv.min.Y);
        const Vector2f LUUV = Vector2f(params.uv.min.X, params.uv.max.Y);
        const Vector2f LBUV = params.uv.min;

        const auto vRU = MakeVertex( RU, RUUV, params.color );
        const auto vLU = MakeVertex( LU, LUUV, params.color );
        const auto vRB = MakeVertex( RB, RBUV, params.color );
        const auto vLB = MakeVertex( LB, LBUV, params.color );

        list.vertices.emplace_back(vRU); // vcount + 0
        list.vertices.emplace_back(vLU); // vcount + 1
        list.vertices.emplace_back(vRB); // vcount + 2
        list.vertices.emplace_back(vLB); // vcount + 3

        list.indices.emplace_back(vcount + 0u);
        list.indices.emplace_back(vcount + 1u);
        list.indices.emplace_back(vcount + 2u);
        list.indices.emplace_back(vcount + 2u);
        list.indices.emplace_back(vcount + 1u);
        list.indices.emplace_back(vcount + 3u);
    }

    PrimitiveDraw::BoxParams PrimitiveDraw::BoxParams::MakeSolid(const Rect rect, const Color4f color)
    {
        Rect uv = {Vector2f::vector_zero(), Vector2f::vector_one()};
        return { rect, uv, color };
    }

    void PrimitiveDraw::CircleShape(PrimDrawList& list, const CircleParams& params, int32_t sampleCount)
	{
		const uint32_t vcount = list.vertices.size();
		//const uint32_t icount = list.indices.size();

        Vector2f uvCenter = (params.uv.min + params.uv.max) / 2;
        Vector2f uvSizeHalf = (params.uv.max - params.uv.min) / 2;

        // center 
        list.vertices.emplace_back(MakeVertex( params.pos, uvCenter, params.color ));

        // add border 
        double DeltaDegree = math::PI_ * 2 / sampleCount;
        for (int i = 0; i < sampleCount; ++i)
        {
			// add vertex 
			Vector2f deltaPos;
			math::sincos(deltaPos.Y, deltaPos.X, DeltaDegree * i);
            list.vertices.emplace_back(MakeVertex( params.pos + deltaPos * params.radius, uvCenter + deltaPos * uvSizeHalf, params.color ));

			// add index 
			list.indices.emplace_back(vcount);
            list.indices.emplace_back(vcount + i + 1);
            list.indices.emplace_back(vcount + (i + 1) % sampleCount + 1);
        }
    }

    void PrimitiveDraw::FanShape(PrimDrawList& list, const FanParams& params, int32_t sampleCount /* = 10 */)
	{
		const uint32_t vcount = list.vertices.size();
		//const uint32_t icount = list.indices.size();

		Vector2f uvCenter = (params.uv.min + params.uv.max) / 2;
		Vector2f uvSizeHalf = (params.uv.max - params.uv.min) / 2;

		// center 
		list.vertices.emplace_back(MakeVertex( params.pos, uvCenter, params.color ));

		// add border 
		double DeltaDegree = params.degree / sampleCount;
		for (int i = 0; i <= sampleCount; ++i) 
        {
			// add vertex 
			Vector2f deltaPos;
			math::sincos(deltaPos.Y, deltaPos.X, params.beginDegree + DeltaDegree * i);
			list.vertices.emplace_back(MakeVertex( params.pos + deltaPos * params.radius, uvCenter + deltaPos * uvSizeHalf, params.color ));

			// add index 
            if (i == 0) 
                continue;
			list.indices.emplace_back(vcount);
			list.indices.emplace_back(vcount + i);
			list.indices.emplace_back(vcount + i + 1);
		}
    }

    // Draw Round Box 2:
    // radius of rounded corner:
    //     lt: left top
    //     rt: right top
    //     rb: right bottom
    //     lb: left bottom
    // 
    // whole box:
    //     (0,y)       (x,y)
    //     (0,0)       (x,0)
    // 
    // split into:
    //             -------------------------------------------------   (x,y)
    //             |       |            top bar                | rt|
    //             |   lt  <-----------------------^---------------|
    //             |       |                       |               |
    //             |-------|         center        |               |
    //   left bar  |       |                       |    right bar  |
    //             |-------v----------------------->               |
    //             |                   |           |---------------|
    //             |                   |           |               |
    //             |                   |           |               |
    //             |        lb         |bottom bar |       rb      |
    //             |                   |           |               |
    //             |                   |           |               |
    //     (0,0)   -------------------------------------------------
    // 
    // cordinates:
    //     top bar: 
    //         (lt,0)      (x-rt,0)
    //         (lt,y-rt)   (x-rt,y-rt)
    // 
    //     left bar:
    //         (0,y-lt)    (lt,y-lt)
    //         (0,lb)      (lt,lb)
    // 
    //     bottom bar:
    //         (lb,lb)     (x-rb,lb)
    //         (lb,0)      (x-rb,0)
    // 
    //     right bar:
    //         (x-rb,y-rt) (x,y-rt)
    //         (x-rb,rb)   (x,rb)
    // 
    //     center:
    //         (lt,y-rt)   (x-rb,y-rt)
    //         (lt,lb)     (x-rb,lb)
    //
    void PrimitiveDraw::RoundBoxShape2(PrimDrawList& list, const PrimitiveDraw::RoundBoxParams& params, int32_t sampleCount)
    {
        auto prect = params.rect;
        FanParams fans[4];
        int fanCount = 0;
        BoxParams boxes[5];
        float radius[4];
        std::memcpy(radius, params.radius, sizeof(radius));
        auto size = prect.max - prect.min;
        float shortEdge = std::min(size.X, size.Y);
        for (int i = 0; i < 4; ++i) radius[i] = std::clamp(radius[i], 0.f, shortEdge/2);
        Vector2f uvS = (params.uv.max - params.uv.min) / (prect.max - prect.min);
        if (radius[0] > 0.f)
        {
            Rect rect = {Vector2f{prect.min.X, prect.min.Y + radius[3]}, Vector2f{prect.min.X + radius[0], prect.max.Y - radius[0]}};
            Rect uv = {Vector2f{params.uv.min.X, params.uv.min.Y + radius[3] * uvS.Y}, Vector2f{params.uv.min.X + radius[3] * uvS.X, params.uv.max.Y - radius[0] * uvS.Y}};
            boxes[fanCount] = {
                rect, uv, params.color
            };
            fans[fanCount++] = {
                Vector2f(prect.min.X + radius[0], prect.max.Y - radius[0]),
                Rect{Vector2f(params.uv.min.X, params.uv.max.Y - radius[0] * uvS.Y * 2), Vector2f(params.uv.min.X + radius[0] * uvS.X * 2, params.uv.max.Y)},
                params.color,
                radius[0],
                math::PI_ / 2,
                math::PI_ / 2};
        }
        if (radius[1] > 0.f)
        {
            Rect rect = {Vector2f{prect.min.X + radius[0], prect.max.Y - radius[1]}, Vector2f{prect.max.X - radius[1], prect.max.Y}};
            Rect uv = {Vector2f{params.uv.min.X + radius[0] * uvS.X, params.uv.max.Y - radius[1] * uvS.Y}, Vector2f{params.uv.max.X - radius[1] * uvS.X, params.uv.max.Y}};
            boxes[fanCount] = {
                rect, uv, params.color
            };

            fans[fanCount++] = {
                Vector2f(prect.max.X - radius[1], prect.max.Y - radius[1]),
                Rect{Vector2f(params.uv.max.X - radius[1] * uvS.X * 2, params.uv.max.Y - radius[0] * uvS.Y * 2), params.uv.max},
                params.color,
                radius[1],
                math::PI_ / 2,
                0};
        }
        if (radius[2] > 0.f)
        {
            Rect rect = {Vector2f{prect.max.X - radius[2], prect.min.Y + radius[2]}, Vector2f{prect.max.X, prect.max.Y - radius[1]}};
            Rect uv = {Vector2f{params.uv.max.X - radius[2] * uvS.X, params.uv.min.Y + radius[2] * uvS.Y}, Vector2f{params.uv.max.X, params.uv.max.Y - radius[1] * uvS.Y}};
            boxes[fanCount] = {
                rect, uv, params.color
            };

            fans[fanCount++] = {
                Vector2f(prect.max.X - radius[2], prect.min.Y + radius[2]),
                Rect{Vector2f(params.uv.max.X - radius[2] * uvS.X * 2, params.uv.min.Y), Vector2f(params.uv.max.X, params.uv.min.Y + radius[2] * uvS.Y * 2)},
                params.color,
                radius[2],
                math::PI_ / 2,
                -math::PI_ / 2};
        }
        if (radius[3] > 0.f)
        {
            Rect rect = {Vector2f{prect.min.X + radius[3], prect.min.Y}, Vector2f{prect.max.X - radius[2], prect.min.Y + radius[3]}};
            Rect uv = {Vector2f{params.uv.min.X + radius[3] * uvS.X, params.uv.min.Y}, Vector2f{params.uv.max.X - radius[2] * uvS.X, params.uv.min.Y + radius[3] * uvS.Y}};
            boxes[fanCount] = {
                rect, uv, params.color
            };

            fans[fanCount++] = {
                Vector2f(prect.min.X + radius[3], prect.min.Y + radius[3]),
                Rect{params.uv.min, Vector2f(params.uv.min.X + radius[3] * uvS.X * 2, params.uv.min.Y + radius[3] * uvS.Y * 2)},
                params.color,
                radius[3],
                math::PI_ / 2,
                -math::PI_};
        }
        {
            Rect rect = {Vector2f{prect.min.X + radius[0], prect.min.Y + radius[3]}, Vector2f{prect.max.X - radius[2], prect.max.Y - radius[1]}};
            Rect uv = {Vector2f{params.uv.min.X + radius[0] * uvS.X, params.uv.min.Y + radius[3] * uvS.Y}, Vector2f{params.uv.max.X - radius[2] * uvS.X, params.uv.max.Y - radius[1] * uvS.Y}};
            boxes[fanCount] = {
                rect, uv, params.color
            };
        }
        for (int i = 0; i < fanCount + 1; i++)
            BoxShape(list, boxes[i]);

        for (int i = 0; i < fanCount; i++)
            FanShape(list, fans[i], sampleCount);
    }
    
    void PrimitiveDraw::RoundBoxShape(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount /* = 10 */)
	{
        float radius = params.radius[0];
		//const uint32_t vcount = list.vertices.size();
		//const uint32_t icount = list.indices.size();

        // calculate info 
        Vector2f uvSize = params.uv.max - params.uv.min;
        Vector2f boxSize = params.rect.max - params.rect.min;
        Vector2f uvBorder = uvSize * radius / boxSize;

        // center rect 
		//    |     |  
		// --------------
		//    |  x  |  
		// --------------
		//    |     |   
        Rect centerRect{ params.rect.min + radius, params.rect.max - radius};
        Rect centerRectUV{ params.uv.min + uvBorder, params.uv.max - uvBorder };
        
        // draw order 
        //  4 |  1  | 5
        // ----  1  -----
        //  2 |  1  | 3
		// ----  1  -----
		//  6 |  1  | 7 
        
        // draw middle rect(1) 
        BoxShape(list, {
			Rect{Vector2f(centerRect.min.X, params.rect.min.Y), Vector2f(centerRect.max.X, params.rect.max.Y)},
			Rect{Vector2f(centerRectUV.min.X, params.rect.min.Y), Vector2f(centerRectUV.max.X, params.rect.max.Y)},
            params.color
        });

        // draw border rect(2->3) 
		BoxShape(list, {
			Rect{Vector2f(params.rect.min.X, centerRect.min.Y), Vector2f(centerRect.min.X, centerRect.max.Y)},
			Rect{Vector2f(params.uv.min.X, centerRectUV.min.Y),Vector2f(centerRectUV.min.X, centerRectUV.max.Y)},
			params.color
        });
		BoxShape(list, {
			Rect{Vector2f(centerRect.max.X, centerRect.min.Y), Vector2f(params.rect.max.X, centerRect.max.Y)},
			Rect{Vector2f(centerRectUV.max.X, centerRectUV.min.Y), Vector2f(params.rect.max.X, centerRectUV.max.Y)},
			params.color
        });

        // draw corner fan(4->7) 
        FanShape(list, {
            Vector2f(centerRect.min.X, centerRect.max.Y),
            Rect{Vector2f(params.uv.min.X, centerRectUV.max.Y), Vector2f(centerRectUV.min.X, params.uv.max.Y)},
            params.color,
            radius,
			math::PI_ / 2,
            math::PI_ / 2 });
		FanShape(list, {
			Vector2f(centerRect.max.X, centerRect.max.Y),
			Rect{Vector2f(centerRectUV.max.X, centerRectUV.max.Y), Vector2f(centerRectUV.max.X, centerRectUV.max.Y)},
			params.color,
            radius,
			math::PI_ / 2,
			0 });
		FanShape(list, {
			Vector2f(centerRect.min.X, centerRect.min.Y),
			Rect{Vector2f(params.uv.min.X, params.uv.min.Y), Vector2f(centerRectUV.min.X, centerRectUV.min.Y)},
			params.color,
            radius,
			math::PI_ / 2,
			-math::PI_ });
		FanShape(list, {
			Vector2f(centerRect.max.X, centerRect.min.Y),
			Rect{Vector2f(centerRectUV.min.X, params.uv.min.Y), Vector2f(params.uv.max.X, centerRectUV.min.Y)},
			params.color,
            radius,
			math::PI_ / 2,
			-math::PI_ / 2 });
    }
}