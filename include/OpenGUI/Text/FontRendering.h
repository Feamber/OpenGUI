// Reference: https://github.com/rougier/freetype-gl/blob/master/texture-font.h
#pragma once
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/Containers/vector.hpp"
#include <memory>

namespace OGUI
{
    struct DynamicAtlasResource;
namespace Font
{
    enum ERenderMode
    {
        Normal,
        OutlineEdge,
        OutlinePositive,
        OutlineNegative,
        SDF
    };

    struct Kerning
    {
        /**
        * Left Unicode codepoint in the kern pair in UTF-32 LE encoding.
        */
        uint32_t codepoint;
        /**
        * Kerning value (in fractional pixels).
        */
        float kerning;
    };

    /*
    * Glyph metrics:
    * --------------
    *
    *                       xmin                     xmax
    *                        |                         |
    *                        |<-------- width -------->|
    *                        |                         |
    *              |         +-------------------------+----------------- ymax
    *              |         |    ggggggggg   ggggg    |     ^        ^
    *              |         |   g:::::::::ggg::::g    |     |        |
    *              |         |  g:::::::::::::::::g    |     |        |
    *              |         | g::::::ggggg::::::gg    |     |        |
    *              |         | g:::::g     g:::::g     |     |        |
    *    offset_x -|-------->| g:::::g     g:::::g     |  offset_y    |
    *              |         | g:::::g     g:::::g     |     |        |
    *              |         | g::::::g    g:::::g     |     |        |
    *              |         | g:::::::ggggg:::::g     |     |        |
    *              |         |  g::::::::::::::::g     |     |      height
    *              |         |   gg::::::::::::::g     |     |        |
    *  baseline ---*---------|---- gggggggg::::::g-----*--------      |
    *            / |         |             g:::::g     |              |
    *     origin   |         | gggggg      g:::::g     |              |
    *              |         | g:::::gg   gg:::::g     |              |
    *              |         |  g::::::ggg:::::::g     |              |
    *              |         |   gg:::::::::::::g      |              |
    *              |         |     ggg::::::ggg        |              |
    *              |         |         gggggg          |              v
    *              |         +-------------------------+----------------- ymin
    *              |                                   |
    *              |------------- advance_x ---------->|
    */

    /**
    * A structure that describe a glyph.
    */
    struct TextureGlyph
    {
        /**
        * Unicode codepoint this glyph represents in UTF-32 LE encoding.
        */
        uint32_t codepoint = -1;
        /**
        * Glyph's width in pixels.
        */
        size_t width = 0;
        /**
        * Glyph's height in pixels.
        */
        size_t height = 0;
        /**
        * Glyph's left bearing expressed in integer pixels.
        */
        int32_t offset_x = 0;
        /**
        * Glyphs's top bearing expressed in integer pixels.
        *
        * Remember that this is the distance from the baseline to the top-most
        * glyph scanline, upwards y coordinates being positive.
        */
        int32_t offset_y = 0;
        /**
        * For horizontal text layouts, this is the horizontal distance (in
        * fractional pixels) used to increment the pen position when the glyph is
        * drawn as part of a string of text.
        */
        float32_t advance_x = 0.f;
        /**
        * For vertical text layouts, this is the vertical distance (in fractional
        * pixels) used to increment the pen position when the glyph is drawn as
        * part of a string of text.
        */
        float32_t advance_y = 0.f;
        /**
        * First normalized texture coordinate (x) of top-left corner
        */
        float32_t s0 = 0.f;
        /**
        * Second normalized texture coordinate (y) of top-left corner
        */
        float32_t t0 = 0.f;
        /**
        * First normalized texture coordinate (x) of bottom-right corner
        */
        float32_t s1 = 0.f;
        /**
        * Second normalized texture coordinate (y) of bottom-right corner
        */
        float32_t t1 = 0.f;
        /**
        * A vector of kerning pairs relative to this glyph.
        */
        OGUI::vector<Kerning> kerning;
        /**
        * Mode this glyph was rendered
        */
        ERenderMode render_mode = ERenderMode::Normal;
        /**
        * Glyph outline thickness
        */
        float32_t outline_thickness = 0.f;
    };

    struct OGUI_API TextureFont
    {
    public:
        static TextureFont* TextureFontFromFile(
            const uint32_t atlas_width, const uint32_t atlas_height,
            const PixelFormat atlas_format,
            const char* filename, const float32_t pt_size);
        ~TextureFont();
        //static TextureFont* TextureFontFromMemory(MemoryResource resource, float32_t pt_size);
        TextureGlyph* CreateGlyph(const char* codepoint_u8);
        TextureGlyph* FindGlyph(const char* codepoint_u8); 
        FORCEINLINE const DynamicAtlasResource* GetAtlas() const { return atlas.get(); }
    protected:
        bool Initialize(void);
        bool LoadGlyph(const char* codepoint_u8);
    public:
        OGUI::vector<std::unique_ptr<TextureGlyph>> glyphs;
        std::unique_ptr<DynamicAtlasResource> atlas = nullptr;
        enum {
            TEXTURE_FONT_FILE = 0,
            TEXTURE_FONT_MEMORY
        } location;
        char* filename;
        MemoryResource memory;

        /**
        * font size
        */
        float32_t size;
        /**
        * Whether to use autohint when rendering font
        */
        int32_t hinting;
        ERenderMode render_mode;
        float32_t outline_thickness;
        /**
        * whether to use our own lcd filter.
        */
        int32_t filtering;
        /**
        * LCD filter weights
        */
        uint8_t lcd_weights[5];
        /**
        * Whether to use kerning if available
        */
        int32_t kerning;
        /**
        * This field is simply used to compute a default line spacing (i.e., the
        * baseline-to-baseline distance) when writing text with this font. Note
        * that it usually is larger than the sum of the ascender and descender
        * taken as absolute values. There is also no guarantee that no glyphs
        * extend above or below subsequent baselines when using this distance.
        */
        float32_t height;
        /**
        * This field is the distance that must be placed between two lines of
        * text. The baseline-to-baseline distance should be computed as:
        * ascender - descender + linegap
        */
        float32_t linegap;
        /**
        * The ascender is the vertical distance from the horizontal baseline to
        * the highest 'character' coordinate in a font face. Unfortunately, font
        * formats define the ascender differently. For some, it represents the
        * ascent of all capital latin characters (without accents), for others it
        * is the ascent of the highest accented character, and finally, other
        * formats define it as being equal to bbox.yMax.
        */
        float32_t ascender;
        /**
        * The descender is the vertical distance from the horizontal baseline to
        * the lowest 'character' coordinate in a font face. Unfortunately, font
        * formats define the descender differently. For some, it represents the
        * descent of all capital latin characters (without accents), for others it
        * is the ascent of the lowest accented character, and finally, other
        * formats define it as being equal to bbox.yMin. This field is negative
        * for values below the baseline.
        */
        float32_t descender;
        /**
        * The position of the underline line for this face. It is the center of
        * the underlining stem. Only relevant for scalable formats.
        */
        float32_t underline_position;
        /**
        * The thickness of the underline for this face. Only relevant for scalable
        * formats.
        */
        float32_t underline_thickness;
        /**
        * The padding to be add to the glyph's texture that are loaded by this font.
        * Usefull when adding effects with shaders.
        */
        int32_t padding;
    };
}
}
