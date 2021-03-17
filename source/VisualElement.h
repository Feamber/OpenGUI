#pragma once
#include <vector>
namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	}

	class VisualElement
	{
	public:
		virtual void DrawPrimitive(PrimitiveDraw::DrawContext& Ctx);
		VisualElement* GetParent();

	protected:
		void DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void DrawBorderPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		std::vector<VisualElement*> _children;
		VisualElement* _physical_parent;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logical_parent;
	};
}