/*************************************************************************/
/*  text_server.cpp                                                      */
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

#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/nanovg.h"
#include "OpenGUI/Style2/Transform.h"
#include "text_server.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Interface/Interfaces.h"

using namespace godot;
/*************************************************************************/

bool TextServer::Glyph::operator==(const Glyph &p_a) const {
	return (p_a.index == index) && (p_a.font_rid == font_rid) && (p_a.font_size == font_size) && (p_a.start == start);
}

bool TextServer::Glyph::operator!=(const Glyph &p_a) const {
	return (p_a.index != index) || (p_a.font_rid != font_rid) || (p_a.font_size != font_size) || (p_a.start != start);
}

bool TextServer::Glyph::operator<(const Glyph &p_a) const {
	if (p_a.start == start) {
		if (p_a.count == count) {
			if ((p_a.flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) {
				return true;
			} else {
				return false;
			}
		}
		return p_a.count > count;
	}
	return p_a.start < start;
}

bool TextServer::Glyph::operator>(const Glyph &p_a) const {
	if (p_a.start == start) {
		if (p_a.count == count) {
			if ((p_a.flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) {
				return false;
			} else {
				return true;
			}
		}
		return p_a.count < count;
	}
	return p_a.start > start;
}

Vector3 TextServer::hex_code_box_font_size[2] = { Vector3(5, 5, 1), Vector3(10, 10, 2) };
OGUI::TextureHandle TextServer::hex_code_box_font_tex[2] = { nullptr, nullptr };


void TextServer::initialize_hex_code_box_fonts() {
	static unsigned int tamsyn5x9_png_len = 175;
	static unsigned char tamsyn5x9_png[] = {
		0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
		0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x05,
		0x04, 0x03, 0x00, 0x00, 0x00, 0x20, 0x7c, 0x76, 0xda, 0x00, 0x00, 0x00,
		0x0f, 0x50, 0x4c, 0x54, 0x45, 0xfd, 0x07, 0x00, 0x00, 0x00, 0x00, 0x06,
		0x7e, 0x74, 0x00, 0x40, 0xc6, 0xff, 0xff, 0xff, 0x47, 0x9a, 0xd4, 0xc7,
		0x00, 0x00, 0x00, 0x01, 0x74, 0x52, 0x4e, 0x53, 0x00, 0x40, 0xe6, 0xd8,
		0x66, 0x00, 0x00, 0x00, 0x4e, 0x49, 0x44, 0x41, 0x54, 0x08, 0x1d, 0x05,
		0xc1, 0x21, 0x01, 0x00, 0x00, 0x00, 0x83, 0x30, 0x04, 0xc1, 0x10, 0xef,
		0x9f, 0xe9, 0x1b, 0x86, 0x2c, 0x17, 0xb9, 0xcc, 0x65, 0x0c, 0x73, 0x38,
		0xc7, 0xe6, 0x22, 0x19, 0x88, 0x98, 0x10, 0x48, 0x4a, 0x29, 0x85, 0x14,
		0x02, 0x89, 0x10, 0xa3, 0x1c, 0x0b, 0x31, 0xd6, 0xe6, 0x08, 0x69, 0x39,
		0x48, 0x44, 0xa0, 0x0d, 0x4a, 0x22, 0xa1, 0x94, 0x42, 0x0a, 0x01, 0x63,
		0x6d, 0x0e, 0x72, 0x18, 0x61, 0x8c, 0x74, 0x38, 0xc7, 0x26, 0x1c, 0xf3,
		0x71, 0x16, 0x15, 0x27, 0x6a, 0xc2, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x49,
		0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
	};

	static unsigned int tamsyn10x20_png_len = 270;
	static unsigned char tamsyn10x20_png[] = {
		0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
		0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x0a,
		0x04, 0x03, 0x00, 0x00, 0x00, 0xc1, 0x66, 0x48, 0x96, 0x00, 0x00, 0x00,
		0x0f, 0x50, 0x4c, 0x54, 0x45, 0x00, 0x00, 0x00, 0xf9, 0x07, 0x00, 0x5d,
		0x71, 0xa5, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x49, 0xdb, 0xcb, 0x7f,
		0x00, 0x00, 0x00, 0x01, 0x74, 0x52, 0x4e, 0x53, 0x00, 0x40, 0xe6, 0xd8,
		0x66, 0x00, 0x00, 0x00, 0xad, 0x49, 0x44, 0x41, 0x54, 0x28, 0xcf, 0xa5,
		0x92, 0x4b, 0x0e, 0x03, 0x31, 0x08, 0x43, 0xdf, 0x82, 0x83, 0x79, 0xe1,
		0xfb, 0x9f, 0xa9, 0x0b, 0x3e, 0x61, 0xa6, 0x1f, 0x55, 0xad, 0x14, 0x31,
		0x66, 0x42, 0x1c, 0x70, 0x0c, 0xb6, 0x00, 0x01, 0xb6, 0x08, 0xdb, 0x00,
		0x8d, 0xc2, 0x14, 0xb2, 0x55, 0xa1, 0xfe, 0x09, 0xc2, 0x26, 0xdc, 0x25,
		0x75, 0x22, 0x97, 0x1a, 0x25, 0x77, 0x28, 0x31, 0x02, 0x80, 0xc8, 0xdd,
		0x2c, 0x11, 0x1a, 0x54, 0x9f, 0xc8, 0xa2, 0x8a, 0x06, 0xa9, 0x93, 0x22,
		0xbd, 0xd4, 0xd0, 0x0c, 0xcf, 0x81, 0x2b, 0xca, 0xbb, 0x83, 0xe0, 0x10,
		0xe6, 0xad, 0xff, 0x10, 0x2a, 0x66, 0x34, 0x41, 0x58, 0x35, 0x54, 0x49,
		0x5a, 0x63, 0xa5, 0xc2, 0x87, 0xab, 0x52, 0x76, 0x9a, 0xba, 0xc6, 0xf4,
		0x75, 0x7a, 0x9e, 0x3c, 0x46, 0x86, 0x5c, 0xa3, 0xfd, 0x87, 0x0e, 0x75,
		0x08, 0x7b, 0xee, 0x7e, 0xea, 0x21, 0x5c, 0x4f, 0xf6, 0xc5, 0xc8, 0x4b,
		0xb9, 0x11, 0xf2, 0xd6, 0xe1, 0x8f, 0x84, 0x62, 0x7b, 0x67, 0xf9, 0x24,
		0xde, 0x6d, 0xbc, 0xb2, 0xcd, 0xb1, 0xf3, 0xf2, 0x2f, 0xe8, 0xe2, 0xe4,
		0xae, 0x4b, 0x4f, 0xcf, 0x2b, 0xdc, 0x8d, 0x0d, 0xf0, 0x00, 0x8f, 0x22,
		0x26, 0x65, 0x75, 0x8a, 0xe6, 0x84, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45,
		0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
	};

	auto&& ctx = OGUI::Context::Get().renderImpl;
	auto& parser = OGUI::Context::Get().bmParserImpl;
	OGUI::Bitmap bitmap1 = parser->LoadFromMemory(tamsyn5x9_png, tamsyn5x9_png_len);
	hex_code_box_font_tex[0] = ctx->RegisterTexture(bitmap1);
	OGUI::Bitmap bitmap2 = parser->LoadFromMemory(tamsyn10x20_png, tamsyn10x20_png_len);
	hex_code_box_font_tex[1] = ctx->RegisterTexture(bitmap2);
	delete bitmap1.resource.bytes;
	delete bitmap2.resource.bytes;
}

void TextServer::finish_hex_code_box_fonts() {
	auto&& ctx = OGUI::Context::Get().renderImpl;
	if (hex_code_box_font_tex[0]) {
		ctx->ReleaseTexture(hex_code_box_font_tex[0]);
	}
	if (hex_code_box_font_tex[1]) {
		ctx->ReleaseTexture(hex_code_box_font_tex[1]);
	}
}

Vector2 TextServer::get_hex_code_box_size(int p_size, char32_t p_index) const {
	int fnt = (p_size < 20) ? 0 : 1;

	real_t w = ((p_index <= 0xFF) ? 1 : ((p_index <= 0xFFFF) ? 2 : 3)) * hex_code_box_font_size[fnt].x;
	real_t h = 2 * hex_code_box_font_size[fnt].y;
	return Vector2(w + 4, h + 3 + 2 * hex_code_box_font_size[fnt].z);
}

void TextServer::draw_hex_code_box(OGUI::PrimDrawContext& list, int p_size, const Vector2 &p_pos, char32_t p_index, const Color &p_color) const {
	int fnt = (p_size < 20) ? 0 : 1;

	ERR_FAIL_COND(hex_code_box_font_tex[fnt]);

	uint8_t a = p_index & 0x0F;
	uint8_t b = (p_index >> 4) & 0x0F;
	uint8_t c = (p_index >> 8) & 0x0F;
	uint8_t d = (p_index >> 12) & 0x0F;
	uint8_t e = (p_index >> 16) & 0x0F;
	uint8_t f = (p_index >> 20) & 0x0F;

	Vector2 pos = p_pos;
	Rect2 dest = Rect2(Vector2(), Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y));

	real_t w = ((p_index <= 0xFF) ? 1 : ((p_index <= 0xFFFF) ? 2 : 3)) * hex_code_box_font_size[fnt].x;
	real_t h = 2 * hex_code_box_font_size[fnt].y;

	pos.y -= std::floor((h + 3 + hex_code_box_font_size[fnt].z) * 0.75);

	canvas_item_add_rect(list, Rect2(pos + Point2(0, 0), Size2(1, h + 2 + 2 * hex_code_box_font_size[fnt].z)), p_color);
	canvas_item_add_rect(list, Rect2(pos + Point2(w + 2, 0), Size2(1, h + 2 + 2 * hex_code_box_font_size[fnt].z)), p_color);
	canvas_item_add_rect(list, Rect2(pos + Point2(0, 0), Size2(w + 2, 1)), p_color);
	canvas_item_add_rect(list, Rect2(pos + Point2(0, h + 2 + 2 * hex_code_box_font_size[fnt].z), Size2(w + 2, 1)), p_color);

	pos += Point2(2, 2);
	if (p_index <= 0xFF) {
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(b * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(a * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
	} else if (p_index <= 0xFFFF) {
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(d * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(1, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(c * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(b * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(1, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(a * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
	} else {
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(f * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(1, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(e * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(2, 0);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(d * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(0, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(c * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(1, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(b * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
		dest.position = pos + Vector2(hex_code_box_font_size[fnt].x, hex_code_box_font_size[fnt].y) * Point2(2, 1) + Point2(0, hex_code_box_font_size[fnt].z);
		canvas_item_add_texture_rect_region(list, dest, hex_code_box_font_tex[fnt], Rect2(Point2(a * hex_code_box_font_size[fnt].x, 0), dest.size), p_color);
	}
}

Vector<Vector2i> TextServer::shaped_text_get_line_breaks_adv(RID p_shaped, const Vector<real_t> &p_width, int p_start, bool p_once, int64_t /*TextBreakFlag*/ p_break_flags) const {
	Vector<Vector2i> lines;

	ERR_FAIL_COND_V(p_width.is_empty(), lines);

	const_cast<TextServer *>(this)->shaped_text_update_breaks(p_shaped);
	const Vector<Glyph> &logical = const_cast<TextServer *>(this)->shaped_text_sort_logical(p_shaped);
	const Vector2i &range = shaped_text_get_range(p_shaped);

	real_t width = 0.f;
	int line_start = MAX(p_start, range.x);
	int last_safe_break = -1;
	int chunk = 0;

	int l_size = logical.size();
	const Glyph *l_gl = logical.ptr();

	for (int i = 0; i < l_size; i++) {
		if (l_gl[i].start < p_start) {
			continue;
		}
		if (l_gl[i].count > 0) {
			if ((p_width[chunk] > 0) && (width + l_gl[i].advance > p_width[chunk]) && (last_safe_break >= 0)) {
				lines.push_back(Vector2i(line_start, l_gl[last_safe_break].end));
				line_start = l_gl[last_safe_break].end;
				i = last_safe_break;
				last_safe_break = -1;
				width = 0;
				chunk++;
				if (chunk >= p_width.size()) {
					chunk = 0;
					if (p_once) {
						return lines;
					}
				}
				continue;
			}
			if ((p_break_flags & BREAK_MANDATORY) == BREAK_MANDATORY) {
				if ((l_gl[i].flags & GRAPHEME_IS_BREAK_HARD) == GRAPHEME_IS_BREAK_HARD) {
					lines.push_back(Vector2i(line_start, l_gl[i].end));
					line_start = l_gl[i].end;
					last_safe_break = -1;
					width = 0;
					chunk = 0;
					if (p_once) {
						return lines;
					}
					continue;
				}
			}
			if ((p_break_flags & BREAK_WORD_BOUND) == BREAK_WORD_BOUND) {
				if ((l_gl[i].flags & GRAPHEME_IS_BREAK_SOFT) == GRAPHEME_IS_BREAK_SOFT) {
					last_safe_break = i;
				}
			}
			if ((p_break_flags & BREAK_GRAPHEME_BOUND) == BREAK_GRAPHEME_BOUND) {
				last_safe_break = i;
			}
		}
		width += l_gl[i].advance;
	}

	if (l_size > 0) {
		lines.push_back(Vector2i(line_start, range.y));
	} else {
		lines.push_back(Vector2i(0, 0));
	}

	return lines;
}

Vector<Vector2i> TextServer::shaped_text_get_line_breaks(RID p_shaped, real_t p_width, int p_start, int64_t /*TextBreakFlag*/ p_break_flags) const {
	Vector<Vector2i> lines;

	const_cast<TextServer *>(this)->shaped_text_update_breaks(p_shaped);
	const Vector<Glyph> &logical = const_cast<TextServer *>(this)->shaped_text_sort_logical(p_shaped);
	const Vector2i &range = shaped_text_get_range(p_shaped);

	real_t width = 0.f;
	int line_start = MAX(p_start, range.x);
	int last_safe_break = -1;
	int word_count = 0;
	int l_size = logical.size();
	const Glyph *l_gl = logical.ptr();

	for (int i = 0; i < l_size; i++) {
		if (l_gl[i].start < p_start) {
			continue;
		}
		if (l_gl[i].count > 0) {
			if ((p_width > 0) && (width + l_gl[i].advance * l_gl[i].repeat > p_width) && (last_safe_break >= 0)) {
				lines.push_back(Vector2i(line_start, l_gl[last_safe_break].end));
				line_start = l_gl[last_safe_break].end;
				i = last_safe_break;
				last_safe_break = -1;
				width = 0;
				word_count = 0;
				continue;
			}
			if ((p_break_flags & BREAK_MANDATORY) == BREAK_MANDATORY) {
				if ((l_gl[i].flags & GRAPHEME_IS_BREAK_HARD) == GRAPHEME_IS_BREAK_HARD) {
					lines.push_back(Vector2i(line_start, l_gl[i].end));
					line_start = l_gl[i].end;
					last_safe_break = -1;
					width = 0;
					continue;
				}
			}
			if ((p_break_flags & BREAK_WORD_BOUND) == BREAK_WORD_BOUND) {
				if ((l_gl[i].flags & GRAPHEME_IS_BREAK_SOFT) == GRAPHEME_IS_BREAK_SOFT) {
					last_safe_break = i;
					word_count++;
				}
				else if(word_count == 0)
				{
					last_safe_break = i;
				}
			}
			if ((p_break_flags & BREAK_GRAPHEME_BOUND) == BREAK_GRAPHEME_BOUND) {
				last_safe_break = i;
			}
		}
		width += l_gl[i].advance * l_gl[i].repeat;
	}

	if (l_size > 0) {
		if (lines.size() == 0 || lines[lines.size() - 1].y < range.y) {
			lines.push_back(Vector2i(line_start, range.y));
		}
	} else {
		lines.push_back(Vector2i(0, 0));
	}

	return lines;
}

Vector<Vector2i> TextServer::shaped_text_get_word_breaks(RID p_shaped, int p_grapheme_flags) const {
	Vector<Vector2i> words;

	const_cast<TextServer *>(this)->shaped_text_update_justification_ops(p_shaped);
	const Vector<Glyph> &logical = const_cast<TextServer *>(this)->shaped_text_sort_logical(p_shaped);
	const Vector2i &range = shaped_text_get_range(p_shaped);

	int word_start = range.x;

	int l_size = logical.size();
	const Glyph *l_gl = logical.ptr();

	for (int i = 0; i < l_size; i++) {
		if (l_gl[i].count > 0) {
			if ((l_gl[i].flags & p_grapheme_flags) != 0) {
				words.push_back(Vector2i(word_start, l_gl[i].start));
				word_start = l_gl[i].end;
			}
		}
	}
	if (l_size > 0) {
		words.push_back(Vector2i(word_start, range.y));
	}

	return words;
}


bool TextServer::shaped_test_is_hard_break(RID p_shaped) const
{
	const_cast<TextServer *>(this)->shaped_text_update_breaks(p_shaped);
	const Vector<Glyph> &logical = const_cast<TextServer *>(this)->shaped_text_sort_logical(p_shaped);
	return logical.size() && (logical.back().flags & GRAPHEME_IS_BREAK_HARD) == GRAPHEME_IS_BREAK_HARD;
}

TextServer::TrimData TextServer::shaped_text_get_trim_data(RID p_shaped) const {
	WARN_PRINT("Getting overrun data not supported by this TextServer.");
	return TrimData();
}

void TextServer::shaped_text_get_carets(RID p_shaped, int p_position, Rect2 &p_leading_caret, Direction &p_leading_dir, Rect2 &p_trailing_caret, Direction &p_trailing_dir) const {
	Vector<Rect2> carets;
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);
	TextServer::Orientation orientation = shaped_text_get_orientation(p_shaped);
	const Vector2 &range = shaped_text_get_range(p_shaped);
	real_t ascent = shaped_text_get_ascent(p_shaped);
	real_t descent = shaped_text_get_descent(p_shaped);
	real_t height = (ascent + descent) / 2;

	real_t off = 0.0f;
	p_leading_dir = DIRECTION_AUTO;
	p_trailing_dir = DIRECTION_AUTO;

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	for (int i = 0; i < v_size; i++) {
		if (glyphs[i].count > 0) {
			// Caret before grapheme (top / left).
			if (p_position == glyphs[i].start && ((glyphs[i].flags & GRAPHEME_IS_VIRTUAL) != GRAPHEME_IS_VIRTUAL)) {
				Rect2 cr;
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (glyphs[i].start == range.x) {
						cr.size.y = height * 2;
					} else {
						cr.size.y = height;
					}
					cr.position.y = -ascent;
					cr.position.x = off;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
						p_trailing_dir = DIRECTION_RTL;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.position.x += glyphs[i + j].advance * glyphs[i + j].repeat;
							cr.size.x -= glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					} else {
						p_trailing_dir = DIRECTION_LTR;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.size.x += glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					}
				} else {
					if (glyphs[i].start == range.x) {
						cr.size.x = height * 2;
					} else {
						cr.size.x = height;
					}
					cr.position.x = -ascent;
					cr.position.y = off;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
						p_trailing_dir = DIRECTION_RTL;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.position.y += glyphs[i + j].advance * glyphs[i + j].repeat;
							cr.size.y -= glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					} else {
						p_trailing_dir = DIRECTION_LTR;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.size.y += glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					}
				}
				p_trailing_caret = cr;
			}
			// Caret after grapheme (bottom / right).
			if (p_position == glyphs[i].end && ((glyphs[i].flags & GRAPHEME_IS_VIRTUAL) != GRAPHEME_IS_VIRTUAL)) {
				Rect2 cr;
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (glyphs[i].end == range.y) {
						cr.size.y = height * 2;
						cr.position.y = -ascent;
					} else {
						cr.size.y = height;
						cr.position.y = -ascent + height;
					}
					cr.position.x = off;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) != GRAPHEME_IS_RTL) {
						p_leading_dir = DIRECTION_LTR;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.position.x += glyphs[i + j].advance * glyphs[i + j].repeat;
							cr.size.x -= glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					} else {
						p_leading_dir = DIRECTION_RTL;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.size.x += glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					}
				} else {
					cr.size.y = 1.0f;
					if (glyphs[i].end == range.y) {
						cr.size.x = height * 2;
						cr.position.x = -ascent;
					} else {
						cr.size.x = height;
						cr.position.x = -ascent + height;
					}
					cr.position.y = off;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) != GRAPHEME_IS_RTL) {
						p_leading_dir = DIRECTION_LTR;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.position.y += glyphs[i + j].advance * glyphs[i + j].repeat;
							cr.size.y -= glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					} else {
						p_leading_dir = DIRECTION_RTL;
						for (int j = 0; j < glyphs[i].count; j++) {
							cr.size.y += glyphs[i + j].advance * glyphs[i + j].repeat;
						}
					}
				}
				p_leading_caret = cr;
			}
			// Caret inside grapheme (middle).
			if (p_position > glyphs[i].start && p_position < glyphs[i].end && (glyphs[i].flags & GRAPHEME_IS_VIRTUAL) != GRAPHEME_IS_VIRTUAL) {
				real_t advance = 0.f;
				for (int j = 0; j < glyphs[i].count; j++) {
					advance += glyphs[i + j].advance * glyphs[i + j].repeat;
				}
				real_t char_adv = advance / (real_t)(glyphs[i].end - glyphs[i].start);
				Rect2 cr;
				if (orientation == ORIENTATION_HORIZONTAL) {
					cr.size.x = 1.0f;
					cr.size.y = height * 2;
					cr.position.y = -ascent;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
						cr.position.x = off + char_adv * (glyphs[i].end - p_position);
					} else {
						cr.position.x = off + char_adv * (p_position - glyphs[i].start);
					}
				} else {
					cr.size.y = 1.0f;
					cr.size.x = height * 2;
					cr.position.x = -ascent;
					if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
						cr.position.y = off + char_adv * (glyphs[i].end - p_position);
					} else {
						cr.position.y = off + char_adv * (p_position - glyphs[i].start);
					}
				}
				p_trailing_caret = cr;
				p_leading_caret = cr;
			}
		}
		off += glyphs[i].advance * glyphs[i].repeat;
	}
}

TextServer::Direction TextServer::shaped_text_get_dominant_direciton_in_range(RID p_shaped, int p_start, int p_end) const {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);

	if (p_start == p_end) {
		return DIRECTION_AUTO;
	}

	int start = MIN(p_start, p_end);
	int end = MAX(p_start, p_end);

	int rtl = 0;
	int ltr = 0;

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	for (int i = 0; i < v_size; i++) {
		if ((glyphs[i].end > start) && (glyphs[i].start < end)) {
			if (glyphs[i].count > 0) {
				if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
					rtl++;
				} else {
					ltr++;
				}
			}
		}
	}
	if (ltr == rtl) {
		return DIRECTION_AUTO;
	} else if (ltr > rtl) {
		return DIRECTION_LTR;
	} else {
		return DIRECTION_RTL;
	}
}

Vector<Vector2> TextServer::shaped_text_get_selection(RID p_shaped, int p_start, int p_end) const {
	Vector<Vector2> ranges;
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);

	if (p_start == p_end) {
		return ranges;
	}

	int start = MIN(p_start, p_end);
	int end = MAX(p_start, p_end);

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	real_t off = 0.0f;
	for (int i = 0; i < v_size; i++) {
		for (int k = 0; k < glyphs[i].repeat; k++) {
			if ((glyphs[i].count > 0) && ((glyphs[i].index != 0) || ((glyphs[i].flags & GRAPHEME_IS_SPACE) == GRAPHEME_IS_SPACE))) {
				if (glyphs[i].start < end && glyphs[i].end > start) {
					// Grapheme fully in selection range.
					if (glyphs[i].start >= start && glyphs[i].end <= end) {
						real_t advance = 0.f;
						for (int j = 0; j < glyphs[i].count; j++) {
							advance += glyphs[i + j].advance;
						}
						ranges.push_back(Vector2(off, off + advance));
					}
					// Only start of grapheme is in selection range.
					if (glyphs[i].start >= start && glyphs[i].end > end) {
						real_t advance = 0.f;
						for (int j = 0; j < glyphs[i].count; j++) {
							advance += glyphs[i + j].advance;
						}
						real_t char_adv = advance / (real_t)(glyphs[i].end - glyphs[i].start);
						if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
							ranges.push_back(Vector2(off + char_adv * (glyphs[i].end - end), off + advance));
						} else {
							ranges.push_back(Vector2(off, off + char_adv * (end - glyphs[i].start)));
						}
					}
					// Only end of grapheme is in selection range.
					if (glyphs[i].start < start && glyphs[i].end <= end) {
						real_t advance = 0.f;
						for (int j = 0; j < glyphs[i].count; j++) {
							advance += glyphs[i + j].advance;
						}
						real_t char_adv = advance / (real_t)(glyphs[i].end - glyphs[i].start);
						if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
							ranges.push_back(Vector2(off, off + char_adv * (start - glyphs[i].start)));
						} else {
							ranges.push_back(Vector2(off + char_adv * (glyphs[i].end - start), off + advance));
						}
					}
					// Selection range is within grapheme.
					if (glyphs[i].start < start && glyphs[i].end > end) {
						real_t advance = 0.f;
						for (int j = 0; j < glyphs[i].count; j++) {
							advance += glyphs[i + j].advance;
						}
						real_t char_adv = advance / (real_t)(glyphs[i].end - glyphs[i].start);
						if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
							ranges.push_back(Vector2(off + char_adv * (glyphs[i].end - end), off + char_adv * (glyphs[i].end - start)));
						} else {
							ranges.push_back(Vector2(off + char_adv * (start - glyphs[i].start), off + char_adv * (end - glyphs[i].start)));
						}
					}
				}
			}
			off += glyphs[i].advance;
		}
	}

	// Merge intersecting ranges.
	int i = 0;
	while (i < ranges.size()) {
		i++;
	}
	i = 0;
	while (i < ranges.size()) {
		int j = i + 1;
		while (j < ranges.size()) {
			if (almost_equal(ranges[i].y, ranges[j].x, (real_t)UNIT_EPSILON)) {
				ranges.data()[i].y = ranges[j].y;
				ranges.remove(j);
				continue;
			}
			j++;
		}
		i++;
	}

	return ranges;
}

int TextServer::shaped_text_hit_test_grapheme(RID p_shaped, real_t p_coords) const {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);

	// Exact grapheme hit test, return -1 if missed.
	real_t off = 0.0f;

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	for (int i = 0; i < v_size; i++) {
		for (int j = 0; j < glyphs[i].repeat; j++) {
			if (p_coords >= off && p_coords < off + glyphs[i].advance) {
				return i;
			}
			off += glyphs[i].advance;
		}
	}
	return -1;
}

int TextServer::shaped_text_hit_test_position(RID p_shaped, real_t p_coords) const {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	// Cursor placement hit test.

	// Place caret to the left of the leftmost grapheme, or to position 0 if string is empty.
	if (p_coords <= 0) {
		if (v_size > 0) {
			if ((glyphs[0].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
				return glyphs[0].end;
			} else {
				return glyphs[0].start;
			}
		} else {
			return 0;
		}
	}

	// Place caret to the right of the rightmost grapheme, or to position 0 if string is empty.
	if (p_coords >= shaped_text_get_width(p_shaped)) {
		if (v_size > 0) {
			if ((glyphs[v_size - 1].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
				return glyphs[v_size - 1].start;
			} else {
				return glyphs[v_size - 1].end;
			}
		} else {
			return 0;
		}
	}

	real_t off = 0.0f;
	for (int i = 0; i < v_size; i++) {
		if (glyphs[i].count > 0) {
			real_t advance = 0.f;
			for (int j = 0; j < glyphs[i].count; j++) {
				advance += glyphs[i + j].advance * glyphs[i + j].repeat;
			}
			if (((glyphs[i].flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) && (p_coords >= off && p_coords < off + advance)) {
				if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
					return glyphs[i].end;
				} else {
					return glyphs[i].start;
				}
			}
			// Place caret to the left of clicked grapheme.
			if (p_coords >= off && p_coords < off + advance / 2) {
				if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
					return glyphs[i].end;
				} else {
					return glyphs[i].start;
				}
			}
			// Place caret to the right of clicked grapheme.
			if (p_coords >= off + advance / 2 && p_coords < off + advance) {
				if ((glyphs[i].flags & GRAPHEME_IS_RTL) == GRAPHEME_IS_RTL) {
					return glyphs[i].start;
				} else {
					return glyphs[i].end;
				}
			}
		}
		off += glyphs[i].advance * glyphs[i].repeat;
	}
	return 0;
}

int TextServer::shaped_text_next_grapheme_pos(RID p_shaped, int p_pos) {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);
	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();
	for (int i = 0; i < v_size; i++) {
		if (p_pos >= glyphs[i].start && p_pos < glyphs[i].end) {
			return glyphs[i].end;
		}
	}
	return p_pos;
}

int TextServer::shaped_text_prev_grapheme_pos(RID p_shaped, int p_pos) {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);
	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();
	for (int i = 0; i < v_size; i++) {
		if (p_pos > glyphs[i].start && p_pos <= glyphs[i].end) {
			return glyphs[i].start;
		}
	}

	return p_pos;
}

namespace godot
{
	template <typename EF> struct scope_exit {
	explicit scope_exit(EF &&f)
		: exit_function(std::move(f)), execute_on_destruction{true} {}

	scope_exit(scope_exit &&rhs)
		: exit_function(std::move(rhs.exit_function)),
			execute_on_destruction{rhs.execute_on_destruction} {
		rhs.release();
	}

	~scope_exit() {
		if (execute_on_destruction) { this->exit_function(); }
	}

	void release() { this->execute_on_destruction = false; }

	private:
	scope_exit(const scope_exit &) = delete;
	void operator=(const scope_exit &) = delete;
	scope_exit &operator=(scope_exit &&) = delete;

	EF exit_function;
	bool execute_on_destruction;
	};
}

void TextServer::shaped_text_draw(RID p_shaped, OGUI::PrimDrawContext& list, const Vector2 &p_pos, real_t p_clip_l, real_t p_clip_r, const Color &p_color) const {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);
	TextServer::Orientation orientation = shaped_text_get_orientation(p_shaped);
	bool hex_codes = shaped_text_get_preserve_control(p_shaped) || shaped_text_get_preserve_invalid(p_shaped);

	bool rtl = shaped_text_get_direction(p_shaped) == DIRECTION_RTL;
	TrimData trim_data = shaped_text_get_trim_data(p_shaped);

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();

	Vector2 ofs = p_pos;
	// Draw RTL ellipsis string when needed.
	if (rtl && trim_data.ellipsis_pos >= 0) {
		for (int i = trim_data.ellipsis_glyph_buf.size() - 1; i >= 0; i--) {
			for (int j = 0; j < trim_data.ellipsis_glyph_buf[i].repeat; j++) {
				font_draw_glyph(p_shaped, list, trim_data.ellipsis_glyph_buf[i], ofs, p_color);
				if (orientation == ORIENTATION_HORIZONTAL) {
					ofs.x += trim_data.ellipsis_glyph_buf[i].advance;
				} else {
					ofs.y += trim_data.ellipsis_glyph_buf[i].advance;
				}
			}
		}
	}
	Vector2 ul_start;
	TextDecorationData ul_data;

	float spos=0;
	float sthk=0;
	Vector2 so_start;
					
	float underline_off = TS->shaped_text_get_underline_position(p_shaped);
	float underline_width = TS->shaped_text_get_underline_thickness(p_shaped);

	nvgBeginPath(list.nvg);
	auto drawStrickout = [&]
	{
		nvgMoveTo(list.nvg, so_start.x, -so_start.y - spos);
		nvgLineTo(list.nvg, ofs.x, -ofs.y - spos);
		auto dcolor = p_color * ul_data.decorationColor;
		nvgStrokeColor(list.nvg, nvgRGBAf(dcolor.r, dcolor.g, dcolor.b, dcolor.a));
		nvgStrokeWidth(list.nvg, sthk * ul_data.decorationThickness);
	};
	auto drawUnderline = [&]
	{
		nvgMoveTo(list.nvg, ul_start.x, -ul_start.y - underline_off);
		nvgLineTo(list.nvg, ofs.x, -ofs.y - underline_off);
		auto dcolor = p_color * ul_data.decorationColor;
		nvgStrokeColor(list.nvg, nvgRGBAf(dcolor.r, dcolor.g, dcolor.b, dcolor.a));
		nvgStrokeWidth(list.nvg, underline_width * ul_data.decorationThickness);
	};

	auto se = scope_exit{[&]{
		if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_UNDERLINE) != 0)
			drawUnderline();
		if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_THROUGH) != 0)
			drawStrickout();
		nvgStroke(list.nvg);
	}};
	// Draw at the baseline.
	for (int i = 0; i < v_size; i++) {
		for (int j = 0; j < glyphs[i].repeat; j++) {
			if (p_clip_r > 0) {
				// Clip right / bottom.
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (ofs.x - p_pos.x > p_clip_r) {
						return;
					}
				} else {
					if (ofs.y - p_pos.y > p_clip_r) {
						return;
					}
				}
			}
			if (p_clip_l > 0) {
				// Clip left / top.
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (ofs.x - p_pos.x < p_clip_l) {
						ofs.x += glyphs[i].advance;
						continue;
					}
				} else {
					if (ofs.y - p_pos.y < p_clip_l) {
						ofs.y += glyphs[i].advance;
						continue;
					}
				}
			}
			if (trim_data.trim_pos >= 0) {
				if (rtl) {
					if (i < trim_data.trim_pos && (glyphs[j].flags & TextServer::GRAPHEME_IS_VIRTUAL) != TextServer::GRAPHEME_IS_VIRTUAL) {
						continue;
					}
				} else {
					if (i >= trim_data.trim_pos && (glyphs[j].flags & TextServer::GRAPHEME_IS_VIRTUAL) != TextServer::GRAPHEME_IS_VIRTUAL) {
						break;
					}
				}
			}
			
			float strickout_off = TS->font_get_strickout_position(glyphs[i].font_rid, glyphs[i].font_size);
			float strickout_width = TS->font_get_strickout_thickness(glyphs[i].font_rid, glyphs[i].font_size);
			TextDecorationData decoration = shaped_text_get_decoration(p_shaped, glyphs[i].span);
			bool decorationChanged = decoration.decorationTexture != ul_data.decorationTexture ||
			decoration.decorationColor != ul_data.decorationColor ||
			decoration.decorationLineFlag != ul_data.decorationLineFlag ||
			decoration.decorationThickness != ul_data.decorationThickness;
			bool strickChanged = strickout_off != spos || strickout_width != sthk;
			if(strickChanged)
			{
				if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_THROUGH) != 0)
				{
					drawStrickout();
					so_start = ofs;
				}				
			}
			if(decorationChanged)
			{
				if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_UNDERLINE) != 0)
					drawUnderline();
				if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_THROUGH) != 0 && !strickChanged)
					drawStrickout();
				nvgStroke(list.nvg);
				nvgBeginPath(list.nvg);
				ul_data = decoration;
				if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_UNDERLINE) != 0)
					ul_start = ofs;
				if((ul_data.decorationLineFlag & DecorationLineFlag::DECORATION_LINE_THROUGH) != 0)
					so_start = ofs;
			}
			spos = strickout_off; sthk = strickout_width;

			if (glyphs[i].font_rid != RID()) {
				font_draw_glyph(p_shaped, list, glyphs[i], ofs, p_color);
			} else if (hex_codes && ((glyphs[i].flags & GRAPHEME_IS_VIRTUAL) != GRAPHEME_IS_VIRTUAL)) {
				TextServer::draw_hex_code_box(list, glyphs[i].font_size, ofs + Vector2(glyphs[i].x_off, glyphs[i].y_off), glyphs[i].index, p_color);
			}
			if (orientation == ORIENTATION_HORIZONTAL) {
				ofs.x += glyphs[i].advance;
			} else {
				ofs.y += glyphs[i].advance;
			}
		}
	}
	// Draw LTR ellipsis string when needed.
	if (!rtl && trim_data.ellipsis_pos >= 0) {
		for (int i = 0; i < trim_data.ellipsis_glyph_buf.size(); i++) {
			for (int j = 0; j < trim_data.ellipsis_glyph_buf[i].repeat; j++) {
				font_draw_glyph(p_shaped, list, trim_data.ellipsis_glyph_buf[i], ofs, p_color);
				if (orientation == ORIENTATION_HORIZONTAL) {
					ofs.x += trim_data.ellipsis_glyph_buf[i].advance;
				} else {
					ofs.y += trim_data.ellipsis_glyph_buf[i].advance;
				}
			}
		}
	}
}

void TextServer::shaped_text_draw_outline(RID p_shaped, OGUI::PrimDrawContext& list, const Vector2 &p_pos, real_t p_clip_l, real_t p_clip_r, int p_outline_size, const Color &p_color) const {
	const Vector<TextServer::Glyph> visual = shaped_text_get_glyphs(p_shaped);
	TextServer::Orientation orientation = shaped_text_get_orientation(p_shaped);

	bool rtl = (shaped_text_get_direction(p_shaped) == DIRECTION_RTL);
	TrimData trim_data = shaped_text_get_trim_data(p_shaped);

	int v_size = visual.size();
	const Glyph *glyphs = visual.ptr();
	Vector2 ofs = p_pos;
	// Draw RTL ellipsis string when needed.
	if (rtl && trim_data.ellipsis_pos >= 0) {
		for (int i = trim_data.ellipsis_glyph_buf.size() - 1; i >= 0; i--) {
			for (int j = 0; j < trim_data.ellipsis_glyph_buf[i].repeat; j++) {
				font_draw_glyph(p_shaped, list, trim_data.ellipsis_glyph_buf[i], ofs, p_color);
				if (orientation == ORIENTATION_HORIZONTAL) {
					ofs.x += trim_data.ellipsis_glyph_buf[i].advance;
				} else {
					ofs.y += trim_data.ellipsis_glyph_buf[i].advance;
				}
			}
		}
	}
	// Draw at the baseline.
	for (int i = 0; i < v_size; i++) {
		for (int j = 0; j < glyphs[i].repeat; j++) {
			if (p_clip_r > 0) {
				// Clip right / bottom.
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (ofs.x - p_pos.x > p_clip_r) {
						return;
					}
				} else {
					if (ofs.y - p_pos.y > p_clip_r) {
						return;
					}
				}
			}
			if (p_clip_l > 0) {
				// Clip left / top.
				if (orientation == ORIENTATION_HORIZONTAL) {
					if (ofs.x - p_pos.x < p_clip_l) {
						ofs.x += glyphs[i].advance;
						continue;
					}
				} else {
					if (ofs.y - p_pos.y < p_clip_l) {
						ofs.y += glyphs[i].advance;
						continue;
					}
				}
			}
			if (trim_data.trim_pos >= 0) {
				if (rtl) {
					if (i < trim_data.trim_pos) {
						continue;
					}
				} else {
					if (i >= trim_data.trim_pos && (glyphs[j].flags & TextServer::GRAPHEME_IS_VIRTUAL) != TextServer::GRAPHEME_IS_VIRTUAL) {
						break;
					}
				}
			}
			if (glyphs[i].font_rid != RID()) {
				font_draw_glyph_outline(p_shaped, list, glyphs[i], p_outline_size, ofs, p_color);
			}
			if (orientation == ORIENTATION_HORIZONTAL) {
				ofs.x += glyphs[i].advance;
			} else {
				ofs.y += glyphs[i].advance;
			}
		}
	}
	// Draw LTR ellipsis string when needed.
	if (!rtl && trim_data.ellipsis_pos >= 0) {
		for (int i = 0; i < trim_data.ellipsis_glyph_buf.size(); i++) {
			for (int j = 0; j < trim_data.ellipsis_glyph_buf[i].repeat; j++) {
				font_draw_glyph(p_shaped, list, trim_data.ellipsis_glyph_buf[i], ofs , p_color);
				if (orientation == ORIENTATION_HORIZONTAL) {
					ofs.x += trim_data.ellipsis_glyph_buf[i].advance;
				} else {
					ofs.y += trim_data.ellipsis_glyph_buf[i].advance;
				}
			}
		}
	}
}

TextServer::TextServer() {
}

TextServer::~TextServer() {
}

OGUI::Vector2f math_cast(const Vector2& v)
{
	return {v.x, v.y};
}

OGUI::Rect math_cast(const Rect2& r)
{
	return {
		math_cast(r.position),
		math_cast(r.position + r.size)
	};
}

OGUI::Color4f math_cast(const Color& p_color)
{
	return {p_color.r, p_color.g, p_color.b, p_color.a};
}

namespace OGUI
{

	uint32_t ToColor32ABGR(Color4f color)
	{
		color.x = std::clamp(color.x, 0.f, 1.f);   
		color.y = std::clamp(color.y, 0.f, 1.f);   
		color.z = std::clamp(color.z, 0.f, 1.f);   
		color.w = std::clamp(color.w, 0.f, 1.f);   
		uint8_t r = color.x * 255;
		uint8_t g = color.y * 255;
		uint8_t b = color.z * 255;
		uint8_t a = color.w * 255;
		return ((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)g << 8) + (uint32_t)r;
	}

	Vertex MakeVertex(Vector2f position, Vector2f UV, Color4f color)
    {
		Vertex v;
		v.position = position;
		v.texcoord = UV;
		v.color = ToColor32ABGR(color);
		v.aa = {0.5f, 1.f};
		return v;
    }

	void BoxShape(PrimDrawList& list, PrimDrawResource& resource, Rect rect, Rect uv, Color4f color)
	{
		Vector2f side_len = rect.max - rect.min;
		if (!side_len.X || !side_len.Y)
			return;

		auto& command = list.GetCommand(resource);
		const uint32_t vcount = list.vertices.size();
		//const uint32_t icount = list.indices.size();
		
		const Vector2f RU = rect.max;
		const Vector2f RB = Vector2f(rect.max.X, rect.min.Y);
		const Vector2f LU = Vector2f(rect.min.X, rect.max.Y);
		const Vector2f LB = rect.min;

		const Vector2f RUUV = uv.max;
		const Vector2f RBUV = Vector2f(uv.max.X, uv.min.Y);
		const Vector2f LUUV = Vector2f(uv.min.X, uv.max.Y);
		const Vector2f LBUV = uv.min;

		const auto vRU = MakeVertex( RU, RUUV, color );
		const auto vLU = MakeVertex( LU, LUUV, color );
		const auto vRB = MakeVertex( RB, RBUV, color );
		const auto vLB = MakeVertex( LB, LBUV, color );

		list.vertices.emplace_back(vRU); // vcount + 0
		list.vertices.emplace_back(vLU); // vcount + 1
		list.vertices.emplace_back(vRB); // vcount + 2
		list.vertices.emplace_back(vLB); // vcount + 3

		list.indices.emplace_back(vcount + 0u);
		list.indices.emplace_back(vcount + 1u);
		list.indices.emplace_back(vcount + 2u);
		list.indices.emplace_back(vcount + 2u);
		list.indices.emplace_back(vcount + 1u);
		list.indices.emplace_back(vcount + 3u);
		command.element_count += 6;
	}
}

void TextServer::canvas_item_add_texture_rect_region(OGUI::PrimDrawContext& list, const Rect2 &p_rect, OGUI::TextureHandle p_texture, const Rect2 &p_src_rect, const Color &p_modulate, GlyphDrawPolicy* policy) const
{
	using namespace OGUI;
	auto dstRect = p_rect;
	dstRect.position.y = - p_rect.position.y - p_rect.size.y;
	auto srcRect = p_src_rect;
	srcRect.position.y = 1 - p_src_rect.position.y - p_src_rect.size.y ;
	if(policy)
		policy->draw(list, math_cast(dstRect), p_texture, math_cast(srcRect), math_cast(p_modulate));
	else
		GlyphDrawPolicy::drawQuad(list, math_cast(dstRect), p_texture, math_cast(srcRect), math_cast(p_modulate));
}

#ifdef MODULE_MSDFGEN_ENABLED
void TextServer::canvas_item_add_msdf_texture_rect_region(OGUI::PrimDrawContext &list, const Rect2 &p_rect, OGUI::TextureHandle p_texture, const Rect2 &p_src_rect, const Color &p_modulate, float p_outline_size, float p_px_range, float p_scale, GlyphDrawPolicy* policy) const
{
	using namespace OGUI;
	auto dstRect = p_rect;
	dstRect.position.y = - p_rect.position.y - p_rect.size.y;
	auto srcRect = p_src_rect;
	srcRect.position.y = 1 - p_src_rect.position.y - p_src_rect.size.y ;
	if(policy)
		policy->drawMsdf(list, math_cast(dstRect), p_texture, math_cast(srcRect), math_cast(p_modulate), p_outline_size, p_px_range, p_scale);
	else
		GlyphDrawPolicy::drawMsdfQuad(list, math_cast(dstRect), p_texture, math_cast(srcRect), math_cast(p_modulate), p_outline_size, p_px_range, p_scale);
}
#endif

void TextServer::GlyphDrawPolicy::draw(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color)
{
	drawQuad(list, rect, texture, uv, color);
}

void TextServer::GlyphDrawPolicy::drawQuad(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color, bool noGamma)
{
	using namespace OGUI;
	PrimDrawResource resource;
	resource.texture = texture;
	resource.compositeOperation = {NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA, NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA};
	resource.noGamma = noGamma;
	OGUI::BoxShape(*list.current, resource, rect, uv, color);
}

#ifdef MODULE_MSDFGEN_ENABLED
void TextServer::GlyphDrawPolicy::drawMsdf(OGUI::PrimDrawContext &list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color, float p_outline_size, float p_px_range, float p_scale)
{
	drawMsdfQuad(list, rect, texture, uv, color, p_outline_size, p_px_range, p_scale);
}

void TextServer::GlyphDrawPolicy::drawMsdfQuad(OGUI::PrimDrawContext &ctx, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color, float p_outline_size, float p_px_range, float p_scale, bool noGamma)
{
	using namespace OGUI;
	PrimDrawResource resource;
	resource.texture = texture;
	resource.compositeOperation = {NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA, NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA};
	resource.noGamma = noGamma;
	resource.msdf = true;
	auto& list = *ctx.current;
	OGUI::BoxShape(list, resource, rect, uv, color);
	for(int i=0; i<4; i++)
	{
		auto& v = list.vertices[list.vertices.size() - 4 + i];
		v.parameters = {p_outline_size, p_px_range, p_scale, 0.f};
	}
}
#endif

void TextServer::canvas_item_add_rect(OGUI::PrimDrawContext& list, const Rect2 &p_rect, const Color &p_color) const
{
	using namespace OGUI;
	PrimDrawResource resource;
	resource.texture = nullptr;
	resource.compositeOperation = {NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA, NVG_ONE, NVG_ONE_MINUS_SRC_ALPHA};
	auto dstRect = p_rect;
	dstRect.position.y = - p_rect.position.y - p_rect.size.y;
	BoxShape(*list.current, resource, math_cast(dstRect), {Vector2f::vector_zero(), Vector2f::vector_one()}, math_cast(p_color));
}