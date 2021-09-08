/*************************************************************************/
/*  font.cpp                                                             */
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
#define DLL_IMPLEMENTATION
#include "font.h"

using namespace godot;
void FontData::emit_changed()
{
	for(auto font : fonts)
	{
        font->_data_changed();
	}
}

_FORCE_INLINE_ void FontData::_clear_cache() {
	for (int i = 0; i < cache.size(); i++) {
		if (cache[i].is_valid()) {
			TS->free(cache[i]);
			cache.data()[i] = RID();
		}
	}
}

_FORCE_INLINE_ void FontData::_ensure_rid(int p_cache_index) const {
	if (unlikely(p_cache_index >= cache.size())) {
		cache.resize(p_cache_index + 1);
	}
	if (unlikely(!cache[p_cache_index].is_valid())) {
		cache.data()[p_cache_index] = TS->create_font();
		TS->font_set_data_ptr(cache[p_cache_index], data_ptr, data_size);
		TS->font_set_antialiased(cache[p_cache_index], antialiased);
		TS->font_set_multichannel_signed_distance_field(cache[p_cache_index], msdf);
		TS->font_set_msdf_pixel_range(cache[p_cache_index], msdf_pixel_range);
		TS->font_set_msdf_size(cache[p_cache_index], msdf_size);
		TS->font_set_fixed_size(cache[p_cache_index], fixed_size);
		TS->font_set_force_autohinter(cache[p_cache_index], force_autohinter);
		TS->font_set_hinting(cache[p_cache_index], hinting);
		TS->font_set_oversampling(cache[p_cache_index], oversampling);
	}
}


void FontData::set_data_ptr(const uint8_t *p_data, size_t p_size) {
	data.clear();
	data_ptr = p_data;
	data_size = p_size;

	if (data_ptr != nullptr) {
		for (int i = 0; i < cache.size(); i++) {
			if (cache[i].is_valid()) {
				TS->font_set_data_ptr(cache[i], data_ptr, data_size);
			}
		}
	}
}

void FontData::set_data(const PackedByteArray &p_data) {
	data = p_data;
	data_ptr = data.ptr();
	data_size = data.size();

	if (data_ptr != nullptr) {
		for (int i = 0; i < cache.size(); i++) {
			if (cache[i].is_valid()) {
				TS->font_set_data_ptr(cache[i], data_ptr, data_size);
			}
		}
	}
}

PackedByteArray FontData::get_data() const {
	return data;
}

void FontData::set_antialiased(bool p_antialiased) {
	if (antialiased != p_antialiased) {
		antialiased = p_antialiased;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_antialiased(cache[i], antialiased);
		}
		emit_changed();
	}
}

bool FontData::is_antialiased() const {
	return antialiased;
}

void FontData::set_multichannel_signed_distance_field(bool p_msdf) {
	if (msdf != p_msdf) {
		msdf = p_msdf;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_multichannel_signed_distance_field(cache[i], msdf);
		}
		emit_changed();
	}
}

bool FontData::is_multichannel_signed_distance_field() const {
	return msdf;
}

void FontData::set_msdf_pixel_range(int p_msdf_pixel_range) {
	if (msdf_pixel_range != p_msdf_pixel_range) {
		msdf_pixel_range = p_msdf_pixel_range;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_msdf_pixel_range(cache[i], msdf_pixel_range);
		}
		emit_changed();
	}
}

int FontData::get_msdf_pixel_range() const {
	return msdf_pixel_range;
}

void FontData::set_msdf_size(int p_msdf_size) {
	if (msdf_size != p_msdf_size) {
		msdf_size = p_msdf_size;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_msdf_size(cache[i], msdf_size);
		}
		emit_changed();
	}
}

int FontData::get_msdf_size() const {
	return msdf_size;
}

void FontData::set_fixed_size(int p_fixed_size) {
	if (fixed_size != p_fixed_size) {
		fixed_size = p_fixed_size;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_fixed_size(cache[i], fixed_size);
		}
		emit_changed();
	}
}

int FontData::get_fixed_size() const {
	return fixed_size;
}

void FontData::set_force_autohinter(bool p_force_autohinter) {
	if (force_autohinter != p_force_autohinter) {
		force_autohinter = p_force_autohinter;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_force_autohinter(cache[i], force_autohinter);
		}
		emit_changed();
	}
}

bool FontData::is_force_autohinter() const {
	return force_autohinter;
}

void FontData::set_hinting(TextServer::Hinting p_hinting) {
	if (hinting != p_hinting) {
		hinting = p_hinting;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_hinting(cache[i], hinting);
		}
		emit_changed();
	}
}

TextServer::Hinting FontData::get_hinting() const {
	return hinting;
}

void FontData::set_oversampling(real_t p_oversampling) {
	if (oversampling != p_oversampling) {
		oversampling = p_oversampling;
		for (int i = 0; i < cache.size(); i++) {
			_ensure_rid(i);
			TS->font_set_oversampling(cache[i], oversampling);
		}
		emit_changed();
	}
}

real_t FontData::get_oversampling() const {
	return oversampling;
}

RID FontData::find_cache(const Map<uint32_t, double> &p_variation_coordinates) const {
	// Find existing variation cache.
	const Map<uint32_t, Vector3i> &supported_coords = get_supported_variation_list();
	for (int i = 0; i < cache.size(); i++) {
		if (cache[i].is_valid()) {
			const Map<uint32_t, double> &cache_var = TS->font_get_variation_coordinates(cache[i]);
			bool match = true;
			for (auto& pair : supported_coords) {
				const Vector3 &def = pair.second;

				real_t c_v = def.z;
				if (cache_var.has(pair.first)) {
					real_t val = cache_var.find(pair.first)->second;
					c_v = CLAMP(val, def.x, def.y);
				}

				real_t s_v = def.z;
				if (p_variation_coordinates.has(pair.first)) {
					real_t val = p_variation_coordinates.find(pair.first)->second;
					s_v = CLAMP(val, def.x, def.y);
				}

				match = match && (c_v == s_v);
			}
			if (match) {
				return cache[i];
			}
		}
	}

	// Create new variation cache.
	int idx = cache.size();
	_ensure_rid(idx);
	TS->font_set_variation_coordinates(cache[idx], p_variation_coordinates);
	return cache[idx];
}

int FontData::get_cache_count() const {
	return cache.size();
}

void FontData::clear_cache() {
	_clear_cache();
	cache.clear();
}

void FontData::remove_cache(int p_cache_index) {
	ERR_FAIL_INDEX(p_cache_index, cache.size());
	if (cache[p_cache_index].is_valid()) {
		TS->free(cache.data()[p_cache_index]);
	}
	cache.remove(p_cache_index);
	emit_changed();
}

Vector<Vector2i> FontData::get_size_cache_list(int p_cache_index) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_size_cache_list(cache[p_cache_index]);
}

void FontData::clear_size_cache(int p_cache_index) {
	_ensure_rid(p_cache_index);
	TS->font_clear_size_cache(cache[p_cache_index]);
}

void FontData::remove_size_cache(int p_cache_index, const Vector2i &p_size) {
	_ensure_rid(p_cache_index);
	TS->font_remove_size_cache(cache[p_cache_index], p_size);
}

void FontData::set_variation_coordinates(int p_cache_index, const Map<uint32_t, double> &p_variation_coordinates) {
	_ensure_rid(p_cache_index);
	TS->font_set_variation_coordinates(cache[p_cache_index], p_variation_coordinates);
	emit_changed();
}

Map<uint32_t, double> FontData::get_variation_coordinates(int p_cache_index) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_variation_coordinates(cache[p_cache_index]);
}

void FontData::set_ascent(int p_cache_index, int p_size, real_t p_ascent) {
	_ensure_rid(p_cache_index);
	TS->font_set_ascent(cache[p_cache_index], p_size, p_ascent);
}

real_t FontData::get_ascent(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_ascent(cache[p_cache_index], p_size);
}

void FontData::set_descent(int p_cache_index, int p_size, real_t p_descent) {
	_ensure_rid(p_cache_index);
	TS->font_set_descent(cache[p_cache_index], p_size, p_descent);
}

real_t FontData::get_descent(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_descent(cache[p_cache_index], p_size);
}

void FontData::set_underline_position(int p_cache_index, int p_size, real_t p_underline_position) {
	_ensure_rid(p_cache_index);
	TS->font_set_underline_position(cache[p_cache_index], p_size, p_underline_position);
}

real_t FontData::get_underline_position(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_underline_position(cache[p_cache_index], p_size);
}

void FontData::set_underline_thickness(int p_cache_index, int p_size, real_t p_underline_thickness) {
	_ensure_rid(p_cache_index);
	TS->font_set_underline_thickness(cache[p_cache_index], p_size, p_underline_thickness);
}

real_t FontData::get_underline_thickness(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_underline_thickness(cache[p_cache_index], p_size);
}

void FontData::set_scale(int p_cache_index, int p_size, real_t p_scale) {
	_ensure_rid(p_cache_index);
	TS->font_set_scale(cache[p_cache_index], p_size, p_scale);
}

real_t FontData::get_scale(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_scale(cache[p_cache_index], p_size);
}

void FontData::set_spacing(int p_cache_index, int p_size, TextServer::SpacingType p_spacing, int p_value) {
	_ensure_rid(p_cache_index);
	TS->font_set_spacing(cache[p_cache_index], p_size, p_spacing, p_value);
}

int FontData::get_spacing(int p_cache_index, int p_size, TextServer::SpacingType p_spacing) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_spacing(cache[p_cache_index], p_size, p_spacing);
}

int FontData::get_texture_count(int p_cache_index, const Vector2i &p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_texture_count(cache[p_cache_index], p_size);
}

void FontData::clear_textures(int p_cache_index, const Vector2i &p_size) {
	_ensure_rid(p_cache_index);
	TS->font_clear_textures(cache[p_cache_index], p_size);
}

void FontData::remove_texture(int p_cache_index, const Vector2i &p_size, int p_texture_index) {
	_ensure_rid(p_cache_index);
	TS->font_remove_texture(cache[p_cache_index], p_size, p_texture_index);
}

void FontData::set_texture_image(int p_cache_index, const Vector2i &p_size, int p_texture_index, const Ref<Image> &p_image) {
	_ensure_rid(p_cache_index);
	TS->font_set_texture_image(cache[p_cache_index], p_size, p_texture_index, p_image);
}

Ref<Image> FontData::get_texture_image(int p_cache_index, const Vector2i &p_size, int p_texture_index) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_texture_image(cache[p_cache_index], p_size, p_texture_index);
}

void FontData::set_texture_offsets(int p_cache_index, const Vector2i &p_size, int p_texture_index, const PackedInt32Array &p_offset) {
	_ensure_rid(p_cache_index);
	TS->font_set_texture_offsets(cache[p_cache_index], p_size, p_texture_index, p_offset);
}

PackedInt32Array FontData::get_texture_offsets(int p_cache_index, const Vector2i &p_size, int p_texture_index) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_texture_offsets(cache[p_cache_index], p_size, p_texture_index);
}

Vector<int32_t> FontData::get_glyph_list(int p_cache_index, const Vector2i &p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_list(cache[p_cache_index], p_size);
}

void FontData::clear_glyphs(int p_cache_index, const Vector2i &p_size) {
	_ensure_rid(p_cache_index);
	TS->font_clear_glyphs(cache[p_cache_index], p_size);
}

void FontData::remove_glyph(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) {
	_ensure_rid(p_cache_index);
	TS->font_remove_glyph(cache[p_cache_index], p_size, p_glyph);
}

void FontData::set_glyph_advance(int p_cache_index, int p_size, int32_t p_glyph, const Vector2 &p_advance) {
	_ensure_rid(p_cache_index);
	TS->font_set_glyph_advance(cache[p_cache_index], p_size, p_glyph, p_advance);
}

Vector2 FontData::get_glyph_advance(int p_cache_index, int p_size, int32_t p_glyph) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_advance(cache[p_cache_index], p_size, p_glyph);
}

void FontData::set_glyph_offset(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_offset) {
	_ensure_rid(p_cache_index);
	TS->font_set_glyph_offset(cache[p_cache_index], p_size, p_glyph, p_offset);
}

Vector2 FontData::get_glyph_offset(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_offset(cache[p_cache_index], p_size, p_glyph);
}

void FontData::set_glyph_size(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_gl_size) {
	_ensure_rid(p_cache_index);
	TS->font_set_glyph_size(cache[p_cache_index], p_size, p_glyph, p_gl_size);
}

Vector2 FontData::get_glyph_size(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_size(cache[p_cache_index], p_size, p_glyph);
}

void FontData::set_glyph_uv_rect(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, const Rect2 &p_uv_rect) {
	_ensure_rid(p_cache_index);
	TS->font_set_glyph_uv_rect(cache[p_cache_index], p_size, p_glyph, p_uv_rect);
}

Rect2 FontData::get_glyph_uv_rect(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_uv_rect(cache[p_cache_index], p_size, p_glyph);
}

void FontData::set_glyph_texture_idx(int p_cache_index, const Vector2i &p_size, int32_t p_glyph, int p_texture_idx) {
	_ensure_rid(p_cache_index);
	TS->font_set_glyph_texture_idx(cache[p_cache_index], p_size, p_glyph, p_texture_idx);
}

int FontData::get_glyph_texture_idx(int p_cache_index, const Vector2i &p_size, int32_t p_glyph) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_glyph_texture_idx(cache[p_cache_index], p_size, p_glyph);
}

Vector<Vector2i> FontData::get_kerning_list(int p_cache_index, int p_size) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_kerning_list(cache[p_cache_index], p_size);
}

void FontData::clear_kerning_map(int p_cache_index, int p_size) {
	_ensure_rid(p_cache_index);
	TS->font_clear_kerning_map(cache[p_cache_index], p_size);
}

void FontData::remove_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair) {
	_ensure_rid(p_cache_index);
	TS->font_remove_kerning(cache[p_cache_index], p_size, p_glyph_pair);
}

void FontData::set_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair, const Vector2 &p_kerning) {
	_ensure_rid(p_cache_index);
	TS->font_set_kerning(cache[p_cache_index], p_size, p_glyph_pair, p_kerning);
}

Vector2 FontData::get_kerning(int p_cache_index, int p_size, const Vector2i &p_glyph_pair) const {
	_ensure_rid(p_cache_index);
	return TS->font_get_kerning(cache[p_cache_index], p_size, p_glyph_pair);
}

void FontData::render_range(int p_cache_index, const Vector2i &p_size, char32_t p_start, char32_t p_end) {
	_ensure_rid(p_cache_index);
	TS->font_render_range(cache[p_cache_index], p_size, p_start, p_end);
}

void FontData::render_glyph(int p_cache_index, const Vector2i &p_size, int32_t p_index) {
	_ensure_rid(p_cache_index);
	TS->font_render_glyph(cache[p_cache_index], p_size, p_index);
}

RID FontData::get_cache_rid(int p_cache_index) const {
	_ensure_rid(p_cache_index);
	return cache[p_cache_index];
}

bool FontData::is_language_supported(const String &p_language) const {
	_ensure_rid(0);
	return TS->font_is_language_supported(cache[0], p_language);
}

void FontData::set_language_support_override(const String &p_language, bool p_supported) {
	_ensure_rid(0);
	TS->font_set_language_support_override(cache[0], p_language, p_supported);
}

bool FontData::get_language_support_override(const String &p_language) const {
	_ensure_rid(0);
	return TS->font_get_language_support_override(cache[0], p_language);
}

void FontData::remove_language_support_override(const String &p_language) {
	_ensure_rid(0);
	TS->font_remove_language_support_override(cache[0], p_language);
}

Vector<String> FontData::get_language_support_overrides() const {
	_ensure_rid(0);
	return TS->font_get_language_support_overrides(cache[0]);
}

bool FontData::is_script_supported(const String &p_script) const {
	_ensure_rid(0);
	return TS->font_is_script_supported(cache[0], p_script);
}

void FontData::set_script_support_override(const String &p_script, bool p_supported) {
	_ensure_rid(0);
	TS->font_set_script_support_override(cache[0], p_script, p_supported);
}

bool FontData::get_script_support_override(const String &p_script) const {
	_ensure_rid(0);
	return TS->font_get_script_support_override(cache[0], p_script);
}

void FontData::remove_script_support_override(const String &p_script) {
	_ensure_rid(0);
	TS->font_remove_script_support_override(cache[0], p_script);
}

Vector<String> FontData::get_script_support_overrides() const {
	_ensure_rid(0);
	return TS->font_get_script_support_overrides(cache[0]);
}

bool FontData::has_char(char32_t p_char) const {
	_ensure_rid(0);
	return TS->font_has_char(cache[0], p_char);
}

String FontData::get_supported_chars() const {
	_ensure_rid(0);
	return TS->font_get_supported_chars(cache[0]);
}

int32_t FontData::get_glyph_index(int p_size, char32_t p_char, char32_t p_variation_selector) const {
	_ensure_rid(0);
	return TS->font_get_glyph_index(cache[0], p_size, p_char, p_variation_selector);
}

Set<uint32_t> FontData::get_supported_feature_list() const {
	_ensure_rid(0);
	return TS->font_supported_feature_list(cache[0]);
}

Map<uint32_t, Vector3i> FontData::get_supported_variation_list() const {
	_ensure_rid(0);
	return TS->font_supported_variation_list(cache[0]);
}

FontData::FontData() {
	/* NOP */
}

FontData::~FontData() {
	_clear_cache();
}

/*************************************************************************/

void Font::_data_changed() { //TODO: ?
	for (int i = 0; i < rids.size(); i++) {
		rids.data()[i] = RID();
	}
	emit_changed();
}

void Font::_ensure_rid(int p_index) const {
	// Find or create cache record.
	for (int i = 0; i < rids.size(); i++) {
		if (!rids[i].is_valid() && data[i]) {
			rids.data()[i] = data[i]->find_cache(variation_coordinates);
		}
	}
}

Set<uint32_t> Font::get_feature_list() const {
	Set<uint32_t> out;
	for (int i = 0; i < data.size(); i++) {
		Set<uint32_t> data_ftrs = data[i]->get_supported_feature_list();
		out.insert(data_ftrs.begin(), data_ftrs.end());
	}
	return out;
}

void Font::add_data(const Ref<FontData> &p_data) {
	ERR_FAIL_COND(!p_data);
	data.push_back(p_data);
	rids.push_back(RID());

    if (data[data.size() - 1]) {
		data[data.size() - 1]->fonts.push_back(this);
	}

	emit_changed();
}

void Font::set_data(int p_idx, const Ref<FontData> &p_data) {
	ERR_FAIL_COND(!p_data);
	ERR_FAIL_INDEX(p_idx, data.size());

    if (data[p_idx]) {
		data[p_idx]->fonts.remove(this);
	}

	data.data()[p_idx] = p_data;
	rids.data()[p_idx] = RID();

    if (data[p_idx]) {
		data[p_idx]->fonts.push_back(this);
	}
}

int Font::get_data_count() const {
	return data.size();
}

Ref<FontData> Font::get_data(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, data.size(), Ref<FontData>());
	return data[p_idx];
}

RID Font::get_data_rid(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, data.size(), RID());
	_ensure_rid(p_idx);
	return rids[p_idx];
}

void Font::clear_data() {
    
	for (int i = 0; i < data.size(); i++) {
		if (data[i]) {
			data[i]->fonts.remove(this);
		}
	}
	data.clear();
	rids.clear();
}

void Font::remove_data(int p_idx) {
	ERR_FAIL_INDEX(p_idx, data.size());
    if(data[p_idx])
    {
        data[p_idx]->fonts.push_back(this);
    }
	data.remove(p_idx);
	rids.remove(p_idx);
}

void Font::set_base_size(int p_size) {
	base_size = p_size;
}

int Font::get_base_size() const {
	return base_size;
}

void Font::set_variation_coordinates(const Map<uint32_t, double> &p_variation_coordinates) {
	_data_changed();
	variation_coordinates = p_variation_coordinates;
}

Map<uint32_t, double> Font::get_variation_coordinates() const {
	return variation_coordinates;
}

void Font::set_spacing(TextServer::SpacingType p_spacing, int p_value) {
	_data_changed();
	switch (p_spacing) {
		case TextServer::SPACING_TOP: {
			spacing_top = p_value;
		} break;
		case TextServer::SPACING_BOTTOM: {
			spacing_bottom = p_value;
		} break;
		default: {
			ERR_FAIL_MSG("Invalid spacing type: " + itos(p_spacing));
		} break;
	}
}

int Font::get_spacing(TextServer::SpacingType p_spacing) const {
	switch (p_spacing) {
		case TextServer::SPACING_TOP: {
			return spacing_top;
		} break;
		case TextServer::SPACING_BOTTOM: {
			return spacing_bottom;
		} break;
		default: {
			ERR_FAIL_V_MSG(0, "Invalid spacing type: " + itos(p_spacing));
		} break;
	}
}

real_t Font::get_height(int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;
	real_t ret = 0.f;
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		ret = MAX(ret, TS->font_get_ascent(rids[i], size) + TS->font_get_descent(rids[i], size));
	}
	return ret + spacing_bottom + spacing_top;
}

real_t Font::get_ascent(int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;
	real_t ret = 0.f;
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		ret = MAX(ret, TS->font_get_ascent(rids[i], size));
	}
	return ret + spacing_top;
}

real_t Font::get_descent(int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;
	real_t ret = 0.f;
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		ret = MAX(ret, TS->font_get_descent(rids[i], size));
	}
	return ret + spacing_bottom;
}

real_t Font::get_underline_position(int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;
	real_t ret = 0.f;
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		ret = MAX(ret, TS->font_get_underline_position(rids[i], size));
	}
	return ret + spacing_top;
}

real_t Font::get_underline_thickness(int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;
	real_t ret = 0.f;
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		ret = MAX(ret, TS->font_get_underline_thickness(rids[i], size));
	}
	return ret;
}

Size2 Font::get_char_size(char32_t p_char, char32_t p_next, int p_size) const {
	int size = (p_size <= 0) ? base_size : p_size;

	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		if (data[i]->has_char(p_char)) {
			int32_t glyph_a = TS->font_get_glyph_index(rids[i], size, p_char, 0);
			Size2 ret = Size2(TS->font_get_glyph_advance(rids[i], size, glyph_a).x, TS->font_get_ascent(rids[i], size) + TS->font_get_descent(rids[i], size));
			if ((p_next != 0) && data[i]->has_char(p_next)) {
				int32_t glyph_b = TS->font_get_glyph_index(rids[i], size, p_next, 0);
				ret.x -= TS->font_get_kerning(rids[i], size, Vector2i(glyph_a, glyph_b)).x;
			}
			return ret;
		}
	}
	return Size2();
}

real_t Font::draw_char(OGUI::PrimDrawList& list, const Point2 &p_pos, char32_t p_char, char32_t p_next, int p_size, const Color &p_modulate, int p_outline_size, const Color &p_outline_modulate) const {
	int size = (p_size <= 0) ? base_size : p_size;

	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
		if (data[i]->has_char(p_char)) {
			int32_t glyph_a = TS->font_get_glyph_index(rids[i], size, p_char, 0);
			real_t ret = TS->font_get_glyph_advance(rids[i], size, glyph_a).x;
			if ((p_next != 0) && data[i]->has_char(p_next)) {
				int32_t glyph_b = TS->font_get_glyph_index(rids[i], size, p_next, 0);
				ret -= TS->font_get_kerning(rids[i], size, Vector2i(glyph_a, glyph_b)).x;
			}

			if (p_outline_size > 0 && p_outline_modulate.a != 0.0f) {
				TS->font_draw_glyph_outline(rids[i], list, size, p_outline_size, p_pos, glyph_a, p_outline_modulate);
			}
			TS->font_draw_glyph(rids[i], list, size, p_pos, glyph_a, p_modulate);
			return ret;
		}
	}
	return 0;
}

bool Font::has_char(char32_t p_char) const {
	for (int i = 0; i < data.size(); i++) {
		if (data[i]->has_char(p_char))
			return true;
	}
	return false;
}

String Font::get_supported_chars() const {
	String chars;
	for (int i = 0; i < data.size(); i++) {
		String data_chars = data[i]->get_supported_chars();
		for (int j = 0; j < data_chars.length(); j++) {
			if (chars.find_char(data_chars[j]) == -1) {
				chars += data_chars[j];
			}
		}
	}
	return chars;
}

Vector<RID> Font::get_rids() const {
	for (int i = 0; i < data.size(); i++) {
		_ensure_rid(i);
	}
	return rids;
}

void Font::update_changes() {
	emit_changed();
}

Font::Font() {
}

Font::~Font() {
	clear_data();
}
