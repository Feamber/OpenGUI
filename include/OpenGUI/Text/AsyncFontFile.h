#ifndef UE4Runtime
#pragma once
#include "OpenGUI/Core/AsyncFile.h"
#include "FontRendering.h"

namespace OGUI
{
    class OGUI_API AsyncFontFile final : public AsyncFile
    {
    public:
        void Initialize(const char* path) final;
        void Finalize() final;
    protected:
        
    };

    // auto word = FontManager::Get().Require("font_name", u8"${character}", ft_size);
}
#endif