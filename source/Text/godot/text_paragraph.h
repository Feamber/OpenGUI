/*************************************************************************/
/*  text_paragraph.h                                                     */
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

#pragma once

#include "OpenGUI/Interface/Interfaces.h"
#include "text_server.h"

namespace godot{
/*************************************************************************/
class Font;

class TextParagraph {

public:
	enum OverrunBehavior {
		OVERRUN_NO_TRIMMING,
		OVERRUN_TRIM_CHAR,
		OVERRUN_TRIM_WORD,
		OVERRUN_TRIM_ELLIPSIS,
		OVERRUN_TRIM_WORD_ELLIPSIS,
	};

private:
	RID dropcap_rid;
	int dropcap_lines = 0;
	Rect2 dropcap_margins;

	RID rid;
	Vector<RID> lines_rid;
	int spacing_top = 0;
	int spacing_bottom = 0;

	bool lines_dirty = true;
	float line_height_scale = 1.0;
	float max_width = -1.0;
	float max_height = -1.0;
	int max_lines_visible = -1;

	int64_t flags = TextServer::BREAK_MANDATORY | TextServer::BREAK_WORD_BOUND | TextServer::JUSTIFICATION_KASHIDA | TextServer::JUSTIFICATION_TRIM_EDGE_SPACES;
	OverrunBehavior overrun_behavior = OVERRUN_TRIM_ELLIPSIS;

	HAlign align = HALIGN_LEFT;

	Vector<float> tab_stops;
	std::function<void()> on_dirty;

protected:

	void _shape_lines();
	void mark_dirty();

public:
	RID get_rid() const;
	RID get_line_rid(int p_line) const;
	RID get_dropcap_rid() const;

	void clear();

	void set_on_dirty(std::function<void()> p_on_dirty) { on_dirty = p_on_dirty; }
	void set_direction(TextServer::Direction p_direction);
	TextServer::Direction get_direction() const;

	void set_orientation(TextServer::Orientation p_orientation);
	TextServer::Orientation get_orientation() const;

	void set_preserve_invalid(bool p_enabled);
	bool get_preserve_invalid() const;

	void set_preserve_control(bool p_enabled);
	bool get_preserve_control() const;

	void set_line_height(float p_line_height);
	float get_line_height();

	void set_bidi_override(const Vector<Vector2i> &p_override);

	bool set_dropcap(const String &p_text, const Ref<Font> &p_fonts, int p_size, const Rect2 &p_dropcap_margins = Rect2(), const Map<uint32_t, double> &p_opentype_features = {}, const String &p_language = "");
	void clear_dropcap();

	bool add_string(const String &p_text, const Ref<Font> &p_fonts, int p_size, int64_t flags = 0, const std::shared_ptr<TextServer::GlyphDrawPolicy> &draw_policy = {}, 
		const Map<uint32_t, double> &p_opentype_features = {}, const String &p_language = "", const TextDecorationData& decoration = {});
	bool add_object(Variant p_key, const Size2 &p_size, InlineAlign p_inline_align = INLINE_ALIGN_CENTER, int p_length = 1);
	bool resize_object(Variant p_key, const Size2 &p_size, InlineAlign p_inline_align = INLINE_ALIGN_CENTER);
	Vector<Variant> get_objects() const;

	void set_align(HAlign p_align);
	HAlign get_align() const;

	void tab_align(const Vector<float> &p_tab_stops);

	void set_flags(int64_t p_flags);
	int64_t get_flags() const;

	void set_text_overrun_behavior(OverrunBehavior p_behavior);
	OverrunBehavior get_text_overrun_behavior() const;

	void set_max_width(float p_width);
	float get_max_width() const;
	void set_max_height(float p_height);
	float get_max_height() const;

	void set_max_lines_visible(int p_lines);
	int get_max_lines_visible() const;

	Size2 get_non_wraped_size() const;

	Size2 get_size() const;

	int get_line_count() const;

	Vector<Variant> get_line_objects(int p_line) const;
	Rect2 get_line_object_rect(int p_line, Variant p_key) const;
	Size2 get_line_size(int p_line) const;
	float get_line_ascent(int p_line) const;
	float get_line_descent(int p_line) const;
	float get_line_width(int p_line) const;
	Vector2i get_line_range(int p_line) const;
	float get_line_underline_position(int p_line) const;
	float get_line_underline_thickness(int p_line) const;

	int get_spacing_top() const;
	int get_spacing_bottom() const;
	void set_spacing_top(int spacing);
	void set_spacing_bottom(int spacing);

	Size2 get_dropcap_size() const;
	int get_dropcap_lines() const;

	void draw(OGUI::PrimDrawContext& list, const Vector2 &p_pos, const Color &p_color = Color(1, 1, 1), const Color &p_dc_color = Color(1, 1, 1)) const;
	void draw_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_outline_size = 1, const Color &p_color = Color(1, 1, 1), const Color &p_dc_color = Color(1, 1, 1)) const;

	void draw_line(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_line, const Color &p_color = Color(1, 1, 1)) const;
	void draw_line_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_line, int p_outline_size = 1, const Color &p_color = Color(1, 1, 1)) const;

	void draw_dropcap(OGUI::PrimDrawContext& list, const Vector2 &p_pos, const Color &p_color = Color(1, 1, 1)) const;
	void draw_dropcap_outline(OGUI::PrimDrawContext& list, const Vector2 &p_pos, int p_outline_size = 1, const Color &p_color = Color(1, 1, 1)) const;

	int hit_test(const Point2 &p_coords) const;

	TextParagraph();
	~TextParagraph();
};
}