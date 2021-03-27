#pragma once
#include "OpenGUI/Core/Types.h"
#include <atomic>

namespace OGUI
{
    class IOThread;
    struct AsyncFile
    {
        friend class IOThread;
        virtual ~AsyncFile();
        virtual void Finalize() = 0;
        virtual void Initialize(const char* path) = 0;
        virtual size_t size() const = 0;
        virtual bool valid() const;
    protected:
        inline void __initialize(const char* path)
        {
            this->Initialize(path);
            is_ready = true;
        }
        std::atomic_bool is_ready = false;
    };  
}