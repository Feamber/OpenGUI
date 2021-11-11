#ifndef UE4Runtime
#pragma once

//https://www.w3.org/TR/2021/CRD-css-text-3-20210316
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/open_string.h"
#include <variant>

namespace godot
{
    class TextParagraph; 
}
namespace OGUI
{
    class TextElement : public VisualElement
    {
    public:
        TextElement();
        ~TextElement();
		std::string_view GetTypeName() override { return "TextElement"; }
		std::string_view GetFullTypeName() override { return "OGUI::TextElement"; }

        using InlineType = std::variant<ostr::string, VisualElement*, TextElement*>;
        std::vector<InlineType> _inlines;

        godot::TextParagraph* _paragraph = nullptr;
        bool _paragraphDirty = false;

        void AddInlineElement(VisualElement* element);
        void AddInlineText(TextElement* text);
        void AddText(ostr::string text);
        void BuildParagraph();

        void MarkLayoutDirty() override;
        void GetChildren(std::vector<VisualElement *>& Children) override;
        void DrawPrimitive(PrimitiveDraw::DrawContext &Ctx) override;
        void SyncYogaStyle() override;

        void BuildParagraphRec(godot::TextParagraph* p, const struct StyleText& txt);
    };
}
#endif