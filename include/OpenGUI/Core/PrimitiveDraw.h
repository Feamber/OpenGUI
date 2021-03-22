#pragma once
#include "Primitive.h"

namespace OGUI
{
	struct ITexture;
	namespace PrimitiveDraw
	{
		struct DrawContext
		{

		}; 
		struct BoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			ITexture* texture;
			//Radius
			//SVG
			//Material

			static BoxParams MakeSolid(Rect rect, Color4f color);
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
			float radius;
			Rect uv;
			Color4f color;
			ITexture* texture;
		};
		struct FanParams
		{
			Vector2f pos;
			float radius;
			float degree;
			float beginDegree;
			Rect uv;
			Color4f color;
			ITexture* texture;
		};
		struct RoundBoxParams
		{
			Rect rect;
			Rect uv;
			Color4f color;
			float radius;
			ITexture* texture;
		};

		// Call from DrawList.
		OGUI_API void DrawBox(PrimDrawList& list, const BoxParams& params);
		OGUI_API void DrawCheckBox0(
			PrimDrawList& context, const CheckBox0Params& params);
		OGUI_API void DrawCircle(PrimDrawList& list, const CircleParams& params, int32_t sampleCount = 10);
		OGUI_API void DrawFan(PrimDrawList& list, const FanParams& params, int32_t sampleCount = 10);
		OGUI_API void DrawRoundBox(PrimDrawList& list, const RoundBoxParams& params, int32_t sampleCount = 10);
	}
}
