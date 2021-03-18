#pragma once
#include <vector>
#include "Core/Math.h"
#include "yoga/Yoga.h"
#include "../thirdparty/function_ref/function_ref.hpp"
namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	}

	struct Matrix4x4f{};

	class VisualElement
	{
	public:
		virtual ~VisualElement();
		virtual void DrawPrimitive(PrimitiveDraw::DrawContext& Ctx);
		VisualElement* GetParent();
		Rect GetLayout();

	protected:
		void DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void DrawBorderPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		void CreateYogaNode();
		std::vector<VisualElement*> _children;
		VisualElement* _physical_parent;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logical_parent;

		Vector3f _position;
		Vector3f _rotation;
		Vector3f _scale;
		Matrix4x4f _worldTransform;
		//Rect _layout;

		YGNodeRef _ygnode;


		bool _hasInlineStyle;
	};
}