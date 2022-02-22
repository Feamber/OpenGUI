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
namespace OGUI reflect
{
    struct OGUI_API BindText : public std::enable_shared_from_this<BindText>
    {
        ostr::string text;
    };

    class OGUI_API reflect TextElement : public VisualElement
    {
    public:
        TextElement();
        ~TextElement();
		ostr::string_view GetTypeName() override { return u"TextElement"; }
		ostr::string_view GetFullTypeName() override { return u"OGUI::TextElement"; }

        using InlineType = std::variant<ostr::string, VisualElement*, TextElement*, std::shared_ptr<BindText>>;
        std::vector<InlineType> _inlines;
        std::shared_ptr<class TextElementGlyphDraw> _drawPolicy;

        godot::TextParagraph* _paragraph = nullptr;
        std::shared_ptr<godot::Font> _font;
        bool _paragraphDirty = false;
        int currShadowPass = 0;

        //TODO: insertion api
        void AddInlineElement(VisualElement* element);
        void AddInlineText(TextElement* text);
        attr("script":true)
        void AddText(ostr::string text);
        void AddBindText(Name attrName);
        attr("script":true)
        void ClearText();
        void BuildParagraph();
        void UpdateInlineLayout();

        void MarkLayoutDirty(bool visibility) override;
        void UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss) override;
        void SyncParagraphStyle();
        void GetChildren(std::vector<VisualElement *>& Children) override;
        void RemoveChild(VisualElement* child) override;
        void ClearChildren() override;
        void DrawPrimitive(PrimDrawContext &Ctx) override;

        void BuildParagraphRec(godot::TextParagraph* p, const struct StyleText& txt);
        void GetShadowPassRec(int& pass);
    };
}