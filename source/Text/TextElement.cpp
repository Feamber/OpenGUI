#include "OpenGUI/Core/ostring/ostr.h"

#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Text/TextTypes.h"
#include "YGValue.h"
#include "Yoga.h"
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
#include "OpenGUI/Style2/ParseUtils.hpp"


// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
namespace OGUI
{

    class TextElementGlyphDraw : public godot::TextServer::GlyphDrawPolicy
    {
        public:
        godot::Color color = godot::Color(1,1,1);
        godot::Color get_color(const godot::TextServer::Glyph &glyph) override
        {
            return color;
        }
        std::optional<ComputedTransform> get_transform(const godot::TextServer::Glyph &glyph) override
        {
            return {};
        }
    };

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
        element->_physicalParent = this;
        UpdateRoot(element);
        _paragraphDirty = true;
    }

    void TextElement::AddInlineText(TextElement* text)
    {
        _inlines.push_back(InlineType{text});
        text->_layoutType = LayoutType::Inline;
        text->_physicalParent = this;
        UpdateRoot(text);
        _paragraphDirty = true;
    }

    void TextElement::AddText(ostr::string text)
    {
        _inlines.push_back(InlineType{text});
        _paragraphDirty = true;
    }

    void TextElement::AddBindText(Name attrName)
    {
        std::shared_ptr<BindText> newBind(new BindText());
        newBind->text = ostr::string("${") + attrName.ToStringView();
        newBind->text += '}';
        AddBind({attrName, &newBind->text, [this, newBind](bool isApply)
        {
            //olog::Info(u"Text binding updated, value:{}"_o.format(newBind->text));
            if(isApply)
                _paragraphDirty = true;
        }});

        _inlines.push_back(InlineType{newBind});
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
            _paragraphDirty = false;
        }
    }

    void TextElement::SyncParagraphStyle()
    {
        _paragraphDirty = true;
        auto& txt = StyleText::Get(_style);
        auto GetHAlign = [&]() //resolve
        {
            switch(txt.textAlign)
            {
                case TextAlign::Left: return godot::HALIGN_LEFT;
                case TextAlign::Right: return godot::HALIGN_RIGHT;
                case TextAlign::Center: return godot::HALIGN_CENTER;
                //TODO: handle direction
                case TextAlign::Start: return godot::HALIGN_LEFT;
                case TextAlign::End: return godot::HALIGN_RIGHT;
                case TextAlign::Justify: return godot::HALIGN_FILL;
                default:
                    break;
            }
            return godot::HALIGN_LEFT;
        };
        _paragraph->set_align(GetHAlign());
        _paragraph->set_line_height(txt.lineHeight);
    
    }

    void TextElement::BuildParagraphRec(godot::TextParagraph* p, const StyleText& txt)
    {
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [&](ostr::string& text) 
                { 
                    godot::Color color(txt.color.X, txt.color.Y, txt.color.Z, txt.color.W);
                    p->add_string((wchar_t*)text.raw().data(), _font, txt.fontSize, _drawPolicy); 
                },
                [&](VisualElement*& child) 
                { 
                    auto esize = child->GetSize();
                    p->add_object(child, {esize.X, esize.Y}); 
                },
                [&](TextElement*& child) 
                { 
                    auto& ctxt = StyleText::Get(child->_style);
                    child->BuildParagraphRec(p, ctxt);
                },
                [&](std::shared_ptr<BindText>& Bind) 
                { 
                    godot::Color color(txt.color.X, txt.color.Y, txt.color.Z, txt.color.W);
                    p->add_string((wchar_t*)Bind->text.raw().data(), _font, txt.fontSize, _drawPolicy); 
                }
            }, inl);
        }
    }
    
    void TextElement::DrawPrimitive(PrimitiveDraw::DrawContext &Ctx)
    {
        if(_layoutType == LayoutType::Inline)
            return;
        BuildParagraph();
        VisualElement::DrawPrimitive(Ctx);
        PrimitiveDraw::BeginDraw(Ctx.prims);
        auto Rect = GetRect();
        //_paragraph->draw_outline(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), 5, godot::Color(0, 0, 0), godot::Color(1, 0, 0));
        _paragraph->draw(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), godot::Color(1, 1, 1), godot::Color(1, 0, 0));
        PrimitiveDraw::EndDraw(Ctx.prims, _worldTransform);
    }

    void TextElement::MarkLayoutDirty()
    {
        Context::Get()._layoutDirty = true;
        YGNodeMarkDirty(_ygnode);
    }

    void TextElement::UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss)
    {
        VisualElement::UpdateStyle(damage, ss);
        if(HasFlag(damage, RestyleDamage::TextLayout))
            SyncParagraphStyle();
        auto text = &StyleText::Get(_style);
        if(HasFlag(damage, RestyleDamage::Text))
        {
            auto color = text->color;
            _drawPolicy->color = godot::Color{color.x, color.y, color.z, color.w};
        }
        if(HasFlag(damage, RestyleDamage::Font) || !_font)
        {
            std::vector<std::string_view> families;
            std::split(text->fontFamily, families, ",");
            if(!families.empty())
            {
                if(!_font)
                    _font.reset(new godot::Font);
                _font->clear_data();
                for(auto family : families)
                {
                    if(std::starts_with(family, "\"") && std::ends_with(family, "\""))
                        family = family.substr(1, family.size() - 2);
                    bool found = false;
                    for(auto sheet : ss)
                    {
                        auto iter = sheet->namedStyleFamilies.find(family);
                        if(iter == sheet->namedStyleFamilies.end())
                            continue;
                        auto& font = sheet->styleFonts[iter->second];
                        for(auto& data : font.datas)
                            _font->add_data(data);
                        found = true;
                        break;
                    }
                    if(!found)
                        olog::Warn(u"unknown font family name:{}"_o.format(family));
                }
                godot::Map<uint32_t, double> map;
                map[godot::TS->name_to_tag("weight")] = text->fontWeight;
                _font->set_variation_coordinates(map);
            }
        }
    }

    void TextElement::GetChildren(std::vector<VisualElement *>& children)
    {
        VisualElement::GetChildren(children);
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [](ostr::string& ) {},
                [](std::shared_ptr<BindText>& ) {},
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
        YGNodeSetMeasureFunc(_ygnode, MeasureText);
        YGNodeSetBaselineFunc(_ygnode, BaselineText);
        _drawPolicy = std::make_shared<TextElementGlyphDraw>();
    }

    TextElement::~TextElement()
    {
        if(_paragraph)
            delete _paragraph;
    }
}