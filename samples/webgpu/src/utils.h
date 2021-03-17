#pragma once
#include "webgpu.h"
#include "Core/Renderer.h"

static char const triangle_vert_wgsl[] = R"(
	const PI : f32 = 3.141592653589793;
	fn radians(degs : f32) -> f32 {
		return (degs * PI) / 180.0;
	}
	[[location(0)]] var<in>  aPos : vec2<f32>;
	[[location(1)]] var<in>  aCol : vec4<f32>;
	[[location(2)]] var<in>  aUV  : vec2<f32>;
	[[location(0)]] var<out> vCol : vec4<f32>;
	[[builtin(position)]] var<out> Position : vec4<f32>;
	[[stage(vertex)]] fn main() -> void {
		Position = vec4<f32>(vec3<f32>(aPos, 1.0), 1.0);
		vCol = aCol;
	}
)";


static char const triangle_frag_wgsl[] = R"(
	[[location(0)]] var<in> vCol : vec4<f32>;
	[[location(0)]] var<out> fragColor : vec4<f32>;
	[[stage(fragment)]] fn main() -> void {
		fragColor = vCol;
	}
)";


/**
 * Helper to create a shader from WGSL source.
 *
 * \param[in] code WGSL shader source
 * \param[in] label optional shader name
 */
inline static WGPUShaderModule createShader(WGPUDevice device, const char* const code, const char* label = nullptr) {
	WGPUShaderModuleWGSLDescriptor wgsl = {};
	wgsl.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
	wgsl.source = code;
	WGPUShaderModuleDescriptor desc = {};
	desc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgsl);
	desc.label = label;
	return wgpuDeviceCreateShaderModule(device, &desc);
}

/**
 * Helper to create a buffer.
 *
 * \param[in] data pointer to the start of the raw data
 * \param[in] size number of bytes in \a data
 * \param[in] usage type of buffer
 */
inline static WGPUBuffer createBuffer(WGPUDevice device, WGPUQueue queue,
    const void* data, size_t size, WGPUBufferUsage usage) {
	WGPUBufferDescriptor desc = {};
	desc.usage = WGPUBufferUsage_CopyDst | usage;
	desc.size  = size;
	WGPUBuffer buffer = wgpuDeviceCreateBuffer(device, &desc);
	wgpuQueueWriteBuffer(queue, buffer, 0, data, size);
	return buffer;
}

WGPUTextureFormat translate(OGUI::PixelFormat format)
{   
    switch(format)
    {
    case OGUI::PixelFormat::PF_R8G8B8A8: return WGPUTextureFormat_RGBA8Unorm;
    case OGUI::PixelFormat::PF_R16G16B16A16: return WGPUTextureFormat_RGBA16Float;
    case OGUI::PixelFormat::PF_R8G8B8A8_SRGB: return WGPUTextureFormat_RGBA8UnormSrgb;
    case OGUI::PixelFormat::PF_R8Uint: return WGPUTextureFormat_R8Uint;
    case OGUI::PixelFormat::PF_R16Uint: return WGPUTextureFormat_R16Uint;
    case OGUI::PixelFormat::PF_R32Uint: return WGPUTextureFormat_R32Uint;
    default: return WGPUTextureFormat_Undefined;
    }
}

uint32_t size_in_bytes(OGUI::PixelFormat format)
{
    switch(format)
    {
    case OGUI::PixelFormat::PF_R8G8B8A8: return 4;
    case OGUI::PixelFormat::PF_R16G16B16A16: return 8;
    case OGUI::PixelFormat::PF_R8G8B8A8_SRGB: return 4;
    case OGUI::PixelFormat::PF_R8Uint: return 1;
    case OGUI::PixelFormat::PF_R16Uint: return 2;
    case OGUI::PixelFormat::PF_R32Uint: return 4;
    default: return -1;
    }
}

inline static WGPUTexture createTexture(WGPUDevice device, WGPUQueue queue,
    const OGUI::BitMap& bitmap)
{
    WGPUTextureDescriptor descriptor = {};
    descriptor.usage = WGPUTextureUsage_Sampled | WGPUTextureUsage_CopyDst;
    descriptor.dimension = WGPUTextureDimension_2D;
    descriptor.size = {bitmap.width, bitmap.height, 1};
    descriptor.mipLevelCount = 0;
    descriptor.sampleCount = 1;
    descriptor.format = translate(bitmap.format);
    auto tex = wgpuDeviceCreateTexture(device, &descriptor);

    WGPUTextureCopyView cpyView = {};
    cpyView.texture = tex;
    cpyView.mipLevel = 0;
    cpyView.origin = { 0, 0, 0 };
    cpyView.aspect = WGPUTextureAspect_All;

    WGPUTextureDataLayout dtLayout = {};
    dtLayout.offset = 0;
    dtLayout.bytesPerRow = bitmap.width * size_in_bytes(bitmap.format);
    dtLayout.rowsPerImage = bitmap.height;

    WGPUExtent3D writeSize = descriptor.size;
    wgpuQueueWriteTexture(queue, &cpyView, bitmap.bytes, bitmap.bytes_size, &dtLayout, &writeSize);
    return tex;
}