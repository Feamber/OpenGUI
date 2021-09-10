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

        using InlineType = std::variant<ostr::string, VisualElement*, TextElement*>;
        std::vector<InlineType> _inlines;

        godot::TextParagraph* _paragraph = nullptr;

        void GetChildren(std::vector<VisualElement *>& Children) override;
        void DrawPrimitive(PrimitiveDraw::DrawContext &Ctx) override;
        void SyncYogaStyle() override;
    };
}
#endif