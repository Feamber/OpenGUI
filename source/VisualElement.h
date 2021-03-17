#pragma once
namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	}

	class VisualElement
	{
		virtual void GeneratePrimitive(PrimitiveDraw::DrawContext& Ctx);
	};
}