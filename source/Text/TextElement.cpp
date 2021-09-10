#define DLL_IMPLEMENTATION
#ifndef UE4Runtime
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
        return result;
    }

    float BaselineText(YGNodeRef node, float width, float height)
    {
        auto te = (TextElement*)YGNodeGetContext(node);
        return height - te->_paragraph->get_line_ascent(0);
    }
    
    void TextElement::DrawPrimitive(PrimitiveDraw::DrawContext &Ctx)
    {
        //VisualElement::DrawPrimitive(Ctx);
        PrimitiveDraw::BeginDraw(Ctx.prims);
        auto Rect = GetRect();
        _paragraph->draw(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), godot::Color(1, 1, 1), godot::Color(1, 0, 0));
        PrimitiveDraw::EndDraw(Ctx.prims, _worldTransform, Ctx.resolution);
    }

    void TextElement::SyncYogaStyle()
    {
        VisualElement::SyncYogaStyle();
        if(_style.width != YGValueAuto && _style.width != YGValueUndefined)
            _paragraph->set_max_width(_style.width.value);
        if(_style.height != YGValueAuto && _style.height != YGValueUndefined)
            _paragraph->set_max_height(_style.height.value);
    };

    // helper type for the visitor #4
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    // explicit deduction guide (not needed as of C++20)
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    void TextElement::GetChildren(std::vector<VisualElement *>& children)
    {
        VisualElement::GetChildren(children);
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [](ostr::string& ) {},
                [&](VisualElement*& child) { children.push_back(child); },
                [&](TextElement*& child) { children.push_back(child); }
            }, inl);
        }
    }
    
    TextElement::TextElement()
    {
        _paragraph = new godot::TextParagraph;
        const wchar_t* literal = L" name is Van.I'm an artist, I'm a performance artist.\nWilliam Shakespeare was an English playwright, poet, and actor, widely regarded as the greatest writer in the English language and the world's greatest dramatist. He is often called England's national poet and the \"Bard of Avon\".";
        const wchar_t* literal2 = L"真的猛士，敢于直面惨淡的人生，敢于正视淋漓的鲜血。";
        const wchar_t* literal3 = L"这是怎样的哀痛者和幸福者？然而造化又常常为庸人设计，以时间的流逝，来洗涤旧迹，仅是留下淡红的血色和微漠的悲哀。在这淡红的血色和微漠的悲哀中，又给人暂得偷生，维持着这似人非人的世界。"; 
        _paragraph->set_dropcap("My", GetTestFont(), 39);
        _paragraph->add_string(literal, GetTestFont(), 20);
        _paragraph->add_string(literal2, GetTestFont(), 25, godot::Color(0, 0, 1));
        _paragraph->add_string(literal3, GetTestFont(), 20);
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