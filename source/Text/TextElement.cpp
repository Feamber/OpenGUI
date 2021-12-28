#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/ostring/ostr.h"

#include "OpenGUI/Event/AttachEvent.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Shadow.h"
#include "OpenGUI/Style2/Transform.h"
#include "OpenGUI/Style2/generated/position.h"
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

    godot::InlineAlign GetInlineAlign(EInlineAlign o)
    {
        switch(o)
        {
            case EInlineAlign::Baseline:
                return godot::INLINE_ALIGN_BOTTOM;
            case EInlineAlign::Top:
                return godot::INLINE_ALIGN_TOP;
            case EInlineAlign::Middle:
                return godot::INLINE_ALIGN_CENTER;
        }
        return godot::INLINE_ALIGN_BOTTOM;
    }

    class TextElementGlyphDraw : public godot::TextServer::GlyphDrawPolicy
    {
        public:
        TextElement* root;
        Color4f color = Color4f::vector_one();
        std::vector<TextShadow> shadows;
        
        void draw(PrimDrawList &list, const Rect &inRect, TextureHandle texture, const Rect &inUv, const Color4f &inColor = Color4f::vector_one()) override
        {
            auto rect = inRect;
            auto uv = inUv;
            auto finalcolor = color * inColor;
            if(root->currShadowPass != -1)
            {
                if(shadows.size() <= root->currShadowPass)
                    return;
                auto& shadow = shadows[root->currShadowPass];
                rect.min += shadow.offset;
                rect.max += shadow.offset;
                finalcolor = shadow.color * inColor;
            }
            godot::TextServer::GlyphDrawPolicy::drawQuad(list, rect, texture, uv, finalcolor);
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
            auto& pos = StylePosition::Get(ie->_style);
            te->_paragraph->resize_object(i, {esize.X, esize.Y}, GetInlineAlign(pos.verticalAlign));
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

        te->UpdateInlineLayout();
        
        return result;
    }
    
    void TextElement::UpdateInlineLayout()
    {
        auto lineCount = _paragraph->get_line_count();
        auto dropcapLines = _paragraph->get_dropcap_lines();
        auto dropcapSize = _paragraph->get_dropcap_size();
        Vector2f off{0, 0};
		float maxWidth = _paragraph->get_max_width();
        for(int i=0; i<lineCount; ++i)
        {
            using namespace godot;
            off.y += _paragraph->get_line_ascent(i);
            auto lineElements = _paragraph->get_line_objects(i);
            auto align = _paragraph->get_align();
            float totalWidth = maxWidth;
            if(i <= dropcapLines)
            {
                if(TS->shaped_text_get_orientation(_paragraph->get_dropcap_rid()) == TextServer::ORIENTATION_HORIZONTAL)
                    totalWidth -= dropcapSize.x;
                else
                    totalWidth -= dropcapSize.y;
            }
            float line_width = _paragraph->get_line_width(i);
            float alignOff = 0.f;
			switch (align) {
				case HALIGN_FILL:
				case HALIGN_LEFT:
					break;
				case HALIGN_CENTER: 
					alignOff = std::floor((totalWidth - line_width) / 2.0);
					break;
				case HALIGN_RIGHT: 
					alignOff = totalWidth - line_width;
					break;
			}
            off.x = alignOff;
            for(auto j : lineElements)
            {
                auto je = (VisualElement*)j;
                auto rect = _paragraph->get_line_object_rect(i, j);
                je->_inlineLayout = {
                    Vector2f{rect.position.x, rect.position.y } + off,
                    Vector2f{rect.position.x+rect.size.x, rect.position.y + rect.size.y } + off
                };
            }
            off.y += _paragraph->get_line_descent(i);
        }
    }

    float BaselineText(YGNodeRef node, float width, float height)
    {
        auto te = (TextElement*)YGNodeGetContext(node);
        return height - te->_paragraph->get_line_ascent(0);
    }

    void TextElement::AddInlineElement(VisualElement* element)
    {
        {
            PreAttachEvent event;
            event.prevParent = element->GetHierachyParent();
            event.nextParent = this;
            RouteEvent(element, event);
        }
        _inlines.push_back(InlineType{element});
        element->_layoutType = LayoutType::Inline;
        element->_physicalParent = this;
        UpdateRoot(element);
        _paragraphDirty = true;
        {
            PostAttachEvent event;
            event.prevParent = element->GetHierachyParent();
            event.nextParent = this;
            RouteEvent(element, event);
        }
    }

    void TextElement::AddInlineText(TextElement* text)
    {
        {
            PreAttachEvent event;
            event.prevParent = text->GetHierachyParent();
            event.nextParent = this;
            RouteEvent(text, event);
        }
        _inlines.push_back(InlineType{text});
        text->_layoutType = LayoutType::Inline;
        text->_physicalParent = this;
        text->_drawPolicy->root = this;
        UpdateRoot(text);
        _paragraphDirty = true;
        {
            PostAttachEvent event;
            event.prevParent = text->GetHierachyParent();
            event.nextParent = this;
            RouteEvent(text, event);
        }
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
            MarkLayoutDirty(false);
            _paragraphDirty = false;
        }
    }

    void TextElement::SyncParagraphStyle()
    {
        _paragraphDirty = true;
        auto& txt = StyleText::Get(_style);
        if(txt.lineHeight.unit == YGUnitPercent)
        {
            _paragraph->set_line_height(txt.lineHeight.value / 100.f);
            _paragraph->set_spacing_bottom(0.f);
        }
        else if(txt.lineHeight.unit == YGUnitPoint)
        {
            _paragraph->set_line_height(1.f);
            _paragraph->set_spacing_bottom(txt.lineHeight.value);
        }
        else
        {
            _paragraph->set_line_height(1.f);
            _paragraph->set_spacing_bottom(0);
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
                    godot::Color color(txt.color.X, txt.color.Y, txt.color.Z, txt.color.W);
                    p->add_string((wchar_t*)text.raw().data(), _font, txt.fontSize, _drawPolicy); 
                },
                [&](VisualElement*& child) 
                { 
                    if(!child->Visible())
                        return;
                    auto& pos = StylePosition::Get(child->_style);
                    p->add_object(child, {0, 0}, GetInlineAlign(pos.verticalAlign)); 
                },
                [&](TextElement*& child) 
                { 
                    if(!child->Visible())
                        return;
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

    void TextElement::GetShadowPassRec(int& pass)
    {
        auto& txt = StyleText::Get(_style);
        pass = std::max(pass, (int)txt.textShadow.size());
        for(auto& inl : _inlines)
        {
            std::visit(overloaded
            {
                [&](ostr::string& text) 
                { 
                },
                [&](VisualElement*& child) 
                { 
                },
                [&](TextElement*& child) 
                { 
                    if(!child->Visible())
                        return;
                    child->GetShadowPassRec(pass);
                },
                [&](std::shared_ptr<BindText>& Bind) 
                { 
                }
            }, inl);
        }
    }
    
    void TextElement::DrawPrimitive(PrimDrawContext &Ctx)
    {
        if(_layoutType == LayoutType::Inline)
            return;
        BuildParagraph();
        //VisualElement::DrawPrimitive(Ctx);
        BeginDraw(Ctx.prims);
        auto Rect = GetRect();
        //_paragraph->draw_outline(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), 5, godot::Color(0, 0, 0), godot::Color(1, 0, 0));
        auto& txt = StyleText::Get(_style);
        int shadowPasses = 0;
        GetShadowPassRec(shadowPasses);
        auto gcolor = godot::Color(1, 1, 1, _opacity);
        for(int i=0;i<shadowPasses;++i)
        {
            currShadowPass = i;
            _paragraph->draw(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), gcolor, gcolor);
        }
        currShadowPass = -1;
        _paragraph->draw(Ctx.prims, godot::Vector2(Rect.min.x, Rect.min.y), gcolor, gcolor);
        EndDraw(Ctx.prims, _worldTransform);
    }

    void TextElement::MarkLayoutDirty(bool visibility)
    {
        if(visibility)
            _paragraphDirty = true;
            
        Context::Get()._layoutDirty = true;
        YGNodeMarkDirty(_ygnode);
    }

    void TextElement::UpdateStyle(RestyleDamage damage, const std::vector<StyleSheet*>& ss)
    {
        VisualElement::UpdateStyle(damage, ss);
        if(HasFlag(damage, RestyleDamage::TextLayout))
            SyncParagraphStyle();
        auto text = StyleText::Get(_style);
        auto color = text.color;
        _drawPolicy->color = color;
        _drawPolicy->shadows = text.textShadow;
        
        auto GetHAlign = [&]() //resolve
        {
            switch(text.textAlign)
            {
                case ETextAlign::Left: return godot::HALIGN_LEFT;
                case ETextAlign::Right: return godot::HALIGN_RIGHT;
                case ETextAlign::Center: return godot::HALIGN_CENTER;
                //TODO: handle direction
                case ETextAlign::Start: return godot::HALIGN_LEFT;
                case ETextAlign::End: return godot::HALIGN_RIGHT;
                case ETextAlign::Justify: return godot::HALIGN_FILL;
                default:
                    break;
            }
            return godot::HALIGN_LEFT;
        };
        if(_paragraph->get_align() != GetHAlign())
        {
            _paragraph->set_align(GetHAlign());
            if(!_paragraphDirty)
                UpdateInlineLayout();
        }
        if(HasFlag(damage, RestyleDamage::Font) || !_font)
        {
            if(!_font)
                _font.reset(new godot::Font);
            _font->clear_data();
            for(auto family : text.fontFamily)
            {
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
            map[godot::TS->name_to_tag("weight")] = text.fontWeight;
            _font->set_variation_coordinates(map);
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
        _paragraph->set_on_dirty([this]() {MarkLayoutDirty(false);});
        YGNodeSetMeasureFunc(_ygnode, MeasureText);
        YGNodeSetBaselineFunc(_ygnode, BaselineText);
        _drawPolicy = std::make_shared<TextElementGlyphDraw>();
        _drawPolicy->root = this;
    }

    TextElement::~TextElement()
    {
        if(_paragraph)
            delete _paragraph;
    }
}