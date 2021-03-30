#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace OGUI
{
    class IOThread;
    class TextureInterface;
    class AsyncBitmap;

    class RenderTextureManager
    {
    public:
        std::shared_ptr<TextureInterface> Require(
            const char* url, bool sync = false,
            std::shared_ptr<AsyncBitmap>* bmOut = nullptr);
    protected:
        std::unordered_map<std::string, std::weak_ptr<TextureInterface>> 
            render_textures;
        std::unordered_map<std::string, std::weak_ptr<AsyncBitmap>> loading;        
    };
}