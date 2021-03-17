#pragma once
#include "webgpu.h"


static char const triangle_vert_wgsl[] = R"(
	const PI : f32 = 3.141592653589793;
	fn radians(degs : f32) -> f32 {
		return (degs * PI) / 180.0;
	}
	[[block]] struct Rotation {
		[[offset(0)]] degs : f32;
	};
	[[set(0), binding(0)]] var<uniform> uRot : Rotation;
	[[location(0)]] var<in>  aPos : vec2<f32>;
	[[location(1)]] var<in>  aCol : vec3<f32>;
	[[location(0)]] var<out> vCol : vec3<f32>;
	[[builtin(position)]] var<out> Position : vec4<f32>;
	[[stage(vertex)]] fn main() -> void {
		var rads : f32 = radians(uRot.degs);
		var cosA : f32 = cos(rads);
		var sinA : f32 = sin(rads);
		var rot : mat3x3<f32> = mat3x3<f32>(
			vec3<f32>( cosA, sinA, 0.0),
			vec3<f32>(-sinA, cosA, 0.0),
			vec3<f32>( 0.0,  0.0,  1.0));
		Position = vec4<f32>(rot * vec3<f32>(aPos, 1.0), 1.0);
		vCol = aCol;
	}
)";


static char const triangle_frag_wgsl[] = R"(
	[[location(0)]] var<in> vCol : vec3<f32>;
	[[location(0)]] var<out> fragColor : vec4<f32>;
	[[stage(fragment)]] fn main() -> void {
		fragColor = vec4<f32>(vCol, 5.0);
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