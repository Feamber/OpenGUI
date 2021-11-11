
#include "YGValue.h"
#include "Yoga.h"
#define DLL_IMPLEMENTATION
#ifndef UE4Runtime
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/Math/Vector.h"
#include <memory>
#include <vector>
#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Core/open_string.h"
#include "godot/text_paragraph.h"
#include "godot/font.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "godot/text_server_adv.h"
#include "OpenGUI/Style2/generated/text.h"


    // helper type for the visitor #4
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    // explicit deduction guide (not needed as of C++20)
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
namespace OGUI
{

    std::shared_ptr<godot::FontData> GetTestFontData()
    {
        static std::shared_ptr<godot::FontData> data;
        if(!data)
        {
            data.reset(new godot::FontData);
            auto& ctx = Context::Get().fileImpl;
            auto f = ctx->Open("res/fireflysung.ttf");
            if(!f)
                return nullptr;
            auto length = ctx->Length(f);
            godot::PackedByteArray buffer;
            buffer.resize(length);
            ctx->Read(buffer.data(), length, f);
            ctx->Close(f);
            data->set_data(buffer);
        }
        return data;
    }

    std::shared_ptr<godot::Font> GetTestFont()
    {
        static std::shared_ptr<godot::Font> instance;
        if(!instance)
        {
           
            instance.reset(new godot::Font);
            instance->add_data(GetTestFontData());
        }
        return instance;
    }

    YGSize MeasureText(
    YGNodeRef node,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode)
    {
        auto te = (TextElement*)YGNodeGetContext(node);
        te->BuildParagraph();
        switch (widthMode) {
        case YGMeasureModeExactly:
        case YGMeasureModeAtMost:
            te->_paragraph->set_max_width(width);
            break;
        default:
            te->_paragraph->set_max_width(-1);
        }

        switch (heightMode) {
        case YGMeasureModeExactly:
        case YGMeasureModeAtMost:
            te->_paragraph->set_max_height(height);
            break;
        default:
            te->_paragraph->set_max_height(-1);
        }

        auto inlineElements = te->_paragraph->get_objects();
        for(auto i : inlineElements)
        {
            auto ie = (VisualElement*)i;
            ie->CalculateLayout(); //TODO: dir
            //YGNodeCalculateLayout(ie->_ygnode, YGUndefined, YGUndefined, dir);
            auto esize = ie->GetSize();
            te->_paragraph->resize_object(i, {esize.X, esize.Y});
        }
        auto size = te->_paragraph->get_size();
        YGSize result;
    
        switch (widthMode) {
        case YGMeasureModeExactly:
            result.width = width;
            break;
        case YGMeasureModeAtMost:
        default:
            result.width = size.width;
        }

        switch (heightMode) {
        case YGMeasureModeExactly:
            result.height = height;
            break;
        case YGMeasureModeAtMost:
        default:
            result.height = size.height;
        }

        auto lineCount = te->_paragraph->get_line_count();
        Vector2f off{0, 0};
        Vector2f start{0, result.height};
        for(int i=0; i<lineCount; ++i)
        {
            off.y -= te->_paragraph->get_line_ascent(i) + te->_paragraph->get_spacing_top();
            auto lineElements = te->_paragraph->get_line_objects(i);
            for(auto j : lineElements)
            {
                auto je = (VisualElement*)j;
                auto rect = te->_paragraph->get_line_object_rect(i, j);
                je->_inlineLayout = {
                    Vector2f{rect.position.x, -rect.position.y - rect.size.y } + off + start,
                    Vector2f{rect.position.x+rect.size.x, -rect.position.y } + off + start
                };
            }
            off.y -= te->_paragraph->get_line_descent(i) + te->_paragraph->get_spacing_bottom();
        }
        return result;
    }

    float BaselineText(YGNodeRef node, float width, float height)
    {
        auto te = (TextElement*)YGNodeGetContext(node);
        return height - te->_paragraph->get_line_ascent(0);
    }

    void TextElement::AddInlineElement(VisualElement* element)
    {
        _inlines.push_back(InlineType{element});
        element->_layoutType = LayoutType::Inline;
        element->_physical_parent = this;
        UpdateRoot(element);
        _paragraphDirty = true;
    }

    void TextElement::AddInlineText(TextElement* text)
    {
        _inlines.push_back(InlineType{text});
        text->_layoutType = LayoutType::Inline;
        text->_physical_parent = this;
        UpdateRoot(text);
        _paragraphDirty = true;
    }

    void TextElement::AddText(ostr::string text)
    {
        _inlines.push_back(InlineType{text});
        _paragraphDirty = true;
    }

    void TextElement::BuildParagraph()
    {
        if(_paragraphDirty)
        {
            _paragraph->clear();
            auto& txt = StyleText::Get(_style);
            BuildParagraphRec(_paragraph, txt);
            MarkLayoutDirty();
        }
    }

    void TextElement::BuildParagraphRec(godot::TextParagraph* p, const StyleText& txt)
    {
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [&](ostr::string& text) 
                { 
                    p->add_string((wchar_t*)text.raw().data(), GetTestFont(), txt.fontSize); 
                },
                [&](VisualElement*& child) 
                { 
                    auto esize = child->GetSize();
                    p->add_object(child, {esize.X, esize.Y}); 
                },
                [&](TextElement*& child) 
                { 
                    child->BuildParagraphRec(p, txt); 
                }
            }, inl);
        }
    }
    
    void TextElement::DrawPrimitive(PrimitiveDraw::DrawContext &Ctx)
    {
        VisualElement::DrawPrimitive(Ctx);
        PrimitiveDraw::BeginDraw(Ctx.prims);
        auto Rect = GetRect();
        _paragraph->draw(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), godot::Color(1, 1, 1), godot::Color(1, 0, 0));
        PrimitiveDraw::EndDraw(Ctx.prims, _worldTransform, Ctx.resolution);
    }

    void TextElement::MarkLayoutDirty()
    {
        YGNodeMarkDirty(_ygnode);
    }

    void TextElement::SyncYogaStyle()
    {
        VisualElement::SyncYogaStyle();
    };

    void TextElement::GetChildren(std::vector<VisualElement *>& children)
    {
        VisualElement::GetChildren(children);
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [](ostr::string& ) {},
                [&](VisualElement*& child) 
                { 
                    children.push_back(child); 
                },
                [&](TextElement*& child) 
                { 
                    children.push_back(child); 
                }
            }, inl);
        }
    }
    
    TextElement::TextElement()
    {
        _paragraph = new godot::TextParagraph;
        _paragraph->set_on_dirty([this]() {MarkLayoutDirty();});
        _paragraph->set_align(godot::HALIGN_FILL);
        YGNodeSetMeasureFunc(_ygnode, MeasureText);
        YGNodeSetBaselineFunc(_ygnode, BaselineText);
    }

    TextElement::~TextElement()
    {
        if(_paragraph)
            delete _paragraph;
    }
}
#endif