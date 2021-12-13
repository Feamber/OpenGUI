#include <memory>
#pragma once

//https://www.w3.org/TR/2021/CRD-css-text-3-20210316
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/open_string.h"
#include <variant>

namespace godot
{
    class TextParagraph; 
    class Font;
}
namespace OGUI
{
    struct OGUI_API BindText : public std::enable_shared_from_this<BindText>
    {
        ostr::string text;
    };

    class TextElement : public VisualElement
    {
    public:
        TextElement();
        ~TextElement();
		std::string_view GetTypeName() override { return "TextElement"; }
		std::string_view GetFullTypeName() override { return "OGUI::TextElement"; }

        using InlineType = std::variant<ostr::string, VisualElement*, TextElement*, std::shared_ptr<BindText>>;
        std::vector<InlineType> _inlines;
        std::shared_ptr<class TextElementGlyphDraw> _drawPolicy;

        godot::TextParagraph* _paragraph = nullptr;
        std::shared_ptr<godot::Font> _font;
        bool _paragraphDirty = false;

        void AddInlineElement(VisualElement* element);
        void AddInlineText(TextElement* text);
        void AddText(ostr::string text);
        void AddBindText(Name attrName);
        void BuildParagraph();

        void MarkLayoutDirty(bool visibility) override;
        void UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss) override;
        void SyncParagraphStyle();
        void GetChildren(std::vector<VisualElement *>& Children) override;
        void DrawPrimitive(PrimitiveDraw::DrawContext &Ctx) override;

        void BuildParagraphRec(godot::TextParagraph* p, const struct StyleText& txt);
    };
}