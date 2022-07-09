#pragma once
#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/Types.h"
#include <string_view>
#include <string.h>
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"

namespace OGUI
{
    using FileHandle = void*;
	using WindowHandle = struct WindowInterface*;

    struct OGUI_API SystemInterface
	{
		virtual ~SystemInterface();
		virtual std::string ToFullPath(const char* path) { return path; };
	};

	struct OGUI_API WindowInterface
	{
		virtual void* GetNativeHandle() = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual bool Update() = 0;
	};

    struct OGUI_API InputInterface
	{
		virtual ~InputInterface();
		virtual bool UseSystemGesture() = 0;

		virtual bool IsKeyDown(EKeyCode key_code) = 0;
		virtual bool IsKeyDown(EMouseKey key_code) = 0;
		virtual bool SetCursorPos(int32 x, int32 y) = 0;
		virtual bool SetCursor(EMouseCursor cursor) = 0;
		virtual bool GetCursorPos(int32& x, int32& y) = 0;
		virtual bool IsKeyToggled(EMouseKey key_code) = 0;
		virtual void CapturePointer(WindowHandle window, bool enabled) = 0;
		virtual OGUI::Vector2f GetDpiScale() = 0;
 		virtual void SetHighPrecisionMouseMode(WindowHandle window, bool Enable) = 0;
	};
    
    struct OGUI_API FileInterface
	{
		virtual ~FileInterface();
		virtual FileHandle Open(const char* path);
		virtual void Close(FileHandle file);
		virtual size_t Read(void* buffer, size_t size, FileHandle file);
		virtual int Seek(FileHandle stream, size_t offset, int fromwhere);
		virtual size_t Tell(const FileHandle file);
		virtual size_t Length(const FileHandle file);
		virtual MemoryResource Load(const char* path);
	};

	struct OGUI_API BitmapParserInterface
	{
		virtual ~BitmapParserInterface();

		virtual Bitmap LoadFromFile(const FileHandle file) = 0;
		virtual Bitmap LoadFromMemory(const void* buffer, size_t length) = 0;
		virtual void Free(Bitmap bm) = 0;
	};

	struct OGUI_API TextureInterface;
	struct OGUI_API MaterialInterface;
	struct OGUI_API PersistantPrimitiveInterface;
	struct OGUI_API PrimDrawContext;
    using TextureHandle = TextureInterface*;
    using MaterialHandle = MaterialInterface*;
    using RenderTargetViewHandle = struct RenderTargetView*;
    using PersistantPrimitiveHandle = PersistantPrimitiveInterface*;
	struct OGUI_API RenderInterface
	{
		virtual ~RenderInterface();
		virtual PersistantPrimitiveHandle RegisterPrimitive(
            Vertex* vertices, uint32_t num_vertices,
            uint16_t* indices, uint32_t num_indices) = 0;
        virtual void ReleasePrimitive(PersistantPrimitiveHandle primitive) = 0;

        virtual void RenderPrimitives(const struct PrimDrawContext&, const class WindowContext&) = 0;
        virtual void RenderPrimitives(const struct PersistantPrimDrawList&, const class WindowContext&) = 0;

		virtual RenderTargetViewHandle RegisterRenderTargetView(const Bitmap&) = 0;
		virtual RenderTargetViewHandle RegisterRenderTargetView(const ostr::string& url) = 0;		
		virtual void UpdateRenderTargetView(RenderTargetViewHandle, const Bitmap&) = 0;
		virtual void ReleaseRenderTargetView(RenderTargetViewHandle) = 0;
		virtual TextureHandle GetTexture(RenderTargetViewHandle) = 0; 

        virtual TextureHandle RegisterTexture(const Bitmap&) = 0;
		virtual void UpdateTexture(TextureHandle, const Bitmap&) = 0;
        virtual void ReleaseTexture(TextureHandle) = 0;

		virtual MaterialHandle RegisterMaterial(const ostr::string& url) = 0;
		virtual void ReleaseMaterial(MaterialHandle) = 0;
		virtual void SetMaterialParameterTexture(MaterialHandle, const ostr::string& name, TextureHandle texture) {}

		virtual Vector2f GetSize(RenderTargetViewHandle) = 0;

		virtual void RegisterWindow(const class WindowContext&) = 0;
		virtual void ReleaseWindow(const class WindowContext& wctx) = 0;
		
        virtual void SetScissor(const Scissor scissor) = 0;
        virtual void ResetScissor() = 0;
	};

	struct OGUI_API LogInterface
	{
		virtual ~LogInterface();

		virtual void Log(olog::Level l, ostr::string_view msg) = 0;
	};

	struct OGUI_API FontLibraryInterface
	{
		virtual ~FontLibraryInterface() {}

		virtual bool LoadFromFile(const char* filename) = 0;
	};
}