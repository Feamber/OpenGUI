#pragma once
#include "Primitive.h"

namespace OGUI
{
	struct ITexture;
	namespace PrimitiveDraw
	{
		struct DrawContext
		{
			PrimDrawList prims;
		}; 

		struct BoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			ITexture* texture;
			static BoxParams MakeSolid(Rect rect, Color4f color);
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
			ITexture* texture;
			float radius;
		};
		struct FanParams
		{
			Vector2f pos;
			Rect uv;
			Color4f color;
			ITexture* texture;
			float radius;
			float degree;
			float beginDegree;
		};
		struct RoundBoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			ITexture* texture;
			float radius;
		};
		struct LineParams
		{
			std::vector<Vector2f> points;
			Rect uv;
			Color4f color;
			ITexture* texture;
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
