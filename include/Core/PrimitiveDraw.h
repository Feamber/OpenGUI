#include "Math.h"

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
		void DrawBox(DrawContext& context, const BoxParams& params);
	}
}
