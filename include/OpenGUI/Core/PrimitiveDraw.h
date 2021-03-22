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

		// Call from DrawList.
		OGUI_API void DrawBox(PrimDrawList& context, const BoxParams& params);
		OGUI_API void DrawCheckBox0(
			PrimDrawList& context, const CheckBox0Params& params);	
	}
}
