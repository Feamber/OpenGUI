#pragma once
#include "AsyncFile.h"

namespace OGUI
{
    class OGUI_API AsyncBitmap final : public AsyncFile
    {
    public:
        void Initialize(const char* path) final;
        void Finalize() final;
        FORCEINLINE const Bitmap GetBitmap() const
        {
            return bitmap;
        }
        inline size_t size() const final {return bitmap.resource.size_in_bytes;}
    protected:
        Bitmap bitmap;
    };
}