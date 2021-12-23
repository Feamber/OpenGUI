#pragma once
#include "OpenGUI/Core/Types.h"
#include <atomic>

namespace OGUI
{
    class IOThread;
    struct OGUI_API AsyncFile
    {
        friend class IOThread;
        virtual ~AsyncFile();
        virtual void Finalize() = 0;
        virtual bool Initialize(const char* path) = 0;
        virtual size_t size() const = 0;
        virtual bool valid() const;
    protected:
        inline void __initialize(const char* path)
        {
            is_ready = this->Initialize(path);
        }
        std::atomic_bool is_ready = false;
    };  

    struct OGUI_API AsyncBlob : public AsyncFile
    {
    public:
        bool Initialize(const char* path) final;
        void Finalize() final;
        FORCEINLINE const MemoryResource& GetResource() const
        {
            return resource;
        }
        FORCEINLINE MemoryResource& GetResource() 
        {
            return resource;
        }
        inline size_t size() const final {return resource.size_in_bytes;}
    protected:
        MemoryResource resource;
    };
}