#pragma once
#include "OpenGUI/Interface/Interfaces.h"
#include <atomic>

namespace OGUI
{
    class AsyncImage
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

	class AsyncRenderTexture
	{
		friend class RenderTextureManager;
	public:
		AsyncRenderTexture(std::shared_ptr<AsyncImage> image_handle);
		inline const TextureHandle Get(void) const
		{
			return device_image->Get();
		}
		inline bool valid(void) const
		{
			return device_image->valid();
		}
	protected:
		std::shared_ptr<AsyncImage> device_image;
	};
}