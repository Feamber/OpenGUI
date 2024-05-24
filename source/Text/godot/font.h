/*************************************************************************/
/*  font.h                                                               */
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

#include "text_server.h"
#include <memory>

namespace godot{
/*************************************************************************/
class Font;
class FontData {

	// Font source data.
	const uint8_t *data_ptr = nullptr;
	size_t data_size = 0;
	PackedByteArray data;

	bool antialiased = true;
	bool msdf = false;
	int msdf_pixel_range = 32;
	int msdf_size = 64;
	int fixed_size = 0;
	bool force_autohinter = false;
	TextServer::Hinting hinting = TextServer::HINTING_LIGHT;
	real_t oversampling = 0.f;

	// Cache.
	mutable Vector<RID> cache;

	_FORCE_INLINE_ void _clear_cache();
	_FORCE_INLINE_ void _ensure_rid(int p_cache_index) const;
    void emit_changed();

public:
    Vector<Font*> fonts;
	// Font source data.
	virtual void set_data_ptr(const uint8_t *p_data, size_t p_size);
	virtual void set_data(const PackedByteArray &p_data);
	virtual PackedByteArray get_data() const;

	// Common properties.
	virtual void set_antialiased(bool p_antialiased);
	virtual bool is_antialiased() const;

	virtual void set_multichannel_signed_distance_field(bool p_msdf);
	virtual bool is_multichannel_signed_distance_field() const;

	virtual void set_msdf_pixel_range(int p_msdf_pixel_range);
	virtual int get_msdf_pixel_range() const;

	virtual void set_msdf_size(int p_msdf_size);
	virtual int get_msdf_size() const;

	virtual void set_fixed_size(int p_fixed_size);
	virtual int get_fixed_size() const;

	virtual void set_force_autohinter(bool p_force_autohinter);
	virtual bool is_force_autohinter() const;

	virtual void set_hinting(TextServer::Hinting p_hinting);
	virtual TextServer::Hinting get_hinting() const;

	virtual void set_oversampling(real_t p_oversampling);
	virtual real_t get_oversampling() const;

	// Cache.
	virtual RID find_cache(const Map<uint32_t, double> &p_variation_coordinates) const;

	virtual int get_cache_count() const;
	virtual void clear_cache();
	virtual void remove_cache(int p_cache_index);

	virtual Vector<Vector2i> get_size_cache_list(int p_cache_index) const;
	virtual void clear_size_cache(int p_cache_index);
	virtual void remove_size_cache(int p_cache_index, const Vector2i &p_size);

	virtual void set_variation_coordinates(int p_cache_index, const Map<uint32_t, double> &p_variation_coordinates);
	virtual Map<uint32_t, double> get_variation_coordinates(int p_cache_index) const;

	virtual void set_ascent(int p_cache_index, int p_size, real_t p_ascent);
	virtual real_t get_ascent(int p_cache_index, int p_size) const;

	virtual void set_descent(int p_cache_index, int p_size, real_t p_descent);
	virtual real_t get_descent(int p_cache_index, int p_size) const;

	virtual void set_underline_position(int p_cache_index, int p_size, real_t p_underline_position);
	virtual real_t get_underline_position(int p_cache_index, int p_size) const;

	virtual void set_underline_thickness(int p_cache_index, int p_size, real_t p_underline_thickness);
	virtual real_t get_underline_thickness(int p_cache_index, int p_size) const;

	virtual void set_scale(int p_cache_index, int p_size, real_t p_scale); // Rendering scale for bitmap fonts (e.g. emoji fonts).
	virtual real_t get_scale(int p_cache_index, int p_size) const;

	virtual int get_texture_count(int p_cache_index, const Vector2i &p_size) const;
	virtual void clear_textures(int p_cache_index, const Vector2i &p_size);
	virtual void remove_texture(int p_cache_index, const Vector2i &p_size, int p_texture_index);

	virtual void set_texture_image(int p_cache_index, const Vector2i &p_size, int p_texture_index, const Ref<Image> &p_image);
	virtual Ref<Image> get_texture_image(int p_cache_index, const Vector2i &p_size, int p_texture_index) const;

	virtual void set_texture_offsets(int p_cache_index, const Vector2i &p_size, int p_texture_index, const PackedInt32Array &p_offset);
	virtual PackedInt32Array get_texture_offsets(int p_cache_index, const Vector2i &p_size, int p_texture_index) const;

	virtual Vector<int32_t> get_glyph_list(int p_cache_index, const Vector2i &p_size) const;
	virtual void clear_glyphs(int p_cache_index, const Vector2i &p_size);
	virtual void remove_glyph(int p_cache_index, const Vector2i &p_size, int32_t p_glyph);

	virtual void set_glyph_advance(int p_cache_index, int p_size, int32_t p_glyph, const Vector2 &p_advance);
	virtual Vector2 get_glyph_advance(int p_cache_index, int p_size, int32_t p_glyph) const;

	virtual void set_glyph_offset(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_offset);
	virtual Vector2 get_glyph_offset(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const;

	virtual void set_glyph_size(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_gl_size);
	virtual Vector2 get_glyph_size(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const;

	virtual void set_glyph_uv_rect(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Rect2 &p_uv_rect);
	virtual Rect2 get_glyph_uv_rect(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const;

	virtual void set_glyph_texture_idx(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, int p_texture_idx);
	virtual int get_glyph_texture_idx(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const;

	virtual Vector<Vector2i> get_kerning_list(int p_cache_index, int p_size) const;
	virtual void clear_kerning_map(int p_cache_index, int p_size);
	virtual void remove_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair);

	virtual void set_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair, const Vector2 &p_kerning);
	virtual Vector2 get_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair) const;

	virtual void render_range(int p_cache_index, const Vector2i &p_size, char32_t p_start, char32_t p_end);
	virtual void render_glyph(int p_cache_index, const Vector2i &p_size, int32_t p_index);

	virtual RID get_cache_rid(int p_cache_index) const;

	// Language/script support override.
	virtual bool is_language_supported(const String &p_language) const;
	virtual void set_language_support_override(const String &p_language, bool p_supported);
	virtual bool get_language_support_override(const String &p_language) const;
	virtual void remove_language_support_override(const String &p_language);
	virtual Vector<String> get_language_support_overrides() const;

	virtual bool is_script_supported(const String &p_script) const;
	virtual void set_script_support_override(const String &p_script, bool p_supported);
	virtual bool get_script_support_override(const String &p_script) const;
	virtual void remove_script_support_override(const String &p_script);
	virtual Vector<String> get_script_support_overrides() const;

	// Base font properties.
	virtual bool has_char(char32_t p_char) const;
	virtual String get_supported_chars() const;

	virtual int32_t get_glyph_index(int p_size, char32_t p_char, char32_t p_variation_selector = 0x0000) const;

	virtual Set<uint32_t> get_supported_feature_list() const;
	virtual Map<uint32_t, Vector3i> get_supported_variation_list() const;

	FontData();
	virtual ~FontData();
};

/*************************************************************************/

class TextLine;
class TextParagraph;

class Font {

	// Font data cache.
	Vector<Ref<FontData>> data;
	mutable Vector<RID> rids;

	// Font config.
	int base_size = 16;
	Map<uint32_t, double> variation_coordinates;
	int spacing_bottom = 0;
	int spacing_top = 0;

	_FORCE_INLINE_ void _data_changed();
	_FORCE_INLINE_ void _ensure_rid(int p_index) const; // Find or create cache record.

    void emit_changed() {}
    friend class FontData;
public:
	Set<uint32_t> get_feature_list() const;

	// Font data.
	virtual void add_data(const Ref<FontData> &p_data);
	virtual void set_data(int p_idx, const Ref<FontData> &p_data);
	virtual int get_data_count() const;
	virtual Ref<FontData> get_data(int p_idx) const;
	virtual RID get_data_rid(int p_idx) const;
	virtual void clear_data();
	virtual void remove_data(int p_idx);

	// Font configuration.
	virtual void set_base_size(int p_size);
	virtual int get_base_size() const;

	virtual void set_variation_coordinates(const Map<uint32_t, double> &p_variation_coordinates);
	virtual Map<uint32_t, double> get_variation_coordinates() const;

	// Font metrics.
	virtual real_t get_height(int p_size = -1) const;
	virtual real_t get_ascent(int p_size = -1) const;
	virtual real_t get_descent(int p_size = -1) const;
	virtual real_t get_underline_position(int p_size = -1) const;
	virtual real_t get_underline_thickness(int p_size = -1) const;

	// Helper functions.
	virtual bool has_char(char32_t p_char) const;
	virtual String get_supported_chars() const;

	// Drawing char.
	virtual Size2 get_char_size(char32_t p_char, char32_t p_next = 0, int p_size = -1) const;

	Vector<RID> get_rids() const;

	void update_changes();

	Font();
	virtual ~Font();
};
}