#define DLL_IMPLEMENTATION
#include <stdlib.h>
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/DynamicAtlasResource.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Core/IOThread.h"
#include "OpenGUI/Core/AsyncBitmap.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
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
        Verbose(msg.to_sv());\
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
    return ::fopen(path, "rb");
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

// DynamicAtlas
inline static uint32_t size_in_bytes(OGUI::PixelFormat format)
{
    switch(format)
    {
    case OGUI::PixelFormat::PF_R8G8B8A8: return 4;
    case OGUI::PixelFormat::PF_R16G16B16A16: return 8;
    case OGUI::PixelFormat::PF_R8G8B8A8_SRGB: return 4;
    case OGUI::PixelFormat::PF_R8: return 1;
    case OGUI::PixelFormat::PF_R16: return 2;
    case OGUI::PixelFormat::PF_R32: return 4;
    default: return -1;
    }
}

const uint32_t DynamicAtlasResource::depth() const
{
    return size_in_bytes(bitmap.format);
}

OGUI::DynamicAtlasResource::~DynamicAtlasResource()
{
    OASSERT(bitmap.resource.bytes);
    free(bitmap.resource.bytes);
}

OGUI::DynamicAtlasResource::DynamicAtlasResource()
{

}

OGUI::DynamicAtlasResource* DynamicAtlasResource::Create(const uint32_t width, const uint32_t height, const PixelFormat format)
{
    DynamicAtlasResource* self = new DynamicAtlasResource();
    Vector3u node = Vector3u(1, 1, width - 2);
    self->nodes = { node }; self->used_surface_size = 0u;
    
    const auto depth = size_in_bytes(format);
    OASSERT(self && "Create Atlas Resource Failed!");
    OASSERT((depth == 1) || (depth == 3) || (depth == 4));
    
    auto& bitmap = self->bitmap;
    bitmap.width = width; bitmap.height = height; bitmap.format = format;
    bitmap.resource.bytes = (uint8_t*)calloc(width * height * depth, sizeof(uint8_t));
    bitmap.resource.size_in_bytes = width * height * depth;
    OASSERT(bitmap.resource.bytes);
    
    return self;
}

void DynamicAtlasResource::SetRegion(const uint32_t x, const uint32_t y, const uint32_t width,
    const uint32_t height, const uint8_t* data, const size_t stride)
{
    uint32_t depth, charsize;
    OASSERT(x > 0); OASSERT(y > 0); 
    OASSERT(x < (bitmap.width - 1)); 
    OASSERT((x + width) <= (bitmap.width - 1));
    OASSERT(y < (bitmap.height-1));
    OASSERT((y + height) <= (bitmap.height - 1));
    OASSERT(height == 0 || (data != NULL && width > 0));

    depth = size_in_bytes(bitmap.format);
    charsize = sizeof(uint8_t);
    for(uint32_t i = 0u; i < height; i++)
    {
        memcpy(bitmap.resource.bytes + ( (y+i) * bitmap.width + x ) * charsize * depth,
            data + (i * stride) * charsize,
            width * charsize * depth  
        );
    }
}

void OGUI::DynamicAtlasResource::Merge()
{
    for(auto i = 0u; i < nodes.size() - 1; i++)
    {
        auto& node = nodes[i];
        auto& next = nodes[i + 1];

        if(node.y == next.y)
        {
            node.z += next.z;
            next = Vector3u(0u, 0u, 0u);
        }
    }
    nodes.erase(std::remove(nodes.begin(),nodes.end(),Vector3u(0, 0, 0)), nodes.end());
}

int DynamicAtlasResource::Fit(const size_t index, const uint32_t width, const uint32_t height)
{
    auto* node = &nodes[index];
    int x = node->x;
    int y = node->y;
    int width_left = width;
    auto i = index;
    if ( (x + width) > (bitmap.width - 1) )
    {
        return -1;
    } 
    y = node->y;
    while( width_left > 0 )
    {
        node = &nodes[i];
        if(node->y > y) y = node->y;
        if((y + height) > (bitmap.height - 1)) return -1;
        width_left -= node->z; i++;
    }
    return y;
}

OGUI::Vector4i DynamicAtlasResource::AllocateRegion(const uint32_t width, const uint32_t height)
{
    int y, best_index;
    uint32_t best_width = UINT32_MAX;
    uint32_t best_height = UINT32_MAX;
    Vector3u* node = nullptr;
    Vector3u* prev = nullptr;

    OGUI::Vector4i region = {{0,0,width,height}};
    best_index  = -1;

    for(size_t i = 0u; i < nodes.size(); i++)
    {
        y = Fit(i, width, height);
        if (y >= 0)
        {
            node = &nodes[i];
            if( ((y + height) < best_height) || 
                (((y + height) == best_height) && (node->z > 0 && (size_t)node->z < best_width)) 
            )
            {
                best_height = y + height;
                best_index = i;
                best_width = node->z;
                region[0] = node->x;
                region[1] = y;
            }
        }
    }

    if( best_index == -1 || !node)
    {
        return Vector4i::vector_zero();
    }

    Vector3u new_node = Vector3u();
    OASSERT(node && "No more memory for allocating data");
    new_node.x = region[0];
    new_node.y = region[1] + height;
    new_node.z = width;
    nodes.insert(nodes.begin() + best_index, new_node);

    for(auto i = best_index + 1; i < nodes.size(); ++i)
    {
        node = &nodes[i]; 
        prev = &nodes[i - 1];

        if (node->x < (prev->x + prev->z) )
        {
            int shrink = prev->x + prev->z - node->x;
            node->x += shrink;
            node->z -= shrink;
            if (node->z <= 0)
            {
                nodes[i] = Vector3u(0u, 0u, 0u);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    nodes.erase(std::remove(nodes.begin(),nodes.end(),Vector3u(0, 0, 0)), nodes.end());

    Merge();
    used_surface_size += width * height;

    return region;
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
void AsyncBitmap::Initialize(const char* path) 
{
    auto& ctx = Context::Get();
    auto f = ctx.fileImpl->Open(path);
    bm = ctx.bmParserImpl->LoadFromFile(f);
    ctx.fileImpl->Close(f);
}

void AsyncBitmap::Finalize() 
{
    auto& ctx = Context::Get();
    ctx.bmParserImpl->Free(bm);
}

OGUI::LogInterface::~LogInterface()
{
}

OGUI::AsyncImage::~AsyncImage()
{
    auto& ctx = Context::Get();
    ctx.renderImpl->ReleaseTexture(_handle); // Release from RenderDevice.
}

OGUI::AsyncRenderTexture::AsyncRenderTexture(std::shared_ptr<AsyncImage> image_handle, ERenderTextureType type)
    :texture_type(type)
{
    device_image = image_handle;
}

OGUI::AsyncRenderTexture::~AsyncRenderTexture()
{
    device_image.reset();
}

}