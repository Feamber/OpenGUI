#pragma once
#include "OpenGUI/Interface/Interfaces.h"
#include <atomic>

namespace OGUI
{
    class AsyncRenderTexture
	{
		friend class RenderTextureManager;
	public:
		inline const TextureHandle Get(void) const
		{
			return _handle;
		}
		inline bool valid(void) const
		{
			return is_ready;
		}
	protected:
		TextureHandle _handle;
		std::atomic_bool is_ready;
	};
}