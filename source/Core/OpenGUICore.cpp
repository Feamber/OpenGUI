
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Core/IOThread.h"
#include "OpenGUI/Core/AsyncBitmap.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/StdLog.h"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Context.h"

namespace OGUI
{
#define __MAKE_LOG_CALL_IMPL(func)\
    void olog::func(ostr::string_view msg)\
    {\
        Context::Get().logImpl->Log(olog::Level::func, msg);\
    }\
    void olog::func(const ostr::string& msg) \
    {\
        func(msg.to_sv());\
    }

    __MAKE_LOG_CALL_IMPL(Verbose)
    __MAKE_LOG_CALL_IMPL(Debug)
    __MAKE_LOG_CALL_IMPL(Info)
    __MAKE_LOG_CALL_IMPL(Warn)
    __MAKE_LOG_CALL_IMPL(Error)
    __MAKE_LOG_CALL_IMPL(Fatal)

#undef __MAKE_LOG_CALL_IMPL
}


namespace OGUI
{
RenderInterface::~RenderInterface()
{
    
}

InputInterface::~InputInterface()
{

}

SystemInterface::~SystemInterface()
{

}

FileInterface::~FileInterface()
{

}

BitmapParserInterface::~BitmapParserInterface()
{

}

FileHandle FileInterface::Open(const char* path)
{
    return fopen(path, "rb");
}

void FileInterface::Close(FileHandle file)
{
    fclose((FILE*) file);
}

size_t FileInterface::Read(void* buffer, size_t size, FileHandle file)
{
    return fread(buffer, 1, size, (FILE*) file);
}

int FileInterface::Seek(FileHandle file, size_t offset, int fromwhere)
{
    return fseek((FILE*)file, offset, fromwhere);
}

size_t FileInterface::Tell(const FileHandle file)
{
    return ftell((FILE*) file);
}

size_t FileInterface::Length(const FileHandle file)
{
    size_t current_position = Tell(file);
    Seek( file, 0, SEEK_END);
    size_t length = Tell( file);
    Seek( file, (long)current_position, SEEK_SET);
    return length;
}

MemoryResource FileInterface::Load(const char* path)
{
    MemoryResource result = {};
    FileHandle f = this->Open(path);
    this->Seek(f, 0l, SEEK_END);
    result.size_in_bytes = this->Tell(f);
    result.bytes = (uint8_t*)::malloc(result.size_in_bytes);
    this->Seek(f, 0l, SEEK_SET);
    this->Read(result.bytes, result.size_in_bytes, f);
    this->Close(f);
    return result;
}

// Async File
OGUI::AsyncFile::~AsyncFile()
{
    
}

bool AsyncFile::valid() const
{
    return is_ready;
}

IOThread::IOThread()
    :is_running(true)
{
    loader_thread = std::thread(&IOThread::loaderThreadFunction, this);
}

IOThread::~IOThread()   
{
    is_running = false;
    if(loader_thread.joinable())
        loader_thread.join();
}

void IOThread::loaderThreadFunction()
{
    while(is_running)
    {
        load_queue_mutex.lock();
        std::vector<FileLoaderTask> tasks = load_queue;
        load_queue.clear();
        load_queue_mutex.unlock();

        for (FileLoaderTask& task : tasks) {
            task.file->__initialize(task.path.c_str());
            task.complete_callback(task.file);
        }

        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }
}

// AsyncBitmap
bool AsyncBitmap::Initialize(const char* path) 
{
    auto& ctx = Context::Get();
    auto f = ctx.fileImpl->Open(path);
    if(f)
    {
        bitmap = ctx.bmParserImpl->LoadFromFile(f);
        ctx.fileImpl->Close(f);
        return true;
    }
    return false;
}

void AsyncBitmap::Finalize() 
{
    if(bitmap.resource.bytes)
    {
        auto& ctx = Context::Get();
        ctx.bmParserImpl->Free(bitmap);
    }
}

// AsyncBlob
bool AsyncBlob::Initialize(const char* path) 
{
    auto& ctx = Context::Get();
    resource = ctx.fileImpl->Load(path);
    return true;
}

void AsyncBlob::Finalize() 
{
    auto& ctx = Context::Get();
    ::free(resource.bytes);
}

OGUI::LogInterface::~LogInterface()
{
}

OGUI::AsyncTexture2D::~AsyncTexture2D()
{
    auto&& ctx = Context::Get();
    ctx.renderImpl->ReleaseTexture(_handle); 
}

OGUI::AsyncRenderTexture::AsyncRenderTexture(std::unique_ptr<AsyncTexture2D> image_handle, ERenderTextureType type)
    :texture_type(type)
{
    device_image = std::move(image_handle);
}

OGUI::AsyncRenderTexture::~AsyncRenderTexture()
{
    
}

StdOutputLog::~StdOutputLog()
{}

void StdOutputLog::Log(olog::Level l, ostr::string_view msg)
{
    const std::string verbosity =
    [l]
    {
        switch(l)
        {
            case olog::Level::Verbose :
            return "Verbose";
            case olog::Level::Debug :
            return "Debug";
            case olog::Level::Info :
            return "Info";
            case olog::Level::Warn :
            return "Warn";
            case olog::Level::Error :
            return "Error";
            case olog::Level::Fatal :
            return "Fatal";
            default: break;
        }
        return "Unknown";
    }();

    std::string str;
    msg.encode_to_utf8(str);
    std::cout << "<" << verbosity << "> " << str << std::endl;
}

}