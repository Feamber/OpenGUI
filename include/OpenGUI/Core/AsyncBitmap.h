#pragma once
#include "AsyncFile.h"

namespace OGUI
{
    class AsyncBitmap final : public AsyncFile
    {
    public:
        void Initialize(const char* path) final;
        void Finalize() final;
        FORCEINLINE const BitMap GetBitmap() const
        {
            BitMap result = {};
            result.bytes = storage.data;
            result.bytes_size = storage.size_in_bytes;
            result.width = width;
            result.height = height;
            result.format = format;
            return result;
        }
        inline size_t size() const final {return storage.size_in_bytes;}
        inline const void* data() const {return storage.data;}
        inline void* data() {return storage.data;}
    protected:
        MemoryResource storage;
        uint32_t width, height;
        PixelFormat format;
    };
}