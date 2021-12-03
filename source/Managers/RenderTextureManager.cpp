
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/AsyncBitmap.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Core/IOThread.h"

namespace OGUI
{
using namespace std;

shared_ptr<AsyncRenderTexture> RenderTextureManager::RequireFromFileSystem(
    const std::string& url, bool sync, shared_ptr<AsyncBitmap>* bmOut)
{
    auto iter = render_textures.find(url);
    const bool needUpload = iter == render_textures.end()  || iter->second.expired();
    auto& ctx = Context::Get();
    std::shared_ptr<AsyncRenderTexture> tex_locked;
    if(needUpload)
    {
        tex_locked = std::make_shared<AsyncRenderTexture>(
            std::make_unique<AsyncTexture2D>(), ERenderTextureType::RawTexture
        );
        render_textures[url] = tex_locked;
    } else {
        tex_locked = iter->second.lock();
    }
    if(bmOut || needUpload)
    {
        if(files.find(url) == files.end())
        {
            files[url] = ctx.ioThread->template Load<AsyncBitmap>(url.c_str());
        }
        if(bmOut)
        {
            *bmOut = files[url];
        }
    }

    while(sync && !tex_locked->valid()) {
        // Wait until complete
    }
    return tex_locked;
}

void RenderTextureManager::Update()
{
    // compile bitmaps to render_textures.
    std::vector<std::string> uploadeds;
    auto& ctx = Context::Get();
    for(auto&& [url, file] : files)
    {
        if(file != nullptr && file->valid())
        {
            auto tex_locked = render_textures[url].lock();
            if(tex_locked)
            {
                // comple/upload texture to render device.
                tex_locked->device_image->_handle = ctx.renderImpl->RegisterTexture(file->GetBitmap());
                tex_locked->device_image->is_ready = true;
            }
            file.reset();
            uploadeds.push_back(url);
        }
    }
    for (auto& uploaded : uploadeds)
        files.erase(uploaded);
}

}