#define DLL_IMPLEMENTATION
#include "OpenGUI/Text/FontRendering.h"
#include "OpenGUI/Core/DynamicAtlasResource.h"
#include "OpenGUI/Core/olog.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H
#include <stdint.h>

#define HRES  64
#define HRESf 64.f
#define DPI   72

static float convert_F26Dot6_to_float(FT_F26Dot6 value)
{
  return ((float)value) / 64.0;
}
static FT_F26Dot6 convert_float_to_F26Dot6(float value)
{
  return (FT_F26Dot6) (value * 64.0);
}

using namespace OGUI;
using namespace OGUI::Font;

static uint32_t
utf8_to_utf32( const char * character )
{
    uint32_t result = -1;

    if( !character )
    {
        return result;
    }

    if( ( character[0] & 0x80 ) == 0x0 )
    {
        result = character[0];
    }

    if( ( character[0] & 0xC0 ) == 0xC0 )
    {
        result = ( ( character[0] & 0x3F ) << 6 ) | ( character[1] & 0x3F );
    }

    if( ( character[0] & 0xE0 ) == 0xE0 )
    {
        result = ( ( character[0] & 0x1F ) << ( 6 + 6 ) ) | ( ( character[1] & 0x3F ) << 6 ) | ( character[2] & 0x3F );
    }

    if( ( character[0] & 0xF0 ) == 0xF0 )
    {
        result = ( ( character[0] & 0x0F ) << ( 6 + 6 + 6 ) ) | ( ( character[1] & 0x3F ) << ( 6 + 6 ) ) | ( ( character[2] & 0x3F ) << 6 ) | ( character[3] & 0x3F );
    }

    if( ( character[0] & 0xF8 ) == 0xF8 )
    {
        result = ( ( character[0] & 0x07 ) << ( 6 + 6 + 6 + 6 ) ) | ( ( character[1] & 0x3F ) << ( 6 + 6 + 6 ) ) | ( ( character[2] & 0x3F ) << ( 6 + 6 ) ) | ( ( character[3] & 0x3F ) << 6 ) | ( character[4] & 0x3F );
    }

    return result;
}


// ---------------------------------------------------- texture_font_delete ---
TextureFont::~TextureFont()
{
    TextureGlyph *glyph;

    if(location == OGUI::Font::TextureFont::TEXTURE_FONT_FILE && filename)
        free(filename);
}


TextureGlyph* TextureFont::FindGlyph(const char * codepoint )
{
    TextureGlyph *glyph;
    uint32_t ucodepoint = utf8_to_utf32( codepoint );

    for(size_t i = 0u; i < glyphs.size(); i++)
    {
        glyph = glyphs[i].get();
        // If codepoint is -1, we don't care about outline type or thickness
        if( (glyph->codepoint == ucodepoint) &&
            ((ucodepoint == -1) ||
             ((glyph->render_mode == render_mode) &&
              (glyph->outline_thickness == outline_thickness)) ))
        {
            return glyph;
        }
    }
    return nullptr;
}


// ------------------------------------------------- texture_font_load_face ---
static bool
texture_font_load_face(TextureFont *self, float size,
        FT_Library *library, FT_Face *face)
{
    FT_Error error;
    FT_Matrix matrix = {
        (int)((1.0/HRES) * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((1.0)      * 0x10000L)};

    OASSERT(library);
    OASSERT(size);

    /* Initialize library */
    error = FT_Init_FreeType(library);
    if( error )
    {
        fprintf( stderr, "FT_Error (line %d, 0x%02x) : %s\n",
            __LINE__, error, FT_Error_String(error) 
        );
        goto cleanup;
    }

    /* Load face */
    switch (self->location) 
    {
    case OGUI::Font::TextureFont::TEXTURE_FONT_FILE:
        error = FT_New_Face(*library, self->filename, 0, face);
        break;

    case OGUI::Font::TextureFont::TEXTURE_FONT_MEMORY:
        error = FT_New_Memory_Face(*library,
            self->memory.bytes, self->memory.size_in_bytes, 0, face);
        break;
    }

    if( error )
    {
        fprintf( stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
            __LINE__, error, FT_Error_String(error) 
        );
        goto cleanup_library;
    }

    /* Select charmap */
    error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
    if( error )
    {
        fprintf( stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
            __LINE__, error, FT_Error_String(error) 
        );
        goto cleanup_face;
    }

    /* Set char size */
    /* See page 24 of “Higher Quality 2D Text Rendering”:
     * http://jcgt.org/published/0002/01/04/
     * “To render high-quality text, Shemarev [2007] recommends using only
     *  vertical hinting and completely discarding the horizontal hints.
     *  Hinting is the responsibility of the rasterization engine (FreeType in
     *  our case) which provides no option to specifically discard horizontal
     *  hinting. In the case of the FreeType library, we can nonetheless trick
     *  the engine by specifying an oversized horizontal DPI (100 times the
     *  vertical) while specifying a transformation matrix that scale down the
     *  glyph as shown in Listing 1.”
     * That horizontal DPI factor is HRES here. */
    error = FT_Set_Char_Size(*face, convert_float_to_F26Dot6(size), 0, DPI * HRES, DPI);

    if( error )
    {
        fprintf( stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
            __LINE__, error, FT_Error_String(error) 
        );
        goto cleanup_face;
    }

    /* Set transform matrix */
    FT_Set_Transform(*face, &matrix, NULL);

    return true;

cleanup_face:
    FT_Done_Face( *face );
cleanup_library:
    FT_Done_FreeType( *library );
cleanup:
    return false;
}

// ------------------------------------------ texture_font_generate_kerning ---
static void texture_font_generate_kerning(TextureFont* self, FT_Library *library, FT_Face *face)
{
    FT_UInt glyph_index, prev_index;
    FT_Vector kerning;

    /* For each glyph couple combination, check if kerning is necessary */
    /* Starts at index 1 since 0 is for the special backgroudn glyph */
    for(size_t i = 1u; i < self->glyphs.size(); i++)
    {
        TextureGlyph* glyph = self->glyphs[i].get();
        TextureGlyph* prev_glyph = nullptr;
        glyph_index = FT_Get_Char_Index( *face, glyph->codepoint );
        glyph->kerning.clear();

        for(size_t j = 1u; j < self->glyphs.size(); j++)
        {
            prev_glyph = self->glyphs[j].get();
            prev_index = FT_Get_Char_Index( *face, prev_glyph->codepoint );
            // FT_KERNING_UNFITTED returns FT_F26Dot6 values.
            FT_Get_Kerning( *face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );
            // printf("%c(%d)-%c(%d): %ld\n",
            //       prev_glyph->codepoint, prev_glyph->codepoint,
            //       glyph_index, glyph_index, kerning.x);
            if( kerning.x )
            {
                Kerning k = {
                  prev_glyph->codepoint,
                  convert_F26Dot6_to_float(kerning.x) / HRESf
                };
                glyph->kerning.emplace_back(k);
            }
        }
    }
}

// ------------------------------------------------------ texture_font_init ---
bool TextureFont::Initialize(void)
{
    FT_Library library;
    FT_Face face;
    FT_Size_Metrics metrics;

    auto self = this;
    OASSERT(self->atlas);
    OASSERT(self->size > 0);
    OASSERT(
        (self->location == OGUI::Font::TextureFont::TEXTURE_FONT_FILE && self->filename)
        || (self->location == OGUI::Font::TextureFont::TEXTURE_FONT_MEMORY
        && self->memory.bytes && self->memory.size_in_bytes)
    );

    self->height = 0;
    self->ascender = 0;
    self->descender = 0;
    self->render_mode = ERenderMode::Normal;
    self->outline_thickness = 0.0;
    self->hinting = 1;
    self->kerning = 1;
    self->filtering = 1;

    // FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
    // FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
    self->lcd_weights[0] = 0x10;
    self->lcd_weights[1] = 0x40;
    self->lcd_weights[2] = 0x70;
    self->lcd_weights[3] = 0x40;
    self->lcd_weights[4] = 0x10;

    if (!texture_font_load_face(self, self->size, &library, &face))
        return false;

    self->underline_position = face->underline_position / (float)(HRESf * HRESf) * self->size;
    self->underline_position = roundf( self->underline_position );
    if( self->underline_position > -2 )
    {
        self->underline_position = -2.0;
    }

    self->underline_thickness = face->underline_thickness / (float)(HRESf * HRESf) * self->size;
    self->underline_thickness = roundf( self->underline_thickness );
    if( self->underline_thickness < 1 )
    {
        self->underline_thickness = 1.0;
    }
    metrics = face->size->metrics;
    self->ascender  = metrics.ascender  >> 6;
    self->descender = metrics.descender >> 6;
    self->height    = metrics.height    >> 6;
    self->linegap = self->height - self->ascender + self->descender;
    FT_Done_Face( face );
    FT_Done_FreeType( library );

    /* NULL is a special glyph */
    self->CreateGlyph( nullptr );
    return true;
}

bool TextureFont::LoadGlyph(const char* codepoint)
{
    size_t i, x, y;
    FT_Library library;
    FT_Error error;
    FT_Face face;
    FT_Glyph ft_glyph;
    FT_GlyphSlot slot;
    FT_Bitmap ft_bitmap;
    auto self = this;

    FT_UInt glyph_index;
    TextureGlyph *glyph;
    FT_Int32 flags = 0;
    int ft_glyph_top = 0;
    int ft_glyph_left = 0;

    Vector4i region;
    size_t missed = 0;


    if (!texture_font_load_face(self, self->size, &library, &face))
        return false;

    /* Check if codepoint has been already loaded */
    if (FindGlyph(codepoint)) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return true;
    }

    /* codepoint NULL is special : it is used for line drawing (overline,
     * underline, strikethrough) and background.
     */
    if( !codepoint )
    {
        Vector4i region = atlas->AllocateRegion(5u, 5u);
        TextureGlyph* glyph = new TextureGlyph();
        static unsigned char data[4*4*3] = 
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
        if ( region[0] < 0 )
        {
            fprintf( stderr, "Texture atlas is full (line %d)\n",  __LINE__ );
            FT_Done_Face( face );
            FT_Done_FreeType( library );
            return 0;
        }
        atlas->SetRegion(region[0], region[1], 4, 4, data, 0);
        glyph->codepoint = -1;
        glyph->s0 = (region[0] + 2)/(float)atlas->width();
        glyph->t0 = (region[1] + 2)/(float)atlas->height();
        glyph->s1 = (region[0] + 3)/(float)atlas->width();
        glyph->t1 = (region[1] + 3)/(float)atlas->height();
        glyphs.emplace_back(glyph);

        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return true;
    }

    flags = 0;
    ft_glyph_top = 0;
    ft_glyph_left = 0;
    glyph_index = FT_Get_Char_Index( face, (FT_ULong)utf8_to_utf32( codepoint ) );
    // WARNING: We use texture-atlas depth to guess if user wants
    //          LCD subpixel rendering

    if( self->render_mode != ERenderMode::Normal && 
        self->render_mode != ERenderMode::SDF 
    )
    {
        flags |= FT_LOAD_NO_BITMAP;
    }
    else
    {
        flags |= FT_LOAD_RENDER;
    }

    if( !self->hinting )
    {
        flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
    }
    else
    {
        flags |= FT_LOAD_FORCE_AUTOHINT;
    }

    if( self->atlas->depth() == 3 )
    {
        FT_Library_SetLcdFilter( library, FT_LCD_FILTER_LIGHT );
        flags |= FT_LOAD_TARGET_LCD;

        if( self->filtering )
        {
            FT_Library_SetLcdFilterWeights( library, self->lcd_weights );
        }
    }
    else if (HRES == 1)
    {
        /* “FT_LOAD_TARGET_LIGHT
         *  A lighter hinting algorithm for gray-level modes. Many generated
         *  glyphs are fuzzier but better resemble their original shape.
         *  This is achieved by snapping glyphs to the pixel grid
         *  only vertically (Y-axis), as is done by FreeType's new CFF engine
         *  or Microsoft's ClearType font renderer.”
         * https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_load_target_xxx
         */
        flags |= FT_LOAD_TARGET_LIGHT;
    }

    error = FT_Load_Glyph( face, glyph_index, flags );
    if( error )
    {
        fprintf( stderr, "FT_Error (line %d, code 0x%02x) : %s\n",
                 __LINE__, error, FT_Error_String(error) );
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    if( self->render_mode == ERenderMode::Normal || self->render_mode == ERenderMode::SDF )
    {
        slot            = face->glyph;
        ft_bitmap       = slot->bitmap;
        ft_glyph_top    = slot->bitmap_top;
        ft_glyph_left   = slot->bitmap_left;
    }
    else
    {
        FT_Stroker stroker;
        FT_BitmapGlyph ft_bitmap_glyph;

        error = FT_Stroker_New( library, &stroker );

        if( error )
        {
            fprintf( stderr, "FT_Error (line %d, 0x%02x) : %s\n",
                     __LINE__, error, FT_Error_String(error) );
            goto cleanup_stroker;
        }

        FT_Stroker_Set(stroker,
                        (int)(self->outline_thickness * HRES),
                        FT_STROKER_LINECAP_ROUND,
                        FT_STROKER_LINEJOIN_ROUND,
                        0);

        error = FT_Get_Glyph( face->glyph, &ft_glyph);

        if( error )
        {
            fprintf( stderr, "FT_Error (line %d, 0x%02x) : %s\n",
                     __LINE__, error, FT_Error_String(error) );
            goto cleanup_stroker;
        }

        if( self->render_mode == ERenderMode::OutlineEdge )
            error = FT_Glyph_Stroke( &ft_glyph, stroker, 1 );
        else if ( self->render_mode == ERenderMode::OutlinePositive )
            error = FT_Glyph_StrokeBorder( &ft_glyph, stroker, 0, 1 );
        else if ( self->render_mode == ERenderMode::OutlinePositive )
            error = FT_Glyph_StrokeBorder( &ft_glyph, stroker, 1, 1 );

        if( error )
        {
            fprintf( stderr, "FT_Error (line %d, 0x%02x) : %s\n",
                     __LINE__, error, FT_Error_String(error) );
            goto cleanup_stroker;
        }

        if( self->atlas->depth() == 1 )
            error = FT_Glyph_To_Bitmap( &ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        else
            error = FT_Glyph_To_Bitmap( &ft_glyph, FT_RENDER_MODE_LCD, 0, 1);

        if( error )
        {
            fprintf( stderr, "FT_Error (line %d, 0x%02x) : %s\n",
                     __LINE__, error, FT_Error_String(error) );
            goto cleanup_stroker;
        }

        ft_bitmap_glyph = (FT_BitmapGlyph) ft_glyph;
        ft_bitmap       = ft_bitmap_glyph->bitmap;
        ft_glyph_top    = ft_bitmap_glyph->top;
        ft_glyph_left   = ft_bitmap_glyph->left;

cleanup_stroker:
        FT_Stroker_Done( stroker );

        if( error )
        {
            FT_Done_Face( face );
            FT_Done_FreeType( library );
            return 0;
        }
    }

    struct {
        int left;
        int top;
        int right;
        int bottom;
    } padding = { 0, 0, 1, 1 };

    if( self->render_mode == ERenderMode::SDF )
    {
        padding.top = 1;
        padding.left = 1;
    }

    if( self->padding != 0 )
    {
        padding.top += self->padding;
        padding.left += self->padding;
        padding.right += self->padding;
        padding.bottom += self->padding;
    }

    size_t src_w = ft_bitmap.width/self->atlas->depth();
    size_t src_h = ft_bitmap.rows;

    size_t tgt_w = src_w + padding.left + padding.right;
    size_t tgt_h = src_h + padding.top + padding.bottom;

    region = atlas->AllocateRegion( tgt_w, tgt_h );

    if ( region[0] < 0 )
    {
        fprintf( stderr, "Texture atlas is full (line %d)\n",  __LINE__ );
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    x = region[0];
    y = region[1];

    uint8_t* buffer = (uint8_t*)calloc( 
        tgt_w * tgt_h * self->atlas->depth(), sizeof(uint8_t) 
    );
    uint8_t* dst_ptr = buffer + (padding.top * tgt_w + padding.left) * self->atlas->depth();
    uint8_t* src_ptr = ft_bitmap.buffer;
    for( i = 0; i < src_h; i++ )
    {
        //difference between width and pitch: https://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#FT_Bitmap
        memcpy( dst_ptr, src_ptr, ft_bitmap.width);
        dst_ptr += tgt_w * self->atlas->depth();
        src_ptr += ft_bitmap.pitch;
    }

    if( self->render_mode == ERenderMode::SDF )
    {
        OASSERT(0);
        //unsigned char *sdf = make_distance_mapb( buffer, tgt_w, tgt_h );
        //free( buffer );
        //buffer = sdf;
    }
    atlas->SetRegion(x, y, tgt_w, tgt_h, buffer, tgt_w * atlas->depth());
    free( buffer );

    glyph = new TextureGlyph();
    glyph->codepoint = utf8_to_utf32( codepoint );
    glyph->width    = tgt_w;
    glyph->height   = tgt_h;
    glyph->render_mode = self->render_mode;
    glyph->outline_thickness = self->outline_thickness;
    glyph->offset_x = ft_glyph_left;
    glyph->offset_y = ft_glyph_top;
    glyph->s0       = x / (float)atlas->width();
    glyph->t0       = y/(float)atlas->height();
    glyph->s1       = (x + glyph->width)/(float)atlas->width();
    glyph->t1       = (y + glyph->height)/(float)atlas->height();

    // Discard hinting to get advance
    FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
    slot = face->glyph;
    glyph->advance_x = convert_F26Dot6_to_float(slot->advance.x);
    glyph->advance_y = convert_F26Dot6_to_float(slot->advance.y);
    self->glyphs.emplace_back(glyph);

    if( self->render_mode != ERenderMode::Normal && self->render_mode != ERenderMode::SDF )
        FT_Done_Glyph( ft_glyph );

    texture_font_generate_kerning( self, &library, &face );

    FT_Done_Face( face );
    FT_Done_FreeType( library );

    return 1;
}

TextureGlyph* TextureFont::CreateGlyph(const char * codepoint )
{
    TextureGlyph *glyph;

    assert( this->filename );
    assert( this->atlas );

    /* Check if codepoint has been already loaded */
    if(glyph = this->FindGlyph(codepoint);glyph)
        return glyph;

    /* Glyph has not been already loaded */
    if( LoadGlyph(codepoint) )
        return FindGlyph( codepoint );

    return nullptr;
}

TextureFont* TextureFont::TextureFontFromFile(
    const uint32_t atlas_width, const uint32_t atlas_height,
    const PixelFormat atlas_format,
    const char* filename, const float32_t pt_size)
{
    TextureFont* self = new TextureFont();
    OASSERT(self);
    self->atlas.reset(
        DynamicAtlasResource::Create(atlas_width, atlas_height, atlas_format)
    );
    self->size = pt_size;
    self->location = OGUI::Font::TextureFont::TEXTURE_FONT_FILE;
    self->filename = _strdup(filename);

    if(!self->Initialize())
    {
        delete self;
        return nullptr;
    }
    return self;
}