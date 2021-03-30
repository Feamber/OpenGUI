#pragma once
#include "AsyncFile.h"

namespace OGUI
{
    class AsyncBitmap final : public AsyncFile
    {
    public:
        void Initialize(const char* path) final;
        void Finalize() final;
        FORCEINLINE const Bitmap GetBitmap() const
        {
            return bm;
        }
        inline size_t size() const final {return bm.size_in_bytes;}
    protected:
        Bitmap bm;
    };
}