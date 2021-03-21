#pragma once
#include <vector>
#include <string>
#include "OpenGUI/Core/Math.h"
#include "OpenGUI/Style/Style.h"
#include "yoga/Yoga.h"
#include "OpenGUI/Style/StyleSheet.h"
#include "OpenGUI/Style/Style.h"


namespace OGUI
{
	namespace PrimitiveDraw
	{
		struct DrawContext;
	}

	struct Matrix4x4f{};

	struct StyleSheet;

	class VisualElement
	{
	public:
		virtual ~VisualElement();
		virtual void DrawPrimitive(PrimitiveDraw::DrawContext& Ctx);
		VisualElement* GetParent();
		VisualElement* GetHierachyParent();
		const std::vector<StyleSheet*>& GetStyleSheets();
		bool IsA(std::string type);
		std::string GetTypeName();
		std::string GetFullTypeName();

		std::string name;
		std::vector<std::string> classes;
	protected:
		void DrawBackgroundPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void DrawBorderPrimitive(PrimitiveDraw::DrawContext& Ctx);
		void ApplyClipping(PrimitiveDraw::DrawContext& Ctx);
		void CreateYogaNode();

#pragma region Hierachy
		std::vector<VisualElement*> _children;
		VisualElement* _physical_parent;
		//There could be some node between logical parent and this widget for layout
		VisualElement* _logical_parent;
#pragma endregion

#pragma region Transform
		Vector3f _position;
		Vector3f _rotation;
		Vector3f _scale;
		Matrix4x4f _worldTransform;
		//Rect _layout;
#pragma endregion

#pragma region Style
	public:
		YGNodeRef _ygnode;
		bool _hasInlineStyle;
		uint32_t _triggerPseudoMask;
		uint32_t _dependencyPseudoMask;
		uint32_t _pseudoMask;
		int _inheritedStylesHash = 0;
		StyleRule _inlineRule;
		StyleSheet* _inlineSheet;
		StyleSheetStorage _procedureSheet;
		StyleRule _procedureRule;

		Style _style;
		Style* _sharedStyle;
		std::vector<StyleSheet*> _styleSheets;

		Rect GetLayout();
		void CalculateLayout();
		void SetSharedStyle(Style* style);
		void SyncYogaStyle();
		bool ContainClass(std::string_view c);
#pragma endregion

	public:
		template<class F>
		void Traverse(F&& f, int depth = 0);
	};
}
#include "VisualElement.inl"