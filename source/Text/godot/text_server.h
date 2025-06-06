/*************************************************************************/
/*  text_server.h                                                        */
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
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Style2/Transform.h"
#include "config.h"
#include "rid.h"
#include "ustring.h"
#include "rect2.h"
#include "vector3.h"
#include "color.h"
#include "mutex.h"
#include "rid_owner.h"
#include "OpenGUI/Interface/Interfaces.h"
#include <stdint.h>
#include <optional>
namespace godot{

struct TextDecorationData
{
	Color decorationColor = Color(1,1,1,1);
	float decorationThickness = 1.f;
	int64_t decorationLineFlag = 0;
	OGUI::TextureHandle decorationTexture = nullptr;
};

class TextServer {

public:
	enum Direction {
		DIRECTION_AUTO,
		DIRECTION_LTR,
		DIRECTION_RTL
	};

	enum Orientation {
		ORIENTATION_HORIZONTAL,
		ORIENTATION_VERTICAL
	};

	enum DecorationLineFlag {
		DECORATION_LINE_UNDERLINE = 0x1,
		DECORATION_LINE_OVERLINE = 0x2,
		DECORATION_LINE_THROUGH = 0x4,
	};
	enum DecorationStyle {
		DECORATION_STYLE_SOLID
	};

	enum JustificationFlag {
		JUSTIFICATION_NONE = 0,
		JUSTIFICATION_KASHIDA = 1 << 0,
		JUSTIFICATION_CHARACTER = 1 << 1,
		JUSTIFICATION_TRIM_EDGE_SPACES = 1 << 2,
		JUSTIFICATION_AFTER_LAST_TAB = 1 << 3,
		JUSTIFICATION_CONSTRAIN_ELLIPSIS = 1 << 4,
	};

	enum LineBreakFlag {
		BREAK_NONE = 0,
		BREAK_MANDATORY = 1 << 5,
		BREAK_WORD_BOUND = 1 << 6,
		BREAK_GRAPHEME_BOUND = 1 << 7
	};

	enum TextOverrunFlag {
		OVERRUN_NO_TRIMMING = 0,
		OVERRUN_TRIM = 1 << 0,
		OVERRUN_TRIM_WORD_ONLY = 1 << 1,
		OVERRUN_ADD_ELLIPSIS = 1 << 2,
		OVERRUN_ENFORCE_ELLIPSIS = 1 << 3,
		OVERRUN_JUSTIFICATION_AWARE = 1 << 4,
	};

	enum GraphemeFlag {
		GRAPHEME_IS_VALID = 1 << 0, // Glyph is valid.
		GRAPHEME_IS_RTL = 1 << 1, // Glyph is right-to-left.
		GRAPHEME_IS_VIRTUAL = 1 << 2, // Glyph is not part of source string (added by fit_to_width function, do not affect caret movement).
		GRAPHEME_IS_SPACE = 1 << 3, // Is whitespace (for justification and word breaks).
		GRAPHEME_IS_BREAK_HARD = 1 << 4, // Is line break (mandatory break, e.g. "\n").
		GRAPHEME_IS_BREAK_SOFT = 1 << 5, // Is line break (optional break, e.g. space).
		GRAPHEME_IS_TAB = 1 << 6, // Is tab or vertical tab.
		GRAPHEME_IS_ELONGATION = 1 << 7, // Elongation (e.g. kashida), glyph can be duplicated or truncated to fit line to width.
		GRAPHEME_IS_PUNCTUATION = 1 << 8, // Punctuation, except underscore (can be used as word break, but not line break or justifiction).
		GRAPHEME_IS_UNDERSCORE = 1 << 9, // Underscore (can be used as word break).
	};

	enum Hinting {
		HINTING_NONE,
		HINTING_LIGHT,
		HINTING_NORMAL
	};

	enum Feature {
		FEATURE_BIDI_LAYOUT = 1 << 0,
		FEATURE_VERTICAL_LAYOUT = 1 << 1,
		FEATURE_SHAPING = 1 << 2,
		FEATURE_KASHIDA_JUSTIFICATION = 1 << 3,
		FEATURE_BREAK_ITERATORS = 1 << 4,
		FEATURE_FONT_SYSTEM = 1 << 5,
		FEATURE_FONT_VARIABLE = 1 << 6,
		FEATURE_USE_SUPPORT_DATA = 1 << 7
	};

	enum ContourPointTag {
		CONTOUR_CURVE_TAG_ON = 0x01,
		CONTOUR_CURVE_TAG_OFF_CONIC = 0x00,
		CONTOUR_CURVE_TAG_OFF_CUBIC = 0x02
	};

	enum SpacingType {
		SPACING_GLYPH,
		SPACING_SPACE,
		SPACING_TOP,
		SPACING_BOTTOM,
	};

	enum FontStyle {
		FONT_BOLD = 1 << 0,
		FONT_ITALIC = 1 << 1,
		FONT_FIXED_WIDTH = 1 << 2,
	};

	struct Glyph {
		int start = -1; // Start offset in the source string.
		int end = -1; // End offset in the source string.

		uint8_t count = 0; // Number of glyphs in the grapheme, set in the first glyph only.
		uint8_t repeat = 1; // Draw multiple times in the row.
		uint16_t flags = 0; // Grapheme flags (valid, rtl, virtual), set in the first glyph only.

		real_t x_off = 0.f; // Offset from the origin of the glyph on baseline.
		real_t y_off = 0.f;
		real_t advance = 0.f; // Advance to the next glyph along baseline(x for horizontal layout, y for vertical).

		RID font_rid; // Font resource.
		int font_size = 0; // Font size;
		int span = 0;
		int32_t index = 0; // Glyph index (font specific) or UTF-32 codepoint (for the invalid glyphs).

		bool operator==(const Glyph &p_a) const;
		bool operator!=(const Glyph &p_a) const;

		bool operator<(const Glyph &p_a) const;
		bool operator>(const Glyph &p_a) const;
	};

	struct GlyphCompare { // For line breaking reordering.
		_FORCE_INLINE_ bool operator()(const Glyph &l, const Glyph &r) const {
			if (l.start == r.start) {
				if (l.count == r.count) {
					if ((l.flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) {
						return false;
					} else {
						return true;
					}
				}
				return l.count > r.count; // Sort first glyph with count & flags, order of the rest are irrelevant.
			} else {
				return l.start < r.start;
			}
		}
	};

	struct TrimData {
		int trim_pos = -1;
		int ellipsis_pos = -1;
		Vector<TextServer::Glyph> ellipsis_glyph_buf;
	};

	struct GlyphDrawPolicy {
		virtual ~GlyphDrawPolicy() {}
		virtual int get_outline_size() const { return 0; }
		virtual void draw(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color = OGUI::Color4f::vector_one());
		static void drawQuad(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color = OGUI::Color4f::vector_one(), bool noGamma = true);
#ifdef MODULE_MSDFGEN_ENABLED
		virtual void drawMsdf(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color = OGUI::Color4f::vector_one(), float p_outline_size = 0, float p_px_range = 1.0, float p_scale = 1.0);
		static void drawMsdfQuad(OGUI::PrimDrawContext& list, const OGUI::Rect &rect, OGUI::TextureHandle texture, const OGUI::Rect &uv, const OGUI::Color4f &color = OGUI::Color4f::vector_one(), float p_outline_size = 0, float p_px_range = 1.0, float p_scale = 1.0, bool noGamma = true);
#endif
	};

	struct ShapedTextData {
		Mutex mutex;

		/* Source data */
		RID parent; // Substring parent ShapedTextData.

		int start = 0; // Substring start offset in the parent string.
		int end = 0; // Substring end offset in the parent string.

		String text;
		TextServer::Direction direction = DIRECTION_LTR; // Desired text direction.
		TextServer::Orientation orientation = ORIENTATION_HORIZONTAL;

		struct Span {
			int start = -1;
			int end = -1;

			Vector<RID> fonts;
			int font_size = 0;
			int64_t flags = 0;
			Variant embedded_key = nullptr;
			std::shared_ptr<GlyphDrawPolicy> draw_policy;

			String language;
			Map<uint32_t, double> features;

			float letter_spacing = 0;
			float word_spacing = 0;
			
			TextDecorationData decoration;
		};
		Vector<Span> spans;

		struct EmbeddedObject {
			int pos = 0;
			InlineAlign inline_align = INLINE_ALIGN_BOTTOM;
			Rect2 rect;
		};
		Map<Variant, EmbeddedObject> objects;

		/* Shaped data */
		TextServer::Direction para_direction = DIRECTION_LTR; // Detected text direction.
		bool valid = false; // String is shaped.
		bool line_breaks_valid = false; // Line and word break flags are populated (and virtual zero width spaces inserted).
		bool justification_ops_valid = false; // Virtual elongation glyphs are added to the string.
		bool sort_valid = false;
		bool text_trimmed = false;

		bool preserve_invalid = true; // Draw hex code box instead of missing characters.
		bool preserve_control = false; // Draw control characters.

		real_t ascent = 0.f; // Ascent for horizontal layout, 1/2 of width for vertical.
		real_t descent = 0.f; // Descent for horizontal layout, 1/2 of width for vertical.
		real_t width = 0.f; // Width for horizontal layout, height for vertical.
		real_t width_trimmed = 0.f;

		real_t upos = 0.f;
		real_t uthk = 0.f;

		TrimData overrun_trim_data;
		bool fit_width_minimum_reached = false;

		Vector<TextServer::Glyph> glyphs;
		Vector<TextServer::Glyph> glyphs_logical;
	};

protected:
	static Vector3 hex_code_box_font_size[2];
	static OGUI::TextureHandle hex_code_box_font_tex[2];

public:
	static void initialize_hex_code_box_fonts();
	static void finish_hex_code_box_fonts();

	virtual bool has_feature(Feature p_feature) = 0;
	virtual String get_name() const = 0;

	virtual void free(RID p_rid) = 0;
	virtual bool has(RID p_rid) = 0;
	virtual bool load_support_data(const String &p_filename) = 0;

#ifdef TOOLS_ENABLED
	virtual String get_support_data_filename() = 0;
	virtual String get_support_data_info() = 0;
	virtual bool save_support_data(const String &p_filename) = 0;
#endif

	virtual bool is_locale_right_to_left(const String &p_locale) = 0;

	virtual int32_t name_to_tag(const String &p_name) const { return 0; };
	virtual String tag_to_name(int32_t p_tag) const { return ""; };

	/* Font interface */
	virtual RID create_font() = 0;

	virtual void font_set_data(RID p_font_rid, const PackedByteArray &p_data) = 0;
	virtual void font_set_data_ptr(RID p_font_rid, const uint8_t *p_data_ptr, size_t p_data_size) = 0;

	virtual void font_set_style(const RID &p_font_rid, int64_t /*FontStyle*/ p_style) = 0;
	virtual int64_t /*FontStyle*/ font_get_style(const RID &p_font_rid) const = 0;

	virtual void font_set_antialiased(RID p_font_rid, bool p_antialiased) = 0;
	virtual bool font_is_antialiased(RID p_font_rid) const = 0;

	virtual void font_set_multichannel_signed_distance_field(RID p_font_rid, bool p_msdf) = 0;
	virtual bool font_is_multichannel_signed_distance_field(RID p_font_rid) const = 0;

	virtual void font_set_msdf_pixel_range(RID p_font_rid, int p_msdf_pixel_range) = 0;
	virtual int font_get_msdf_pixel_range(RID p_font_rid) const = 0;

	virtual void font_set_msdf_size(RID p_font_rid, int p_msdf_size) = 0;
	virtual int font_get_msdf_size(RID p_font_rid) const = 0;

	virtual void font_set_fixed_size(RID p_font_rid, int p_fixed_size) = 0;
	virtual int font_get_fixed_size(RID p_font_rid) const = 0;

	virtual void font_set_force_autohinter(RID p_font_rid, bool p_force_autohinter) = 0;
	virtual bool font_is_force_autohinter(RID p_font_rid) const = 0;

	virtual void font_set_hinting(RID p_font_rid, TextServer::Hinting p_hinting) = 0;
	virtual TextServer::Hinting font_get_hinting(RID p_font_rid) const = 0;

	virtual void font_set_variation_coordinates(RID p_font_rid, const Map<uint32_t, double> &p_variation_coordinates) = 0;
	virtual Map<uint32_t, double> font_get_variation_coordinates(RID p_font_rid) const = 0;

	virtual void font_set_oversampling(RID p_font_rid, real_t p_oversampling) = 0;
	virtual real_t font_get_oversampling(RID p_font_rid) const = 0;

	virtual Vector<Vector2i> font_get_size_cache_list(RID p_font_rid) const = 0;
	virtual void font_clear_size_cache(RID p_font_rid) = 0;
	virtual void font_remove_size_cache(RID p_font_rid, const Vector2i &p_size) = 0;

	virtual void font_set_ascent(RID p_font_rid, int p_size, real_t p_ascent) = 0;
	virtual real_t font_get_ascent(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_descent(RID p_font_rid, int p_size, real_t p_descent) = 0;
	virtual real_t font_get_descent(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_underline_position(RID p_font_rid, int p_size, real_t p_underline_position) = 0;
	virtual real_t font_get_underline_position(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_strickout_position(RID p_font_rid, int p_size, real_t p_underline_position) = 0;
	virtual real_t font_get_strickout_position(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_underline_thickness(RID p_font_rid, int p_size, real_t p_underline_thickness) = 0;
	virtual real_t font_get_underline_thickness(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_strickout_thickness(RID p_font_rid, int p_size, real_t p_underline_thickness) = 0;
	virtual real_t font_get_strickout_thickness(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_scale(RID p_font_rid, int p_size, real_t p_scale) = 0;
	virtual real_t font_get_scale(RID p_font_rid, int p_size) const = 0;

	virtual void font_set_spacing(RID p_font_rid, int p_size, SpacingType p_spacing, int p_value) = 0;
	virtual int font_get_spacing(RID p_font_rid, int p_size, SpacingType p_spacing) const = 0;

	virtual int font_get_texture_count(RID p_font_rid, const Vector2i &p_size) const = 0;
	virtual void font_clear_textures(RID p_font_rid, const Vector2i &p_size) = 0;
	virtual void font_remove_texture(RID p_font_rid, const Vector2i &p_size, int p_texture_index) = 0;

	virtual void font_set_texture_image(RID p_font_rid, const Vector2i &p_size, int p_texture_index, const Ref<Image> &p_image) = 0;
	virtual Ref<Image> font_get_texture_image(RID p_font_rid, const Vector2i &p_size, int p_texture_index) const = 0;

	virtual void font_set_texture_offsets(RID p_font_rid, const Vector2i &p_size, int p_texture_index, const PackedInt32Array &p_offset) = 0;
	virtual PackedInt32Array font_get_texture_offsets(RID p_font_rid, const Vector2i &p_size, int p_texture_index) const = 0;

	virtual Vector<int32_t> font_get_glyph_list(RID p_font_rid, const Vector2i &p_size) const = 0;
	virtual void font_clear_glyphs(RID p_font_rid, const Vector2i &p_size) = 0;
	virtual void font_remove_glyph(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) = 0;

	virtual Vector2 font_get_glyph_advance(RID p_font_rid, int p_size, int32_t p_glyph) const = 0;
	virtual void font_set_glyph_advance(RID p_font_rid, int p_size, int32_t p_glyph, const Vector2 &p_advance) = 0;

	virtual Vector2 font_get_glyph_offset(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const = 0;
	virtual void font_set_glyph_offset(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_offset) = 0;

	virtual Vector2 font_get_glyph_size(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const = 0;
	virtual void font_set_glyph_size(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_gl_size) = 0;

	virtual Rect2 font_get_glyph_uv_rect(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const = 0;
	virtual void font_set_glyph_uv_rect(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Rect2 &p_uv_rect) = 0;

	virtual int font_get_glyph_texture_idx(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const = 0;
	virtual void font_set_glyph_texture_idx(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, int p_texture_idx) = 0;

	virtual bool font_get_glyph_contours(RID p_font, int p_size, int32_t p_index, Vector<Vector3> &r_points, Vector<int32_t> &r_contours, bool &r_orientation) const = 0;

	virtual Vector<Vector2i> font_get_kerning_list(RID p_font_rid, int p_size) const = 0;
	virtual void font_clear_kerning_map(RID p_font_rid, int p_size) = 0;
	virtual void font_remove_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair) = 0;

	virtual void font_set_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair, const Vector2 &p_kerning) = 0;
	virtual Vector2 font_get_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair) const = 0;

	virtual int32_t font_get_glyph_index(RID p_font_rid, int p_size, char32_t p_char, char32_t p_variation_selector) const = 0;

	virtual bool font_has_char(RID p_font_rid, char32_t p_char) const = 0;
	virtual String font_get_supported_chars(RID p_font_rid) const = 0;

	virtual void font_render_range(RID p_font, const Vector2i &p_size, char32_t p_start, char32_t p_end) = 0;
	virtual void font_render_glyph(RID p_font_rid, const Vector2i &p_size, int32_t p_index) = 0;

	virtual void font_draw_glyph(RID p_shaped, OGUI::PrimDrawContext& list, const Glyph& glyph, const Vector2 &p_pos, const Color &p_color = Color(1, 1, 1)) const = 0;
	virtual void font_draw_glyph_outline(RID p_shaped, OGUI::PrimDrawContext& list, const Glyph& glyph, int p_outline_size, const Vector2 &p_pos, const Color &p_color = Color(1, 1, 1)) const = 0;

	virtual bool font_is_language_supported(RID p_font_rid, const String &p_language) const = 0;
	virtual void font_set_language_support_override(RID p_font_rid, const String &p_language, bool p_supported) = 0;
	virtual bool font_get_language_support_override(RID p_font_rid, const String &p_language) = 0;
	virtual void font_remove_language_support_override(RID p_font_rid, const String &p_language) = 0;
	virtual Vector<String> font_get_language_support_overrides(RID p_font_rid) = 0;

	virtual bool font_is_script_supported(RID p_font_rid, const String &p_script) const = 0;
	virtual void font_set_script_support_override(RID p_font_rid, const String &p_script, bool p_supported) = 0;
	virtual bool font_get_script_support_override(RID p_font_rid, const String &p_script) = 0;
	virtual void font_remove_script_support_override(RID p_font_rid, const String &p_script) = 0;
	virtual Vector<String> font_get_script_support_overrides(RID p_font_rid) = 0;

	virtual Set<uint32_t> font_supported_feature_list(RID p_font_rid) const = 0;
	virtual Map<uint32_t, Vector3i> font_supported_variation_list(RID p_font_rid) const = 0;

	virtual real_t font_get_global_oversampling() const = 0;
	virtual void font_set_global_oversampling(real_t p_oversampling) = 0;

	Vector2 get_hex_code_box_size(int p_size, char32_t p_index) const;
	void draw_hex_code_box(OGUI::PrimDrawContext& list, int p_size, const Vector2 &p_pos, char32_t p_index, const Color &p_color) const;

	/* Shaped text buffer interface */

	virtual RID create_shaped_text(Direction p_direction = DIRECTION_AUTO, Orientation p_orientation = ORIENTATION_HORIZONTAL) = 0;

	virtual void shaped_text_clear(RID p_shaped) = 0;

	virtual void shaped_text_set_direction(RID p_shaped, Direction p_direction = DIRECTION_AUTO) = 0;
	virtual Direction shaped_text_get_direction(RID p_shaped) const = 0;

	virtual void shaped_text_set_bidi_override(RID p_shaped, const Vector<Vector2i> &p_override) = 0;

	virtual void shaped_text_set_orientation(RID p_shaped, Orientation p_orientation = ORIENTATION_HORIZONTAL) = 0;
	virtual Orientation shaped_text_get_orientation(RID p_shaped) const = 0;

	virtual void shaped_text_set_preserve_invalid(RID p_shaped, bool p_enabled) = 0;
	virtual bool shaped_text_get_preserve_invalid(RID p_shaped) const = 0;

	virtual void shaped_text_set_preserve_control(RID p_shaped, bool p_enabled) = 0;
	virtual bool shaped_text_get_preserve_control(RID p_shaped) const = 0;

	virtual bool shaped_text_add_string(RID p_shaped, const String &p_text, const Vector<RID> &p_fonts, int p_size, int64_t flags = 0, const std::shared_ptr<GlyphDrawPolicy> &draw_policy = {}, const Map<uint32_t, double> &p_opentype_features = {}, const String &p_language = "", const TextDecorationData& decoration = {}, float letter_spacing = 0, float word_spacing = 0) = 0;
	virtual bool shaped_text_add_object(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align = INLINE_ALIGN_CENTER, int p_length = 1) = 0;
	virtual bool shaped_text_resize_object(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align = INLINE_ALIGN_CENTER) = 0;
	virtual bool shaped_text_resize_object_raw(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align = INLINE_ALIGN_CENTER) = 0;

	virtual RID shaped_text_substr(RID p_shaped, int p_start, int p_length) const = 0; // Copy shaped substring (e.g. line break) without reshaping, but correctly reordered, preservers range.
	virtual RID shaped_text_get_parent(RID p_shaped) const = 0;
	
	virtual TextDecorationData shaped_text_get_decoration(RID p_shaped, int p_span) const = 0;

	virtual real_t shaped_text_fit_to_width(RID p_shaped, real_t p_width, int64_t /*JustificationFlag*/ p_jst_flags = JUSTIFICATION_KASHIDA) = 0;
	virtual real_t shaped_text_tab_align(RID p_shaped, const Vector<real_t> &p_tab_stops) = 0;

	virtual bool shaped_text_shape(RID p_shaped) = 0;
	virtual bool shaped_text_update_breaks(RID p_shaped) = 0;
	virtual bool shaped_text_update_justification_ops(RID p_shaped) = 0;

	virtual bool shaped_text_is_ready(RID p_shaped) const = 0;
	virtual bool shaped_test_is_hard_break(RID p_shaped) const;

	virtual Vector<Glyph> shaped_text_get_glyphs(RID p_shaped) const = 0;

	virtual Vector2i shaped_text_get_range(RID p_shaped) const = 0;

	virtual Vector<Glyph> shaped_text_sort_logical(RID p_shaped) = 0;

	virtual Vector<Vector2i> shaped_text_get_line_breaks_adv(RID p_shaped, const Vector<real_t> &p_width, int p_start = 0, bool p_once = true, int64_t /*TextBreakFlag*/ p_break_flags = BREAK_MANDATORY | BREAK_WORD_BOUND) const;
	virtual Vector<Vector2i> shaped_text_get_line_breaks(RID p_shaped, real_t p_width, int p_start = 0, int64_t /*TextBreakFlag*/ p_break_flags = BREAK_MANDATORY | BREAK_WORD_BOUND) const;
	virtual Vector<Vector2i> shaped_text_get_word_breaks(RID p_shaped, int p_grapheme_flags = GRAPHEME_IS_SPACE | GRAPHEME_IS_PUNCTUATION) const;

	virtual TrimData shaped_text_get_trim_data(RID p_shaped) const;
	virtual void shaped_text_overrun_trim_to_width(RID p_shaped, real_t p_width, int64_t p_trim_flags) = 0;
	virtual Vector<Variant> shaped_text_get_objects(RID p_shaped) const = 0;
	virtual Rect2 shaped_text_get_object_rect(RID p_shaped, Variant p_key) const = 0;

	virtual Size2 shaped_text_get_size(RID p_shaped) const = 0;
	virtual real_t shaped_text_get_ascent(RID p_shaped) const = 0;
	virtual real_t shaped_text_get_descent(RID p_shaped) const = 0;
	virtual real_t shaped_text_get_width(RID p_shaped) const = 0;
	virtual real_t shaped_text_get_underline_position(RID p_shaped) const = 0;
	virtual real_t shaped_text_get_underline_thickness(RID p_shaped) const = 0;

	virtual Direction shaped_text_get_dominant_direciton_in_range(RID p_shaped, int p_start, int p_end) const;

	virtual void shaped_text_get_carets(RID p_shaped, int p_position, Rect2 &p_leading_caret, Direction &p_leading_dir, Rect2 &p_trailing_caret, Direction &p_trailing_dir) const;
	virtual Vector<Vector2> shaped_text_get_selection(RID p_shaped, int p_start, int p_end) const;

	virtual int shaped_text_hit_test_grapheme(RID p_shaped, real_t p_coords) const; // Return grapheme index.
	virtual int shaped_text_hit_test_position(RID p_shaped, real_t p_coords) const; // Return caret/selection position.

	virtual int shaped_text_next_grapheme_pos(RID p_shaped, int p_pos);
	virtual int shaped_text_prev_grapheme_pos(RID p_shaped, int p_pos);

	// The pen position is always placed on the baseline and moveing left to right.
	virtual void shaped_text_draw(RID p_shaped, OGUI::PrimDrawContext& list, const Vector2 &p_pos, real_t p_clip_l = -1.f, real_t p_clip_r = -1.f, const Color &p_color = Color(1, 1, 1)) const;
	virtual void shaped_text_draw_outline(RID p_shaped, OGUI::PrimDrawContext& list, const Vector2 &p_pos, real_t p_clip_l = -1.f, real_t p_clip_r = -1.f, int p_outline_size = 1, const Color &p_color = Color(1, 1, 1)) const;

	// Number conversion.
	virtual String format_number(const String &p_string, const String &p_language = "") const { return p_string; };
	virtual String parse_number(const String &p_string, const String &p_language = "") const { return p_string; };
	virtual String percent_sign(const String &p_language = "") const { return "%"; };

	void canvas_item_add_rect(OGUI::PrimDrawContext& list, const Rect2 &p_rect, const Color &p_color) const;
	void canvas_item_add_texture_rect(OGUI::PrimDrawContext& list, const Rect2 &p_rect, const Color &p_color, OGUI::TextureHandle p_texture) const;
	void canvas_item_add_texture_rect_region(OGUI::PrimDrawContext& list, const Rect2 &p_rect, OGUI::TextureHandle p_texture, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), GlyphDrawPolicy* policy = nullptr) const;
	void canvas_item_add_msdf_texture_rect_region(OGUI::PrimDrawContext& list, const Rect2 &p_rect, OGUI::TextureHandle p_texture, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), float p_outline_size = 0, float p_px_range = 1.0, float p_scale = 1.0, GlyphDrawPolicy* policy = nullptr) const;

	TextServer();
	virtual ~TextServer();
};
//#define TS TextServerManager::get_primary_interface()
}