/*************************************************************************/
/*  text_server_adv.cpp                                                  */
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

#include "text_server_adv.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Style2/Transform.h"
#include "freetype/ftbitmap.h"
#include "freetype/tttables.h"
#include "unicode/ubrk.h"

using namespace godot;
#ifdef ICU_STATIC_DATA
#include "thirdparty/icu4c/icudata.gen.h"
#endif

#ifdef MODULE_MSDFGEN_ENABLED
#include "core/ShapeDistanceFinder.h"
#include "core/contour-combiners.h"
#include "core/edge-selectors.h"
#include "msdfgen.h"
#endif

/*************************************************************************/
/*  hb_bmp_font_t HarfBuzz Bitmap font interface                         */
/*************************************************************************/

hb_font_funcs_t *TextServerAdvanced::funcs = nullptr;

TextServerAdvanced::hb_bmp_font_t *TextServerAdvanced::_hb_bmp_font_create(TextServerAdvanced::FontDataForSizeAdvanced *p_face, bool p_unref) {
	hb_bmp_font_t *bm_font = memnew(hb_bmp_font_t);

	if (!bm_font) {
		return nullptr;
	}

	bm_font->face = p_face;
	bm_font->unref = p_unref;

	return bm_font;
}

void TextServerAdvanced::_hb_bmp_font_destroy(void *p_data) {
	hb_bmp_font_t *bm_font = reinterpret_cast<hb_bmp_font_t *>(p_data);
	memdelete(bm_font);
}

hb_bool_t TextServerAdvanced::hb_bmp_get_nominal_glyph(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_unicode, hb_codepoint_t *r_glyph, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return false;
	}

	if (!bm_font->face->glyph_map.has(p_unicode)) {
		if (bm_font->face->glyph_map.has(0xF000u + p_unicode)) {
			*r_glyph = 0xF000u + p_unicode;
			return true;
		} else {
			return false;
		}
	}

	*r_glyph = p_unicode;
	return true;
}

hb_position_t TextServerAdvanced::hb_bmp_get_glyph_h_advance(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_glyph, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return 0;
	}

	if (!bm_font->face->glyph_map.has(p_glyph)) {
		return 0;
	}

	return bm_font->face->glyph_map[p_glyph].advance.x * 64;
}

hb_position_t TextServerAdvanced::hb_bmp_get_glyph_v_advance(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_glyph, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return 0;
	}

	if (!bm_font->face->glyph_map.has(p_glyph)) {
		return 0;
	}

	return -bm_font->face->glyph_map[p_glyph].advance.y * 64;
}

hb_position_t TextServerAdvanced::hb_bmp_get_glyph_h_kerning(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_left_glyph, hb_codepoint_t p_right_glyph, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return 0;
	}

	if (!bm_font->face->kerning_map.has(Vector2i(p_left_glyph, p_right_glyph))) {
		return 0;
	}

	return bm_font->face->kerning_map[Vector2i(p_left_glyph, p_right_glyph)].x * 64;
}

hb_position_t TextServerAdvanced::hb_bmp_get_glyph_v_kerning(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_left_glyph, hb_codepoint_t p_right_glyph, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return 0;
	}

	if (!bm_font->face->kerning_map.has(Vector2i(p_left_glyph, p_right_glyph))) {
		return 0;
	}

	return bm_font->face->kerning_map[Vector2i(p_left_glyph, p_right_glyph)].y * 64;
}

hb_bool_t TextServerAdvanced::hb_bmp_get_glyph_v_origin(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_glyph, hb_position_t *r_x, hb_position_t *r_y, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return false;
	}

	if (!bm_font->face->glyph_map.has(p_glyph)) {
		return false;
	}

	*r_x = bm_font->face->glyph_map[p_glyph].advance.x * 32;
	*r_y = -bm_font->face->ascent * 64;

	return true;
}

hb_bool_t TextServerAdvanced::hb_bmp_get_glyph_extents(hb_font_t *p_font, void *p_font_data, hb_codepoint_t p_glyph, hb_glyph_extents_t *r_extents, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return false;
	}

	if (!bm_font->face->glyph_map.has(p_glyph)) {
		return false;
	}

	r_extents->x_bearing = 0;
	r_extents->y_bearing = 0;
	r_extents->width = bm_font->face->glyph_map[p_glyph].rect.size.x * 64;
	r_extents->height = bm_font->face->glyph_map[p_glyph].rect.size.y * 64;

	return true;
}

hb_bool_t TextServerAdvanced::hb_bmp_get_font_h_extents(hb_font_t *p_font, void *p_font_data, hb_font_extents_t *r_metrics, void *p_user_data) {
	const hb_bmp_font_t *bm_font = reinterpret_cast<const hb_bmp_font_t *>(p_font_data);

	if (!bm_font->face) {
		return false;
	}

	r_metrics->ascender = bm_font->face->ascent;
	r_metrics->descender = bm_font->face->descent;
	r_metrics->line_gap = 0;

	return true;
}

void TextServerAdvanced::hb_bmp_create_font_funcs() {
	if (funcs == nullptr) {
		funcs = hb_font_funcs_create();

		hb_font_funcs_set_font_h_extents_func(funcs, hb_bmp_get_font_h_extents, nullptr, nullptr);
		hb_font_funcs_set_nominal_glyph_func(funcs, hb_bmp_get_nominal_glyph, nullptr, nullptr);
		hb_font_funcs_set_glyph_h_advance_func(funcs, hb_bmp_get_glyph_h_advance, nullptr, nullptr);
		hb_font_funcs_set_glyph_v_advance_func(funcs, hb_bmp_get_glyph_v_advance, nullptr, nullptr);
		hb_font_funcs_set_glyph_v_origin_func(funcs, hb_bmp_get_glyph_v_origin, nullptr, nullptr);
		hb_font_funcs_set_glyph_h_kerning_func(funcs, hb_bmp_get_glyph_h_kerning, nullptr, nullptr);
		hb_font_funcs_set_glyph_v_kerning_func(funcs, hb_bmp_get_glyph_v_kerning, nullptr, nullptr);
		hb_font_funcs_set_glyph_extents_func(funcs, hb_bmp_get_glyph_extents, nullptr, nullptr);

		hb_font_funcs_make_immutable(funcs);
	}
}

void TextServerAdvanced::hb_bmp_free_font_funcs() {
	if (funcs != nullptr) {
		hb_font_funcs_destroy(funcs);
		funcs = nullptr;
	}
}

void TextServerAdvanced::_hb_bmp_font_set_funcs(hb_font_t *p_font, TextServerAdvanced::FontDataForSizeAdvanced *p_face, bool p_unref) {
	hb_font_set_funcs(p_font, funcs, _hb_bmp_font_create(p_face, p_unref), _hb_bmp_font_destroy);
}

hb_font_t *TextServerAdvanced::hb_bmp_font_create(TextServerAdvanced::FontDataForSizeAdvanced *p_face, hb_destroy_func_t p_destroy) {
	hb_font_t *font;
	hb_face_t *face = hb_face_create(nullptr, 0);

	font = hb_font_create(face);
	hb_face_destroy(face);
	_hb_bmp_font_set_funcs(font, p_face, false);
	return font;
}

/*************************************************************************/
/*  Character properties.                                                */
/*************************************************************************/

_FORCE_INLINE_ bool is_ain(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_AIN;
}

_FORCE_INLINE_ bool is_alef(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_ALEF;
}

_FORCE_INLINE_ bool is_beh(char32_t p_chr) {
	int32_t prop = u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP);
	return (prop == U_JG_BEH) || (prop == U_JG_NOON) || (prop == U_JG_AFRICAN_NOON) || (prop == U_JG_NYA) || (prop == U_JG_YEH) || (prop == U_JG_FARSI_YEH);
}

_FORCE_INLINE_ bool is_dal(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_DAL;
}

_FORCE_INLINE_ bool is_feh(char32_t p_chr) {
	return (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_FEH) || (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_AFRICAN_FEH);
}

_FORCE_INLINE_ bool is_gaf(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_GAF;
}

_FORCE_INLINE_ bool is_heh(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_HEH;
}

_FORCE_INLINE_ bool is_kaf(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_KAF;
}

_FORCE_INLINE_ bool is_lam(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_LAM;
}

_FORCE_INLINE_ bool is_qaf(char32_t p_chr) {
	return (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_QAF) || (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_AFRICAN_QAF);
}

_FORCE_INLINE_ bool is_reh(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_REH;
}

_FORCE_INLINE_ bool is_seen_sad(char32_t p_chr) {
	return (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_SAD) || (u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_SEEN);
}

_FORCE_INLINE_ bool is_tah(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_TAH;
}

_FORCE_INLINE_ bool is_teh_marbuta(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_TEH_MARBUTA;
}

_FORCE_INLINE_ bool is_yeh(char32_t p_chr) {
	int32_t prop = u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP);
	return (prop == U_JG_YEH) || (prop == U_JG_FARSI_YEH) || (prop == U_JG_YEH_BARREE) || (prop == U_JG_BURUSHASKI_YEH_BARREE) || (prop == U_JG_YEH_WITH_TAIL);
}

_FORCE_INLINE_ bool is_waw(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_GROUP) == U_JG_WAW;
}

_FORCE_INLINE_ bool is_transparent(char32_t p_chr) {
	return u_getIntPropertyValue(p_chr, UCHAR_JOINING_TYPE) == U_JT_TRANSPARENT;
}

_FORCE_INLINE_ bool is_ligature(char32_t p_chr, char32_t p_nchr) {
	return (is_lam(p_chr) && is_alef(p_nchr));
}

_FORCE_INLINE_ bool is_connected_to_prev(char32_t p_chr, char32_t p_pchr) {
	int32_t prop = u_getIntPropertyValue(p_pchr, UCHAR_JOINING_TYPE);
	return (prop != U_JT_RIGHT_JOINING) && (prop != U_JT_NON_JOINING) ? !is_ligature(p_pchr, p_chr) : false;
}

_FORCE_INLINE_ bool is_control(char32_t p_char) {
	return (p_char <= 0x001f) || (p_char >= 0x007f && p_char <= 0x009F);
}

_FORCE_INLINE_ bool is_whitespace(char32_t p_char) {
	return (p_char == 0x0020) || (p_char == 0x00A0) || (p_char == 0x1680) || (p_char >= 0x2000 && p_char <= 0x200a) || (p_char == 0x202f) || (p_char == 0x205f) || (p_char == 0x3000) || (p_char == 0x2028) || (p_char == 0x2029) || (p_char >= 0x0009 && p_char <= 0x000d) || (p_char == 0x0085);
}

_FORCE_INLINE_ bool is_linebreak(char32_t p_char) {
	return (p_char >= 0x000a && p_char <= 0x000d) || (p_char == 0x0085) || (p_char == 0x2028) || (p_char == 0x2029);
}

_FORCE_INLINE_ bool is_underscore(char32_t p_char) {
	return (p_char == 0x005F);
}

/*************************************************************************/

String TextServerAdvanced::interface_name = "ICU / HarfBuzz / Graphite";
uint32_t TextServerAdvanced::interface_features = FEATURE_BIDI_LAYOUT | FEATURE_VERTICAL_LAYOUT | FEATURE_SHAPING | FEATURE_KASHIDA_JUSTIFICATION | FEATURE_BREAK_ITERATORS | FEATURE_USE_SUPPORT_DATA | FEATURE_FONT_VARIABLE;

bool TextServerAdvanced::has_feature(Feature p_feature) {
	return (interface_features & p_feature) == p_feature;
}

String TextServerAdvanced::get_name() const {
	return interface_name;
}

void TextServerAdvanced::free(RID p_rid) {
	_THREAD_SAFE_METHOD_
	if (font_owner.owns(p_rid)) {
		FontDataAdvanced *fd = font_owner.getornull(p_rid);
		font_owner.free(p_rid);
		memdelete(fd);
	} else if (shaped_owner.owns(p_rid)) {
		ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_rid);
		shaped_owner.free(p_rid);
		memdelete(sd);
	}
}

bool TextServerAdvanced::has(RID p_rid) {
	_THREAD_SAFE_METHOD_
	return font_owner.owns(p_rid) || shaped_owner.owns(p_rid);
}

bool TextServerAdvanced::load_support_data(const String &p_filename) {
	_THREAD_SAFE_METHOD_

#ifdef ICU_STATIC_DATA
	if (icu_data == nullptr) {
		UErrorCode err = U_ZERO_ERROR;
		u_init(&err); // Do not check for errors, since we only load part of the data.
		icu_data = (uint8_t *)&U_ICUDATA_ENTRY_POINT;
	}
#else
	if (icu_data == nullptr) {
		String filename = "res/icudt67l.dat";
        auto& ctx = OGUI::Context::Get().fileImpl;
        auto f = ctx->Open(filename.ascii().ptr());
		if (!f) {
			return false;
		}

		UErrorCode err = U_ZERO_ERROR;

		// ICU data found.
        
		uint64_t len = ctx->Length(f);
		icu_data = (uint8_t *)memalloc(len);
        ctx->Read(icu_data, len, f);
        ctx->Close(f);

		udata_setCommonData(icu_data, &err);
		if (U_FAILURE(err)) {
			memfree(icu_data);
			icu_data = nullptr;
			ERR_FAIL_V_MSG(false, u_errorName(err));
		}

		err = U_ZERO_ERROR;
		u_init(&err);
		if (U_FAILURE(err)) {
			memfree(icu_data);
			icu_data = nullptr;
			ERR_FAIL_V_MSG(false, u_errorName(err));
		}
	}
#endif
	return true;
}

#ifdef TOOLS_ENABLED

bool TextServerAdvanced::save_support_data(const String &p_filename) {
	_THREAD_SAFE_METHOD_
#ifdef ICU_STATIC_DATA

	// Store data to the res file if it's available.
	FileAccess *f = FileAccess::open(p_filename, FileAccess::WRITE);
	if (!f) {
		return false;
	}
	f->store_buffer(U_ICUDATA_ENTRY_POINT, U_ICUDATA_SIZE);
	f->close();
	memdelete(f);
	return true;

#else
	return false;
#endif
}

#endif

bool TextServerAdvanced::is_locale_right_to_left(const String &p_locale) {
	String l = p_locale.get_slicec('_', 0);
	if ((l == "ar") || (l == "dv") || (l == "he") || (l == "fa") || (l == "ff") || (l == "ku") || (l == "ur")) {
		return true;
	} else {
		return false;
	}
}

struct FeatureInfo {
	int32_t tag;
	String name;
};

static FeatureInfo feature_set[] = {
	// Registered OpenType feature tags.
	{ HB_TAG('a', 'a', 'l', 't'), "access_all_alternates" },
	{ HB_TAG('a', 'b', 'v', 'f'), "above_base_forms" },
	{ HB_TAG('a', 'b', 'v', 'm'), "above_base_mark_positioning" },
	{ HB_TAG('a', 'b', 'v', 's'), "above_base_substitutions" },
	{ HB_TAG('a', 'f', 'r', 'c'), "alternative_fractions" },
	{ HB_TAG('a', 'k', 'h', 'n'), "akhands" },
	{ HB_TAG('b', 'l', 'w', 'f'), "below_base_forms" },
	{ HB_TAG('b', 'l', 'w', 'm'), "below_base_mark_positioning" },
	{ HB_TAG('b', 'l', 'w', 's'), "below_base_substitutions" },
	{ HB_TAG('c', 'a', 'l', 't'), "contextual_alternates" },
	{ HB_TAG('c', 'a', 's', 'e'), "case_sensitive_forms" },
	{ HB_TAG('c', 'c', 'm', 'p'), "glyph_composition" },
	{ HB_TAG('c', 'f', 'a', 'r'), "conjunct_form_after_ro" },
	{ HB_TAG('c', 'j', 'c', 't'), "conjunct_forms" },
	{ HB_TAG('c', 'l', 'i', 'g'), "contextual_ligatures" },
	{ HB_TAG('c', 'p', 'c', 't'), "centered_cjk_punctuation" },
	{ HB_TAG('c', 'p', 's', 'p'), "capital_spacing" },
	{ HB_TAG('c', 's', 'w', 'h'), "contextual_swash" },
	{ HB_TAG('c', 'u', 'r', 's'), "cursive_positioning" },
	{ HB_TAG('c', 'v', '0', '1'), "character_variant_01" },
	{ HB_TAG('c', 'v', '0', '2'), "character_variant_02" },
	{ HB_TAG('c', 'v', '0', '3'), "character_variant_03" },
	{ HB_TAG('c', 'v', '0', '4'), "character_variant_04" },
	{ HB_TAG('c', 'v', '0', '5'), "character_variant_05" },
	{ HB_TAG('c', 'v', '0', '6'), "character_variant_06" },
	{ HB_TAG('c', 'v', '0', '7'), "character_variant_07" },
	{ HB_TAG('c', 'v', '0', '8'), "character_variant_08" },
	{ HB_TAG('c', 'v', '0', '9'), "character_variant_09" },
	{ HB_TAG('c', 'v', '1', '0'), "character_variant_10" },
	{ HB_TAG('c', 'v', '1', '1'), "character_variant_11" },
	{ HB_TAG('c', 'v', '1', '2'), "character_variant_12" },
	{ HB_TAG('c', 'v', '1', '3'), "character_variant_13" },
	{ HB_TAG('c', 'v', '1', '4'), "character_variant_14" },
	{ HB_TAG('c', 'v', '1', '5'), "character_variant_15" },
	{ HB_TAG('c', 'v', '1', '6'), "character_variant_16" },
	{ HB_TAG('c', 'v', '1', '7'), "character_variant_17" },
	{ HB_TAG('c', 'v', '1', '8'), "character_variant_18" },
	{ HB_TAG('c', 'v', '1', '9'), "character_variant_19" },
	{ HB_TAG('c', 'v', '2', '0'), "character_variant_20" },
	{ HB_TAG('c', 'v', '2', '1'), "character_variant_21" },
	{ HB_TAG('c', 'v', '2', '2'), "character_variant_22" },
	{ HB_TAG('c', 'v', '2', '3'), "character_variant_23" },
	{ HB_TAG('c', 'v', '2', '4'), "character_variant_24" },
	{ HB_TAG('c', 'v', '2', '5'), "character_variant_25" },
	{ HB_TAG('c', 'v', '2', '6'), "character_variant_26" },
	{ HB_TAG('c', 'v', '2', '7'), "character_variant_27" },
	{ HB_TAG('c', 'v', '2', '8'), "character_variant_28" },
	{ HB_TAG('c', 'v', '2', '9'), "character_variant_29" },
	{ HB_TAG('c', 'v', '3', '0'), "character_variant_30" },
	{ HB_TAG('c', 'v', '3', '1'), "character_variant_31" },
	{ HB_TAG('c', 'v', '3', '2'), "character_variant_32" },
	{ HB_TAG('c', 'v', '3', '3'), "character_variant_33" },
	{ HB_TAG('c', 'v', '3', '4'), "character_variant_34" },
	{ HB_TAG('c', 'v', '3', '5'), "character_variant_35" },
	{ HB_TAG('c', 'v', '3', '6'), "character_variant_36" },
	{ HB_TAG('c', 'v', '3', '7'), "character_variant_37" },
	{ HB_TAG('c', 'v', '3', '8'), "character_variant_38" },
	{ HB_TAG('c', 'v', '3', '9'), "character_variant_39" },
	{ HB_TAG('c', 'v', '4', '0'), "character_variant_40" },
	{ HB_TAG('c', 'v', '4', '1'), "character_variant_41" },
	{ HB_TAG('c', 'v', '4', '2'), "character_variant_42" },
	{ HB_TAG('c', 'v', '4', '3'), "character_variant_43" },
	{ HB_TAG('c', 'v', '4', '4'), "character_variant_44" },
	{ HB_TAG('c', 'v', '4', '5'), "character_variant_45" },
	{ HB_TAG('c', 'v', '4', '6'), "character_variant_46" },
	{ HB_TAG('c', 'v', '4', '7'), "character_variant_47" },
	{ HB_TAG('c', 'v', '4', '8'), "character_variant_48" },
	{ HB_TAG('c', 'v', '4', '9'), "character_variant_49" },
	{ HB_TAG('c', 'v', '5', '0'), "character_variant_50" },
	{ HB_TAG('c', 'v', '5', '1'), "character_variant_51" },
	{ HB_TAG('c', 'v', '5', '2'), "character_variant_52" },
	{ HB_TAG('c', 'v', '5', '3'), "character_variant_53" },
	{ HB_TAG('c', 'v', '5', '4'), "character_variant_54" },
	{ HB_TAG('c', 'v', '5', '5'), "character_variant_55" },
	{ HB_TAG('c', 'v', '5', '6'), "character_variant_56" },
	{ HB_TAG('c', 'v', '5', '7'), "character_variant_57" },
	{ HB_TAG('c', 'v', '5', '8'), "character_variant_58" },
	{ HB_TAG('c', 'v', '5', '9'), "character_variant_59" },
	{ HB_TAG('c', 'v', '6', '0'), "character_variant_60" },
	{ HB_TAG('c', 'v', '6', '1'), "character_variant_61" },
	{ HB_TAG('c', 'v', '6', '2'), "character_variant_62" },
	{ HB_TAG('c', 'v', '6', '3'), "character_variant_63" },
	{ HB_TAG('c', 'v', '6', '4'), "character_variant_64" },
	{ HB_TAG('c', 'v', '6', '5'), "character_variant_65" },
	{ HB_TAG('c', 'v', '6', '6'), "character_variant_66" },
	{ HB_TAG('c', 'v', '6', '7'), "character_variant_67" },
	{ HB_TAG('c', 'v', '6', '8'), "character_variant_68" },
	{ HB_TAG('c', 'v', '6', '9'), "character_variant_69" },
	{ HB_TAG('c', 'v', '7', '0'), "character_variant_70" },
	{ HB_TAG('c', 'v', '7', '1'), "character_variant_71" },
	{ HB_TAG('c', 'v', '7', '2'), "character_variant_72" },
	{ HB_TAG('c', 'v', '7', '3'), "character_variant_73" },
	{ HB_TAG('c', 'v', '7', '4'), "character_variant_74" },
	{ HB_TAG('c', 'v', '7', '5'), "character_variant_75" },
	{ HB_TAG('c', 'v', '7', '6'), "character_variant_76" },
	{ HB_TAG('c', 'v', '7', '7'), "character_variant_77" },
	{ HB_TAG('c', 'v', '7', '8'), "character_variant_78" },
	{ HB_TAG('c', 'v', '7', '9'), "character_variant_79" },
	{ HB_TAG('c', 'v', '8', '0'), "character_variant_80" },
	{ HB_TAG('c', 'v', '8', '1'), "character_variant_81" },
	{ HB_TAG('c', 'v', '8', '2'), "character_variant_82" },
	{ HB_TAG('c', 'v', '8', '3'), "character_variant_83" },
	{ HB_TAG('c', 'v', '8', '4'), "character_variant_84" },
	{ HB_TAG('c', 'v', '8', '5'), "character_variant_85" },
	{ HB_TAG('c', 'v', '8', '6'), "character_variant_86" },
	{ HB_TAG('c', 'v', '8', '7'), "character_variant_87" },
	{ HB_TAG('c', 'v', '8', '8'), "character_variant_88" },
	{ HB_TAG('c', 'v', '8', '9'), "character_variant_89" },
	{ HB_TAG('c', 'v', '9', '0'), "character_variant_90" },
	{ HB_TAG('c', 'v', '9', '1'), "character_variant_91" },
	{ HB_TAG('c', 'v', '9', '2'), "character_variant_92" },
	{ HB_TAG('c', 'v', '9', '3'), "character_variant_93" },
	{ HB_TAG('c', 'v', '9', '4'), "character_variant_94" },
	{ HB_TAG('c', 'v', '9', '5'), "character_variant_95" },
	{ HB_TAG('c', 'v', '9', '6'), "character_variant_96" },
	{ HB_TAG('c', 'v', '9', '7'), "character_variant_97" },
	{ HB_TAG('c', 'v', '9', '8'), "character_variant_98" },
	{ HB_TAG('c', 'v', '9', '9'), "character_variant_99" },
	{ HB_TAG('c', '2', 'p', 'c'), "petite_capitals_from_capitals" },
	{ HB_TAG('c', '2', 's', 'c'), "small_capitals_from_capitals" },
	{ HB_TAG('d', 'i', 's', 't'), "distances" },
	{ HB_TAG('d', 'l', 'i', 'g'), "discretionary_ligatures" },
	{ HB_TAG('d', 'n', 'o', 'm'), "denominators" },
	{ HB_TAG('d', 't', 'l', 's'), "dotless_forms" },
	{ HB_TAG('e', 'x', 'p', 't'), "expert_forms" },
	{ HB_TAG('f', 'a', 'l', 't'), "final_glyph_on_line_alternates" },
	{ HB_TAG('f', 'i', 'n', '2'), "terminal_forms_2" },
	{ HB_TAG('f', 'i', 'n', '3'), "terminal_forms_3" },
	{ HB_TAG('f', 'i', 'n', 'a'), "terminal_forms" },
	{ HB_TAG('f', 'l', 'a', 'c'), "flattened_accent_forms" },
	{ HB_TAG('f', 'r', 'a', 'c'), "fractions" },
	{ HB_TAG('f', 'w', 'i', 'd'), "full_widths" },
	{ HB_TAG('h', 'a', 'l', 'f'), "half_forms" },
	{ HB_TAG('h', 'a', 'l', 'n'), "halant_forms" },
	{ HB_TAG('h', 'a', 'l', 't'), "alternate_half_widths" },
	{ HB_TAG('h', 'i', 's', 't'), "historical_forms" },
	{ HB_TAG('h', 'k', 'n', 'a'), "horizontal_kana_alternates" },
	{ HB_TAG('h', 'l', 'i', 'g'), "historical_ligatures" },
	{ HB_TAG('h', 'n', 'g', 'l'), "hangul" },
	{ HB_TAG('h', 'o', 'j', 'o'), "hojo_kanji_forms" },
	{ HB_TAG('h', 'w', 'i', 'd'), "half_widths" },
	{ HB_TAG('i', 'n', 'i', 't'), "initial_forms" },
	{ HB_TAG('i', 's', 'o', 'l'), "isolated_forms" },
	{ HB_TAG('i', 't', 'a', 'l'), "italics" },
	{ HB_TAG('j', 'a', 'l', 't'), "justification_alternates" },
	{ HB_TAG('j', 'p', '7', '8'), "jis78_forms" },
	{ HB_TAG('j', 'p', '8', '3'), "jis83_forms" },
	{ HB_TAG('j', 'p', '9', '0'), "jis90_forms" },
	{ HB_TAG('j', 'p', '0', '4'), "jis2004_forms" },
	{ HB_TAG('k', 'e', 'r', 'n'), "kerning" },
	{ HB_TAG('l', 'f', 'b', 'd'), "left_bounds" },
	{ HB_TAG('l', 'i', 'g', 'a'), "standard_ligatures" },
	{ HB_TAG('l', 'j', 'm', 'o'), "leading_jamo_forms" },
	{ HB_TAG('l', 'n', 'u', 'm'), "lining_figures" },
	{ HB_TAG('l', 'o', 'c', 'l'), "localized_forms" },
	{ HB_TAG('l', 't', 'r', 'a'), "left_to_right_alternates" },
	{ HB_TAG('l', 't', 'r', 'm'), "left_to_right_mirrored_forms" },
	{ HB_TAG('m', 'a', 'r', 'k'), "mark_positioning" },
	{ HB_TAG('m', 'e', 'd', '2'), "medial_forms_2" },
	{ HB_TAG('m', 'e', 'd', 'i'), "medial_forms" },
	{ HB_TAG('m', 'g', 'r', 'k'), "mathematical_greek" },
	{ HB_TAG('m', 'k', 'm', 'k'), "mark_to_mark_positioning" },
	{ HB_TAG('m', 's', 'e', 't'), "mark_positioning_via_substitution" },
	{ HB_TAG('n', 'a', 'l', 't'), "alternate_annotation_forms" },
	{ HB_TAG('n', 'l', 'c', 'k'), "nlc_kanji_forms" },
	{ HB_TAG('n', 'u', 'k', 't'), "nukta_forms" },
	{ HB_TAG('n', 'u', 'm', 'r'), "numerators" },
	{ HB_TAG('o', 'n', 'u', 'm'), "oldstyle_figures" },
	{ HB_TAG('o', 'p', 'b', 'd'), "optical_bounds" },
	{ HB_TAG('o', 'r', 'd', 'n'), "ordinals" },
	{ HB_TAG('o', 'r', 'n', 'm'), "ornaments" },
	{ HB_TAG('p', 'a', 'l', 't'), "proportional_alternate_widths" },
	{ HB_TAG('p', 'c', 'a', 'p'), "petite_capitals" },
	{ HB_TAG('p', 'k', 'n', 'a'), "proportional_kana" },
	{ HB_TAG('p', 'n', 'u', 'm'), "proportional_figures" },
	{ HB_TAG('p', 'r', 'e', 'f'), "pre_base_forms" },
	{ HB_TAG('p', 'r', 'e', 's'), "pre_base_substitutions" },
	{ HB_TAG('p', 's', 't', 'f'), "post_base_forms" },
	{ HB_TAG('p', 's', 't', 's'), "post_base_substitutions" },
	{ HB_TAG('p', 'w', 'i', 'd'), "proportional_widths" },
	{ HB_TAG('q', 'w', 'i', 'd'), "quarter_widths" },
	{ HB_TAG('r', 'a', 'n', 'd'), "randomize" },
	{ HB_TAG('r', 'c', 'l', 't'), "required_contextual_alternates" },
	{ HB_TAG('r', 'k', 'r', 'f'), "rakar_forms" },
	{ HB_TAG('r', 'l', 'i', 'g'), "required_ligatures" },
	{ HB_TAG('r', 'p', 'h', 'f'), "reph_forms" },
	{ HB_TAG('r', 't', 'b', 'd'), "right_bounds" },
	{ HB_TAG('r', 't', 'l', 'a'), "right_to_left_alternates" },
	{ HB_TAG('r', 't', 'l', 'm'), "right_to_left_mirrored_forms" },
	{ HB_TAG('r', 'u', 'b', 'y'), "ruby_notation_forms" },
	{ HB_TAG('r', 'v', 'r', 'n'), "required_variation_alternates" },
	{ HB_TAG('s', 'a', 'l', 't'), "stylistic_alternates" },
	{ HB_TAG('s', 'i', 'n', 'f'), "scientific_inferiors" },
	{ HB_TAG('s', 'i', 'z', 'e'), "optical_size" },
	{ HB_TAG('s', 'm', 'c', 'p'), "small_capitals" },
	{ HB_TAG('s', 'm', 'p', 'l'), "simplified_forms" },
	{ HB_TAG('s', 's', '0', '1'), "stylistic_set_01" },
	{ HB_TAG('s', 's', '0', '2'), "stylistic_set_02" },
	{ HB_TAG('s', 's', '0', '3'), "stylistic_set_03" },
	{ HB_TAG('s', 's', '0', '4'), "stylistic_set_04" },
	{ HB_TAG('s', 's', '0', '5'), "stylistic_set_05" },
	{ HB_TAG('s', 's', '0', '6'), "stylistic_set_06" },
	{ HB_TAG('s', 's', '0', '7'), "stylistic_set_07" },
	{ HB_TAG('s', 's', '0', '8'), "stylistic_set_08" },
	{ HB_TAG('s', 's', '0', '9'), "stylistic_set_09" },
	{ HB_TAG('s', 's', '1', '0'), "stylistic_set_10" },
	{ HB_TAG('s', 's', '1', '1'), "stylistic_set_11" },
	{ HB_TAG('s', 's', '1', '2'), "stylistic_set_12" },
	{ HB_TAG('s', 's', '1', '3'), "stylistic_set_13" },
	{ HB_TAG('s', 's', '1', '4'), "stylistic_set_14" },
	{ HB_TAG('s', 's', '1', '5'), "stylistic_set_15" },
	{ HB_TAG('s', 's', '1', '6'), "stylistic_set_16" },
	{ HB_TAG('s', 's', '1', '7'), "stylistic_set_17" },
	{ HB_TAG('s', 's', '1', '8'), "stylistic_set_18" },
	{ HB_TAG('s', 's', '1', '9'), "stylistic_set_19" },
	{ HB_TAG('s', 's', '2', '0'), "stylistic_set_20" },
	{ HB_TAG('s', 's', 't', 'y'), "math_script_style_alternates" },
	{ HB_TAG('s', 't', 'c', 'h'), "stretching_glyph_decomposition" },
	{ HB_TAG('s', 'u', 'b', 's'), "subscript" },
	{ HB_TAG('s', 'u', 'p', 's'), "superscript" },
	{ HB_TAG('s', 'w', 's', 'h'), "swash" },
	{ HB_TAG('t', 'i', 't', 'l'), "titling" },
	{ HB_TAG('t', 'j', 'm', 'o'), "trailing_jamo_forms" },
	{ HB_TAG('t', 'n', 'a', 'm'), "traditional_name_forms" },
	{ HB_TAG('t', 'n', 'u', 'm'), "tabular_figures" },
	{ HB_TAG('t', 'r', 'a', 'd'), "traditional_forms" },
	{ HB_TAG('t', 'w', 'i', 'd'), "third_widths" },
	{ HB_TAG('u', 'n', 'i', 'c'), "unicase" },
	{ HB_TAG('v', 'a', 'l', 't'), "alternate_vertical_metrics" },
	{ HB_TAG('v', 'a', 't', 'u'), "vattu_variants" },
	{ HB_TAG('v', 'e', 'r', 't'), "vertical_writing" },
	{ HB_TAG('v', 'h', 'a', 'l'), "alternate_vertical_half_metrics" },
	{ HB_TAG('v', 'j', 'm', 'o'), "vowel_jamo_forms" },
	{ HB_TAG('v', 'k', 'n', 'a'), "vertical_kana_alternates" },
	{ HB_TAG('v', 'k', 'r', 'n'), "vertical_kerning" },
	{ HB_TAG('v', 'p', 'a', 'l'), "proportional_alternate_vertical_metrics" },
	{ HB_TAG('v', 'r', 't', '2'), "vertical_alternates_and_rotation" },
	{ HB_TAG('v', 'r', 't', 'r'), "vertical_alternates_for_rotation" },
	{ HB_TAG('z', 'e', 'r', 'o'), "slashed_zero" },
	// Registered OpenType variation tags.
	{ HB_TAG('i', 't', 'a', 'l'), "italic" },
	{ HB_TAG('o', 'p', 's', 'z'), "optical_size" },
	{ HB_TAG('s', 'l', 'n', 't'), "slant" },
	{ HB_TAG('w', 'd', 't', 'h'), "width" },
	{ HB_TAG('w', 'g', 'h', 't'), "weight" },
	{ 0, String() },
};

int32_t TextServerAdvanced::name_to_tag(const String &p_name) const {
	for (int i = 0; feature_set[i].tag != 0; i++) {
		if (feature_set[i].name == p_name) {
			return feature_set[i].tag;
		}
	}

	// No readable name, use tag string.
	return hb_tag_from_string(p_name.replace("custom_", "").ascii().get_data(), -1);
}

String TextServerAdvanced::tag_to_name(int32_t p_tag) const {
	for (int i = 0; feature_set[i].tag != 0; i++) {
		if (feature_set[i].tag == p_tag) {
			return feature_set[i].name;
		}
	}

	// No readable name, use tag string.
	char name[5];
	memset(name, 0, 5);
	hb_tag_to_string(p_tag, name);
	return String("custom_") + String(name);
}

/*************************************************************************/
/* Font Glyph Rendering                                                  */
/*************************************************************************/

_FORCE_INLINE_ TextServerAdvanced::FontTexturePosition TextServerAdvanced::find_texture_pos_for_glyph(FontDataForSizeAdvanced *p_data, int p_color_size, OGUI::PixelFormat p_image_format, int p_width, int p_height) const {
	FontTexturePosition ret;
	ret.index = -1;

	int mw = p_width;
	int mh = p_height;

	for (int i = 0; i < p_data->font_textures.size(); i++) {
		const FontTexture &ct = p_data->font_textures[i];

        if (ct.format != p_image_format) {
            continue;
        }

		if (mw > ct.texture_w || mh > ct.texture_h) { // Too big for this texture.
			continue;
		}

		ret.y = 0x7FFFFFFF;
		ret.x = 0;

		for (int j = 0; j < ct.texture_w - mw; j++) {
			int max_y = 0;

			for (int k = j; k < j + mw; k++) {
				int y = ct.offsets[k];
				if (y > max_y) {
					max_y = y;
				}
			}

			if (max_y < ret.y) {
				ret.y = max_y;
				ret.x = j;
			}
		}

		if (ret.y == 0x7FFFFFFF || ret.y + mh > ct.texture_h) {
			continue; // Fail, could not fit it here.
		}

		ret.index = i;
		break;
	}

	if (ret.index == -1) {
		// Could not find texture to fit, create one.
		ret.x = 0;
		ret.y = 0;

		int texsize = MAX(p_data->size.x * p_data->oversampling * 8, 256 << MIN(p_data->font_textures.size(), 4));
		if (mw > texsize) {
			texsize = mw; // Special case, adapt to it?
		}
		if (mh > texsize) {
			texsize = mh; // Special case, adapt to it?
		}

		texsize = next_power_of_2(texsize);

		texsize = MIN(texsize, 4096);

		FontTexture tex;
		tex.texture_w = texsize;
		tex.texture_h = texsize;
		tex.format = p_image_format;
		tex.imgdata.resize(texsize * texsize * p_color_size);

		{
			// Zero texture.
			uint8_t *w = tex.imgdata.ptrw();
			ERR_FAIL_COND_V(texsize * texsize * p_color_size > tex.imgdata.size(), ret);
			// Initialize the texture to all-white pixels to prevent artifacts when the
			// font is displayed at a non-default scale with filtering enabled.
			if (p_color_size == 2) {
				for (int i = 0; i < texsize * texsize * p_color_size; i += 2) { // FORMAT_LA8, BW font.
					w[i + 0] = 255;
					w[i + 1] = 0;
				}
			} else if (p_color_size == 4) {
				for (int i = 0; i < texsize * texsize * p_color_size; i += 4) { // FORMAT_RGBA8, Color font, Multichannel(+True) SDF.
					w[i + 0] = 255;
					w[i + 1] = 255;
					w[i + 2] = 255;
					w[i + 3] = 0;
				}
			} else {
				ERR_FAIL_V(ret);
			}
		}
		tex.offsets.resize(texsize);
		for (int i = 0; i < texsize; i++) { // Zero offsets.
			tex.offsets.data()[i] = 0;
		}

		p_data->font_textures.push_back(std::move(tex));
		ret.index = p_data->font_textures.size() - 1;
	}

	return ret;
}

#ifdef MODULE_MSDFGEN_ENABLED

struct MSContext {
	msdfgen::Point2 position;
	msdfgen::Shape *shape;
	msdfgen::Contour *contour;
};

class DistancePixelConversion {
	double invRange;

public:
	_FORCE_INLINE_ explicit DistancePixelConversion(double range) :
			invRange(1 / range) {}
	_FORCE_INLINE_ void operator()(float *pixels, const msdfgen::MultiAndTrueDistance &distance) const {
		pixels[0] = float(invRange * distance.r + .5);
		pixels[1] = float(invRange * distance.g + .5);
		pixels[2] = float(invRange * distance.b + .5);
		pixels[3] = float(invRange * distance.a + .5);
	}
};

struct MSDFThreadData {
	msdfgen::Bitmap<float, 4> *output;
	msdfgen::Shape *shape;
	msdfgen::Projection *projection;
	DistancePixelConversion *distancePixelConversion;
};

static msdfgen::Point2 ft_point2(const FT_Vector &vector) {
	return msdfgen::Point2(vector.x / 60.0f, vector.y / 60.0f);
}

static int ft_move_to(const FT_Vector *to, void *user) {
	MSContext *context = reinterpret_cast<MSContext *>(user);
	if (!(context->contour && context->contour->edges.empty())) {
		context->contour = &context->shape->addContour();
	}
	context->position = ft_point2(*to);
	return 0;
}

static int ft_line_to(const FT_Vector *to, void *user) {
	MSContext *context = reinterpret_cast<MSContext *>(user);
	msdfgen::Point2 endpoint = ft_point2(*to);
	if (endpoint != context->position) {
		context->contour->addEdge(new msdfgen::LinearSegment(context->position, endpoint));
		context->position = endpoint;
	}
	return 0;
}

static int ft_conic_to(const FT_Vector *control, const FT_Vector *to, void *user) {
	MSContext *context = reinterpret_cast<MSContext *>(user);
	context->contour->addEdge(new msdfgen::QuadraticSegment(context->position, ft_point2(*control), ft_point2(*to)));
	context->position = ft_point2(*to);
	return 0;
}

static int ft_cubic_to(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
	MSContext *context = reinterpret_cast<MSContext *>(user);
	context->contour->addEdge(new msdfgen::CubicSegment(context->position, ft_point2(*control1), ft_point2(*control2), ft_point2(*to)));
	context->position = ft_point2(*to);
	return 0;
}

void TextServerAdvanced::_generateMTSDF_threaded(uint32_t y, void *p_td) const {
	MSDFThreadData *td = (MSDFThreadData *)p_td;

	msdfgen::ShapeDistanceFinder<msdfgen::OverlappingContourCombiner<msdfgen::MultiAndTrueDistanceSelector>> distanceFinder(*td->shape);
	int row = td->shape->inverseYAxis ? td->output->height() - y - 1 : y;
	for (int col = 0; col < td->output->width(); ++col) {
		int x = (y % 2) ? td->output->width() - col - 1 : col;
		msdfgen::Point2 p = td->projection->unproject(msdfgen::Point2(x + .5, y + .5));
		msdfgen::MultiAndTrueDistance distance = distanceFinder.distance(p);
		td->distancePixelConversion->operator()(td->output->operator()(x, row), distance);
	}
}

_FORCE_INLINE_ TextServerAdvanced::FontGlyph TextServerAdvanced::rasterize_msdf(FontDataAdvanced *p_font_data, FontDataForSizeAdvanced *p_data, int p_pixel_range, int p_rect_margin, FT_Outline *outline, const Vector2 &advance) const {
	msdfgen::Shape shape;

	shape.contours.clear();
	shape.inverseYAxis = false;

	MSContext context = {};
	context.shape = &shape;
	FT_Outline_Funcs ft_functions;
	ft_functions.move_to = &ft_move_to;
	ft_functions.line_to = &ft_line_to;
	ft_functions.conic_to = &ft_conic_to;
	ft_functions.cubic_to = &ft_cubic_to;
	ft_functions.shift = 0;
	ft_functions.delta = 0;

	int error = FT_Outline_Decompose(outline, &ft_functions, &context);
	ERR_FAIL_COND_V_MSG(error, FontGlyph(), "FreeType: Outline decomposition error: '" + String(FT_Error_String(error)) + "'.");
	if (!shape.contours.empty() && shape.contours.back().edges.empty()) {
		shape.contours.pop_back();
	}

	if (FT_Outline_Get_Orientation(outline) == 1) {
		for (int i = 0; i < (int)shape.contours.size(); ++i) {
			shape.contours[i].reverse();
		}
	}

	shape.inverseYAxis = true;
	shape.normalize();

	msdfgen::Shape::Bounds bounds = shape.getBounds(p_pixel_range);

	FontGlyph chr;
	chr.found = true;
	chr.advance = advance.round();

	if (shape.validate() && shape.contours.size() > 0) {
		int w = (bounds.r - bounds.l);
		int h = (bounds.t - bounds.b);

		int mw = w + p_rect_margin * 2;
		int mh = h + p_rect_margin * 2;

		ERR_FAIL_COND_V(mw > 4096, FontGlyph());
		ERR_FAIL_COND_V(mh > 4096, FontGlyph());

		FontTexturePosition tex_pos = find_texture_pos_for_glyph(p_data, 4, OGUI::PixelFormat::PF_R8G8B8A8, mw, mh);
		ERR_FAIL_COND_V(tex_pos.index < 0, FontGlyph());
		FontTexture &tex = p_data->font_textures[tex_pos.index];

		edgeColoringSimple(shape, 3.0); // Max. angle.
		msdfgen::Bitmap<float, 4> image(w, h); // Texture size.
		//msdfgen::generateMTSDF(image, shape, p_pixel_range, 1.0, msdfgen::Vector2(-bounds.l, -bounds.b)); // Range, scale, translation.

		DistancePixelConversion distancePixelConversion(p_pixel_range);
		msdfgen::Projection projection(msdfgen::Vector2(1.0, 1.0), msdfgen::Vector2(-bounds.l, -bounds.b));
		msdfgen::MSDFGeneratorConfig config(true, msdfgen::ErrorCorrectionConfig());

		MSDFThreadData td;
		td.output = &image;
		td.shape = &shape;
		td.projection = &projection;
		td.distancePixelConversion = &distancePixelConversion;

		p_font_data->work_pool.do_work(h, this, &TextServerAdvanced::_generateMTSDF_threaded, &td);

		msdfgen::msdfErrorCorrection(image, shape, projection, p_pixel_range, config);

		{
			uint8_t *wr = tex.imgdata.ptrw();

			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					int ofs = ((i + tex_pos.y + p_rect_margin) * tex.texture_w + j + tex_pos.x + p_rect_margin) * 4;
					ERR_FAIL_COND_V(ofs >= tex.imgdata.size(), FontGlyph());
					wr[ofs + 0] = (uint8_t)(CLAMP(image(j, i)[0] * 256.f, 0.f, 255.f));
					wr[ofs + 1] = (uint8_t)(CLAMP(image(j, i)[1] * 256.f, 0.f, 255.f));
					wr[ofs + 2] = (uint8_t)(CLAMP(image(j, i)[2] * 256.f, 0.f, 255.f));
					wr[ofs + 3] = (uint8_t)(CLAMP(image(j, i)[3] * 256.f, 0.f, 255.f));
					//wr[ofs + 0] = 100;
					//wr[ofs + 1] = 100;
					//wr[ofs + 2] = 100;
					//wr[ofs + 3] = 100;
				}
			}
		}

		

		// Blit to image and texture.
		{
			auto&& ctx = OGUI::Context::Get().renderImpl;

			OGUI::Bitmap bitmap;
			bitmap.resource = { tex.imgdata.data(), tex.imgdata.size() };
			bitmap.format = tex.format;
			bitmap.height = tex.texture_h;
			bitmap.width = tex.texture_w;
			if (!tex.texture.handle) {
				tex.texture.handle = ctx->RegisterTexture(bitmap);
			} else if(auto impl  = ctx.get();impl) {
				impl->UpdateTexture(tex.texture.handle, bitmap);
			}
		}

		// Update height array.
		for (int k = tex_pos.x; k < tex_pos.x + mw; k++) {
			tex.offsets.data()[k] = tex_pos.y + mh;
		}

		chr.texture_idx = tex_pos.index;

		chr.uv_rect = Rect2(tex_pos.x + p_rect_margin, tex_pos.y + p_rect_margin, w, h);
		chr.rect.position = Vector2(bounds.l, -bounds.t);
		chr.rect.size = chr.uv_rect.size;
		auto tex_size = Vector2{(real_t)tex.texture_w, (real_t)tex.texture_h};
		chr.uv_rect.position /= tex_size; chr.uv_rect.size /= tex_size;
	}
	return chr;
}
#endif

void gaussian_blur(uint8_t* data, int32 texW, Vector2i pos, int32 w, int32 h, int32 passes)
{

	// Calculate the kernel size.
	constexpr int32 kernel_size = 5;

	float kernel[kernel_size] = { 0.06136f, 0.3f, 0.5f, 0.3f, 0.06136f };
	constexpr int32 radius = kernel_size / 2;

	// Create the temporary buffer.
	float* temp = new float[w * h * 4];
	float* temp2 = new float[w * h * 4];
	
	for (int32 y = 0; y < h; y++)
	{
		for (int32 x = 0; x < w; x++)
		{
			temp2[y * w * 4 + x * 4 + 0] = data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 0];
			temp2[y * w * 4 + x * 4 + 1] = data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 1];
			temp2[y * w * 4 + x * 4 + 2] = data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 2];
			temp2[y * w * 4 + x * 4 + 3] = data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 3];
		}
	}

	for(int p = 0; p < passes; ++p)
	{
		// Apply the horizontal pass.
	for (int32 y = 0; y < h; y++)
	{
		for (int32 x = 0; x < w; x++)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;
			for (int32 i = 0; i < kernel_size; i++)
			{
				int32 sx = x + i - radius;
				if (sx < 0)
				{
					sx = 0;
				}
				if (sx >= w)
				{
					sx = w - 1;
				}
				int32 ofs = y * w * 4 + sx * 4;
				float weight = kernel[i];
				r += temp2[ofs + 0] * weight;
				g += temp2[ofs + 1] * weight;
				b += temp2[ofs + 2] * weight;
				a += temp2[ofs + 3] * weight;
			}
			int32 ofs = y * w * 4 + x * 4;
			temp[ofs + 0] = r;
			temp[ofs + 1] = g;
			temp[ofs + 2] = b;
			temp[ofs + 3] = a;
		}
	}

	// Apply the vertical pass.
	for (int32 y = 0; y < h; y++)
	{
		for (int32 x = 0; x < w; x++)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;
			for (int32 i = 0; i < kernel_size; i++)
			{
				int32 sy = y + i - radius;
				if (sy < 0)
				{
					sy = 0;
				}
				if (sy >= h)
				{
					sy = h - 1;
				}
				int32 ofs = sy * w * 4 + x * 4;
				float weight = kernel[i];
				r += temp[ofs + 0] * weight;
				g += temp[ofs + 1] * weight;
				b += temp[ofs + 2] * weight;
				a += temp[ofs + 3] * weight;
			}
			int32 ofs = y * w * 4 + x * 4;
			temp2[ofs + 0] = r;
			temp2[ofs + 1] = g;
			temp2[ofs + 2] = b;
			temp2[ofs + 3] = a;
		}
	}
	}

	// Copy the result back to the original buffer.
	for (int32 y = 0; y < h; y++)
	{
		for (int32 x = 0; x < w; x++)
		{
			data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 0] = (uint8_t)CLAMP(temp2[y * w * 4 + x * 4 + 0], 0.0f, 255.0f);
			data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 1] = (uint8_t)CLAMP(temp2[y * w * 4 + x * 4 + 1], 0.0f, 255.0f);
			data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 2] = (uint8_t)CLAMP(temp2[y * w * 4 + x * 4 + 2], 0.0f, 255.0f);
			data[(y + pos.y) * texW * 4 + (x + pos.x) * 4 + 3] = (uint8_t)CLAMP(temp2[y * w * 4 + x * 4 + 3], 0.0f, 255.0f);
		}
	}
	
	// Delete the temporary buffer.
	delete[] temp;
	delete[] temp2;
}

#ifdef MODULE_FREETYPE_ENABLED
_FORCE_INLINE_ TextServerAdvanced::FontGlyph TextServerAdvanced::rasterize_bitmap(FontDataForSizeAdvanced *p_data, int p_rect_margin, FT_Bitmap bitmap, int yofs, int xofs, const Vector2 &advance) const {
	bool blur = p_data->size.y < 0;
	if(blur)
	{
		p_rect_margin += -p_data->size.y;
	}
	
	int w = bitmap.width;
	int h = bitmap.rows;

	int mw = w + p_rect_margin * 2;
	int mh = h + p_rect_margin * 2;

	ERR_FAIL_COND_V(mw > 4096, FontGlyph());
	ERR_FAIL_COND_V(mh > 4096, FontGlyph());

	int color_size = 4;//bitmap.pixel_mode == FT_PIXEL_MODE_BGRA ? 4 : 2;
	//OGUI::PixelFormat require_format = color_size == 4 ? OGUI::PixelFormat::PF_R8G8B8A8 : OGUI::PixelFormat::PF_R8A8;
	OGUI::PixelFormat require_format = OGUI::PixelFormat::PF_R8G8B8A8;
	FontTexturePosition tex_pos = find_texture_pos_for_glyph(p_data, color_size, require_format, mw, mh);
	ERR_FAIL_COND_V(tex_pos.index < 0, FontGlyph());

	// Fit character in char texture.

	FontTexture &tex = p_data->font_textures[tex_pos.index];

	{
		uint8_t *wr = tex.imgdata.ptrw();

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				int ofs = ((i + tex_pos.y + p_rect_margin) * tex.texture_w + j + tex_pos.x + p_rect_margin) * color_size;
				ERR_FAIL_COND_V(ofs >= tex.imgdata.size(), FontGlyph());
				switch (bitmap.pixel_mode) {
					case FT_PIXEL_MODE_MONO: {
						int byte = i * bitmap.pitch + (j >> 3);
						int bit = 1 << (7 - (j % 8));
						//wr[ofs + 0] = 255; //grayscale as 1
						//wr[ofs + 1] = (bitmap.buffer[byte] & bit) ? 255 : 0;

						wr[ofs + 0] = 255; //grayscale as 1
						wr[ofs + 1] = 255; //grayscale as 1
						wr[ofs + 2] = 255; //grayscale as 1
						wr[ofs + 3] = (bitmap.buffer[byte] & bit) ? 255 : 0;
					} break;
					case FT_PIXEL_MODE_GRAY:
						//wr[ofs + 0] = 255; //grayscale as 1
						//wr[ofs + 1] = bitmap.buffer[i * bitmap.pitch + j];
						
						wr[ofs + 0] = 255; //grayscale as 1
						wr[ofs + 1] = 255; //grayscale as 1
						wr[ofs + 2] = 255; //grayscale as 1
						wr[ofs + 3] = bitmap.buffer[i * bitmap.pitch + j];
						break;
					case FT_PIXEL_MODE_BGRA: {
						int ofs_color = i * bitmap.pitch + (j << 2);
						wr[ofs + 2] = bitmap.buffer[ofs_color + 0];
						wr[ofs + 1] = bitmap.buffer[ofs_color + 1];
						wr[ofs + 0] = bitmap.buffer[ofs_color + 2];
						wr[ofs + 3] = bitmap.buffer[ofs_color + 3];
					} break;
					default:
						ERR_FAIL_V_MSG(FontGlyph(), "Font uses unsupported pixel format: " + itos(bitmap.pixel_mode) + ".");
						break;
				}
			}
		}
	}

	if(blur)
	{
		gaussian_blur(tex.imgdata.ptrw(), tex.texture_w, {tex_pos.x, tex_pos.y} , mw, mh, -p_data->size.y);
		p_rect_margin += p_data->size.y;
		w += -p_data->size.y * 2;
		h += -p_data->size.y * 2;
	}

	// Blit to image and texture.
	{
		auto&& ctx = OGUI::Context::Get().renderImpl;

		OGUI::Bitmap bitmap;
		bitmap.resource = { tex.imgdata.data(), tex.imgdata.size() };
		bitmap.format = tex.format;
		bitmap.height = tex.texture_h;
		bitmap.width = tex.texture_w;
		if (!tex.texture.handle) {
			tex.texture.handle = ctx->RegisterTexture(bitmap);
		} else if(auto impl  = ctx.get();impl) {
			impl->UpdateTexture(tex.texture.handle, bitmap);
		}
	}

	// Update height array.
	for (int k = tex_pos.x; k < tex_pos.x + mw; k++) {
		tex.offsets[k] = tex_pos.y + mh;
	}

	FontGlyph chr;
	chr.advance = (advance * p_data->scale / p_data->oversampling).round();
	chr.texture_idx = tex_pos.index;
	chr.found = true;

	chr.uv_rect = Rect2(tex_pos.x + p_rect_margin, tex_pos.y + p_rect_margin, w, h);
	chr.rect.position = (Vector2(xofs, -yofs) * p_data->scale / p_data->oversampling).round();
	chr.rect.size = chr.uv_rect.size * p_data->scale / p_data->oversampling;
	auto tex_size = Vector2{(real_t)tex.texture_w, (real_t)tex.texture_h};
	chr.uv_rect.position /= tex_size; chr.uv_rect.size /= tex_size;
	return chr;
}
#endif

/*************************************************************************/
/* Font Cache                                                            */
/*************************************************************************/

_FORCE_INLINE_ bool TextServerAdvanced::_ensure_glyph(FontDataAdvanced *p_font_data, const Vector2i &p_size, int32_t p_glyph) const {
	ERR_FAIL_COND_V(!_ensure_cache_for_size(p_font_data, p_size), false);

	FontDataForSizeAdvanced *fd = p_font_data->cache[p_size];
	if (fd->glyph_map.has(p_glyph)) {
		return fd->glyph_map[p_glyph].found;
	}

	if (p_glyph == 0) { // Non graphical or invalid glyph, do not render.
		fd->glyph_map[p_glyph] = FontGlyph();
		return true;
	}

#ifdef MODULE_FREETYPE_ENABLED
	FontGlyph gl;
	if (fd->face) {
		FT_Int32 flags = FT_LOAD_DEFAULT;

		bool outline = p_size.y > 0;
		switch (p_font_data->hinting) {
			case TextServer::HINTING_NONE:
				flags |= FT_LOAD_NO_HINTING;
				break;
			case TextServer::HINTING_LIGHT:
				flags |= FT_LOAD_TARGET_LIGHT;
				break;
			default:
				flags |= FT_LOAD_TARGET_NORMAL;
				break;
		}
		if (p_font_data->force_autohinter) {
			flags |= FT_LOAD_FORCE_AUTOHINT;
		}
		if (outline) {
			flags |= FT_LOAD_NO_BITMAP;
		} else if (FT_HAS_COLOR(fd->face)) {
			flags |= FT_LOAD_COLOR;
		}

		FT_Fixed v, h;
		FT_Get_Advance(fd->face, p_glyph, flags, &h);
		FT_Get_Advance(fd->face, p_glyph, flags | FT_LOAD_VERTICAL_LAYOUT, &v);

		int error = FT_Load_Glyph(fd->face, p_glyph, flags);
		if (error) {
			fd->glyph_map[p_glyph] = FontGlyph();
			ERR_FAIL_V_MSG(false, "FreeType: Failed to load glyph.");
		}
		if (!outline) {
			if (!p_font_data->msdf) {
				error = FT_Render_Glyph(fd->face->glyph, p_font_data->antialiased ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO);
			}
			FT_GlyphSlot slot = fd->face->glyph;
			if (!error) {
				if (p_font_data->msdf) {
#ifdef MODULE_MSDFGEN_ENABLED
					gl = rasterize_msdf(p_font_data, fd, p_font_data->msdf_range, rect_range, &slot->outline, Vector2((h + (1 << 9)) >> 10, (v + (1 << 9)) >> 10) / 64.0);
#else
					fd->glyph_map[p_glyph] = FontGlyph();
					ERR_FAIL_V_MSG(false, "Compiled without MSDFGEN support!");
#endif
				} else {
					gl = rasterize_bitmap(fd, rect_range, slot->bitmap, slot->bitmap_top, slot->bitmap_left, Vector2((h + (1 << 9)) >> 10, (v + (1 << 9)) >> 10) / 64.0);
				}
			}
		} else {
			FT_Stroker stroker;
			if (FT_Stroker_New(library, &stroker) != 0) {
				fd->glyph_map[p_glyph] = FontGlyph();
				ERR_FAIL_V_MSG(false, "FreeType: Failed to load glyph stroker.");
			}

			FT_Stroker_Set(stroker, (int)(fd->size.y * fd->oversampling * 16.0), FT_STROKER_LINECAP_BUTT, FT_STROKER_LINEJOIN_ROUND, 0);
			FT_Glyph glyph;
			FT_BitmapGlyph glyph_bitmap;

			if (FT_Get_Glyph(fd->face->glyph, &glyph) != 0) {
				goto cleanup_stroker;
			}
			if (FT_Glyph_Stroke(&glyph, stroker, 1) != 0) {
				goto cleanup_glyph;
			}
			if (FT_Glyph_To_Bitmap(&glyph, p_font_data->antialiased ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO, nullptr, 1) != 0) {
				goto cleanup_glyph;
			}
			glyph_bitmap = (FT_BitmapGlyph)glyph;
			gl = rasterize_bitmap(fd, rect_range, glyph_bitmap->bitmap, glyph_bitmap->top, glyph_bitmap->left, Vector2());

		cleanup_glyph:
			FT_Done_Glyph(glyph);
		cleanup_stroker:
			FT_Stroker_Done(stroker);
		}
		fd->glyph_map[p_glyph] = gl;
		return gl.found;
	}
#endif
	fd->glyph_map[p_glyph] = FontGlyph();
	return false;
}

_FORCE_INLINE_ bool TextServerAdvanced::_ensure_cache_for_size(FontDataAdvanced *p_font_data, const Vector2i &p_size) const {
	if (p_font_data->cache.has(p_size)) {
		return true;
	}

	FontDataForSizeAdvanced *fd = memnew(FontDataForSizeAdvanced);
	fd->size = p_size;
	if (p_font_data->data_ptr) {
		// Init dynamic font.
#ifdef MODULE_FREETYPE_ENABLED
		int error = 0;
		if (!library) {
			error = FT_Init_FreeType(&library);
			ERR_FAIL_COND_V_MSG(error != 0, false, TTR("FreeType: Error initializing library:") + " '" + String(FT_Error_String(error)) + "'.");
		}

		memset(&fd->stream, 0, sizeof(FT_StreamRec));
		fd->stream.base = (unsigned char *)p_font_data->data_ptr;
		fd->stream.size = p_font_data->data_size;
		fd->stream.pos = 0;

		FT_Open_Args fargs;
		memset(&fargs, 0, sizeof(FT_Open_Args));
		fargs.memory_base = (unsigned char *)p_font_data->data_ptr;
		fargs.memory_size = p_font_data->data_size;
		fargs.flags = FT_OPEN_MEMORY;
		fargs.stream = &fd->stream;
		error = FT_Open_Face(library, &fargs, 0, &fd->face);
		if (error) {
			FT_Done_Face(fd->face);
			fd->face = nullptr;
			ERR_FAIL_V_MSG(false, TTR("FreeType: Error loading font:") + " '" + String(FT_Error_String(error)) + "'.");
		}
		fd->hb_handle = hb_ft_font_create(fd->face, nullptr);
		if (fd->hb_handle == nullptr) {
			FT_Done_Face(fd->face);
			fd->face = nullptr;
			ERR_FAIL_V_MSG(false, TTR("HarfBuzz: Error creating FreeType font object."));
		}

		if (p_font_data->msdf) {
			fd->oversampling = 1.0f;
			fd->size.x = p_font_data->msdf_source_size;
		} else if (p_font_data->oversampling <= 0.0f) {
			fd->oversampling = font_get_global_oversampling();
		} else {
			fd->oversampling = p_font_data->oversampling;
		}

		if (FT_HAS_COLOR(fd->face) && fd->face->num_fixed_sizes > 0) {
			int best_match = 0;
			int diff = ABS(fd->size.x - ((int64_t)fd->face->available_sizes[0].width));
			fd->scale = real_t(fd->size.x * fd->oversampling) / fd->face->available_sizes[0].width;
			for (int i = 1; i < fd->face->num_fixed_sizes; i++) {
				int ndiff = ABS(fd->size.x - ((int64_t)fd->face->available_sizes[i].width));
				if (ndiff < diff) {
					best_match = i;
					diff = ndiff;
					fd->scale = real_t(fd->size.x * fd->oversampling) / fd->face->available_sizes[i].width;
				}
			}
			FT_Select_Size(fd->face, best_match);
		} else {
			FT_Set_Pixel_Sizes(fd->face, 0, fd->size.x * fd->oversampling);
		}

		fd->ascent = (fd->face->size->metrics.ascender / 64.0) / fd->oversampling * fd->scale;
		fd->descent = (-fd->face->size->metrics.descender / 64.0) / fd->oversampling * fd->scale;
		fd->underline_position = (-FT_MulFix(fd->face->underline_position, fd->face->size->metrics.y_scale) / 64.0) / fd->oversampling * fd->scale;
		fd->underline_thickness = (FT_MulFix(fd->face->underline_thickness, fd->face->size->metrics.y_scale) / 64.0) / fd->oversampling * fd->scale;
		auto os2 = (TT_OS2*) FT_Get_Sfnt_Table(fd->face, FT_SFNT_OS2);
		if(os2)
		{
			fd->strickout_position = (-FT_MulFix(os2->yStrikeoutPosition, fd->face->size->metrics.y_scale) / 64.0) / fd->oversampling * fd->scale;
			fd->strickout_thickness = (FT_MulFix(os2->yStrikeoutSize, fd->face->size->metrics.y_scale) / 64.0) / fd->oversampling * fd->scale;
		}
		else
		{
			fd->strickout_position = (-FT_MulFix(fd->face->height / 2.f + fd->ascent, fd->face->size->metrics.y_scale) / 64.0) / fd->oversampling * fd->scale;
			fd->strickout_thickness = fd->underline_thickness;
		}

		if (!p_font_data->face_init) {
			p_font_data->style_flags = 0;
			if (fd->face->style_flags & FT_STYLE_FLAG_BOLD) {
				p_font_data->style_flags |= FONT_BOLD;
			}
			if (fd->face->style_flags & FT_STYLE_FLAG_ITALIC) {
				p_font_data->style_flags |= FONT_ITALIC;
			}
			if (fd->face->face_flags & FT_FACE_FLAG_FIXED_WIDTH) {
				p_font_data->style_flags |= FONT_FIXED_WIDTH;
			}
			// Get supported scripts from OpenType font data.
			p_font_data->supported_scripts.clear();
			unsigned int count = hb_ot_layout_table_get_script_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GSUB, 0, nullptr, nullptr);
			if (count != 0) {
				hb_tag_t *script_tags = (hb_tag_t *)memalloc(count * sizeof(hb_tag_t));
				hb_ot_layout_table_get_script_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GSUB, 0, &count, script_tags);
				for (unsigned int i = 0; i < count; i++) {
					p_font_data->supported_scripts.insert(script_tags[i]);
				}
				memfree(script_tags);
			}
			count = hb_ot_layout_table_get_script_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GPOS, 0, nullptr, nullptr);
			if (count != 0) {
				hb_tag_t *script_tags = (hb_tag_t *)memalloc(count * sizeof(hb_tag_t));
				hb_ot_layout_table_get_script_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GPOS, 0, &count, script_tags);
				for (unsigned int i = 0; i < count; i++) {
					p_font_data->supported_scripts.insert(script_tags[i]);
				}
				memfree(script_tags);
			}

			// Get supported scripts from OS2 table.
			TT_OS2 *os2_ = (TT_OS2 *)FT_Get_Sfnt_Table(fd->face, FT_SFNT_OS2);
			if (os2_) {
				if ((os2_->ulUnicodeRange1 & 1L << 4) || (os2_->ulUnicodeRange1 & 1L << 5) || (os2_->ulUnicodeRange1 & 1L << 6) || (os2_->ulUnicodeRange1 & 1L << 31) || (os2_->ulUnicodeRange2 & 1L << 0) || (os2_->ulUnicodeRange2 & 1L << 1) || (os2_->ulUnicodeRange2 & 1L << 2) || (os2_->ulUnicodeRange2 & 1L << 3) || (os2_->ulUnicodeRange2 & 1L << 4) || (os2_->ulUnicodeRange2 & 1L << 5) || (os2_->ulUnicodeRange2 & 1L << 6) || (os2_->ulUnicodeRange2 & 1L << 7) || (os2_->ulUnicodeRange2 & 1L << 8) || (os2_->ulUnicodeRange2 & 1L << 9) || (os2_->ulUnicodeRange2 & 1L << 10) || (os2_->ulUnicodeRange2 & 1L << 11) || (os2_->ulUnicodeRange2 & 1L << 12) || (os2_->ulUnicodeRange2 & 1L << 13) || (os2_->ulUnicodeRange2 & 1L << 14) || (os2_->ulUnicodeRange2 & 1L << 15) || (os2_->ulUnicodeRange2 & 1L << 30) || (os2_->ulUnicodeRange3 & 1L << 0) || (os2_->ulUnicodeRange3 & 1L << 1) || (os2_->ulUnicodeRange3 & 1L << 2) || (os2_->ulUnicodeRange3 & 1L << 4) || (os2_->ulUnicodeRange3 & 1L << 5) || (os2_->ulUnicodeRange3 & 1L << 18) || (os2_->ulUnicodeRange3 & 1L << 24) || (os2_->ulUnicodeRange3 & 1L << 25) || (os2_->ulUnicodeRange3 & 1L << 26) || (os2_->ulUnicodeRange3 & 1L << 27) || (os2_->ulUnicodeRange3 & 1L << 28) || (os2_->ulUnicodeRange4 & 1L << 3) || (os2_->ulUnicodeRange4 & 1L << 6) || (os2_->ulUnicodeRange4 & 1L << 15) || (os2_->ulUnicodeRange4 & 1L << 23) || (os2_->ulUnicodeRange4 & 1L << 24) || (os2_->ulUnicodeRange4 & 1L << 26)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_COMMON);
				}
				if ((os2_->ulUnicodeRange1 & 1L << 0) || (os2_->ulUnicodeRange1 & 1L << 1) || (os2_->ulUnicodeRange1 & 1L << 2) || (os2_->ulUnicodeRange1 & 1L << 3) || (os2_->ulUnicodeRange1 & 1L << 29)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_LATIN);
				}
				if ((os2_->ulUnicodeRange1 & 1L << 7) || (os2_->ulUnicodeRange1 & 1L << 30)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GREEK);
				}
				if (os2_->ulUnicodeRange1 & 1L << 8) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_COPTIC);
				}
				if (os2_->ulUnicodeRange1 & 1L << 9) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CYRILLIC);
				}
				if (os2_->ulUnicodeRange1 & 1L << 10) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_ARMENIAN);
				}
				if (os2_->ulUnicodeRange1 & 1L << 11) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_HEBREW);
				}
				if (os2_->ulUnicodeRange1 & 1L << 12) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_VAI);
				}
				if ((os2_->ulUnicodeRange1 & 1L << 13) || (os2_->ulUnicodeRange2 & 1L << 31) || (os2_->ulUnicodeRange3 & 1L << 3)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_ARABIC);
				}
				if (os2_->ulUnicodeRange1 & 1L << 14) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_NKO);
				}
				if (os2_->ulUnicodeRange1 & 1L << 15) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_DEVANAGARI);
				}
				if (os2_->ulUnicodeRange1 & 1L << 16) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_BENGALI);
				}
				if (os2_->ulUnicodeRange1 & 1L << 17) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GURMUKHI);
				}
				if (os2_->ulUnicodeRange1 & 1L << 18) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GUJARATI);
				}
				if (os2_->ulUnicodeRange1 & 1L << 19) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_ORIYA);
				}
				if (os2_->ulUnicodeRange1 & 1L << 20) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TAMIL);
				}
				if (os2_->ulUnicodeRange1 & 1L << 21) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TELUGU);
				}
				if (os2_->ulUnicodeRange1 & 1L << 22) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_KANNADA);
				}
				if (os2_->ulUnicodeRange1 & 1L << 23) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_MALAYALAM);
				}
				if (os2_->ulUnicodeRange1 & 1L << 24) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_THAI);
				}
				if (os2_->ulUnicodeRange1 & 1L << 25) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_LAO);
				}
				if (os2_->ulUnicodeRange1 & 1L << 26) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GEORGIAN);
				}
				if (os2_->ulUnicodeRange1 & 1L << 27) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_BALINESE);
				}
				if ((os2_->ulUnicodeRange1 & 1L << 28) || (os2_->ulUnicodeRange2 & 1L << 20) || (os2_->ulUnicodeRange2 & 1L << 24)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_HANGUL);
				}
				if ((os2_->ulUnicodeRange2 & 1L << 21) || (os2_->ulUnicodeRange2 & 1L << 22) || (os2_->ulUnicodeRange2 & 1L << 23) || (os2_->ulUnicodeRange2 & 1L << 26) || (os2_->ulUnicodeRange2 & 1L << 27) || (os2_->ulUnicodeRange2 & 1L << 29)) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_HAN);
				}
				if (os2_->ulUnicodeRange2 & 1L << 17) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_HIRAGANA);
				}
				if (os2_->ulUnicodeRange2 & 1L << 18) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_KATAKANA);
				}
				if (os2_->ulUnicodeRange2 & 1L << 19) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_BOPOMOFO);
				}
				if (os2_->ulUnicodeRange3 & 1L << 6) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TIBETAN);
				}
				if (os2_->ulUnicodeRange3 & 1L << 7) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SYRIAC);
				}
				if (os2_->ulUnicodeRange3 & 1L << 8) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_THAANA);
				}
				if (os2_->ulUnicodeRange3 & 1L << 9) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SINHALA);
				}
				if (os2_->ulUnicodeRange3 & 1L << 10) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_MYANMAR);
				}
				if (os2_->ulUnicodeRange3 & 1L << 11) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_ETHIOPIC);
				}
				if (os2_->ulUnicodeRange3 & 1L << 12) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CHEROKEE);
				}
				if (os2_->ulUnicodeRange3 & 1L << 13) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CANADIAN_SYLLABICS);
				}
				if (os2_->ulUnicodeRange3 & 1L << 14) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_OGHAM);
				}
				if (os2_->ulUnicodeRange3 & 1L << 15) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_RUNIC);
				}
				if (os2_->ulUnicodeRange3 & 1L << 16) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_KHMER);
				}
				if (os2_->ulUnicodeRange3 & 1L << 17) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_MONGOLIAN);
				}
				if (os2_->ulUnicodeRange3 & 1L << 19) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_YI);
				}
				if (os2_->ulUnicodeRange3 & 1L << 20) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_HANUNOO);
					p_font_data->supported_scripts.insert(HB_SCRIPT_TAGBANWA);
					p_font_data->supported_scripts.insert(HB_SCRIPT_BUHID);
					p_font_data->supported_scripts.insert(HB_SCRIPT_TAGALOG);
				}
				if (os2_->ulUnicodeRange3 & 1L << 21) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_OLD_ITALIC);
				}
				if (os2_->ulUnicodeRange3 & 1L << 22) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GOTHIC);
				}
				if (os2_->ulUnicodeRange3 & 1L << 23) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_DESERET);
				}
				if (os2_->ulUnicodeRange3 & 1L << 29) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_LIMBU);
				}
				if (os2_->ulUnicodeRange3 & 1L << 30) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TAI_LE);
				}
				if (os2_->ulUnicodeRange3 & 1L << 31) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_NEW_TAI_LUE);
				}
				if (os2_->ulUnicodeRange4 & 1L << 0) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_BUGINESE);
				}
				if (os2_->ulUnicodeRange4 & 1L << 1) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_GLAGOLITIC);
				}
				if (os2_->ulUnicodeRange4 & 1L << 2) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TIFINAGH);
				}
				if (os2_->ulUnicodeRange4 & 1L << 4) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SYLOTI_NAGRI);
				}
				if (os2_->ulUnicodeRange4 & 1L << 5) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_LINEAR_B);
				}
				if (os2_->ulUnicodeRange4 & 1L << 7) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_UGARITIC);
				}
				if (os2_->ulUnicodeRange4 & 1L << 8) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_OLD_PERSIAN);
				}
				if (os2_->ulUnicodeRange4 & 1L << 9) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SHAVIAN);
				}
				if (os2_->ulUnicodeRange4 & 1L << 10) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_OSMANYA);
				}
				if (os2_->ulUnicodeRange4 & 1L << 11) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CYPRIOT);
				}
				if (os2_->ulUnicodeRange4 & 1L << 12) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_KHAROSHTHI);
				}
				if (os2_->ulUnicodeRange4 & 1L << 13) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_TAI_VIET);
				}
				if (os2_->ulUnicodeRange4 & 1L << 14) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CUNEIFORM);
				}
				if (os2_->ulUnicodeRange4 & 1L << 16) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SUNDANESE);
				}
				if (os2_->ulUnicodeRange4 & 1L << 17) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_LEPCHA);
				}
				if (os2_->ulUnicodeRange4 & 1L << 18) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_OL_CHIKI);
				}
				if (os2_->ulUnicodeRange4 & 1L << 19) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_SAURASHTRA);
				}
				if (os2_->ulUnicodeRange4 & 1L << 20) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_KAYAH_LI);
				}
				if (os2_->ulUnicodeRange4 & 1L << 21) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_REJANG);
				}
				if (os2_->ulUnicodeRange4 & 1L << 22) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_CHAM);
				}
				if (os2_->ulUnicodeRange4 & 1L << 25) {
					p_font_data->supported_scripts.insert(HB_SCRIPT_ANATOLIAN_HIEROGLYPHS);
				}
			}

			// Read OpenType feature tags.
			p_font_data->supported_features.clear();
			count = hb_ot_layout_table_get_feature_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GSUB, 0, nullptr, nullptr);
			if (count != 0) {
				hb_tag_t *feature_tags = (hb_tag_t *)memalloc(count * sizeof(hb_tag_t));
				hb_ot_layout_table_get_feature_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GSUB, 0, &count, feature_tags);
				for (unsigned int i = 0; i < count; i++) {
					p_font_data->supported_features.insert(feature_tags[i]);
				}
				memfree(feature_tags);
			}
			count = hb_ot_layout_table_get_feature_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GPOS, 0, nullptr, nullptr);
			if (count != 0) {
				hb_tag_t *feature_tags = (hb_tag_t *)memalloc(count * sizeof(hb_tag_t));
				hb_ot_layout_table_get_feature_tags(hb_font_get_face(fd->hb_handle), HB_OT_TAG_GPOS, 0, &count, feature_tags);
				for (unsigned int i = 0; i < count; i++) {
					p_font_data->supported_features.insert(feature_tags[i]);
				}
				memfree(feature_tags);
			}

			// Read OpenType variations.
			p_font_data->supported_varaitions.clear();
			if (fd->face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS) {
				FT_MM_Var *amaster;
				FT_Get_MM_Var(fd->face, &amaster);
				for (FT_UInt i = 0; i < amaster->num_axis; i++) {
					p_font_data->supported_varaitions[(int32_t)amaster->axis[i].tag] = Vector3i(amaster->axis[i].minimum / 65536, amaster->axis[i].maximum / 65536, amaster->axis[i].def / 65536);
				}
				FT_Done_MM_Var(library, amaster);
			}
			p_font_data->face_init = true;
		}

		// Write variations.
		if (fd->face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS) {
			FT_MM_Var *amaster;

			FT_Get_MM_Var(fd->face, &amaster);

			Vector<hb_variation_t> hb_vars;
			Vector<FT_Fixed> coords;
			coords.resize(amaster->num_axis);

			FT_Get_Var_Design_Coordinates(fd->face, coords.size(), coords.ptrw());

			for (FT_UInt i = 0; i < amaster->num_axis; i++) {
				hb_variation_t var;

				// Reset to default.
				var.tag = amaster->axis[i].tag;
				var.value = (double)amaster->axis[i].def / 65536.f;
				coords.data()[i] = amaster->axis[i].def;

				if (p_font_data->variation_coordinates.has(var.tag)) {
					var.value = p_font_data->variation_coordinates[var.tag];
					coords.data()[i] = CLAMP(var.value * 65536.f, amaster->axis[i].minimum, amaster->axis[i].maximum);
				}

				hb_vars.push_back(var);
			}

			FT_Set_Var_Design_Coordinates(fd->face, coords.size(), coords.ptrw());
			hb_font_set_variations(fd->hb_handle, hb_vars.is_empty() ? nullptr : &hb_vars[0], hb_vars.size());
			FT_Done_MM_Var(library, amaster);
		}
#else
		ERR_FAIL_V_MSG(false, TTR("FreeType: Can't load dynamic font, engine is compiled without FreeType support!"));
#endif
	} else {
		// Init bitmap font.
		fd->hb_handle = hb_bmp_font_create(fd, nullptr);
		if (!fd->hb_handle) {
			ERR_FAIL_V_MSG(false, TTR("HarfBuzz: Error creating bitmap font object."));
		}
	}
	p_font_data->cache[p_size] = fd;
	return true;
}

_FORCE_INLINE_ void TextServerAdvanced::_font_clear_cache(FontDataAdvanced *p_font_data) {
    for(auto& pair : p_font_data->cache)
    {
        memdelete(pair.second);
    }
	p_font_data->cache.clear();
	p_font_data->face_init = false;
	p_font_data->supported_features.clear();
	p_font_data->supported_varaitions.clear();
	p_font_data->supported_scripts.clear();
}

hb_font_t *TextServerAdvanced::_font_get_hb_handle(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, nullptr);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), nullptr);

	return fd->cache[size]->hb_handle;
}

RID TextServerAdvanced::create_font() {
	FontDataAdvanced *fd = memnew(FontDataAdvanced);

	return font_owner.make_rid(fd);
}

void TextServerAdvanced::font_set_data(RID p_font_rid, const PackedByteArray &p_data) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	_font_clear_cache(fd);
	fd->data = p_data;
	fd->data_ptr = fd->data.ptr();
	fd->data_size = fd->data.size();
}

void TextServerAdvanced::font_set_data_ptr(RID p_font_rid, const uint8_t *p_data_ptr, size_t p_data_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	_font_clear_cache(fd);
	fd->data.clear();
	fd->data_ptr = p_data_ptr;
	fd->data_size = p_data_size;
}

void TextServerAdvanced::font_set_style(const RID &p_font_rid, int64_t /*FontStyle*/ p_style) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, 16);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->style_flags = p_style;
}

int64_t /*FontStyle*/ TextServerAdvanced::font_get_style(const RID &p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, 16);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0);
	return fd->style_flags;
}

void TextServerAdvanced::font_set_antialiased(RID p_font_rid, bool p_antialiased) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->antialiased != p_antialiased) {
		_font_clear_cache(fd);
		fd->antialiased = p_antialiased;
	}
}

bool TextServerAdvanced::font_is_antialiased(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->antialiased;
}

void TextServerAdvanced::font_set_multichannel_signed_distance_field(RID p_font_rid, bool p_msdf) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->msdf != p_msdf) {
		_font_clear_cache(fd);
		fd->msdf = p_msdf;
	}
}

bool TextServerAdvanced::font_is_multichannel_signed_distance_field(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->msdf;
}

void TextServerAdvanced::font_set_msdf_pixel_range(RID p_font_rid, int p_msdf_pixel_range) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->msdf_range != p_msdf_pixel_range) {
		_font_clear_cache(fd);
		fd->msdf_range = p_msdf_pixel_range;
	}
}

int TextServerAdvanced::font_get_msdf_pixel_range(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->msdf_range;
}

void TextServerAdvanced::font_set_msdf_size(RID p_font_rid, int p_msdf_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->msdf_source_size != p_msdf_size) {
		_font_clear_cache(fd);
		fd->msdf_source_size = p_msdf_size;
	}
}

int TextServerAdvanced::font_get_msdf_size(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->msdf_source_size;
}

void TextServerAdvanced::font_set_fixed_size(RID p_font_rid, int p_fixed_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->fixed_size != p_fixed_size) {
		fd->fixed_size = p_fixed_size;
	}
}

int TextServerAdvanced::font_get_fixed_size(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->fixed_size;
}

void TextServerAdvanced::font_set_force_autohinter(RID p_font_rid, bool p_force_autohinter) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->force_autohinter != p_force_autohinter) {
		_font_clear_cache(fd);
		fd->force_autohinter = p_force_autohinter;
	}
}

bool TextServerAdvanced::font_is_force_autohinter(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->force_autohinter;
}

void TextServerAdvanced::font_set_hinting(RID p_font_rid, TextServer::Hinting p_hinting) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->hinting != p_hinting) {
		_font_clear_cache(fd);
		fd->hinting = p_hinting;
	}
}

TextServer::Hinting TextServerAdvanced::font_get_hinting(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, HINTING_NONE);

	MutexLock lock(fd->mutex);
	return fd->hinting;
}

void TextServerAdvanced::font_set_variation_coordinates(RID p_font_rid, const Map<uint32_t, double> &p_variation_coordinates) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->variation_coordinates != p_variation_coordinates) {
		_font_clear_cache(fd);
		fd->variation_coordinates = p_variation_coordinates;
	}
}

Map<uint32_t, double> TextServerAdvanced::font_get_variation_coordinates(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, (Map<uint32_t, double>{}));

	MutexLock lock(fd->mutex);
	return fd->variation_coordinates;
}

void TextServerAdvanced::font_set_oversampling(RID p_font_rid, real_t p_oversampling) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->oversampling != p_oversampling) {
		_font_clear_cache(fd);
		fd->oversampling = p_oversampling;
	}
}

real_t TextServerAdvanced::font_get_oversampling(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	return fd->oversampling;
}

Vector<Vector2i> TextServerAdvanced::font_get_size_cache_list(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, (Vector<Vector2i>{}));

	MutexLock lock(fd->mutex);
	Vector<Vector2i> ret;
	for (auto& pair : fd->cache) {
		ret.push_back(pair.first);
	}
	return ret;
}

void TextServerAdvanced::font_clear_size_cache(RID p_font_rid) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
    
	for (auto& pair : fd->cache) {
		memdelete(pair.second);
	}
	fd->cache.clear();
}

void TextServerAdvanced::font_remove_size_cache(RID p_font_rid, const Vector2i &p_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	if (fd->cache.has(p_size)) {
		memdelete(fd->cache[p_size]);
		fd->cache.erase(p_size);
	}
}

void TextServerAdvanced::font_set_ascent(RID p_font_rid, int p_size, real_t p_ascent) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->ascent = p_ascent;
}

real_t TextServerAdvanced::font_get_ascent(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->ascent * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->ascent;
	}
}

void TextServerAdvanced::font_set_descent(RID p_font_rid, int p_size, real_t p_descent) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->descent = p_descent;
}

real_t TextServerAdvanced::font_get_descent(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->descent * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->descent;
	}
}

void TextServerAdvanced::font_set_underline_position(RID p_font_rid, int p_size, real_t p_underline_position) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->underline_position = p_underline_position;
}

real_t TextServerAdvanced::font_get_underline_position(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->underline_position * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->underline_position;
	}
}

void TextServerAdvanced::font_set_strickout_position(RID p_font_rid, int p_size, real_t p_strickout_position) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->strickout_position = p_strickout_position;
}

real_t TextServerAdvanced::font_get_strickout_position(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->strickout_position * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->strickout_position;
	}
}

void TextServerAdvanced::font_set_underline_thickness(RID p_font_rid, int p_size, real_t p_underline_thickness) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->underline_thickness = p_underline_thickness;
}

real_t TextServerAdvanced::font_get_underline_thickness(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->underline_thickness * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->underline_thickness;
	}
}

void TextServerAdvanced::font_set_strickout_thickness(RID p_font_rid, int p_size, real_t p_strickout_thickness) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->strickout_thickness = p_strickout_thickness;
}

real_t TextServerAdvanced::font_get_strickout_thickness(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->strickout_thickness * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->strickout_thickness;
	}
}

void TextServerAdvanced::font_set_scale(RID p_font_rid, int p_size, real_t p_scale) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->scale = p_scale;
}

real_t TextServerAdvanced::font_get_scale(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0.f);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0.f);

	if (fd->msdf) {
		return fd->cache[size]->scale * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return fd->cache[size]->scale / fd->cache[size]->oversampling;
	}
}

void TextServerAdvanced::font_set_spacing(RID p_font_rid, int p_size, TextServer::SpacingType p_spacing, int p_value) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	switch (p_spacing) {
		case TextServer::SPACING_GLYPH: {
			fd->cache[size]->spacing_glyph = p_value;
		} break;
		case TextServer::SPACING_SPACE: {
			fd->cache[size]->spacing_space = p_value;
		} break;
		default: {
			ERR_FAIL_MSG("Invalid spacing type: " + itos(p_spacing));
		} break;
	}
}

int TextServerAdvanced::font_get_spacing(RID p_font_rid, int p_size, TextServer::SpacingType p_spacing) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0);

	switch (p_spacing) {
		case TextServer::SPACING_GLYPH: {
			if (fd->msdf) {
				return fd->cache[size]->spacing_glyph * (real_t)p_size / (real_t)fd->msdf_source_size;
			} else {
				return fd->cache[size]->spacing_glyph;
			}
		} break;
		case TextServer::SPACING_SPACE: {
			if (fd->msdf) {
				return fd->cache[size]->spacing_space * (real_t)p_size / (real_t)fd->msdf_source_size;
			} else {
				return fd->cache[size]->spacing_space;
			}
		} break;
		default: {
			ERR_FAIL_V_MSG(0, "Invalid spacing type: " + itos(p_spacing));
		} break;
	}
	return 0;
}

int TextServerAdvanced::font_get_texture_count(RID p_font_rid, const Vector2i &p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0);

	return fd->cache[size]->font_textures.size();
}

void TextServerAdvanced::font_clear_textures(RID p_font_rid, const Vector2i &p_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);
	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->font_textures.clear();
}

void TextServerAdvanced::font_remove_texture(RID p_font_rid, const Vector2i &p_size, int p_texture_index) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	ERR_FAIL_INDEX(p_texture_index, fd->cache[size]->font_textures.size());

	fd->cache[size]->font_textures.remove(p_texture_index);
}

void TextServerAdvanced::font_set_texture_image(RID p_font_rid, const Vector2i &p_size, int p_texture_index, const Ref<Image> &p_image) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);
	ERR_FAIL_COND(!!p_image);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	ERR_FAIL_COND(p_texture_index < 0);
	if (p_texture_index >= fd->cache[size]->font_textures.size()) {
		fd->cache[size]->font_textures.resize(p_texture_index + 1);
	}

	FontTexture &tex = fd->cache[size]->font_textures.data()[p_texture_index];

    tex.imgdata.resize(p_image->resource.size_in_bytes);
    memcpy(tex.imgdata.data(), p_image->resource.bytes, p_image->resource.size_in_bytes);
	tex.texture_w = p_image->width;
	tex.texture_h = p_image->height;
	tex.format = p_image->format;
    
	auto&& ctx = OGUI::Context::Get().renderImpl;
    tex.texture.handle = ctx->RegisterTexture(*p_image);
}

Ref<Image> TextServerAdvanced::font_get_texture_image(RID p_font_rid, const Vector2i &p_size, int p_texture_index) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Ref<Image>());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Ref<Image>());
	ERR_FAIL_INDEX_V(p_texture_index, fd->cache[size]->font_textures.size(), Ref<Image>());

	const FontTexture &tex = fd->cache[size]->font_textures.data()[p_texture_index];
	Ref<Image> img = {};//memnew(Image(tex.texture_w, tex.texture_h, 0, tex.format, tex.imgdata));
    //TODO: ???
	return img;
}

void TextServerAdvanced::font_set_texture_offsets(RID p_font_rid, const Vector2i &p_size, int p_texture_index, const PackedInt32Array &p_offset) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	if (p_texture_index >= fd->cache[size]->font_textures.size()) {
		fd->cache[size]->font_textures.resize(p_texture_index + 1);
	}

	FontTexture &tex = fd->cache[size]->font_textures.data()[p_texture_index];
	tex.offsets = p_offset;
}

PackedInt32Array TextServerAdvanced::font_get_texture_offsets(RID p_font_rid, const Vector2i &p_size, int p_texture_index) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, PackedInt32Array());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), PackedInt32Array());
	ERR_FAIL_INDEX_V(p_texture_index, fd->cache[size]->font_textures.size(), PackedInt32Array());

	const FontTexture &tex = fd->cache[size]->font_textures.data()[p_texture_index];
	return tex.offsets;
}

Vector<int32_t> TextServerAdvanced::font_get_glyph_list(RID p_font_rid, const Vector2i &p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector<int32_t>());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector<int32_t>());

	Vector<int32_t> ret;
    for(auto& pair : fd->cache[size]->glyph_map)
    {
        ret.push_back(pair.first);
    }
	return ret;
}

void TextServerAdvanced::font_clear_glyphs(RID p_font_rid, const Vector2i &p_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	fd->cache[size]->glyph_map.clear();
}

void TextServerAdvanced::font_remove_glyph(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	fd->cache[size]->glyph_map.erase(p_glyph);
}

Vector2 TextServerAdvanced::font_get_glyph_advance(RID p_font_rid, int p_size, int32_t p_glyph) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector2());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector2());
	if (!_ensure_glyph(fd, size, p_glyph)) {
		return Vector2(); // Invalid or non graphicl glyph, do not display errors.
	}

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	if (fd->msdf) {
		return gl[p_glyph].advance * (real_t)p_size / (real_t)fd->msdf_source_size;
	} else {
		return gl[p_glyph].advance;
	}
}

void TextServerAdvanced::font_set_glyph_advance(RID p_font_rid, int p_size, int32_t p_glyph, const Vector2 &p_advance) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	gl[p_glyph].advance = p_advance;
	gl[p_glyph].found = true;
}

Vector2 TextServerAdvanced::font_get_glyph_offset(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector2());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector2());
	if (!_ensure_glyph(fd, size, p_glyph)) {
		return Vector2(); // Invalid or non graphicl glyph, do not display errors.
	}

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	if (fd->msdf) {
		return gl[p_glyph].rect.position * (real_t)p_size.x / (real_t)fd->msdf_source_size;
	} else {
		return gl[p_glyph].rect.position;
	}
}

void TextServerAdvanced::font_set_glyph_offset(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_offset) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	gl[p_glyph].rect.position = p_offset;
	gl[p_glyph].found = true;
}

Vector2 TextServerAdvanced::font_get_glyph_size(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector2());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector2());
	if (!_ensure_glyph(fd, size, p_glyph)) {
		return Vector2(); // Invalid or non graphicl glyph, do not display errors.
	}

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	if (fd->msdf) {
		return gl[p_glyph].rect.size * (real_t)p_size.x / (real_t)fd->msdf_source_size;
	} else {
		return gl[p_glyph].rect.size;
	}
}

void TextServerAdvanced::font_set_glyph_size(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Vector2 &p_gl_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	gl[p_glyph].rect.size = p_gl_size;
	gl[p_glyph].found = true;
}

Rect2 TextServerAdvanced::font_get_glyph_uv_rect(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Rect2());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Rect2());
	if (!_ensure_glyph(fd, size, p_glyph)) {
		return Rect2(); // Invalid or non graphicl glyph, do not display errors.
	}

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;
	return gl[p_glyph].uv_rect;
}

void TextServerAdvanced::font_set_glyph_uv_rect(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, const Rect2 &p_uv_rect) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	gl[p_glyph].uv_rect = p_uv_rect;
	gl[p_glyph].found = true;
}

int TextServerAdvanced::font_get_glyph_texture_idx(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, -1);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), -1);
	if (!_ensure_glyph(fd, size, p_glyph)) {
		return -1; // Invalid or non graphicl glyph, do not display errors.
	}

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;
	return gl[p_glyph].texture_idx;
}

void TextServerAdvanced::font_set_glyph_texture_idx(RID p_font_rid, const Vector2i &p_size, int32_t p_glyph, int p_texture_idx) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));

	HashMap<int32_t, FontGlyph> &gl = fd->cache[size]->glyph_map;

	gl[p_glyph].texture_idx = p_texture_idx;
	gl[p_glyph].found = true;
}

bool TextServerAdvanced::font_get_glyph_contours(RID p_font_rid, int p_size, int32_t p_index, Vector<Vector3> &r_points, Vector<int32_t> &r_contours, bool &r_orientation) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), false);

#ifdef MODULE_FREETYPE_ENABLED
	int error = FT_Load_Glyph(fd->cache[size]->face, p_index, FT_LOAD_NO_BITMAP | (fd->force_autohinter ? FT_LOAD_FORCE_AUTOHINT : 0));
	ERR_FAIL_COND_V(error, false);

	r_points.clear();
	r_contours.clear();

	real_t h = fd->cache[size]->ascent;
	real_t scale = (1.0 / 64.0) / fd->cache[size]->oversampling * fd->cache[size]->scale;
	if (fd->msdf) {
		scale = scale * (real_t)p_size / (real_t)fd->msdf_source_size;
	}
	for (short i = 0; i < fd->cache[size]->face->glyph->outline.n_points; i++) {
		r_points.push_back(Vector3(fd->cache[size]->face->glyph->outline.points[i].x * scale, h - fd->cache[size]->face->glyph->outline.points[i].y * scale, FT_CURVE_TAG(fd->cache[size]->face->glyph->outline.tags[i])));
	}
	for (short i = 0; i < fd->cache[size]->face->glyph->outline.n_contours; i++) {
		r_contours.push_back(fd->cache[size]->face->glyph->outline.contours[i]);
	}
	r_orientation = (FT_Outline_Get_Orientation(&fd->cache[size]->face->glyph->outline) == FT_ORIENTATION_FILL_RIGHT);
#else
	return false;
#endif
	return true;
}

Vector<Vector2i> TextServerAdvanced::font_get_kerning_list(RID p_font_rid, int p_size) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector<Vector2i>());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector<Vector2i>());

	Vector<Vector2i> ret;
	for (auto& pair : fd->cache[size]->kerning_map) {
		ret.push_back(pair.first);
	}
	return ret;
}

void TextServerAdvanced::font_clear_kerning_map(RID p_font_rid, int p_size) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->kerning_map.clear();
}

void TextServerAdvanced::font_remove_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->kerning_map.erase(p_glyph_pair);
}

void TextServerAdvanced::font_set_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair, const Vector2 &p_kerning) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	fd->cache[size]->kerning_map[p_glyph_pair] = p_kerning;
}

Vector2 TextServerAdvanced::font_get_kerning(RID p_font_rid, int p_size, const Vector2i &p_glyph_pair) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector2());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);

	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Vector2());

	Map<Vector2i, Vector2> &kern = fd->cache[size]->kerning_map;

	if (kern.has(p_glyph_pair)) {
		if (fd->msdf) {
			return kern[p_glyph_pair] * (real_t)p_size / (real_t)fd->msdf_source_size;
		} else {
			return kern[p_glyph_pair];
		}
	} else {
#ifdef MODULE_FREETYPE_ENABLED
		if (fd->cache[size]->face) {
			FT_Vector delta;
			FT_Get_Kerning(fd->cache[size]->face, p_glyph_pair.x, p_glyph_pair.y, FT_KERNING_DEFAULT, &delta);
			if (fd->msdf) {
				return Vector2(delta.x, delta.y) * (real_t)p_size / (real_t)fd->msdf_source_size;
			} else {
				return Vector2(delta.x, delta.y);
			}
		}
#endif
	}
	return Vector2();
}

int32_t TextServerAdvanced::font_get_glyph_index(RID p_font_rid, int p_size, char32_t p_char, char32_t p_variation_selector) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, 0);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, p_size);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), 0);

#ifdef MODULE_FREETYPE_ENABLED
	if (fd->cache[size]->face) {
		if (p_variation_selector) {
			return FT_Face_GetCharVariantIndex(fd->cache[size]->face, p_char, p_variation_selector);
		} else {
			return FT_Get_Char_Index(fd->cache[size]->face, p_char);
		}
	} else {
		return 0;
	}
#else
	return (int32_t)p_char;
#endif
}

bool TextServerAdvanced::font_has_char(RID p_font_rid, char32_t p_char) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	if (fd->cache.is_empty()) {
		ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, fd->msdf ? Vector2i(fd->msdf_source_size, 0) : Vector2i(16, 0)), false);
	}
	FontDataForSizeAdvanced *at_size = fd->cache.begin()->second;

#ifdef MODULE_FREETYPE_ENABLED
	if (at_size && at_size->face) {
		return FT_Get_Char_Index(at_size->face, p_char) != 0;
	}
#endif
	return (at_size) ? at_size->glyph_map.has((int32_t)p_char) : false;
}

String TextServerAdvanced::font_get_supported_chars(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, String());

	MutexLock lock(fd->mutex);
	if (fd->cache.is_empty()) {
		ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, fd->msdf ? Vector2i(fd->msdf_source_size, 0) : Vector2i(16, 0)), String());
	}
	FontDataForSizeAdvanced *at_size = fd->cache.begin()->second;

	String chars;
#ifdef MODULE_FREETYPE_ENABLED
	if (at_size && at_size->face) {
		FT_UInt gindex;
		FT_ULong charcode = FT_Get_First_Char(at_size->face, &gindex);
		while (gindex != 0) {
			if (charcode != 0) {
				chars += char32_t(charcode);
			}
			charcode = FT_Get_Next_Char(at_size->face, charcode, &gindex);
		}
		return chars;
	}
#endif
	if (at_size) {
		const HashMap<int32_t, FontGlyph> &gl = at_size->glyph_map;
        for(auto& pair : at_size->glyph_map)
        {
            chars += pair.first;
        }
	}
	return chars;
}

void TextServerAdvanced::font_render_range(RID p_font_rid, const Vector2i &p_size, char32_t p_start, char32_t p_end) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	for (char32_t i = p_start; i <= p_end; i++) {
#ifdef MODULE_FREETYPE_ENABLED
		if (fd->cache[size]->face) {
			_ensure_glyph(fd, size, FT_Get_Char_Index(fd->cache[size]->face, i));
			continue;
		}
#endif
		_ensure_glyph(fd, size, (int32_t)i);
	}
}

void TextServerAdvanced::font_render_glyph(RID p_font_rid, const Vector2i &p_size, int32_t p_index) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, p_size);
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	ERR_FAIL_COND(!_ensure_glyph(fd, size, p_index));
}

std::shared_ptr<TextServer::GlyphDrawPolicy> get_policy(TextServerAdvanced::ShapedTextDataAdvanced *sd, const TextServerAdvanced::Glyph& glyph)
{
	return sd->spans[glyph.span].draw_policy;
}

void TextServerAdvanced::font_draw_glyph(RID p_shaped, OGUI::PrimDrawContext& list, const Glyph& glyph, const Vector2 &p_pos, const Color &p_color) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	while(sd->parent != RID())
		sd = shaped_owner.getornull(sd->parent);
	FontDataAdvanced *fd = font_owner.getornull(glyph.font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	auto policy = get_policy(sd, glyph);

	int outline_size = 0;
	if(policy)
	{
		outline_size = policy->get_outline_size();
	}

	Vector2i size = _get_size_outline(fd, { glyph.font_size, outline_size} );
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	if (!_ensure_glyph(fd, size, glyph.index)) {
		return; // // Invalid or non graphicl glyph, do not display errors, nothing to draw.
	}

	const FontGlyph &gl = fd->cache[size]->glyph_map[glyph.index];
	if (gl.found) {
		ERR_FAIL_COND(gl.texture_idx < -1 || gl.texture_idx >= fd->cache[size]->font_textures.size());

		if (gl.texture_idx != -1) {
			Color modulate = p_color;
#ifdef MODULE_FREETYPE_ENABLED
			if (fd->cache[size]->face && FT_HAS_COLOR(fd->cache[size]->face)) {
				modulate.r = modulate.g = modulate.b = 1.0;
			}
#endif
            auto& texture = fd->cache[size]->font_textures[gl.texture_idx].texture;
#ifdef MODULE_MSDFGEN_ENABLED
            if (fd->msdf) {
                Point2 cpos = p_pos;
                cpos += gl.rect.position * (real_t)glyph.font_size / (real_t)fd->msdf_source_size;
                Size2 csize = gl.rect.size * (real_t)glyph.font_size / (real_t)fd->msdf_source_size;
                canvas_item_add_msdf_texture_rect_region(list, Rect2(cpos, csize), texture.handle, gl.uv_rect, modulate, 0, fd->msdf_range, (double)glyph.font_size / (double)fd->msdf_source_size, policy.get());
            } else 
#endif
            {
				Point2i cpos = p_pos;
				cpos += gl.rect.position;
				Size2i csize = gl.rect.size;
				canvas_item_add_texture_rect_region(list, Rect2(cpos, csize), texture.handle, gl.uv_rect, modulate, policy.get());
            }
		}
	}
}

void TextServerAdvanced::font_draw_glyph_outline(RID p_shaped, OGUI::PrimDrawContext& list, const Glyph& glyph, int p_outline_size, const Vector2 &p_pos, const Color &p_color) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	while(sd->parent != RID())
		sd = shaped_owner.getornull(sd->parent);
	FontDataAdvanced *fd = font_owner.getornull(glyph.font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size_outline(fd, Vector2i(glyph.font_size, p_outline_size));
	ERR_FAIL_COND(!_ensure_cache_for_size(fd, size));
	if (!_ensure_glyph(fd, size, glyph.index)) {
		return; // // Invalid or non graphicl glyph, do not display errors, nothing to draw.
	}

	const FontGlyph &gl = fd->cache[size]->glyph_map[glyph.index];
	if (gl.found) {
		ERR_FAIL_COND(gl.texture_idx < -1 || gl.texture_idx >= fd->cache[size]->font_textures.size());
		
		if (gl.texture_idx != -1) {
			Color modulate = p_color;
#ifdef MODULE_FREETYPE_ENABLED
			if (fd->cache[size]->face && FT_HAS_COLOR(fd->cache[size]->face)) {
				modulate.r = modulate.g = modulate.b = 1.0;
			}
#endif
            auto& texture = fd->cache[size]->font_textures[gl.texture_idx].texture;
			auto policy = get_policy(sd, glyph);
#ifdef MODULE_MSDFGEN_ENABLED
            if (fd->msdf) {
                Point2 cpos = p_pos;
                cpos += gl.rect.position * (real_t)glyph.font_size / (real_t)fd->msdf_source_size;
                Size2 csize = gl.rect.size * (real_t)glyph.font_size / (real_t)fd->msdf_source_size;
                canvas_item_add_msdf_texture_rect_region(list, Rect2(cpos, csize), texture.handle, gl.uv_rect, modulate, p_outline_size * 2, fd->msdf_range, (double)glyph.font_size / (double)fd->msdf_source_size, policy.get());
            } else 
#endif
            {
				Point2i cpos = p_pos;
				cpos += gl.rect.position;
				Size2i csize = gl.rect.size;
				canvas_item_add_texture_rect_region(list, Rect2(cpos, csize), texture.handle, gl.uv_rect, modulate, policy.get());
            }
		}
	}
}

bool TextServerAdvanced::font_is_language_supported(RID p_font_rid, const String &p_language) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	if (fd->language_support_overrides.has(p_language)) {
		return fd->language_support_overrides[p_language];
	} else {
		return true;
	}
}

void TextServerAdvanced::font_set_language_support_override(RID p_font_rid, const String &p_language, bool p_supported) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	fd->language_support_overrides[p_language] = p_supported;
}

bool TextServerAdvanced::font_get_language_support_override(RID p_font_rid, const String &p_language) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->language_support_overrides[p_language];
}

void TextServerAdvanced::font_remove_language_support_override(RID p_font_rid, const String &p_language) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	fd->language_support_overrides.erase(p_language);
}

Vector<String> TextServerAdvanced::font_get_language_support_overrides(RID p_font_rid) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector<String>());

	MutexLock lock(fd->mutex);
	Vector<String> out;
	for (auto& pair : fd->language_support_overrides) {
		out.push_back(pair.first);
	}
	return out;
}

bool TextServerAdvanced::font_is_script_supported(RID p_font_rid, const String &p_script) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	if (fd->script_support_overrides.has(p_script)) {
		return fd->script_support_overrides[p_script];
	} else {
		Vector2i size = _get_size(fd, 16);
		ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), false);
		return fd->supported_scripts.has(name_to_tag(p_script));
	}
}

void TextServerAdvanced::font_set_script_support_override(RID p_font_rid, const String &p_script, bool p_supported) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	fd->script_support_overrides[p_script] = p_supported;
}

bool TextServerAdvanced::font_get_script_support_override(RID p_font_rid, const String &p_script) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, false);

	MutexLock lock(fd->mutex);
	return fd->script_support_overrides[p_script];
}

void TextServerAdvanced::font_remove_script_support_override(RID p_font_rid, const String &p_script) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND(!fd);

	MutexLock lock(fd->mutex);
	fd->script_support_overrides.erase(p_script);
}

Vector<String> TextServerAdvanced::font_get_script_support_overrides(RID p_font_rid) {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Vector<String>());

	MutexLock lock(fd->mutex);
	Vector<String> out;
	for (auto& pair : fd->script_support_overrides) {
		out.push_back(pair.first);
	}
	return out;
}

Set<uint32_t> TextServerAdvanced::font_supported_feature_list(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, Set<uint32_t>());

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, 16);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), Set<uint32_t>());
	return fd->supported_features;
}

Map<uint32_t, Vector3i> TextServerAdvanced::font_supported_variation_list(RID p_font_rid) const {
	FontDataAdvanced *fd = font_owner.getornull(p_font_rid);
	ERR_FAIL_COND_V(!fd, (Map<uint32_t, Vector3i>{}));

	MutexLock lock(fd->mutex);
	Vector2i size = _get_size(fd, 16);
	ERR_FAIL_COND_V(!_ensure_cache_for_size(fd, size), (Map<uint32_t, Vector3i>{}));
	return fd->supported_varaitions;
}

real_t TextServerAdvanced::font_get_global_oversampling() const {
	return oversampling;
}

void TextServerAdvanced::font_set_global_oversampling(real_t p_oversampling) {
	_THREAD_SAFE_METHOD_
	if (oversampling != p_oversampling) {
		oversampling = p_oversampling;
		List<RID> fonts;
		font_owner.get_owned_list(&fonts);
		bool font_cleared = false;
		for (const RID &E : fonts) {
			if (!font_is_multichannel_signed_distance_field(E) && font_get_oversampling(E) <= 0) {
				font_clear_size_cache(E);
				font_cleared = true;
			}
		}

		if (font_cleared) {
			List<RID> text_bufs;
			shaped_owner.get_owned_list(&text_bufs);
			for (const RID &E : text_bufs) {
				invalidate(shaped_owner.getornull(E));
			}
		}
	}
}

/*************************************************************************/
/* Shaped text buffer interface                                          */
/*************************************************************************/

int TextServerAdvanced::_convert_pos(const ShapedTextDataAdvanced *p_sd, int p_pos) const {
	int32_t limit = p_pos;
	if (p_sd->text.length() != p_sd->utf16.length()) {
		const UChar *data = p_sd->utf16.ptr();
		for (int i = 0; i < p_pos; i++) {
			if (U16_IS_LEAD(data[i])) {
				limit--;
			}
		}
	}
	return limit;
}

int TextServerAdvanced::_convert_pos_inv(const ShapedTextDataAdvanced *p_sd, int p_pos) const {
	int32_t limit = p_pos;
	if (p_sd->text.length() != p_sd->utf16.length()) {
		for (int i = 0; i < p_pos; i++) {
			if (p_sd->text[i] > 0xFFFF) {
				limit++;
			}
		}
	}
	return limit;
}

void TextServerAdvanced::invalidate(TextServerAdvanced::ShapedTextDataAdvanced *p_shaped) {
	p_shaped->valid = false;
	p_shaped->sort_valid = false;
	p_shaped->line_breaks_valid = false;
	p_shaped->justification_ops_valid = false;
	p_shaped->text_trimmed = false;
	p_shaped->ascent = 0.f;
	p_shaped->descent = 0.f;
	p_shaped->width = 0.f;
	p_shaped->upos = 0.f;
	p_shaped->uthk = 0.f;
	p_shaped->glyphs.clear();
	p_shaped->glyphs_logical.clear();
	p_shaped->overrun_trim_data = TrimData();
	p_shaped->utf16 = Char16String();
	if (p_shaped->script_iter != nullptr) {
		memdelete(p_shaped->script_iter);
		p_shaped->script_iter = nullptr;
	}
	for (int i = 0; i < p_shaped->bidi_iter.size(); i++) {
		ubidi_close(p_shaped->bidi_iter[i]);
	}
	p_shaped->bidi_iter.clear();
}

void TextServerAdvanced::full_copy(ShapedTextDataAdvanced *p_shaped) {
	ShapedTextDataAdvanced *parent = shaped_owner.getornull(p_shaped->parent);

	for (auto pair : parent->objects) {
		if (pair.second.pos >= p_shaped->start && pair.second.pos < p_shaped->end) {
			p_shaped->objects[pair.first] = pair.second;
		}
	}

	for (int k = 0; k < parent->spans.size(); k++) {
		ShapedTextDataAdvanced::Span span = parent->spans[k];
		if (span.start >= p_shaped->end || span.end <= p_shaped->start) {
			continue;
		}
		span.start = MAX(p_shaped->start, span.start);
		span.end = MIN(p_shaped->end, span.end);
		p_shaped->spans.push_back(span);
	}

	p_shaped->parent = RID();
}

RID TextServerAdvanced::create_shaped_text(TextServer::Direction p_direction, TextServer::Orientation p_orientation) {
	_THREAD_SAFE_METHOD_
	ShapedTextDataAdvanced *sd = memnew(ShapedTextDataAdvanced);
	sd->hb_buffer = hb_buffer_create();
	sd->direction = p_direction;
	sd->orientation = p_orientation;
	return shaped_owner.make_rid(sd);
}

void TextServerAdvanced::shaped_text_clear(RID p_shaped) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	sd->parent = RID();
	sd->start = 0;
	sd->end = 0;
	sd->text = String();
	sd->spans.clear();
	sd->objects.clear();
	sd->bidi_override.clear();
	invalidate(sd);
}

void TextServerAdvanced::shaped_text_set_direction(RID p_shaped, TextServer::Direction p_direction) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	if (sd->direction != p_direction) {
		if (sd->parent != RID()) {
			full_copy(sd);
		}
		sd->direction = p_direction;
		invalidate(sd);
	}
}

TextServer::Direction TextServerAdvanced::shaped_text_get_direction(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, TextServer::DIRECTION_LTR);

	MutexLock lock(sd->mutex);
	return sd->direction;
}

void TextServerAdvanced::shaped_text_set_bidi_override(RID p_shaped, const Vector<Vector2i> &p_override) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	if (sd->parent != RID()) {
		full_copy(sd);
	}
	sd->bidi_override = p_override;
	invalidate(sd);
}

void TextServerAdvanced::shaped_text_set_orientation(RID p_shaped, TextServer::Orientation p_orientation) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	if (sd->orientation != p_orientation) {
		if (sd->parent != RID()) {
			full_copy(sd);
		}
		sd->orientation = p_orientation;
		invalidate(sd);
	}
}

void TextServerAdvanced::shaped_text_set_preserve_invalid(RID p_shaped, bool p_enabled) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	ERR_FAIL_COND(sd->parent != RID());
	if (sd->preserve_invalid != p_enabled) {
		sd->preserve_invalid = p_enabled;
		invalidate(sd);
	}
}

bool TextServerAdvanced::shaped_text_get_preserve_invalid(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	return sd->preserve_invalid;
}

void TextServerAdvanced::shaped_text_set_preserve_control(RID p_shaped, bool p_enabled) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND(!sd);

	MutexLock lock(sd->mutex);
	if (sd->preserve_control != p_enabled) {
		if (sd->parent != RID()) {
			full_copy(sd);
		}
		sd->preserve_control = p_enabled;
		invalidate(sd);
	}
}

bool TextServerAdvanced::shaped_text_get_preserve_control(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	return sd->preserve_control;
}

TextServer::Orientation TextServerAdvanced::shaped_text_get_orientation(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, TextServer::ORIENTATION_HORIZONTAL);

	MutexLock lock(sd->mutex);
	return sd->orientation;
}

bool TextServerAdvanced::shaped_text_add_string(RID p_shaped, const String &p_text, const Vector<RID> &p_fonts, int p_size, int64_t p_flags, const std::shared_ptr<GlyphDrawPolicy> &draw_policy, const Map<uint32_t, double> &p_opentype_features, const String &p_language, const TextDecorationData& p_decoration, float p_letter_spacing, float p_word_spacing) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);
	ERR_FAIL_COND_V(p_size <= 0, false);

	MutexLock lock(sd->mutex);
	for (int i = 0; i < p_fonts.size(); i++) {
		ERR_FAIL_COND_V(!font_owner.getornull(p_fonts[i]), false);
	}

	if (p_text.is_empty()) {
		return true;
	}

	if (sd->parent != RID()) {
		full_copy(sd);
	}

	ShapedTextDataAdvanced::Span span;
	span.draw_policy = draw_policy;
	span.start = sd->text.length();
	span.end = span.start + p_text.length();
	span.fonts = p_fonts; // Do not pre-sort, spans will be divided to subruns later.
	span.font_size = p_size;
	span.language = p_language;
	span.features = p_opentype_features;
	span.flags = p_flags;
	span.decoration = p_decoration;
	span.letter_spacing = p_letter_spacing;
	span.word_spacing = p_word_spacing;

	sd->spans.push_back(span);
	sd->text += p_text;
	sd->end += p_text.length();
	invalidate(sd);

	return true;
}

bool TextServerAdvanced::shaped_text_add_object(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align, int p_length) {
	_THREAD_SAFE_METHOD_
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);
	ERR_FAIL_COND_V(p_key == Variant(), false);
	ERR_FAIL_COND_V(sd->objects.has(p_key), false);

	if (sd->parent != RID()) {
		full_copy(sd);
	}

	ShapedTextDataAdvanced::Span span;
	span.start = sd->text.length();
	span.end = span.start + p_length;
	span.embedded_key = p_key;

	ShapedTextDataAdvanced::EmbeddedObject obj;
	obj.inline_align = p_inline_align;
	obj.rect.size = p_size;
	obj.pos = span.start;

	sd->spans.push_back(span);
	sd->text += String::chr(0xfffc).repeat(p_length);
	sd->end += p_length;
	sd->objects[p_key] = obj;
	invalidate(sd);

	return true;
}

bool TextServerAdvanced::shaped_text_resize_object_raw(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align) {
	ShapedTextData *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	ERR_FAIL_COND_V(!sd->objects.has(p_key), false);
	auto& embedded = sd->objects[p_key];
	if(embedded.rect.size == p_size && embedded.inline_align == p_inline_align)
		return false;
	embedded.rect.size = p_size;
	embedded.inline_align = p_inline_align;
	return true;
}

bool TextServerAdvanced::shaped_text_resize_object(RID p_shaped, Variant p_key, const Size2 &p_size, InlineAlign p_inline_align) {
	ShapedTextData *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	ERR_FAIL_COND_V(!sd->objects.has(p_key), false);
	auto& embedded = sd->objects[p_key];
	if(embedded.rect.size == p_size && embedded.inline_align == p_inline_align)
		return false;
	embedded.rect.size = p_size;
	embedded.inline_align = p_inline_align;
	if (sd->valid) {
		// Recalc string metrics.
		sd->ascent = 0;
		sd->descent = 0;
		sd->width = 0;
		sd->upos = 0;
		sd->uthk = 0;
		int sd_size = sd->glyphs.size();

		for (int i = 0; i < sd_size; i++) {
			Glyph gl = sd->glyphs[i];
			Variant key = nullptr;
			if (gl.count == 1) {
				for (auto& pair : sd->objects) {
					if (pair.second.pos == gl.start) {
						key = pair.first;
						break;
					}
				}
			}
			if (key != Variant()) {
				if (sd->orientation == ORIENTATION_HORIZONTAL) {
					sd->objects[key].rect.position.x = sd->width;
					sd->width += sd->objects[key].rect.size.x;
					sd->glyphs[i].advance = sd->objects[key].rect.size.x;
				} else {
					sd->objects[key].rect.position.y = sd->width;
					sd->width += sd->objects[key].rect.size.y;
					sd->glyphs[i].advance = sd->objects[key].rect.size.y;
				}
			} else {
				if (gl.font_rid.is_valid()) {
					if (sd->orientation == ORIENTATION_HORIZONTAL) {
						sd->ascent = MAX(sd->ascent, MAX(font_get_ascent(gl.font_rid, gl.font_size), -gl.y_off));
						sd->descent = MAX(sd->descent, MAX(font_get_descent(gl.font_rid, gl.font_size), gl.y_off));
					} else {
						sd->ascent = MAX(sd->ascent, std::round(font_get_glyph_advance(gl.font_rid, gl.font_size, gl.index).x * 0.5));
						sd->descent = MAX(sd->descent, std::round(font_get_glyph_advance(gl.font_rid, gl.font_size, gl.index).x * 0.5));
					}
					sd->upos = MAX(sd->upos, font_get_underline_position(gl.font_rid, gl.font_size));
					sd->uthk = MAX(sd->uthk, font_get_underline_thickness(gl.font_rid, gl.font_size));
				} else if (sd->preserve_invalid || (sd->preserve_control && is_control(gl.index))) {
					// Glyph not found, replace with hex code box.
					if (sd->orientation == ORIENTATION_HORIZONTAL) {
						sd->ascent = MAX(sd->ascent, std::round(get_hex_code_box_size(gl.font_size, gl.index).y * 0.75f));
						sd->descent = MAX(sd->descent, std::round(get_hex_code_box_size(gl.font_size, gl.index).y * 0.25f));
					} else {
						sd->ascent = MAX(sd->ascent, std::round(get_hex_code_box_size(gl.font_size, gl.index).x * 0.5f));
						sd->descent = MAX(sd->descent, std::round(get_hex_code_box_size(gl.font_size, gl.index).x * 0.5f));
					}
				}
				sd->width += gl.advance * gl.repeat;
			}
		}

		// Align embedded objects to baseline.
		real_t full_ascent = sd->ascent;
		real_t full_descent = sd->descent;
		for (auto& pair : sd->objects) {
			if ((pair.second.pos >= sd->start) && (pair.second.pos < sd->end)) {
				if (sd->orientation == ORIENTATION_HORIZONTAL) {
					switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
						case INLINE_ALIGN_TO_TOP: {
							pair.second.rect.position.y = -sd->ascent;
						} break;
						case INLINE_ALIGN_TO_CENTER: {
							pair.second.rect.position.y = (-sd->ascent + sd->descent) / 2;
						} break;
						case INLINE_ALIGN_TO_BASELINE: {
							pair.second.rect.position.y = 0;
						} break;
						case INLINE_ALIGN_TO_BOTTOM: {
							pair.second.rect.position.y = sd->descent;
						} break;
					}
					switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
						case INLINE_ALIGN_BOTTOM_TO: {
							pair.second.rect.position.y -= pair.second.rect.size.y;
						} break;
						case INLINE_ALIGN_CENTER_TO: {
							pair.second.rect.position.y -= pair.second.rect.size.y / 2;
						} break;
						case INLINE_ALIGN_TOP_TO: {
							//NOP
						} break;
					}
					full_ascent = MAX(full_ascent, -pair.second.rect.position.y);
					full_descent = MAX(full_descent, pair.second.rect.position.y + pair.second.rect.size.y);
				} else {
					switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
						case INLINE_ALIGN_TO_TOP: {
							pair.second.rect.position.x = -sd->ascent;
						} break;
						case INLINE_ALIGN_TO_CENTER: {
							pair.second.rect.position.x = (-sd->ascent + sd->descent) / 2;
						} break;
						case INLINE_ALIGN_TO_BASELINE: {
							pair.second.rect.position.x = 0;
						} break;
						case INLINE_ALIGN_TO_BOTTOM: {
							pair.second.rect.position.x = sd->descent;
						} break;
					}
					switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
						case INLINE_ALIGN_BOTTOM_TO: {
							pair.second.rect.position.x -= pair.second.rect.size.x;
						} break;
						case INLINE_ALIGN_CENTER_TO: {
							pair.second.rect.position.x -= pair.second.rect.size.x / 2;
						} break;
						case INLINE_ALIGN_TOP_TO: {
							//NOP
						} break;
					}
					full_ascent = MAX(full_ascent, -pair.second.rect.position.x);
					full_descent = MAX(full_descent, pair.second.rect.position.x + pair.second.rect.size.x);
				}
			}
		}
		sd->ascent = full_ascent;
		sd->descent = full_descent;
	}
	return true;
}

RID TextServerAdvanced::shaped_text_substr(RID p_shaped, int p_start, int p_length) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, RID());

	MutexLock lock(sd->mutex);
	if (sd->parent != RID()) {
		return shaped_text_substr(sd->parent, p_start, p_length);
	}
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	ERR_FAIL_COND_V(p_start < 0 || p_length < 0, RID());
	ERR_FAIL_COND_V(sd->start > p_start || sd->end < p_start, RID());
	ERR_FAIL_COND_V(sd->end < p_start + p_length, RID());

	ShapedTextDataAdvanced *new_sd = memnew(ShapedTextDataAdvanced);

	new_sd->hb_buffer = hb_buffer_create();
	new_sd->parent = p_shaped;
	new_sd->start = p_start;
	new_sd->end = p_start + p_length;

	new_sd->orientation = sd->orientation;
	new_sd->direction = sd->direction;
	new_sd->para_direction = sd->para_direction;
	new_sd->line_breaks_valid = sd->line_breaks_valid;
	new_sd->justification_ops_valid = sd->justification_ops_valid;
	new_sd->sort_valid = false;
	new_sd->upos = sd->upos;
	new_sd->uthk = sd->uthk;

	if (p_length > 0) {
		new_sd->text = sd->text.substr(p_start, p_length);
		new_sd->utf16 = new_sd->text.utf16();
		new_sd->script_iter = memnew(ScriptIterator(new_sd->text, 0, new_sd->text.length()));

		int sd_size = sd->glyphs.size();
		const Glyph *sd_glyphs = sd->glyphs.ptr();
		for (int ov = 0; ov < sd->bidi_override.size(); ov++) {
			UErrorCode err = U_ZERO_ERROR;

			if (sd->bidi_override[ov].x >= p_start + p_length || sd->bidi_override[ov].y <= p_start) {
				continue;
			}
			int start = _convert_pos_inv(sd, MAX(0, p_start - sd->bidi_override[ov].x));
			int end = _convert_pos_inv(sd, MIN(p_start + p_length, sd->bidi_override[ov].y) - sd->bidi_override[ov].x);

			ERR_FAIL_COND_V_MSG((start < 0 || end - start > new_sd->utf16.length()), RID(), "Invalid BiDi override range.");

			// Create temporary line bidi & shape.
			UBiDi *bidi_iter = ubidi_openSized(end - start, 0, &err);
			ERR_FAIL_COND_V_MSG(U_FAILURE(err), RID(), u_errorName(err));
			ubidi_setLine(sd->bidi_iter[ov], start, end, bidi_iter, &err);
			if (U_FAILURE(err)) {
				ubidi_close(bidi_iter);
				ERR_FAIL_V_MSG(RID(), u_errorName(err));
			}
			new_sd->bidi_iter.push_back(bidi_iter);

			err = U_ZERO_ERROR;
			int bidi_run_count = ubidi_countRuns(bidi_iter, &err);
			ERR_FAIL_COND_V_MSG(U_FAILURE(err), RID(), u_errorName(err));
			for (int i = 0; i < bidi_run_count; i++) {
				int32_t _bidi_run_start = 0;
				int32_t _bidi_run_length = 0;
				ubidi_getVisualRun(bidi_iter, i, &_bidi_run_start, &_bidi_run_length);

				int32_t bidi_run_start = _convert_pos(sd, sd->bidi_override[ov].x + start + _bidi_run_start);
				int32_t bidi_run_end = _convert_pos(sd, sd->bidi_override[ov].x + start + _bidi_run_start + _bidi_run_length);

				for (int j = 0; j < sd_size; j++) {
					if ((sd_glyphs[j].start >= bidi_run_start) && (sd_glyphs[j].end <= bidi_run_end)) {
						// Copy glyphs.
						Glyph gl = sd_glyphs[j];
						Variant key;
						bool find_embedded = false;
						if (gl.count == 1) {
							for (auto& pair : sd->objects) {
								if (pair.second.pos == gl.start) {
									find_embedded = true;
									key = pair.first;
									new_sd->objects[key] = pair.second;
									break;
								}
							}
						}
						if (find_embedded) {
							if (new_sd->orientation == ORIENTATION_HORIZONTAL) {
								new_sd->objects[key].rect.position.x = new_sd->width;
								new_sd->width += new_sd->objects[key].rect.size.x;
							} else {
								new_sd->objects[key].rect.position.y = new_sd->width;
								new_sd->width += new_sd->objects[key].rect.size.y;
							}
						} else {
							if (gl.font_rid.is_valid()) {
								if (new_sd->orientation == ORIENTATION_HORIZONTAL) {
									new_sd->ascent = MAX(new_sd->ascent, MAX(font_get_ascent(gl.font_rid, gl.font_size), -gl.y_off));
									new_sd->descent = MAX(new_sd->descent, MAX(font_get_descent(gl.font_rid, gl.font_size), gl.y_off));
								} else {
									new_sd->ascent = MAX(new_sd->ascent, std::round(font_get_glyph_advance(gl.font_rid, gl.font_size, gl.index).x * 0.5));
									new_sd->descent = MAX(new_sd->descent, std::round(font_get_glyph_advance(gl.font_rid, gl.font_size, gl.index).x * 0.5));
								}
							} else if (new_sd->preserve_invalid || (new_sd->preserve_control && is_control(gl.index))) {
								// Glyph not found, replace with hex code box.
								if (new_sd->orientation == ORIENTATION_HORIZONTAL) {
									new_sd->ascent = MAX(new_sd->ascent, std::round(get_hex_code_box_size(gl.font_size, gl.index).y * 0.75f));
									new_sd->descent = MAX(new_sd->descent, std::round(get_hex_code_box_size(gl.font_size, gl.index).y * 0.25f));
								} else {
									new_sd->ascent = MAX(new_sd->ascent, std::round(get_hex_code_box_size(gl.font_size, gl.index).x * 0.5f));
									new_sd->descent = MAX(new_sd->descent, std::round(get_hex_code_box_size(gl.font_size, gl.index).x * 0.5f));
								}
							}
							new_sd->width += gl.advance * gl.repeat;
						}
						new_sd->glyphs.push_back(gl);
					}
				}
			}
		}

		// Align embedded objects to baseline.
		real_t full_ascent = new_sd->ascent;
		real_t full_descent = new_sd->descent;
		for (auto& pair : new_sd->objects) {
			if ((pair.second.pos >= new_sd->start) && (pair.second.pos < new_sd->end)) {
				if (sd->orientation == ORIENTATION_HORIZONTAL) {
					switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
						case INLINE_ALIGN_TO_TOP: {
							pair.second.rect.position.y = -new_sd->ascent;
						} break;
						case INLINE_ALIGN_TO_CENTER: {
							pair.second.rect.position.y = (-new_sd->ascent + new_sd->descent) / 2;
						} break;
						case INLINE_ALIGN_TO_BASELINE: {
							pair.second.rect.position.y = 0;
						} break;
						case INLINE_ALIGN_TO_BOTTOM: {
							pair.second.rect.position.y = new_sd->descent;
						} break;
					}
					switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
						case INLINE_ALIGN_BOTTOM_TO: {
							pair.second.rect.position.y -= pair.second.rect.size.y;
						} break;
						case INLINE_ALIGN_CENTER_TO: {
							pair.second.rect.position.y -= pair.second.rect.size.y / 2;
						} break;
						case INLINE_ALIGN_TOP_TO: {
							//NOP
						} break;
					}
					full_ascent = MAX(full_ascent, -pair.second.rect.position.y);
					full_descent = MAX(full_descent, pair.second.rect.position.y + pair.second.rect.size.y);
				} else {
					switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
						case INLINE_ALIGN_TO_TOP: {
							pair.second.rect.position.x = -new_sd->ascent;
						} break;
						case INLINE_ALIGN_TO_CENTER: {
							pair.second.rect.position.x = (-new_sd->ascent + new_sd->descent) / 2;
						} break;
						case INLINE_ALIGN_TO_BASELINE: {
							pair.second.rect.position.x = 0;
						} break;
						case INLINE_ALIGN_TO_BOTTOM: {
							pair.second.rect.position.x = new_sd->descent;
						} break;
					}
					switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
						case INLINE_ALIGN_BOTTOM_TO: {
							pair.second.rect.position.x -= pair.second.rect.size.x;
						} break;
						case INLINE_ALIGN_CENTER_TO: {
							pair.second.rect.position.x -= pair.second.rect.size.x / 2;
						} break;
						case INLINE_ALIGN_TOP_TO: {
							//NOP
						} break;
					}
					full_ascent = MAX(full_ascent, -pair.second.rect.position.x);
					full_descent = MAX(full_descent, pair.second.rect.position.x + pair.second.rect.size.x);
				}
			}
		}
		new_sd->ascent = full_ascent;
		new_sd->descent = full_descent;
	}
	new_sd->valid = true;

	return shaped_owner.make_rid(new_sd);
}

RID TextServerAdvanced::shaped_text_get_parent(RID p_shaped) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, RID());

	MutexLock lock(sd->mutex);
	return sd->parent;
}

TextDecorationData TextServerAdvanced::shaped_text_get_decoration(RID p_shaped, int p_span) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	while(sd->parent != RID())
		sd = shaped_owner.getornull(sd->parent);
	return sd->spans[p_span].decoration;
}

real_t TextServerAdvanced::shaped_text_fit_to_width(RID p_shaped, real_t p_width, int64_t /*JustificationFlag*/ p_jst_flags) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	if (!sd->justification_ops_valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_update_justification_ops(p_shaped);
	}

	sd->fit_width_minimum_reached = false;
	int start_pos = 0;
	int end_pos = sd->glyphs.size() - 1;

	if ((p_jst_flags & JUSTIFICATION_AFTER_LAST_TAB) == JUSTIFICATION_AFTER_LAST_TAB) {
		int start, end, delta;
		if (sd->para_direction == DIRECTION_LTR) {
			start = sd->glyphs.size() - 1;
			end = -1;
			delta = -1;
		} else {
			start = 0;
			end = sd->glyphs.size();
			delta = +1;
		}

		for (int i = start; i != end; i += delta) {
			if ((sd->glyphs[i].flags & GRAPHEME_IS_TAB) == GRAPHEME_IS_TAB) {
				if (sd->para_direction == DIRECTION_LTR) {
					start_pos = i;
					break;
				} else {
					end_pos = i;
					break;
				}
			}
		}
	}

	real_t justification_width;
	if ((p_jst_flags & JUSTIFICATION_CONSTRAIN_ELLIPSIS) == JUSTIFICATION_CONSTRAIN_ELLIPSIS) {
		if (sd->overrun_trim_data.trim_pos >= 0) {
			start_pos = sd->overrun_trim_data.trim_pos;
			justification_width = sd->width_trimmed;
		} else {
			return sd->width;
		}
	} else {
		justification_width = sd->width;
	}

	if ((p_jst_flags & JUSTIFICATION_TRIM_EDGE_SPACES) == JUSTIFICATION_TRIM_EDGE_SPACES) {
		float trimmed = 0;
		while ((start_pos < end_pos) && ((sd->glyphs[start_pos].flags & GRAPHEME_IS_SPACE) == GRAPHEME_IS_SPACE || (sd->glyphs[start_pos].flags & GRAPHEME_IS_BREAK_HARD) == GRAPHEME_IS_BREAK_HARD || (sd->glyphs[start_pos].flags & GRAPHEME_IS_BREAK_SOFT) == GRAPHEME_IS_BREAK_SOFT)) {
			trimmed += sd->glyphs[start_pos].advance * sd->glyphs[start_pos].repeat;
			justification_width -= sd->glyphs[start_pos].advance * sd->glyphs[start_pos].repeat;
			sd->glyphs.data()[start_pos].advance = 0;
			start_pos += sd->glyphs[start_pos].count;
		}
		for(auto& pair : sd->objects)
			pair.second.rect.position.x -= trimmed;
		while ((start_pos < end_pos) && ((sd->glyphs[end_pos].flags & GRAPHEME_IS_SPACE) == GRAPHEME_IS_SPACE || (sd->glyphs[end_pos].flags & GRAPHEME_IS_BREAK_HARD) == GRAPHEME_IS_BREAK_HARD || (sd->glyphs[end_pos].flags & GRAPHEME_IS_BREAK_SOFT) == GRAPHEME_IS_BREAK_SOFT)) {
			justification_width -= sd->glyphs[end_pos].advance * sd->glyphs[end_pos].repeat;
			sd->glyphs.data()[end_pos].advance = 0;
			end_pos -= sd->glyphs[end_pos].count;
		}
	}

	int space_count = 0;
	int elongation_count = 0;
	for (int i = start_pos; i <= end_pos; i++) {
		const Glyph &gl = sd->glyphs[i];
		if (gl.count > 0) {
			if ((gl.flags & GRAPHEME_IS_ELONGATION) == GRAPHEME_IS_ELONGATION) {
				elongation_count++;
			}
			if ((gl.flags & GRAPHEME_IS_SPACE) == GRAPHEME_IS_SPACE) {
				space_count++;
			}
		}
	}

	if ((elongation_count > 0) && ((p_jst_flags & JUSTIFICATION_KASHIDA) == JUSTIFICATION_KASHIDA)) {
		real_t delta_width_per_kashida = (p_width - justification_width) / elongation_count;
		for (int i = start_pos; i <= end_pos; i++) {
			Glyph &gl = sd->glyphs.data()[i];
			if (gl.count > 0) {
				if (((gl.flags & GRAPHEME_IS_ELONGATION) == GRAPHEME_IS_ELONGATION) && (gl.advance > 0)) {
					int count = delta_width_per_kashida / gl.advance;
					int prev_count = gl.repeat;
					if ((gl.flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) {
						gl.repeat = MAX(count, 0);
					}
					justification_width += (gl.repeat - prev_count) * gl.advance;
				}
			}
		}
	}
	if (space_count > 0) {
		real_t delta_width_per_space = (p_width - justification_width) / space_count;
		real_t old_width = justification_width;
		for (int i = start_pos; i <= end_pos; i++) {
			Glyph &gl = sd->glyphs.data()[i];
			if (gl.count > 0) {
				if ((gl.flags & GRAPHEME_IS_SPACE) == GRAPHEME_IS_SPACE) {
					real_t old_adv = gl.advance;
					real_t new_advance;
					if ((gl.flags & GRAPHEME_IS_VIRTUAL) == GRAPHEME_IS_VIRTUAL) {
						new_advance = MAX(gl.advance + delta_width_per_space, 0.f);
					} else {
						new_advance = MAX(gl.advance + delta_width_per_space, 0.1 * gl.font_size);
					}
					gl.advance = new_advance;
					justification_width += (gl.advance - old_adv);
				}
				for (auto& pair : sd->objects)
					if (pair.second.pos == gl.start)
						pair.second.rect.position.x += justification_width - old_width;
			}
		}
	}

	if (std::floor(p_width) < std::floor(justification_width)) {
		sd->fit_width_minimum_reached = true;
	}

	if ((p_jst_flags & JUSTIFICATION_CONSTRAIN_ELLIPSIS) != JUSTIFICATION_CONSTRAIN_ELLIPSIS) {
		sd->width = justification_width;
	}

	return sd->width;
}

real_t TextServerAdvanced::shaped_text_tab_align(RID p_shaped, const Vector<real_t> &p_tab_stops) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	if (!sd->line_breaks_valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_update_breaks(p_shaped);
	}

	int tab_index = 0;
	real_t off = 0.f;

	int start, end, delta;
	if (sd->para_direction == DIRECTION_LTR) {
		start = 0;
		end = sd->glyphs.size();
		delta = +1;
	} else {
		start = sd->glyphs.size() - 1;
		end = -1;
		delta = -1;
	}

	Glyph *gl = sd->glyphs.ptrw();

	for (int i = start; i != end; i += delta) {
		if ((gl[i].flags & GRAPHEME_IS_TAB) == GRAPHEME_IS_TAB) {
			real_t tab_off = 0.f;
			while (tab_off <= off) {
				tab_off += p_tab_stops[tab_index];
				tab_index++;
				if (tab_index >= p_tab_stops.size()) {
					tab_index = 0;
				}
			}
			real_t old_adv = gl[i].advance;
			gl[i].advance = tab_off - off;
			sd->width += gl[i].advance - old_adv;
			off = 0;
			continue;
		}
		off += gl[i].advance * gl[i].repeat;
	}

	return 0.f;
}

void TextServerAdvanced::shaped_text_overrun_trim_to_width(RID p_shaped_line, real_t p_width, int64_t p_trim_flags) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped_line);
	ERR_FAIL_COND_MSG(!sd, "ShapedTextDataAdvanced invalid.");

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		shaped_text_shape(p_shaped_line);
	}

	sd->overrun_trim_data.ellipsis_glyph_buf.clear();

	bool add_ellipsis = (p_trim_flags & OVERRUN_ADD_ELLIPSIS) == OVERRUN_ADD_ELLIPSIS;
	bool cut_per_word = (p_trim_flags & OVERRUN_TRIM_WORD_ONLY) == OVERRUN_TRIM_WORD_ONLY;
	bool enforce_ellipsis = (p_trim_flags & OVERRUN_ENFORCE_ELLIPSIS) == OVERRUN_ENFORCE_ELLIPSIS;
	bool justification_aware = (p_trim_flags & OVERRUN_JUSTIFICATION_AWARE) == OVERRUN_JUSTIFICATION_AWARE;

	Glyph *sd_glyphs = sd->glyphs.ptrw();

	if ((p_trim_flags & OVERRUN_TRIM) == OVERRUN_NO_TRIMMING || sd_glyphs == nullptr || p_width <= 0 || !(sd->width > p_width || enforce_ellipsis)) {
		sd->overrun_trim_data.trim_pos = -1;
		sd->overrun_trim_data.ellipsis_pos = -1;
		return;
	}

	if (justification_aware && !sd->fit_width_minimum_reached) {
		return;
	}

	int sd_size = sd->glyphs.size();
	RID last_gl_font_rid = sd_glyphs[sd_size - 1].font_rid;
	int last_gl_span = sd_glyphs[sd_size - 1].span;
	int last_gl_font_size = sd_glyphs[sd_size - 1].font_size;
	int32_t dot_gl_idx = font_get_glyph_index(last_gl_font_rid, last_gl_font_size, '.');
	Vector2 dot_adv = font_get_glyph_advance(last_gl_font_rid, last_gl_font_size, dot_gl_idx);
	int32_t whitespace_gl_idx = font_get_glyph_index(last_gl_font_rid, last_gl_font_size, ' ');
	Vector2 whitespace_adv = font_get_glyph_advance(last_gl_font_rid, last_gl_font_size, whitespace_gl_idx);

	int ellipsis_width = 0;
	if (add_ellipsis) {
		ellipsis_width = 3 * dot_adv.x + font_get_spacing(last_gl_font_rid, last_gl_font_size, SPACING_GLYPH) + (cut_per_word ? whitespace_adv.x : 0);
	}

	int ell_min_characters = 6;
	real_t width = sd->width;

	bool is_rtl = sd->direction == DIRECTION_RTL || (sd->direction == DIRECTION_AUTO && sd->para_direction == DIRECTION_RTL);

	int trim_pos = (is_rtl) ? sd_size : 0;
	int ellipsis_pos = (enforce_ellipsis) ? 0 : -1;

	int last_valid_cut = 0;
	bool found = false;

	int glyphs_from = (is_rtl) ? 0 : sd_size - 1;
	int glyphs_to = (is_rtl) ? sd_size - 1 : -1;
	int glyphs_delta = (is_rtl) ? +1 : -1;

	for (int i = glyphs_from; i != glyphs_to; i += glyphs_delta) {
		if (!is_rtl) {
			width -= sd_glyphs[i].advance * sd_glyphs[i].repeat;
		}
		if (sd_glyphs[i].count > 0) {
			bool above_min_char_treshold = ((is_rtl) ? sd_size - 1 - i : i) >= ell_min_characters;

			if (width + (((above_min_char_treshold && add_ellipsis) || enforce_ellipsis) ? ellipsis_width : 0) <= p_width) {
				if (cut_per_word && above_min_char_treshold) {
					if ((sd_glyphs[i].flags & GRAPHEME_IS_BREAK_SOFT) == GRAPHEME_IS_BREAK_SOFT) {
						last_valid_cut = i;
						found = true;
					}
				} else {
					last_valid_cut = i;
					found = true;
				}
				if (found) {
					trim_pos = last_valid_cut;

					if (add_ellipsis && (above_min_char_treshold || enforce_ellipsis) && width - ellipsis_width <= p_width) {
						ellipsis_pos = trim_pos;
					}
					break;
				}
			}
		}
		if (is_rtl) {
			width -= sd_glyphs[i].advance * sd_glyphs[i].repeat;
		}
	}

	sd->overrun_trim_data.trim_pos = trim_pos;
	sd->overrun_trim_data.ellipsis_pos = ellipsis_pos;
	if (trim_pos == 0 && enforce_ellipsis && add_ellipsis) {
		sd->overrun_trim_data.ellipsis_pos = 0;
	}

	if ((trim_pos >= 0 && sd->width > p_width) || enforce_ellipsis) {
		if (add_ellipsis && (ellipsis_pos > 0 || enforce_ellipsis)) {
			// Insert an additional space when cutting word bound for aesthetics.
			if (cut_per_word && (ellipsis_pos > 0)) {
				TextServer::Glyph gl;
				gl.count = 1;
				gl.advance = whitespace_adv.x;
				gl.index = whitespace_gl_idx;
				gl.font_rid = last_gl_font_rid;
				gl.span = last_gl_span;
				gl.font_size = last_gl_font_size;
				gl.flags = GRAPHEME_IS_SPACE | GRAPHEME_IS_BREAK_SOFT | GRAPHEME_IS_VIRTUAL | (is_rtl ? GRAPHEME_IS_RTL : 0);

				sd->overrun_trim_data.ellipsis_glyph_buf.push_back(gl);
			}
			// Add ellipsis dots.
			TextServer::Glyph gl;
			gl.count = 1;
			gl.repeat = 3;
			gl.advance = dot_adv.x;
			gl.index = dot_gl_idx;
			gl.font_rid = last_gl_font_rid;
			gl.span = last_gl_span;
			gl.font_size = last_gl_font_size;
			gl.flags = GRAPHEME_IS_PUNCTUATION | GRAPHEME_IS_VIRTUAL | (is_rtl ? GRAPHEME_IS_RTL : 0);

			sd->overrun_trim_data.ellipsis_glyph_buf.push_back(gl);
		}

		sd->text_trimmed = true;
		sd->width_trimmed = width + ((ellipsis_pos != -1) ? ellipsis_width : 0);
	}
}

TextServer::TrimData TextServerAdvanced::shaped_text_get_trim_data(RID p_shaped) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V_MSG(!sd, TrimData(), "ShapedTextDataAdvanced invalid.");

	MutexLock lock(sd->mutex);
	return sd->overrun_trim_data;
}

bool TextServerAdvanced::shaped_text_update_breaks(RID p_shaped) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		shaped_text_shape(p_shaped);
	}

	if (sd->line_breaks_valid) {
		return true; // Nothing to do.
	}

	const UChar *data = sd->utf16.ptr();

	HashMap<int, bool> breaks;
	UErrorCode err = U_ZERO_ERROR;
	int i = 0;
	while (i < sd->spans.size()) {
		String language = sd->spans[i].language;
		int r_start = sd->spans[i].start;
		while (i + 1 < sd->spans.size() && language == sd->spans[i + 1].language) {
			i++;
		}
		int r_end = sd->spans[i].end;
		UBreakIterator *bi = ubrk_open(UBRK_LINE, language.ascii().get_data(), data + _convert_pos_inv(sd, r_start), _convert_pos_inv(sd, r_end - r_start), &err);
		if (U_FAILURE(err)) {
			// No data loaded - use fallback.
			for (int j = r_start; j < r_end; j++) {
				char32_t c = sd->text[j - sd->start];
				if (is_whitespace(c)) {
					breaks[j] = false;
				}
				if (is_linebreak(c)) {
					breaks[j] = true;
				}
			}
		} else {
			while (ubrk_next(bi) != UBRK_DONE) {
				int pos = _convert_pos(sd, ubrk_current(bi)) + r_start - 1;
				if (pos != r_end) {
					if ((ubrk_getRuleStatus(bi) >= UBRK_LINE_HARD) && (ubrk_getRuleStatus(bi) < UBRK_LINE_HARD_LIMIT)) {
						breaks[pos] = true;
					} else if ((ubrk_getRuleStatus(bi) >= UBRK_LINE_SOFT) && (ubrk_getRuleStatus(bi) < UBRK_LINE_SOFT_LIMIT)) {
						breaks[pos] = false;
					}
				}
			}
		}
		ubrk_close(bi);
		i++;
	}

	sd->sort_valid = false;
	sd->glyphs_logical.clear();
	int sd_size = sd->glyphs.size();
	const char32_t *ch = sd->text.ptr();
	Glyph *sd_glyphs = sd->glyphs.ptrw();

	for (i = 0; i < sd_size; i++) {
		if (sd_glyphs[i].count > 0) {
			char32_t c = ch[sd_glyphs[i].start - sd->start];
			if (c == 0xfffc) {
				continue;
			}
			if (c == 0x0009 || c == 0x000b) {
				sd_glyphs[i].flags |= GRAPHEME_IS_TAB;
			}
			if (is_whitespace(c)) {
				sd_glyphs[i].flags |= GRAPHEME_IS_SPACE;
			}
			if (is_underscore(c)) {
				sd_glyphs[i].flags |= GRAPHEME_IS_UNDERSCORE;
			}
			if (u_ispunct(c) && c != 0x005F) {
				sd_glyphs[i].flags |= GRAPHEME_IS_PUNCTUATION;
			}
			if (breaks.has(sd->glyphs[i].start)) {
				if (breaks[sd->glyphs[i].start]) {
					sd_glyphs[i].flags |= GRAPHEME_IS_BREAK_HARD;
				} else {
					if (is_whitespace(c)) {
						sd_glyphs[i].flags |= GRAPHEME_IS_BREAK_SOFT;
					} else {
						TextServer::Glyph gl;
						gl.start = sd_glyphs[i].start;
						gl.end = sd_glyphs[i].end;
						gl.count = 1;
						gl.font_rid = sd_glyphs[i].font_rid;
						gl.span = sd_glyphs[i].span;
						gl.font_size = sd_glyphs[i].font_size;
						gl.flags = GRAPHEME_IS_BREAK_SOFT | GRAPHEME_IS_VIRTUAL;
						sd->glyphs.insert(i + sd_glyphs[i].count, gl); // Insert after.

						// Update write pointer and size.
						sd_size = sd->glyphs.size();
						sd_glyphs = sd->glyphs.ptrw();

						i += sd_glyphs[i].count;
						continue;
					}
				}
			}

			i += (sd_glyphs[i].count - 1);
		}
	}

	sd->line_breaks_valid = true;

	return sd->line_breaks_valid;
}

_FORCE_INLINE_ int _generate_kashida_justification_opportunies(const String &p_data, int p_start, int p_end) {
	int kashida_pos = -1;
	int8_t priority = 100;
	int i = p_start;

	char32_t pc = 0;

	while ((p_end > p_start) && is_transparent(p_data[p_end - 1])) {
		p_end--;
	}

	while (i < p_end) {
		uint32_t c = p_data[i];

		if (c == 0x0640) {
			kashida_pos = i;
			priority = 0;
		}
		if (priority >= 1 && i < p_end - 1) {
			if (is_seen_sad(c) && (p_data[i + 1] != 0x200C)) {
				kashida_pos = i;
				priority = 1;
			}
		}
		if (priority >= 2 && i > p_start) {
			if (is_teh_marbuta(c) || is_dal(c) || (is_heh(c) && i == p_end - 1)) {
				if (is_connected_to_prev(c, pc)) {
					kashida_pos = i - 1;
					priority = 2;
				}
			}
		}
		if (priority >= 3 && i > p_start) {
			if (is_alef(c) || ((is_lam(c) || is_tah(c) || is_kaf(c) || is_gaf(c)) && i == p_end - 1)) {
				if (is_connected_to_prev(c, pc)) {
					kashida_pos = i - 1;
					priority = 3;
				}
			}
		}
		if (priority >= 4 && i > p_start && i < p_end - 1) {
			if (is_beh(c)) {
				if (is_reh(p_data[i + 1]) || is_yeh(p_data[i + 1])) {
					if (is_connected_to_prev(c, pc)) {
						kashida_pos = i - 1;
						priority = 4;
					}
				}
			}
		}
		if (priority >= 5 && i > p_start) {
			if (is_waw(c) || ((is_ain(c) || is_qaf(c) || is_feh(c)) && i == p_end - 1)) {
				if (is_connected_to_prev(c, pc)) {
					kashida_pos = i - 1;
					priority = 5;
				}
			}
		}
		if (priority >= 6 && i > p_start) {
			if (is_reh(c)) {
				if (is_connected_to_prev(c, pc)) {
					kashida_pos = i - 1;
					priority = 6;
				}
			}
		}
		if (!is_transparent(c)) {
			pc = c;
		}
		i++;
	}

	return kashida_pos;
}

bool TextServerAdvanced::shaped_text_update_justification_ops(RID p_shaped) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		shaped_text_shape(p_shaped);
	}
	if (!sd->line_breaks_valid) {
		shaped_text_update_breaks(p_shaped);
	}

	if (sd->justification_ops_valid) {
		return true; // Nothing to do.
	}

	const UChar *data = sd->utf16.ptr();
	int32_t data_size = sd->utf16.length();

	Map<int, bool> jstops;

	// Use ICU word iterator and custom kashida detection.
	UErrorCode err = U_ZERO_ERROR;
	UBreakIterator *bi = nullptr;
	int i=0;
	while (i < sd->spans.size()) {
		bool character = (sd->spans[i].flags & JUSTIFICATION_CHARACTER) == JUSTIFICATION_CHARACTER;
		int r_start = sd->spans[i].start;
		while (i + 1 < sd->spans.size() && character == ((sd->spans[i + 1].flags & JUSTIFICATION_CHARACTER) == JUSTIFICATION_CHARACTER)) {
			i++;
		}
		int r_end = sd->spans[i].end;
		if(!character)
		{
			bi = ubrk_open(UBRK_WORD, "", data + _convert_pos_inv(sd, r_start), _convert_pos_inv(sd, r_end - r_start), &err);
		}
		else
		{
			bi = ubrk_open(UBRK_CHARACTER, "", data + _convert_pos_inv(sd, r_start), _convert_pos_inv(sd, r_end - r_start), &err);
		}
		if (U_FAILURE(err)) {
			// No data - use fallback.
			int limit = 0;
			for (int j = 0; j < sd->text.length(); j++) {
				if (is_whitespace(data[j])) {
					int ks = _generate_kashida_justification_opportunies(sd->text, limit, j) + sd->start;
					if (ks != -1) {
						jstops[ks] = true;
					}
					limit = j + 1;
				}
			}
			int ks = _generate_kashida_justification_opportunies(sd->text, limit, sd->text.length()) + sd->start;
			if (ks != -1) {
				jstops[ks] = true;
			}
		} else {
			int limit = 0;
			while (ubrk_next(bi) != UBRK_DONE) {
				//if (ubrk_getRuleStatus(bi) != UBRK_WORD_NONE) 
				{
					int j = _convert_pos(sd, ubrk_current(bi)) - 1;
					jstops[j + sd->start] = false;
					int ks = _generate_kashida_justification_opportunies(sd->text, limit, j);
					if (ks != -1) {
						jstops[ks + sd->start] = true;
					}
					limit = j;
				}
			}
			ubrk_close(bi);
		}
		++i;
	}

	sd->sort_valid = false;
	sd->glyphs_logical.clear();

	if (jstops.size() > 0) {
		for (int j = 0; j < sd->glyphs.size(); j++) {
			if (sd->glyphs[j].count > 0 && (sd->glyphs[j].flags & GRAPHEME_IS_VIRTUAL) == 0) {
				if (jstops.has(sd->glyphs[j].start)) {
					char32_t c = sd->text[sd->glyphs[j].start - sd->start];
					if (c == 0xfffc) {
						continue;
					}
					if (jstops[sd->glyphs[j].start]) {
						if (c == 0x0640) {
							sd->glyphs.data()[j].flags |= GRAPHEME_IS_ELONGATION;
						} else {
							if (sd->glyphs[j].font_rid != RID()) {
								TextServer::Glyph gl = _shape_single_glyph(sd, 0x0640, HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, sd->glyphs[j].font_rid, sd->glyphs[j].font_size, j);
								if ((gl.flags & GRAPHEME_IS_VALID) == GRAPHEME_IS_VALID) {
									gl.start = sd->glyphs[j].start;
									gl.end = sd->glyphs[j].end;
									gl.repeat = 0;
									gl.count = 1;
									if (sd->orientation == ORIENTATION_HORIZONTAL) {
										gl.y_off = sd->glyphs[j].y_off;
									} else {
										gl.x_off = sd->glyphs[j].x_off;
									}
									gl.flags |= GRAPHEME_IS_ELONGATION | GRAPHEME_IS_VIRTUAL;
									sd->glyphs.insert(j, gl);
									j++;
								}
							}
						}
					} else if (!is_whitespace(c)) {
						TextServer::Glyph gl;
						gl.start = sd->glyphs[j].start;
						gl.end = sd->glyphs[j].end;
						gl.count = 1;
						gl.font_rid = sd->glyphs[j].font_rid;
						gl.span = sd->glyphs[j].span;
						gl.font_size = sd->glyphs[j].font_size;
						gl.flags = GRAPHEME_IS_SPACE | GRAPHEME_IS_VIRTUAL;
						sd->glyphs.insert(j + sd->glyphs[j].count, gl); // Insert after.
						j += sd->glyphs[j].count;
						continue;
					}
				}
			}
		}
	}

	sd->justification_ops_valid = true;
	return sd->justification_ops_valid;
}

TextServer::Glyph TextServerAdvanced::_shape_single_glyph(ShapedTextDataAdvanced *p_sd, char32_t p_char, hb_script_t p_script, hb_direction_t p_direction, RID p_font, int p_font_size, int p_span) {
	hb_font_t *hb_font = _font_get_hb_handle(p_font, p_font_size);
	ERR_FAIL_COND_V(hb_font == nullptr, TextServer::Glyph());

	hb_buffer_clear_contents(p_sd->hb_buffer);
	hb_buffer_set_direction(p_sd->hb_buffer, p_direction);
	hb_buffer_set_flags(p_sd->hb_buffer, (hb_buffer_flags_t)(HB_BUFFER_FLAG_DEFAULT));
	hb_buffer_set_script(p_sd->hb_buffer, p_script);
	hb_buffer_add_utf32(p_sd->hb_buffer, (const uint32_t *)&p_char, 1, 0, 1);

	hb_shape(hb_font, p_sd->hb_buffer, nullptr, 0);

	unsigned int glyph_count = 0;
	hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(p_sd->hb_buffer, &glyph_count);
	hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(p_sd->hb_buffer, &glyph_count);

	// Process glyphs.
	TextServer::Glyph gl;

	if (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) {
		gl.flags |= TextServer::GRAPHEME_IS_RTL;
	}

	gl.font_rid = p_font;
	gl.span = p_span;
	gl.font_size = p_font_size;

	if (glyph_count > 0) {
		real_t scale = font_get_scale(p_font, p_font_size);
		if (p_sd->orientation == ORIENTATION_HORIZONTAL) {
			gl.advance = std::round(glyph_pos[0].x_advance / (64.0 / scale));
		} else {
			gl.advance = -std::round(glyph_pos[0].y_advance / (64.0 / scale));
		}
		gl.count = 1;

		gl.index = glyph_info[0].codepoint;
		gl.x_off = std::round(glyph_pos[0].x_offset / (64.0 / scale));
		gl.y_off = -std::round(glyph_pos[0].y_offset / (64.0 / scale));

		if ((glyph_info[0].codepoint != 0) || !u_isgraph(p_char)) {
			gl.flags |= GRAPHEME_IS_VALID;
		}
	}
	return gl;
}

void TextServerAdvanced::_shape_run(ShapedTextDataAdvanced *p_sd, int32_t p_start, int32_t p_end, hb_script_t p_script, hb_direction_t p_direction, Vector<RID> p_fonts, int p_span, int p_fb_index) {
	int fs = p_sd->spans[p_span].font_size;
	float letter_spacing = p_sd->spans[p_span].letter_spacing;
	float word_spacing = p_sd->spans[p_span].word_spacing;
	if (p_fb_index >= p_fonts.size()) {
		// Add fallback glyphs.
		for (int i = p_start; i < p_end; i++) {
			if (p_sd->preserve_invalid || (p_sd->preserve_control && is_control(p_sd->text[i]))) {
				TextServer::Glyph gl;
				gl.start = i;
				gl.end = i + 1;
				gl.count = 1;
				gl.index = p_sd->text[i];
				gl.font_size = fs;
				gl.font_rid = RID();
				gl.span = p_span;
				if (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) {
					gl.flags |= TextServer::GRAPHEME_IS_RTL;
				}
				if (p_sd->orientation == ORIENTATION_HORIZONTAL) {
					gl.advance = get_hex_code_box_size(fs, gl.index).x;
					p_sd->ascent = MAX(p_sd->ascent, std::round(get_hex_code_box_size(fs, gl.index).y * 0.75f));
					p_sd->descent = MAX(p_sd->descent, std::round(get_hex_code_box_size(fs, gl.index).y * 0.25f));
				} else {
					gl.advance = get_hex_code_box_size(fs, gl.index).y;
					p_sd->ascent = MAX(p_sd->ascent, std::round(get_hex_code_box_size(fs, gl.index).x * 0.5f));
					p_sd->descent = MAX(p_sd->descent, std::round(get_hex_code_box_size(fs, gl.index).x * 0.5f));
				}
				gl.advance += letter_spacing;
				p_sd->width += gl.advance;

				p_sd->glyphs.push_back(gl);
			}
		}
		return;
	}

	RID f = p_fonts[p_fb_index];
	hb_font_t *hb_font = _font_get_hb_handle(f, fs);
	ERR_FAIL_COND(hb_font == nullptr);

	hb_buffer_clear_contents(p_sd->hb_buffer);
	hb_buffer_set_direction(p_sd->hb_buffer, p_direction);
	if (p_sd->preserve_control) {
		hb_buffer_set_flags(p_sd->hb_buffer, (hb_buffer_flags_t)(HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES | (p_start == 0 ? HB_BUFFER_FLAG_BOT : 0) | (p_end == p_sd->text.length() ? HB_BUFFER_FLAG_EOT : 0)));
	} else {
		hb_buffer_set_flags(p_sd->hb_buffer, (hb_buffer_flags_t)(HB_BUFFER_FLAG_DEFAULT | (p_start == 0 ? HB_BUFFER_FLAG_BOT : 0) | (p_end == p_sd->text.length() ? HB_BUFFER_FLAG_EOT : 0)));
	}
	hb_buffer_set_script(p_sd->hb_buffer, p_script);

	if (p_sd->spans[p_span].language != String()) {
		hb_language_t lang = hb_language_from_string(p_sd->spans[p_span].language.ascii().get_data(), -1);
		hb_buffer_set_language(p_sd->hb_buffer, lang);
	}

	hb_buffer_add_utf32(p_sd->hb_buffer, (const uint32_t *)p_sd->text.ptr(), p_sd->text.length(), p_start, p_end - p_start);

	Vector<hb_feature_t> ftrs;
	for (auto pair : p_sd->spans[p_span].features) {
		double values = pair.second;
		if (values >= 0) {
			hb_feature_t feature;
			feature.tag = pair.first;
			feature.value = values;
			feature.start = 0;
			feature.end = -1;
			ftrs.push_back(feature);
		}
	}
	hb_shape(hb_font, p_sd->hb_buffer, ftrs.is_empty() ? nullptr : &ftrs[0], ftrs.size());

	unsigned int glyph_count = 0;
	hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(p_sd->hb_buffer, &glyph_count);
	hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(p_sd->hb_buffer, &glyph_count);

	// Process glyphs.
	if (glyph_count > 0) {
		TextServer::Glyph *w = (TextServer::Glyph *)memalloc(glyph_count * sizeof(TextServer::Glyph));

		int end = (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) ? p_end : 0;
		uint32_t last_cluster_id = UINT32_MAX;
		unsigned int last_cluster_index = 0;
		bool last_cluster_valid = true;

		for (unsigned int i = 0; i < glyph_count; i++) {
			if ((i > 0) && (last_cluster_id != glyph_info[i].cluster)) {
				if (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) {
					end = w[last_cluster_index].start;
				} else {
					for (unsigned int j = last_cluster_index; j < i; j++) {
						w[j].end = glyph_info[i].cluster;
					}
				}
				if (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) {
					w[last_cluster_index].flags |= TextServer::GRAPHEME_IS_RTL;
				}
				if (last_cluster_valid) {
					w[last_cluster_index].flags |= GRAPHEME_IS_VALID;
				}
				w[last_cluster_index].count = i - last_cluster_index;
				last_cluster_index = i;
				last_cluster_valid = true;
			}

			last_cluster_id = glyph_info[i].cluster;

			TextServer::Glyph &gl = w[i];
			gl = TextServer::Glyph();

			gl.start = glyph_info[i].cluster;
			gl.end = end;
			gl.count = 0;

			gl.font_rid = p_fonts[p_fb_index];
			gl.span = p_span;
			gl.font_size = fs;

			gl.index = glyph_info[i].codepoint;
			if (gl.index != 0) {
				real_t scale = font_get_scale(f, fs);
				if (p_sd->orientation == ORIENTATION_HORIZONTAL) {
					gl.advance = std::round(glyph_pos[i].x_advance / (64.0 / scale));
				} else {
					gl.advance = -std::round(glyph_pos[i].y_advance / (64.0 / scale));
				}
				gl.x_off = std::round(glyph_pos[i].x_offset / (64.0 / scale));
				gl.y_off = -std::round(glyph_pos[i].y_offset / (64.0 / scale));
			}
			if (is_whitespace(p_sd->text[glyph_info[i].cluster])) {
				gl.advance += font_get_spacing(f, fs, SPACING_SPACE);
				gl.advance += word_spacing;
			} else {
				gl.advance += font_get_spacing(f, fs, SPACING_GLYPH);
			}
			gl.advance += letter_spacing;

			if (p_sd->preserve_control) {
				last_cluster_valid = last_cluster_valid && ((glyph_info[i].codepoint != 0) || is_whitespace(p_sd->text[glyph_info[i].cluster]) || is_linebreak(p_sd->text[glyph_info[i].cluster]));
			} else {
				last_cluster_valid = last_cluster_valid && ((glyph_info[i].codepoint != 0) || !u_isgraph(p_sd->text[glyph_info[i].cluster]));
			}
		}
		if (p_direction == HB_DIRECTION_LTR || p_direction == HB_DIRECTION_TTB) {
			for (unsigned int j = last_cluster_index; j < glyph_count; j++) {
				w[j].end = p_end;
			}
		}
		w[last_cluster_index].count = glyph_count - last_cluster_index;
		if (p_direction == HB_DIRECTION_RTL || p_direction == HB_DIRECTION_BTT) {
			w[last_cluster_index].flags |= TextServer::GRAPHEME_IS_RTL;
		}
		if (last_cluster_valid) {
			w[last_cluster_index].flags |= GRAPHEME_IS_VALID;
		}

		// Fallback.
		int failed_subrun_start = p_end + 1;
		int failed_subrun_end = p_start;

		for (unsigned int i = 0; i < glyph_count; i++) {
			if ((w[i].flags & GRAPHEME_IS_VALID) == GRAPHEME_IS_VALID) {
				if (failed_subrun_start != p_end + 1) {
					_shape_run(p_sd, failed_subrun_start, failed_subrun_end, p_script, p_direction, p_fonts, p_span, p_fb_index + 1);
					failed_subrun_start = p_end + 1;
					failed_subrun_end = p_start;
				}
				for (int j = 0; j < w[i].count; j++) {
					if (p_sd->orientation == ORIENTATION_HORIZONTAL) {
						p_sd->ascent = MAX(p_sd->ascent, -w[i + j].y_off);
						p_sd->descent = MAX(p_sd->descent, w[i + j].y_off);
					} else {
						p_sd->ascent = MAX(p_sd->ascent, std::round(font_get_glyph_advance(f, fs, w[i + j].index).x * 0.5));
						p_sd->descent = MAX(p_sd->descent, std::round(font_get_glyph_advance(f, fs, w[i + j].index).x * 0.5));
					}
					p_sd->width += w[i + j].advance;
					p_sd->glyphs.push_back(w[i + j]);
				}
			} else {
				if (failed_subrun_start >= w[i].start) {
					failed_subrun_start = w[i].start;
				}
				if (failed_subrun_end <= w[i].end) {
					failed_subrun_end = w[i].end;
				}
			}
			i += w[i].count - 1;
		}
		memfree(w);
		if (failed_subrun_start != p_end + 1) {
			_shape_run(p_sd, failed_subrun_start, failed_subrun_end, p_script, p_direction, p_fonts, p_span, p_fb_index + 1);
		}
		p_sd->ascent = MAX(p_sd->ascent, font_get_ascent(f, fs));
		p_sd->descent = MAX(p_sd->descent, font_get_descent(f, fs));
		p_sd->upos = MAX(p_sd->upos, font_get_underline_position(f, fs));
		p_sd->uthk = MAX(p_sd->uthk, font_get_underline_thickness(f, fs));
	}
}

bool TextServerAdvanced::shaped_text_shape(RID p_shaped) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	if (sd->valid) {
		return true;
	}

	if (sd->parent != RID()) {
		full_copy(sd);
	}
	invalidate(sd);

	if (sd->text.length() == 0) {
		sd->valid = true;
		return true;
	}

	sd->utf16 = sd->text.utf16();
	const UChar *data = sd->utf16.ptr();

	// Create script iterator.
	if (sd->script_iter == nullptr) {
		sd->script_iter = memnew(ScriptIterator(sd->text, 0, sd->text.length()));
	}

	if (sd->bidi_override.is_empty()) {
		sd->bidi_override.push_back(Vector2i(0, sd->end));
	}

	for (int ov = 0; ov < sd->bidi_override.size(); ov++) {
		// Create BiDi iterator.
		int start = _convert_pos_inv(sd, sd->bidi_override[ov].x);
		int end = _convert_pos_inv(sd, sd->bidi_override[ov].y);

		ERR_FAIL_COND_V_MSG((start < 0 || end - start > sd->utf16.length()), false, "Invalid BiDi override range.");

		UErrorCode err = U_ZERO_ERROR;
		UBiDi *bidi_iter = ubidi_openSized(end, 0, &err);
		ERR_FAIL_COND_V_MSG(U_FAILURE(err), false, u_errorName(err));

		switch (sd->direction) {
			case DIRECTION_LTR: {
				ubidi_setPara(bidi_iter, data + start, end - start, UBIDI_LTR, nullptr, &err);
				sd->para_direction = DIRECTION_LTR;
			} break;
			case DIRECTION_RTL: {
				ubidi_setPara(bidi_iter, data + start, end - start, UBIDI_RTL, nullptr, &err);
				sd->para_direction = DIRECTION_RTL;
			} break;
			case DIRECTION_AUTO: {
				UBiDiDirection direction = ubidi_getBaseDirection(data + start, end - start);
				if (direction != UBIDI_NEUTRAL) {
					ubidi_setPara(bidi_iter, data + start, end - start, direction, nullptr, &err);
					sd->para_direction = (direction == UBIDI_RTL) ? DIRECTION_RTL : DIRECTION_LTR;
				} else {
					ubidi_setPara(bidi_iter, data + start, end - start, UBIDI_DEFAULT_LTR, nullptr, &err);
					sd->para_direction = DIRECTION_LTR;
				}
			} break;
		}
		ERR_FAIL_COND_V_MSG(U_FAILURE(err), false, u_errorName(err));
		sd->bidi_iter.push_back(bidi_iter);

		err = U_ZERO_ERROR;
		int bidi_run_count = ubidi_countRuns(bidi_iter, &err);
		ERR_FAIL_COND_V_MSG(U_FAILURE(err), false, u_errorName(err));
		for (int i = 0; i < bidi_run_count; i++) {
			int32_t _bidi_run_start = 0;
			int32_t _bidi_run_length = 0;
			hb_direction_t bidi_run_direction = HB_DIRECTION_INVALID;
			bool is_rtl = (ubidi_getVisualRun(bidi_iter, i, &_bidi_run_start, &_bidi_run_length) == UBIDI_LTR);
			switch (sd->orientation) {
				case ORIENTATION_HORIZONTAL: {
					if (is_rtl) {
						bidi_run_direction = HB_DIRECTION_LTR;
					} else {
						bidi_run_direction = HB_DIRECTION_RTL;
					}
				} break;
				case ORIENTATION_VERTICAL: {
					if (is_rtl) {
						bidi_run_direction = HB_DIRECTION_TTB;
					} else {
						bidi_run_direction = HB_DIRECTION_BTT;
					}
				}
			}

			int32_t bidi_run_start = _convert_pos(sd, sd->bidi_override[ov].x + _bidi_run_start);
			int32_t bidi_run_end = _convert_pos(sd, sd->bidi_override[ov].x + _bidi_run_start + _bidi_run_length);

			// Shape runs.

			int scr_from = (is_rtl) ? 0 : sd->script_iter->script_ranges.size() - 1;
			int scr_to = (is_rtl) ? sd->script_iter->script_ranges.size() : -1;
			int scr_delta = (is_rtl) ? +1 : -1;

			for (int j = scr_from; j != scr_to; j += scr_delta) {
				if ((sd->script_iter->script_ranges[j].start < bidi_run_end) && (sd->script_iter->script_ranges[j].end > bidi_run_start)) {
					int32_t script_run_start = MAX(sd->script_iter->script_ranges[j].start, bidi_run_start);
					int32_t script_run_end = MIN(sd->script_iter->script_ranges[j].end, bidi_run_end);
					char scr_buffer[5] = { 0, 0, 0, 0, 0 };
					hb_tag_to_string(hb_script_to_iso15924_tag(sd->script_iter->script_ranges[j].script), scr_buffer);
					String script = String(scr_buffer);

					int spn_from = (is_rtl) ? 0 : sd->spans.size() - 1;
					int spn_to = (is_rtl) ? sd->spans.size() : -1;
					int spn_delta = (is_rtl) ? +1 : -1;

					for (int k = spn_from; k != spn_to; k += spn_delta) {
						const ShapedTextDataAdvanced::Span &span = sd->spans[k];
						if (span.start >= script_run_end || span.end <= script_run_start) {
							continue;
						}
						if (span.embedded_key != Variant()) {
							// Embedded object.
							if (sd->orientation == ORIENTATION_HORIZONTAL) {
								sd->objects[span.embedded_key].rect.position.x = sd->width;
								sd->width += sd->objects[span.embedded_key].rect.size.x;
							} else {
								sd->objects[span.embedded_key].rect.position.y = sd->width;
								sd->width += sd->objects[span.embedded_key].rect.size.y;
							}
							Glyph gl;
							gl.start = span.start;
							gl.end = span.end;
							gl.count = 1;
							gl.flags = GRAPHEME_IS_VALID | GRAPHEME_IS_VIRTUAL;
							if (sd->orientation == ORIENTATION_HORIZONTAL) {
								gl.advance = sd->objects[span.embedded_key].rect.size.x;
							} else {
								gl.advance = sd->objects[span.embedded_key].rect.size.y;
							}
							sd->glyphs.push_back(gl);
						} else {
							Vector<RID> fonts;
							Vector<RID> fonts_scr_only;
							Vector<RID> fonts_no_match;
							int font_count = span.fonts.size();
							for (int l = 0; l < font_count; l++) {
								if (font_is_script_supported(span.fonts[l], script)) {
									if (font_is_language_supported(span.fonts[l], span.language)) {
										fonts.push_back(sd->spans[k].fonts[l]);
									} else {
										fonts_scr_only.push_back(sd->spans[k].fonts[l]);
									}
								} else {
									fonts_no_match.push_back(sd->spans[k].fonts[l]);
								}
							}
							fonts.append_array(fonts_scr_only);
							fonts.append_array(fonts_no_match);
							_shape_run(sd, MAX(sd->spans[k].start, script_run_start), MIN(sd->spans[k].end, script_run_end), sd->script_iter->script_ranges[j].script, bidi_run_direction, fonts, k, 0);
						}
					}
				}
			}
		}
	}

	// Align embedded objects to baseline.
	real_t full_ascent = sd->ascent;
	real_t full_descent = sd->descent;
	for (auto& pair : sd->objects) {
		if (sd->orientation == ORIENTATION_HORIZONTAL) {
			switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
				case INLINE_ALIGN_TO_TOP: {
					pair.second.rect.position.y = -sd->ascent;
				} break;
				case INLINE_ALIGN_TO_CENTER: {
					pair.second.rect.position.y = (-sd->ascent + sd->descent) / 2;
				} break;
				case INLINE_ALIGN_TO_BASELINE: {
					pair.second.rect.position.y = 0;
				} break;
				case INLINE_ALIGN_TO_BOTTOM: {
					pair.second.rect.position.y = sd->descent;
				} break;
			}
			switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
				case INLINE_ALIGN_BOTTOM_TO: {
					pair.second.rect.position.y -= pair.second.rect.size.y;
				} break;
				case INLINE_ALIGN_CENTER_TO: {
					pair.second.rect.position.y -= pair.second.rect.size.y / 2;
				} break;
				case INLINE_ALIGN_TOP_TO: {
					//NOP
				} break;
			}
			full_ascent = MAX(full_ascent, -pair.second.rect.position.y);
			full_descent = MAX(full_descent, pair.second.rect.position.y + pair.second.rect.size.y);
		} else {
			switch (pair.second.inline_align & INLINE_ALIGN_TEXT_MASK) {
				case INLINE_ALIGN_TO_TOP: {
					pair.second.rect.position.x = -sd->ascent;
				} break;
				case INLINE_ALIGN_TO_CENTER: {
					pair.second.rect.position.x = (-sd->ascent + sd->descent) / 2;
				} break;
				case INLINE_ALIGN_TO_BASELINE: {
					pair.second.rect.position.x = 0;
				} break;
				case INLINE_ALIGN_TO_BOTTOM: {
					pair.second.rect.position.x = sd->descent;
				} break;
			}
			switch (pair.second.inline_align & INLINE_ALIGN_IMAGE_MASK) {
				case INLINE_ALIGN_BOTTOM_TO: {
					pair.second.rect.position.x -= pair.second.rect.size.x;
				} break;
				case INLINE_ALIGN_CENTER_TO: {
					pair.second.rect.position.x -= pair.second.rect.size.x / 2;
				} break;
				case INLINE_ALIGN_TOP_TO: {
					//NOP
				} break;
			}
			full_ascent = MAX(full_ascent, -pair.second.rect.position.x);
			full_descent = MAX(full_descent, pair.second.rect.position.x + pair.second.rect.size.x);
		}
	}
	sd->ascent = full_ascent;
	sd->descent = full_descent;
	sd->valid = true;
	return sd->valid;
}

bool TextServerAdvanced::shaped_text_is_ready(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, false);

	MutexLock lock(sd->mutex);
	return sd->valid;
}

Vector<TextServer::Glyph> TextServerAdvanced::shaped_text_get_glyphs(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, Vector<TextServer::Glyph>());

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	return sd->glyphs;
}

Vector2i TextServerAdvanced::shaped_text_get_range(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, Vector2i());

	MutexLock lock(sd->mutex);
	return Vector2(sd->start, sd->end);
}

Vector<TextServer::Glyph> TextServerAdvanced::shaped_text_sort_logical(RID p_shaped) {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, Vector<TextServer::Glyph>());

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}

	if (!sd->sort_valid) {
		sd->glyphs_logical = sd->glyphs;
        std::sort(sd->glyphs_logical.begin(), sd->glyphs_logical.end(), TextServer::GlyphCompare{});
		sd->sort_valid = true;
	}

	return sd->glyphs_logical;
}

 Vector<void*> TextServerAdvanced::shaped_text_get_objects(RID p_shaped) const {
	 Vector<void*> ret;
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, ret);

	MutexLock lock(sd->mutex);
	for (auto& pair : sd->objects) {
		ret.push_back(pair.first);
	}

	return ret;
}

Rect2 TextServerAdvanced::shaped_text_get_object_rect(RID p_shaped, Variant p_key) const {
	ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, Rect2());

	MutexLock lock(sd->mutex);
	ERR_FAIL_COND_V(!sd->objects.has(p_key), Rect2());
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	return sd->objects[p_key].rect;
}

Size2 TextServerAdvanced::shaped_text_get_size(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, Size2());

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	if (sd->orientation == TextServer::ORIENTATION_HORIZONTAL) {
		return Size2((sd->text_trimmed ? sd->width_trimmed : sd->width), sd->ascent + sd->descent);
	} else {
		return Size2(sd->ascent + sd->descent, (sd->text_trimmed ? sd->width_trimmed : sd->width));
	}
}

real_t TextServerAdvanced::shaped_text_get_ascent(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	return sd->ascent;
}

real_t TextServerAdvanced::shaped_text_get_descent(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	return sd->descent;
}

real_t TextServerAdvanced::shaped_text_get_width(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}
	return (sd->text_trimmed ? sd->width_trimmed : sd->width);
}

real_t TextServerAdvanced::shaped_text_get_underline_position(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}

	return sd->upos;
}

real_t TextServerAdvanced::shaped_text_get_underline_thickness(RID p_shaped) const {
	const ShapedTextDataAdvanced *sd = shaped_owner.getornull(p_shaped);
	ERR_FAIL_COND_V(!sd, 0.f);

	MutexLock lock(sd->mutex);
	if (!sd->valid) {
		const_cast<TextServerAdvanced *>(this)->shaped_text_shape(p_shaped);
	}

	return sd->uthk;
}

struct num_system_data {
	String lang;
	String digits;
	String percent_sign;
	String exp;
};

static num_system_data num_systems[]{
	{ "ar,ar_AR,ar_BH,ar_DJ,ar_EG,ar_ER,ar_IL,ar_IQ,ar_JO,ar_KM,ar_KW,ar_LB,ar_MR,ar_OM,ar_PS,ar_QA,ar_SA,ar_SD,ar_SO,ar_SS,ar_SY,ar_TD,ar_YE", U"٠١٢٣٤٥٦٧٨٩٫", U"٪", U"اس" },
	{ "fa,ks,pa_Arab,ps,ug,ur_IN,ur,uz_Arab", U"۰۱۲۳۴۵۶۷۸۹٫", U"٪", U"اس" },
	{ "as,bn,mni", U"০১২৩৪৫৬৭৮৯.", U"%", U"e" },
	{ "mr,ne", U"०१२३४५६७८९.", U"%", U"e" },
	{ "dz", U"༠༡༢༣༤༥༦༧༨༩.", U"%", U"e" },
	{ "sat", U"᱐᱑᱒᱓᱔᱕᱖᱗᱘᱙.", U"%", U"e" },
	{ "my", U"၀၁၂၃၄၅၆၇၈၉.", U"%", U"e" },
	{ String(), String(), String(), String() },
};

String TextServerAdvanced::format_number(const String &p_string, const String &p_language) const {
	String lang = p_language;

	String res = p_string;
	for (int i = 0; num_systems[i].lang != String(); i++) {
		Vector<String> langs = num_systems[i].lang.split(",");
		if (langs.has(lang)) {
			if (num_systems[i].digits == String()) {
				return p_string;
			}
			res.replace("e", num_systems[i].exp);
			res.replace("E", num_systems[i].exp);
			char32_t *data = res.ptrw();
			for (int j = 0; j < res.size(); j++) {
				if (data[j] >= 0x30 && data[j] <= 0x39) {
					data[j] = num_systems[i].digits[data[j] - 0x30];
				} else if (data[j] == '.' || data[j] == ',') {
					data[j] = num_systems[i].digits[10];
				}
			}
			break;
		}
	}
	return res;
}

String TextServerAdvanced::parse_number(const String &p_string, const String &p_language) const {
	String lang = p_language;

	String res = p_string;
	for (int i = 0; num_systems[i].lang != String(); i++) {
		Vector<String> langs = num_systems[i].lang.split(",");
		if (langs.has(lang)) {
			if (num_systems[i].digits == String()) {
				return p_string;
			}
			res.replace(num_systems[i].exp, "e");
			char32_t *data = res.ptrw();
			for (int j = 0; j < res.size(); j++) {
				if (data[j] == num_systems[i].digits[10]) {
					data[j] = '.';
				} else {
					for (int k = 0; k < 10; k++) {
						if (data[j] == num_systems[i].digits[k]) {
							data[j] = 0x30 + k;
						}
					}
				}
			}
			break;
		}
	}
	return res;
}

String TextServerAdvanced::percent_sign(const String &p_language) const {
	String lang = p_language;

	for (int i = 0; num_systems[i].lang != String(); i++) {
		Vector<String> langs = num_systems[i].lang.split(",");
		if (langs.has(lang)) {
			if (num_systems[i].percent_sign == String()) {
				return "%";
			}
			return num_systems[i].percent_sign;
		}
	}
	return "%";
}

TextServer *TextServerAdvanced::create_func(Error &r_error, void *p_user_data) {
	r_error = OK;
	return memnew(TextServerAdvanced());
}

TextServerAdvanced::TextServerAdvanced() {
	hb_bmp_create_font_funcs();
	load_support_data("");
}

TextServerAdvanced::~TextServerAdvanced() {
	hb_bmp_free_font_funcs();
#ifdef MODULE_FREETYPE_ENABLED
	if (library != nullptr) {
		FT_Done_FreeType(library);
	}
#endif
	if(icu_data)
		u_cleanup();
#ifndef ICU_STATIC_DATA
	if (icu_data != nullptr) {
		memfree(icu_data);
		icu_data = nullptr;
	}
#endif
}

TextureRef::TextureRef(TextureRef&& other)
{
	handle= other.handle;
	other.handle = nullptr;
}

TextureRef::~TextureRef()
{
	if(!handle)
		return;
	OGUI::Context::Get().renderImpl->ReleaseTexture(handle);
}