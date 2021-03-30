#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace OGUI
{
    class IOThread;
    class AsyncRenderTexture;
    class AsyncBitmap;

    class RenderTextureManager
    {
    public:
        std::shared_ptr<AsyncRenderTexture> Require(
            const std::string& url, bool sync = false,
            std::shared_ptr<AsyncBitmap>* bmOut = nullptr);
        void Update();
    protected:
        std::unordered_map<std::string, std::weak_ptr<AsyncRenderTexture>> 
            render_textures;
        std::unordered_map<std::string, std::shared_ptr<AsyncBitmap>> files;        
    };
}