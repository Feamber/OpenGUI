#ifndef UE4Runtime
#pragma once
#include "OpenGUI/Text/FontRendering.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>
#include "OpenGUI/Core/Containers/vector.hpp"

namespace OGUI::Font
{
struct OGUI_API AsyncGlyph
{
public:
    //virtual void Initialize(const char* path) = 0;
protected:
    inline void __initialize(const char* path)
    {
        //this->Initialize(path);
        is_ready = true;
    }
    std::atomic_bool is_ready = false;
};

class OGUI_API GlyphThread
{
public:
    template<typename T = AsyncGlyph>
    std::shared_ptr<T> Load(const char* path,
        const std::function<void(std::shared_ptr<AsyncGlyph>)>& completeCallback = [](std::shared_ptr<AsyncGlyph> tex) {})
    {
        
    }
protected:
    //void glyphThreadFunction();

    struct GlyphMakerTask 
    {
		std::string font;
        std::shared_ptr<AsyncGlyph> glyph;
	};
    std::atomic_bool is_running;
    std::thread glyph_thread;
	OGUI::vector<FontMakerTask> glyph_queue;
	std::mutex glyph_queue_mutex;
};
}
#endif