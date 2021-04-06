#pragma once
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/AsyncFile.h"

namespace OGUI
{
    // Reference: https://github.com/rougier/freetype-gl
	struct OGUI_API DynamicAtlasResource
	{
    public:
		static DynamicAtlasResource* Create(
            const uint32_t width, const uint32_t height, const PixelFormat format);
        Vector4i AllocateRegion(const uint32_t width, const uint32_t height);
        void SetRegion(const uint32_t x, const uint32_t y, const uint32_t width,
            const uint32_t height, const uint8_t* data, const size_t stride);
        void Clear();
        inline const Bitmap GetBitmap() const { return bitmap; }
        ~DynamicAtlasResource();
        FORCEINLINE const uint32_t width() const { return bitmap.width; }
        FORCEINLINE const uint32_t height() const { return bitmap.height; }
        FORCEINLINE const PixelFormat format() const { return bitmap.format; }
        const uint32_t depth() const;

    protected:
        void Merge();
        int Fit(const size_t index, const uint32_t width, const uint32_t height);
        DynamicAtlasResource();
        std::vector<Vector3u> nodes;
		Bitmap bitmap;
        uint32_t used_surface_size = 0;
	};
    
    /*
    struct OGUI_API AsyncAtlasMemoryFile : public AsyncFile
    {
    public:

    protected:

    };
    */
}