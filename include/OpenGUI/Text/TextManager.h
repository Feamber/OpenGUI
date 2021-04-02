//reference: https://github1s.com/godotengine/godot/blob/HEAD/modules/text_server_adv/text_server_adv.h

namespace OGUI::Text
{
	enum Direction {
		DIRECTION_AUTO,
		DIRECTION_LTR,
		DIRECTION_RTL
	};

	enum Orientation {
		ORIENTATION_HORIZONTAL,
		ORIENTATION_VERTICAL
	};

	enum JustificationFlag {
		JUSTIFICATION_NONE = 0,
		JUSTIFICATION_KASHIDA = 1 << 0,
		JUSTIFICATION_WORD_BOUND = 1 << 1,
		JUSTIFICATION_TRIM_EDGE_SPACES = 1 << 2,
		JUSTIFICATION_AFTER_LAST_TAB = 1 << 3
	};

	enum LineBreakFlag {
		BREAK_NONE = 0,
		BREAK_MANDATORY = 1 << 4,
		BREAK_WORD_BOUND = 1 << 5,
		BREAK_GRAPHEME_BOUND = 1 << 6
		//RESERVED = 1 << 7
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
		GRAPHEME_IS_PUNCTUATION = 1 << 8 // Punctuation (can be used as word break, but not line break or justifiction).
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
    
    struct TextServer
    {
    }
}
