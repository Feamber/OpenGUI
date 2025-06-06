/*************************************************************************/
/*  text_paragraph.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "text_paragraph.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "text_server_adv.h"
#include "font.h"
using namespace godot;
void TextParagraph::_shape_lines() {
	if (lines_dirty) {
		for (int i = 0; i < lines_rid.size(); i++) {
			TS->free(lines_rid[i]);
		}
		lines_rid.clear();

		if (!tab_stops.is_empty()) {
			TS->shaped_text_tab_align(rid, tab_stops);
		}

		float h_offset = 0.f;
		float v_offset = 0.f;
		float h_total = 0.f;
		int start = 0;
		dropcap_lines = 0;

		int64_t overrun_flags = TextServer::OVERRUN_NO_TRIMMING;
		if (overrun_behavior != OVERRUN_NO_TRIMMING) {
			switch (overrun_behavior) {
				case OVERRUN_TRIM_WORD_ELLIPSIS:
					overrun_flags |= TextServer::OVERRUN_TRIM;
					overrun_flags |= TextServer::OVERRUN_TRIM_WORD_ONLY;
					overrun_flags |= TextServer::OVERRUN_ADD_ELLIPSIS;
					break;
				case OVERRUN_TRIM_ELLIPSIS:
					overrun_flags |= TextServer::OVERRUN_TRIM;
					overrun_flags |= TextServer::OVERRUN_ADD_ELLIPSIS;
					break;
				case OVERRUN_TRIM_WORD:
					overrun_flags |= TextServer::OVERRUN_TRIM;
					overrun_flags |= TextServer::OVERRUN_TRIM_WORD_ONLY;
					break;
				case OVERRUN_TRIM_CHAR:
					overrun_flags |= TextServer::OVERRUN_TRIM;
					break;
				case OVERRUN_NO_TRIMMING:
					break;
			}
		}

		if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
			h_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
			v_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
		} else {
			h_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
			v_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
		}
		bool lines_overflow = false;
		if (h_offset > 0) {
			// Dropcap, flow around.
			Vector<Vector2i> line_breaks = TS->shaped_text_get_line_breaks(rid, max_width - h_offset, 0, flags);
			for (int i = 0; i < line_breaks.size(); i++) {
				RID line = TS->shaped_text_substr(rid, line_breaks[i].x, line_breaks[i].y - line_breaks[i].x);
				float h = ((TS->shaped_text_get_orientation(line) == TextServer::ORIENTATION_HORIZONTAL) ? TS->shaped_text_get_size(line).y : TS->shaped_text_get_size(line).x) * line_height_scale + spacing_top + spacing_bottom;
				if (!tab_stops.is_empty()) {
					TS->shaped_text_tab_align(line, tab_stops);
				}
				dropcap_lines++;
				v_offset -= h;
				h_total += h;
				start = line_breaks[i].y;
				lines_rid.push_back(line);
				if (v_offset < 0) {
					break;
				}
			}
		}
		// Use fixed for the rest of lines.
		TS->shaped_text_update_justification_ops(rid);
		Vector<Vector2i> line_breaks = TS->shaped_text_get_line_breaks(rid, max_width, start, flags);
		for (int i = 0; i < line_breaks.size(); i++) {
			RID line = TS->shaped_text_substr(rid, line_breaks[i].x, line_breaks[i].y - line_breaks[i].x);
			if (!tab_stops.is_empty()) {
				TS->shaped_text_tab_align(line, tab_stops);
			}
			float h = ((TS->shaped_text_get_orientation(line) == TextServer::ORIENTATION_HORIZONTAL) ? TS->shaped_text_get_size(line).y : TS->shaped_text_get_size(line).x) * line_height_scale + spacing_top + spacing_bottom;
			h_total += h;
			if(max_height > 0 && h_total > max_height && lines_rid.size() > 0)
			{
				TS->free(line);
				TS->free(lines_rid.back());
				line = TS->shaped_text_substr(rid, line_breaks[i-1].x, line_breaks.back().y - line_breaks[i-1].x);
				if (!tab_stops.is_empty()) {
					TS->shaped_text_tab_align(line, tab_stops);
				}
				lines_rid.back() = line;
				lines_overflow = true;
				break;
			}
			lines_rid.push_back(line);
		}

		bool autowrap_enabled = ((flags & TextServer::BREAK_WORD_BOUND) == TextServer::BREAK_WORD_BOUND) || ((flags & TextServer::BREAK_GRAPHEME_BOUND) == TextServer::BREAK_GRAPHEME_BOUND);

		// Fill after min_size calculation.
		if (autowrap_enabled) {
			int visible_lines = (max_lines_visible >= 0) ? MIN(max_lines_visible, lines_rid.size()) : lines_rid.size();
			bool lines_hidden = visible_lines > 0 && visible_lines < lines_rid.size();
			if (lines_hidden) {
				overrun_flags |= TextServer::OVERRUN_ENFORCE_ELLIPSIS;
			}
			if (align == HALIGN_FILL) {
				for (int i = 0; i < lines_rid.size(); i++) {
					if ((i < visible_lines - 1 || lines_rid.size() == 1) && !TS->shaped_test_is_hard_break(lines_rid[i])) {
						
						TS->shaped_text_fit_to_width(lines_rid[i], i < dropcap_lines ? max_width - h_offset : max_width , flags);
					} else if (i == (visible_lines - 1)) {
						TS->shaped_text_overrun_trim_to_width(lines_rid[visible_lines - 1], max_width, overrun_flags);
					}
				}

			} else if (lines_hidden || lines_overflow) {
				TS->shaped_text_overrun_trim_to_width(lines_rid[visible_lines - 1], max_width, overrun_flags);
			}

		} else {
			// Autowrap disabled.
			for (int i = 0; i < lines_rid.size(); i++) {
				if (align == HALIGN_FILL) {
					if(!TS->shaped_test_is_hard_break(lines_rid[i]))
					{
						TS->shaped_text_fit_to_width(lines_rid[i], i < dropcap_lines ? max_width - h_offset : max_width , flags);
						overrun_flags |= TextServer::OVERRUN_JUSTIFICATION_AWARE;
						TS->shaped_text_overrun_trim_to_width(lines_rid[i], max_width, overrun_flags);
						TS->shaped_text_fit_to_width(lines_rid[i], i < dropcap_lines ? max_width - h_offset : max_width , flags | TextServer::JUSTIFICATION_CONSTRAIN_ELLIPSIS);
					}
					else 
					{
						TS->shaped_text_overrun_trim_to_width(lines_rid[i], max_width, overrun_flags);
					}
				} else {
					TS->shaped_text_overrun_trim_to_width(lines_rid[i], max_width, overrun_flags);
				}
			}
		}
		lines_dirty = false;
	}
}

void TextParagraph::mark_dirty()
{
	if(!lines_dirty)
	{
		lines_dirty = true;
		if(on_dirty)
			on_dirty();
	}
}

RID TextParagraph::get_rid() const {
	return rid;
}

RID TextParagraph::get_line_rid(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), RID());
	return lines_rid[p_line];
}

RID TextParagraph::get_dropcap_rid() const {
	return dropcap_rid;
}

void TextParagraph::clear() {
	for (int i = 0; i < lines_rid.size(); i++) {
		TS->free(lines_rid[i]);
	}
	lines_rid.clear();
	TS->shaped_text_clear(rid);
	TS->shaped_text_clear(dropcap_rid);
}

void TextParagraph::set_preserve_invalid(bool p_enabled) {
	TS->shaped_text_set_preserve_invalid(rid, p_enabled);
	TS->shaped_text_set_preserve_invalid(dropcap_rid, p_enabled);
	mark_dirty();
}

bool TextParagraph::get_preserve_invalid() const {
	return TS->shaped_text_get_preserve_invalid(rid);
}

void TextParagraph::set_preserve_control(bool p_enabled) {
	TS->shaped_text_set_preserve_control(rid, p_enabled);
	TS->shaped_text_set_preserve_control(dropcap_rid, p_enabled);
	mark_dirty();
}

bool TextParagraph::get_preserve_control() const {
	return TS->shaped_text_get_preserve_control(rid);
}

void TextParagraph::set_line_height(float p_line_height)
{
	if(line_height_scale != p_line_height)
	{
		line_height_scale = p_line_height;
		mark_dirty();
	}
}
float TextParagraph::get_line_height()
{
	return line_height_scale;
}

void TextParagraph::set_direction(TextServer::Direction p_direction) {
	TS->shaped_text_set_direction(rid, p_direction);
	TS->shaped_text_set_direction(dropcap_rid, p_direction);
	mark_dirty();
}

TextServer::Direction TextParagraph::get_direction() const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	return TS->shaped_text_get_direction(rid);
}

void TextParagraph::set_orientation(TextServer::Orientation p_orientation) {
	TS->shaped_text_set_orientation(rid, p_orientation);
	TS->shaped_text_set_orientation(dropcap_rid, p_orientation);
	mark_dirty();
}

TextServer::Orientation TextParagraph::get_orientation() const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	return TS->shaped_text_get_orientation(rid);
}

bool TextParagraph::set_dropcap(const String &p_text, const Ref<Font> &p_fonts, int p_size, const Rect2 &p_dropcap_margins, const Map<uint32_t, double> &p_opentype_features, const String &p_language) {
	ERR_FAIL_COND_V(!p_fonts, false);
	TS->shaped_text_clear(dropcap_rid);
	dropcap_margins = p_dropcap_margins;
	bool res = TS->shaped_text_add_string(dropcap_rid, p_text, p_fonts->get_rids(), p_size, flags, {}, p_opentype_features, p_language);
	mark_dirty();
	return res;
}

void TextParagraph::clear_dropcap() {
	dropcap_margins = Rect2();
	TS->shaped_text_clear(dropcap_rid);
	mark_dirty();
}

bool TextParagraph::add_string(const String &p_text, const Ref<Font> &p_fonts, int p_size, int64_t p_flags, 
	const std::shared_ptr<TextServer::GlyphDrawPolicy> &draw_policy, const Map<uint32_t, double> &p_opentype_features, const String &p_language, const TextDecorationData& decoration, float p_letter_spacing, float p_word_spacing) {
	ERR_FAIL_COND_V(!p_fonts, false);
	p_flags |= flags;
	bool res = TS->shaped_text_add_string(rid, p_text, p_fonts->get_rids(), p_size, p_flags, draw_policy, p_opentype_features, p_language, decoration, p_letter_spacing, p_word_spacing);
	mark_dirty();
	return res;
}

int TextParagraph::get_spacing_top() const {
	return spacing_top;
}

int TextParagraph::get_spacing_bottom() const {
	return spacing_bottom;
}

void TextParagraph::set_bidi_override(const Vector<Vector2i> &p_override) {
	TS->shaped_text_set_bidi_override(rid, p_override);
	mark_dirty();
}

bool TextParagraph::add_object(Variant p_key, const Size2 &p_size, InlineAlign p_inline_align, int p_length) {
	bool res = TS->shaped_text_add_object(rid, p_key, p_size, p_inline_align, p_length);
	mark_dirty();
	return res;
}

bool TextParagraph::resize_object(Variant p_key, const Size2 &p_size, InlineAlign p_inline_align) {
	if(TS->shaped_text_resize_object_raw(rid, p_key, p_size, p_inline_align))
	{
		mark_dirty();
		return true;
	}
	return false;
}

Vector<Variant> TextParagraph::get_objects() const
{
	return TS->shaped_text_get_objects(rid);
}

void TextParagraph::set_align(HAlign p_align) {
	if (align != p_align) {
		if (align == HALIGN_FILL || p_align == HALIGN_FILL) {
			align = p_align;
			mark_dirty();
		} else {
			align = p_align;
		}
	}
}

HAlign TextParagraph::get_align() const {
	return align;
}

void TextParagraph::tab_align(const Vector<float> &p_tab_stops) {
	tab_stops = p_tab_stops;
	mark_dirty();
}

void TextParagraph::set_flags(int64_t p_flags) {
	if (flags != p_flags) {
		flags = p_flags;
		mark_dirty();
	}
}

void TextParagraph::set_spacing_top(int spacing)
{
	if(spacing_top != spacing)
	{
		spacing_top = spacing;
		mark_dirty();
	}
}

void TextParagraph::set_spacing_bottom(int spacing)
{
	if(spacing_bottom != spacing)
	{
		spacing_bottom = spacing;
		mark_dirty();
	}
}

int64_t TextParagraph::get_flags() const {
	return flags;
}

void TextParagraph::set_text_overrun_behavior(TextParagraph::OverrunBehavior p_behavior) {
	if (overrun_behavior != p_behavior) {
		overrun_behavior = p_behavior;
		mark_dirty();
	}
}

TextParagraph::OverrunBehavior TextParagraph::get_text_overrun_behavior() const {
	return overrun_behavior;
}

void TextParagraph::set_max_width(float p_width) {
	if (max_width != p_width) {
		max_width = p_width;
		mark_dirty();
	}
}

float TextParagraph::get_max_width() const {
	return max_width;
}

void TextParagraph::set_max_height(float p_height)
{
	if (max_height != p_height) {
		max_height = p_height;
		mark_dirty();
	}
}

float TextParagraph::get_max_height() const
{
	return max_height;
}

Size2 TextParagraph::get_non_wraped_size() const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	if (TS->shaped_text_get_orientation(rid) == TextServer::ORIENTATION_HORIZONTAL) {
		return Size2(TS->shaped_text_get_size(rid).x, TS->shaped_text_get_size(rid).y + spacing_top + spacing_bottom);
	} else {
		return Size2(TS->shaped_text_get_size(rid).x + spacing_top + spacing_bottom, TS->shaped_text_get_size(rid).y);
	}
}

Size2 TextParagraph::get_size() const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	Size2 size;
	int visible_lines = (max_lines_visible >= 0) ? MIN(max_lines_visible, lines_rid.size()) : lines_rid.size();
	for (int i = 0; i < visible_lines; i++) {
		Size2 lsize = TS->shaped_text_get_size(lines_rid[i]);
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			size.x = MAX(size.x, lsize.x);
			size.y += lsize.y + spacing_top + spacing_bottom;
		} else {
			size.x += lsize.x + spacing_top + spacing_bottom;
			size.y = MAX(size.y, lsize.y);
		}
	}
	return size;
}

int TextParagraph::get_line_count() const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	return lines_rid.size();
}

void TextParagraph::set_max_lines_visible(int p_lines) {
	if (p_lines != max_lines_visible) {
		max_lines_visible = p_lines;
		mark_dirty();
	}
}

int TextParagraph::get_max_lines_visible() const {
	return max_lines_visible;
}

Vector<Variant> TextParagraph::get_line_objects(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), Vector<Variant>{});
	return TS->shaped_text_get_objects(lines_rid[p_line]);
}

Rect2 TextParagraph::get_line_object_rect(int p_line, Variant p_key) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), Rect2());
	return TS->shaped_text_get_object_rect(lines_rid[p_line], p_key);
}

Size2 TextParagraph::get_line_size(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), Size2());
	auto Size = TS->shaped_text_get_size(lines_rid[p_line]);
	if (TS->shaped_text_get_orientation(lines_rid[p_line]) == TextServer::ORIENTATION_HORIZONTAL) {
		return Size2(Size.x, Size.y * line_height_scale + spacing_top + spacing_bottom);
	} else {
		return Size2(Size.x * line_height_scale + spacing_top + spacing_bottom, Size.y);
	}
}

Vector2i TextParagraph::get_line_range(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), Vector2i());
	return TS->shaped_text_get_range(lines_rid[p_line]);
}

float TextParagraph::get_line_ascent(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), 0.f);
	return TS->shaped_text_get_ascent(lines_rid[p_line]) * line_height_scale + spacing_top;
}

float TextParagraph::get_line_descent(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), 0.f);
	return TS->shaped_text_get_descent(lines_rid[p_line]) * line_height_scale + spacing_bottom;
}

float TextParagraph::get_line_width(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), 0.f);
	return TS->shaped_text_get_width(lines_rid[p_line]);
}

float TextParagraph::get_line_underline_position(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), 0.f);
	return TS->shaped_text_get_underline_position(lines_rid[p_line]);
}

float TextParagraph::get_line_underline_thickness(int p_line) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND_V(p_line < 0 || p_line >= lines_rid.size(), 0.f);
	return TS->shaped_text_get_underline_thickness(lines_rid[p_line]);
}

Size2 TextParagraph::get_dropcap_size() const {
	return TS->shaped_text_get_size(dropcap_rid) + dropcap_margins.size + dropcap_margins.position;
}

int TextParagraph::get_dropcap_lines() const {
	return dropcap_lines;
}

void TextParagraph::draw(OGUI::PrimDrawContext& list, const Vector2 &p_pos, const Color &p_color, const Color &p_dc_color) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	Vector2 ofs = p_pos;
	float h_offset = 0.f;
	if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
		h_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
	} else {
		h_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
	}

	if (h_offset > 0) {
		// Draw dropcap.
		Vector2 dc_off = ofs;
		if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_RTL) {
			if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
				dc_off.x += max_width - h_offset;
			} else {
				dc_off.y += max_width - h_offset;
			}
		}
		TS->shaped_text_draw(dropcap_rid, list, dc_off + Vector2(0, TS->shaped_text_get_ascent(dropcap_rid) + dropcap_margins.size.y + dropcap_margins.position.y / 2), -1, -1, p_dc_color);
	}

	int lines_visible = (max_lines_visible >= 0) ? MIN(max_lines_visible, lines_rid.size()) : lines_rid.size();

	for (int i = 0; i < lines_visible; i++) {
		float l_width = max_width;
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			ofs.x = p_pos.x;
			ofs.y += TS->shaped_text_get_ascent(lines_rid[i]) * line_height_scale + spacing_top;
			if (i < dropcap_lines) {
				if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_LTR || TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_AUTO) {
					ofs.x += h_offset;
				}
				l_width -= h_offset;
			}
		} else {
			ofs.y = p_pos.y;
			ofs.x += TS->shaped_text_get_ascent(lines_rid[i]) * line_height_scale + spacing_top;
			if (i < dropcap_lines) {
				if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_LTR || TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_AUTO) {
					ofs.x += h_offset;
				}
				l_width -= h_offset;
			}
		}
		float line_width = TS->shaped_text_get_width(lines_rid[i]);
		if (max_width > 0) {
			float offset = 0.f;
			switch (align) {
				case HALIGN_FILL:
				case HALIGN_LEFT:
					break;
				case HALIGN_CENTER: 
					offset = std::floor((l_width - line_width) / 2.0);
					break;
				case HALIGN_RIGHT: 
					offset = l_width - line_width;
					break;
			}
			
			if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
				ofs.x += offset;
			} else {
				ofs.y += offset;
			}
		}
		float clip_l;
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			clip_l = MAX(0, p_pos.x - ofs.x);
		} else {
			clip_l = MAX(0, p_pos.y - ofs.y);
		}
		TS->shaped_text_draw(lines_rid[i], list, ofs, clip_l, clip_l + l_width, p_color);
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			ofs.x = p_pos.x;
			ofs.y += TS->shaped_text_get_descent(lines_rid[i]) * line_height_scale + spacing_bottom;
		} else {
			ofs.y = p_pos.y;
			ofs.x += TS->shaped_text_get_descent(lines_rid[i]) * line_height_scale + spacing_bottom;
		}
	}
}

void TextParagraph::draw_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_outline_size, const Color &p_color, const Color &p_dc_color) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	Vector2 ofs = p_pos;

	float h_offset = 0.f;
	if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
		h_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
	} else {
		h_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
	}

	if (h_offset > 0) {
		// Draw dropcap.
		Vector2 dc_off = ofs;
		if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_RTL) {
			if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
				dc_off.x += max_width - h_offset;
			} else {
				dc_off.y += max_width - h_offset;
			}
		}
		TS->shaped_text_draw_outline(dropcap_rid, list, dc_off + Vector2(dropcap_margins.position.x, TS->shaped_text_get_ascent(dropcap_rid) + dropcap_margins.position.y), -1, -1, p_outline_size, p_dc_color);
	}

	for (int i = 0; i < lines_rid.size(); i++) {
		float l_width = max_width;
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			ofs.x = p_pos.x;
			ofs.y += TS->shaped_text_get_ascent(lines_rid[i]) + spacing_top;
			if (i <= dropcap_lines) {
				if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_LTR) {
					ofs.x += h_offset;
				}
				l_width -= h_offset;
			}
		} else {
			ofs.y = p_pos.y;
			ofs.x += TS->shaped_text_get_ascent(lines_rid[i]) + spacing_top;
			if (i <= dropcap_lines) {
				if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_LTR) {
					ofs.x += h_offset;
				}
				l_width -= h_offset;
			}
		}
		float length = TS->shaped_text_get_width(lines_rid[i]);
		if (max_width > 0) {
			switch (align) {
				case HALIGN_FILL:
					if (TS->shaped_text_get_direction(lines_rid[i]) == TextServer::DIRECTION_RTL) {
						if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
							ofs.x += l_width - length;
						} else {
							ofs.y += l_width - length;
						}
					}
					break;
				case HALIGN_LEFT:
					break;
				case HALIGN_CENTER: {
					if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
						ofs.x += std::floor((l_width - length) / 2.0);
					} else {
						ofs.y += std::floor((l_width - length) / 2.0);
					}
				} break;
				case HALIGN_RIGHT: {
					if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
						ofs.x += l_width - length;
					} else {
						ofs.y += l_width - length;
					}
				} break;
			}
		}
		float clip_l;
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			clip_l = MAX(0, p_pos.x - ofs.x);
		} else {
			clip_l = MAX(0, p_pos.y - ofs.y);
		}
		TS->shaped_text_draw_outline(lines_rid[i], list, ofs, clip_l, clip_l + l_width, p_outline_size, p_color);
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			ofs.x = p_pos.x;
			ofs.y += TS->shaped_text_get_descent(lines_rid[i]) + spacing_bottom;
		} else {
			ofs.y = p_pos.y;
			ofs.x += TS->shaped_text_get_descent(lines_rid[i]) + spacing_bottom;
		}
	}
}

int TextParagraph::hit_test(const Point2 &p_coords) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	Vector2 ofs;
	if (TS->shaped_text_get_orientation(rid) == TextServer::ORIENTATION_HORIZONTAL) {
		if (ofs.y < 0) {
			return 0;
		}
	} else {
		if (ofs.x < 0) {
			return 0;
		}
	}
	for (int i = 0; i < lines_rid.size(); i++) {
		if (TS->shaped_text_get_orientation(lines_rid[i]) == TextServer::ORIENTATION_HORIZONTAL) {
			if ((p_coords.y >= ofs.y) && (p_coords.y <= ofs.y + TS->shaped_text_get_size(lines_rid[i]).y)) {
				return TS->shaped_text_hit_test_position(lines_rid[i], p_coords.x);
			}
			ofs.y += TS->shaped_text_get_size(lines_rid[i]).y + spacing_bottom + spacing_top;
		} else {
			if ((p_coords.x >= ofs.x) && (p_coords.x <= ofs.x + TS->shaped_text_get_size(lines_rid[i]).x)) {
				return TS->shaped_text_hit_test_position(lines_rid[i], p_coords.y);
			}
			ofs.y += TS->shaped_text_get_size(lines_rid[i]).x + spacing_bottom + spacing_top;
		}
	}
	return TS->shaped_text_get_range(rid).y;
}

void TextParagraph::draw_dropcap(OGUI::PrimDrawContext& list, const Vector2 &p_pos, const Color &p_color) const {
	Vector2 ofs = p_pos;
	float h_offset = 0.f;
	if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
		h_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
	} else {
		h_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
	}

	if (h_offset > 0) {
		// Draw dropcap.
		if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_RTL) {
			if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
				ofs.x += max_width - h_offset;
			} else {
				ofs.y += max_width - h_offset;
			}
		}
		TS->shaped_text_draw(dropcap_rid, list, ofs + Vector2(dropcap_margins.position.x, TS->shaped_text_get_ascent(dropcap_rid) + dropcap_margins.position.y), -1, -1, p_color);
	}
}

void TextParagraph::draw_dropcap_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_outline_size, const Color &p_color) const {
	Vector2 ofs = p_pos;
	float h_offset = 0.f;
	if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
		h_offset = TS->shaped_text_get_size(dropcap_rid).x + dropcap_margins.size.x + dropcap_margins.position.x;
	} else {
		h_offset = TS->shaped_text_get_size(dropcap_rid).y + dropcap_margins.size.y + dropcap_margins.position.y;
	}

	if (h_offset > 0) {
		// Draw dropcap.
		if (TS->shaped_text_get_direction(dropcap_rid) == TextServer::DIRECTION_RTL) {
			if (TS->shaped_text_get_orientation(dropcap_rid) == TextServer::ORIENTATION_HORIZONTAL) {
				ofs.x += max_width - h_offset;
			} else {
				ofs.y += max_width - h_offset;
			}
		}
		TS->shaped_text_draw_outline(dropcap_rid, list, ofs + Vector2(dropcap_margins.position.x, TS->shaped_text_get_ascent(dropcap_rid) + dropcap_margins.position.y), -1, -1, p_outline_size, p_color);
	}
}

void TextParagraph::draw_line(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_line, const Color &p_color) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND(p_line < 0 || p_line >= lines_rid.size());

	Vector2 ofs = p_pos;

	if (TS->shaped_text_get_orientation(lines_rid[p_line]) == TextServer::ORIENTATION_HORIZONTAL) {
		ofs.y += TS->shaped_text_get_ascent(lines_rid[p_line]) + spacing_top;
	} else {
		ofs.x += TS->shaped_text_get_ascent(lines_rid[p_line]) + spacing_top;
	}
	return TS->shaped_text_draw(lines_rid[p_line], list, ofs, -1, -1, p_color);
}

void TextParagraph::draw_line_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_line, int p_outline_size, const Color &p_color) const {
	const_cast<TextParagraph *>(this)->_shape_lines();
	ERR_FAIL_COND(p_line < 0 || p_line >= lines_rid.size());

	Vector2 ofs = p_pos;
	if (TS->shaped_text_get_orientation(lines_rid[p_line]) == TextServer::ORIENTATION_HORIZONTAL) {
		ofs.y += TS->shaped_text_get_ascent(lines_rid[p_line]) + spacing_top;
	} else {
		ofs.x += TS->shaped_text_get_ascent(lines_rid[p_line]) + spacing_top;
	}
	return TS->shaped_text_draw_outline(lines_rid[p_line], list, ofs, -1, -1, p_outline_size, p_color);
}

TextParagraph::TextParagraph() {
	rid = TS->create_shaped_text();
	dropcap_rid = TS->create_shaped_text();
}

TextParagraph::~TextParagraph() {
	for (int i = 0; i < lines_rid.size(); i++) {
		TS->free(lines_rid[i]);
	}
	lines_rid.clear();
	TS->free(rid);
	TS->free(dropcap_rid);
}
