#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace OGUI
{
    class IOThread;
    class AsyncRenderTexture;
    class AsyncBitmap;
    class WindowContext;

    class RenderTextureManager
    {
    public:
        WindowContext& windowContext;
        RenderTextureManager(WindowContext& windowContext)
            :windowContext(windowContext)
        {}
        std::shared_ptr<AsyncRenderTexture> RequireFromFileSystem(
            const std::string& url, bool sync = false,
            std::shared_ptr<AsyncBitmap>* bmOut = nullptr);

        void Update();
    protected:
        std::unordered_map<std::string, std::weak_ptr<AsyncRenderTexture>> 
            render_textures;
        std::unordered_map<std::string, std::shared_ptr<AsyncBitmap>> files;        
    };
}