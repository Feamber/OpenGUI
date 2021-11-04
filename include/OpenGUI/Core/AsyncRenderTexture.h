#pragma once
#include "OpenGUI/Interface/Interfaces.h"
#include <atomic>

namespace OGUI
{
    class AsyncTexture2D
	{
		friend class RenderTextureManager;
	public:
		~AsyncTexture2D();
		inline const TextureHandle Get(void) const
		{
			return _handle;
		}
		inline bool valid(void) const
		{
			return is_ready;
		}
	protected:
		// Setup in RenderTextureManager::Update
		TextureHandle _handle = nullptr;
		std::atomic_bool is_ready;
	};

	using Region = Rect;
	class AsyncStaticAtlas : public AsyncTexture2D
	{
		friend class RenderTextureManager;
	public:
		
	protected:
		OGUI::vector<std::string_view> region_names;
	};

	class AsyncDynamicAtlas : public AsyncStaticAtlas
	{
		
	public:
		
	};

	enum ERenderTextureType
	{
		RawTexture,
		StaticAtlas,
		DynmicAtlas,
		SVG,
		Count
	};

	class AsyncRenderTexture 
	{
		friend class RenderTextureManager;
	public:
		~AsyncRenderTexture();
		AsyncRenderTexture(std::unique_ptr<AsyncTexture2D> image_handle, ERenderTextureType type);
		inline const TextureHandle Get(void) const
		{
			return device_image->Get();
		}
		inline bool valid(void) const
		{
			return device_image->valid();
		}
	protected:
		std::unique_ptr<AsyncTexture2D> device_image;
		std::unique_ptr<AsyncStaticAtlas> static_atlas;

		ERenderTextureType texture_type;
	};

}