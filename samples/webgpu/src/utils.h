#pragma once
#include "OpenGUI/Core/Primitive.h"
#include "stb_image.h"
#include "webgpu.h"
#include "OpenGUI/Interface/RenderInterface.h"

// 0 ~ 1 => -1 ~ 1
static char const triangle_vert_wgsl[] = R"(
	[[location(0)]] var<in>  aPos : vec2<f32>;
	[[location(1)]] var<in>  aUV  : vec2<f32>;
	[[location(2)]] var<in>  aCol : vec4<f32>;
	[[location(0)]] var<out> vCol : vec4<f32>;
	[[location(1)]] var<out> vUV : vec2<f32>;
	[[builtin(position)]] var<out> Position : vec4<f32>;
	[[stage(vertex)]] fn main() -> void {
		Position = vec4<f32>(vec3<f32>((aPos * 2.0) - vec2<f32>(1.0, 1.0), 1.0), 1.0);
		vCol = aCol;
        vUV = aUV;
	}
)";


static char const triangle_frag_wgsl[] = R"(
	[[location(0)]] var<in> vCol : vec4<f32>;
	[[location(1)]] var<in> vUV : vec2<f32>;
	[[location(0)]] var<out> fragColor : vec4<f32>;

    [[binding(0), set(0)]] var<uniform_constant> myTexture : texture_2d<f32>;
    [[binding(1), set(0)]] var<uniform_constant> mySampler : sampler;

	[[stage(fragment)]] fn main() -> void {
        var alpha : f32 = vCol.a; 
		fragColor = vCol * textureSample(myTexture, mySampler, vUV);
        fragColor.a = alpha;
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

inline static WGPUTextureFormat translate(OGUI::PixelFormat format)
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

inline static uint32_t size_in_bytes(OGUI::PixelFormat format)
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

struct WGPU_OGUI_Texture : public OGUI::ITexture
{
    WGPUTexture texture;
    WGPUTextureView texture_view;
	WGPUBindGroup bind_group;

    inline void Release()
    {
        wgpuTextureRelease(texture);
		wgpuTextureViewRelease(texture_view);
		if(bind_group) 
            wgpuBindGroupRelease(bind_group);
    }
};

inline static WGPU_OGUI_Texture* createTexture(
    WGPUDevice device, WGPUQueue queue,
    const OGUI::BitMap& bitmap
)
{
    WGPU_OGUI_Texture* result = new WGPU_OGUI_Texture();

    WGPUTextureDescriptor descriptor = {};
    descriptor.usage = WGPUTextureUsage_Sampled | WGPUTextureUsage_CopyDst;
    descriptor.dimension = WGPUTextureDimension_2D;
    descriptor.size = {bitmap.width, bitmap.height, 1};
    descriptor.mipLevelCount = 1;
    descriptor.sampleCount = 1;
    descriptor.format = translate(bitmap.format);
    auto tex = wgpuDeviceCreateTexture(device, &descriptor);
    result->texture = tex;

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

	WGPUTextureViewDescriptor viewDesc = {};
	viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
	viewDesc.dimension = WGPUTextureViewDimension_2D;
	viewDesc.baseMipLevel = 0;
	viewDesc.mipLevelCount = 1;
	viewDesc.baseArrayLayer = 0;
	viewDesc.arrayLayerCount = 1;
	viewDesc.aspect = WGPUTextureAspect_All;
	result->texture_view = wgpuTextureCreateView(tex, &viewDesc);

    return result;
}

inline static void createBitMapFromJPG(const char* filename, OGUI::BitMap& bm)
{
    int x, y, n;
    auto data = stbi_load(filename, &x, &y, &n, STBI_rgb_alpha);
    bm.bytes = data;
    bm.bytes_size = x * y * n * sizeof(*data);
    bm.format = OGUI::PF_R8G8B8A8;
    bm.height = y;
    bm.width = x;
}

inline static void freeBitMap(OGUI::BitMap& bm)
{
    stbi_image_free(bm.bytes);
}