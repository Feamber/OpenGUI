#include "utils.h"
#include <string.h>

WGPUDevice device;
WGPUQueue queue;
WGPUSwapChain swapchain;

WGPURenderPipeline pipeline;
WGPUBindGroup bindGroup;
WGPUTexture texture;
WGPUTextureView texture_view;
WGPUSampler sampler;

using namespace OGUI;
class OGUIWebGPURenderer : public OGUI::IRenderer
{
public:
	virtual ~OGUIWebGPURenderer()
	{
		if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		if(index_buffer) wgpuBufferRelease(index_buffer);
	}
	PersistantPrimitiveHandle register_primitive(
		Vertex* vertices, uint32_t num_vertices,
		uint16_t* indices, uint32_t num_indices)
	{
		return nullptr;
	}
	
	void release_primitive(PersistantPrimitiveHandle primitive)
	{
		
	}

	void render_primitives(const PrimDrawList& list)
	{
		if(list.command_list.size() <= 0) return;
		// upload buffer
		if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		if(index_buffer) wgpuBufferRelease(index_buffer);

		vertex_buffer = createBuffer(device, queue,
			list.vertices.data(), list.vertices.size() * sizeof(OGUI::Vertex), WGPUBufferUsage_Vertex);
		index_buffer = createBuffer(device, queue,
			list.indices.data(), list.indices.capacity() * sizeof(uint16_t), WGPUBufferUsage_Index);

		WGPUTextureView backBufView = wgpuSwapChainGetCurrentTextureView(swapchain);			// create textureView
		WGPURenderPassColorAttachmentDescriptor colorDesc = {};
		colorDesc.attachment = backBufView;
		colorDesc.loadOp  = WGPULoadOp_Clear;
		colorDesc.storeOp = WGPUStoreOp_Store;
		colorDesc.clearColor = {0.3f, 0.3f, 0.3f, 1.f};
		WGPURenderPassDescriptor renderPass = {};
		renderPass.colorAttachmentCount = 1;
		renderPass.colorAttachments = &colorDesc;
		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);			// create encoder
		WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);	// create pass
		// draw the triangle (comment these five lines to simply clear the screen)
		wgpuRenderPassEncoderSetPipeline(pass, pipeline);

		int last_index = -1;
		for(auto& cmd : list.command_list)
		{
			auto& last_cmd = list.command_list[last_index < 0 ? 0 : last_index];
			if(last_cmd.texture.value != cmd.texture.value || last_index < 0)
			{
				// update texture binding

			}
			wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, 0);
			wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertex_buffer,
				cmd.vertex_offset * sizeof(Vertex),
				0
			);
			wgpuRenderPassEncoderSetIndexBuffer(pass, index_buffer, WGPUIndexFormat_Uint16,
				cmd.index_offset * sizeof(uint16_t),
				0
			);
			wgpuRenderPassEncoderDrawIndexed(pass, cmd.element_count, 1, 0, 0, 0);
			last_index++;
		}

		wgpuRenderPassEncoderEndPass(pass);
		wgpuRenderPassEncoderRelease(pass);														// release pass
		WGPUCommandBuffer commands = wgpuCommandEncoderFinish(encoder, nullptr);				// create commands
		wgpuCommandEncoderRelease(encoder);														// release encoder

		wgpuQueueSubmit(queue, 1, &commands);
		wgpuCommandBufferRelease(commands);														// release commands
	#ifndef __EMSCRIPTEN__
		/*
		* TODO: wgpuSwapChainPresent is unsupported in Emscripten, so what do we do?
		*/
		wgpuSwapChainPresent(swapchain);
	#endif
		wgpuTextureViewRelease(backBufView);													

	}

	void render_primitives(const PersistantPrimDrawList&)
	{
		
	}

	TextureHandle register_texture(const BitMap& bitmap)
	{
		ITexture* t = (ITexture*)createTexture(device, queue, bitmap);
		return t;
	}

	void release_texture(TextureHandle h)
	{
		WGPUTexture t = (WGPUTexture)h.value;
		wgpuTextureRelease(t);
	}

	void set_scissor(const Scissor scissor)
	{

	}

	void reset_scissor()
	{

	}
	WGPUBuffer vertex_buffer;
	WGPUBuffer index_buffer;
};

/**
 * Bare minimum pipeline to draw a triangle using the above shaders.
 */
uint8_t white_tex[4 * 1024 * 1024];
static void createPipelineAndBuffers() {
	// compile shaders
	// NOTE: these are now the WGSL shaders (tested with Dawn and Chrome Canary)
	WGPUShaderModule vertMod = createShader(device, triangle_vert_wgsl);
	WGPUShaderModule fragMod = createShader(device, triangle_frag_wgsl);
	
	// bind group layout (used by both the pipeline layout and uniform bind group, released at the end of this function)
	WGPUBindGroupLayoutEntry bglEntry[2] = {{}, {}};
	bglEntry[0].binding = 0;
	bglEntry[0].visibility = WGPUShaderStage_Fragment;
	bglEntry[0].type = WGPUBindingType_Undefined;
	bglEntry[0].texture.sampleType = WGPUTextureSampleType_Float;
	bglEntry[0].texture.viewDimension = WGPUTextureViewDimension_2D;
	bglEntry[0].texture.multisampled = false;
	bglEntry[1].binding = 1;
	bglEntry[1].visibility = WGPUShaderStage_Fragment;
	bglEntry[1].type = WGPUBindingType_Undefined;
	bglEntry[1].sampler.type = WGPUSamplerBindingType_Filtering;

	WGPUBindGroupLayoutDescriptor bglDesc = {};
	bglDesc.entryCount = 2;
	bglDesc.entries = bglEntry;
	WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);
	
	// pipeline layout (used by the render pipeline, released after its creation)
	WGPUPipelineLayoutDescriptor layoutDesc = {};
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = &bindGroupLayout;
	WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &layoutDesc);
	
	// begin pipeline set-up
	WGPURenderPipelineDescriptor desc = {};
	desc.layout = pipelineLayout;
	desc.vertexStage.module = vertMod;
	desc.vertexStage.entryPoint = "main";
	WGPUProgrammableStageDescriptor fragStage = {};
	fragStage.module = fragMod;
	fragStage.entryPoint = "main";
	desc.fragmentStage = &fragStage;
	// describe buffer layouts
	WGPUVertexAttributeDescriptor vertAttrs[3] = {};
	vertAttrs[0].format = WGPUVertexFormat_Float2;
	vertAttrs[0].offset = 0;
	vertAttrs[0].shaderLocation = 0;
	vertAttrs[1].format = WGPUVertexFormat_Float2;
	vertAttrs[1].offset = 2 * sizeof(float);
	vertAttrs[1].shaderLocation = 1;
	vertAttrs[2].format = WGPUVertexFormat_Float4;
	vertAttrs[2].offset = 4 * sizeof(float);
	vertAttrs[2].shaderLocation = 2;

	WGPUVertexBufferLayoutDescriptor vertDesc = {};
	vertDesc.arrayStride = 8 * sizeof(float);
	vertDesc.attributeCount = 3;
	vertDesc.attributes = vertAttrs;
	WGPUVertexStateDescriptor vertState = {};
	vertState.vertexBufferCount = 1;
	vertState.vertexBuffers = &vertDesc;
	desc.vertexState = &vertState;
	desc.primitiveTopology = WGPUPrimitiveTopology_TriangleList;
	desc.sampleCount = 1;
	// describe blend
	WGPUBlendDescriptor blendDesc = {};
	blendDesc.operation = WGPUBlendOperation_Add;
	blendDesc.srcFactor = WGPUBlendFactor_SrcAlpha;
	blendDesc.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	WGPUColorStateDescriptor colorDesc = {};
	colorDesc.format = webgpu::getSwapChainFormat(device);
	colorDesc.alphaBlend = blendDesc;
	colorDesc.colorBlend = blendDesc;
	colorDesc.writeMask = WGPUColorWriteMask_All;
	desc.colorStateCount = 1;
	desc.colorStates = &colorDesc;
	desc.sampleMask = 0xFFFFFFFF; // <-- Note: this currently causes Emscripten to fail (sampleMask ends up as -1, which trips an assert)
	pipeline = wgpuDeviceCreateRenderPipeline(device, &desc);
	// partial clean-up (just move to the end, no?)
	wgpuPipelineLayoutRelease(pipelineLayout);
	wgpuShaderModuleRelease(fragMod);
	wgpuShaderModuleRelease(vertMod);

	memset(white_tex, 0, 4 * 1024 * 1024 * sizeof(uint8_t)); 
	memset(white_tex, 255, 2 * 1024 * 1024 * sizeof(uint8_t)); 
	BitMap bitmap = {};
	bitmap.bytes = white_tex;
	bitmap.bytes_size = 4 * 1024 * 1024;
	bitmap.width = 1024; bitmap.height = 1024;
	bitmap.format = PF_R8G8B8A8;
	texture = createTexture(device, queue, bitmap);

	WGPUTextureViewDescriptor viewDesc = {};
	viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
	viewDesc.dimension = WGPUTextureViewDimension_2D;
	viewDesc.baseMipLevel = 0;
	viewDesc.mipLevelCount = 1;
	viewDesc.baseArrayLayer = 0;
	viewDesc.arrayLayerCount = 1;
	viewDesc.aspect = WGPUTextureAspect_All;
	texture_view = wgpuTextureCreateView(texture, &viewDesc);

	WGPUSamplerDescriptor sampDesc = {};
	sampDesc.addressModeU = WGPUAddressMode_ClampToEdge;
	sampDesc.addressModeV = WGPUAddressMode_ClampToEdge;
	sampDesc.addressModeW = WGPUAddressMode_ClampToEdge;
	sampDesc.magFilter = WGPUFilterMode_Nearest;
	sampDesc.minFilter = WGPUFilterMode_Nearest;
	sampDesc.mipmapFilter = WGPUFilterMode_Nearest;
	sampDesc.lodMinClamp = 0;
	sampDesc.lodMaxClamp = 1000.f;
	sampDesc.maxAnisotropy = 1.f;
	sampler = wgpuDeviceCreateSampler(device, &sampDesc);

	WGPUBindGroupEntry bgEntry[2] = {{}, {}};
	bgEntry[0].binding = 0;
	bgEntry[0].textureView = texture_view;
	bgEntry[1].binding = 1;
	bgEntry[1].sampler = sampler;
	WGPUBindGroupDescriptor bgDesc = {};
	bgDesc.layout = bindGroupLayout;
	bgDesc.entryCount = 2;
	bgDesc.entries = bgEntry;
	bindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);

	// last bit of clean-up
	wgpuBindGroupLayoutRelease(bindGroupLayout);
}

/**
 * Draws using the above pipeline and buffers.
 */
static bool redraw() {
	PrimDrawList list;
	list.vertices = {
		Vertex{Vector2f(+0.8f, +0.8f), Vector2f(1.f, 1.f), Color4f(0.2f, 0.2f, 0.2f, .8f)}, //-1 /*offset test*/
		Vertex{Vector2f(+0.8f, +0.8f), Vector2f(1.f, 1.f), Color4f(0.2f, 0.2f, 0.2f, .8f)}, //0
		Vertex{Vector2f(-0.8f, +0.8f), Vector2f(0.f, 1.f), Color4f(0.2f, 0.2f, 0.2f, .8f)}, //1
		Vertex{Vector2f(+0.8f, -0.8f), Vector2f(1.f, 0.f), Color4f(0.2f, 0.2f, 0.2f, .8f)},//2
		Vertex{Vector2f(-0.8f, -0.8f), Vector2f(0.f, 0.f), Color4f(0.2f, 0.2f, 0.2f, .8f)},//3
	};
	list.indices = {0u/*offset test*/, 0u, 1u, 2u, 2u, 1u, 3u};
	list.command_list = {
		PrimDraw{1u, 1u, 9u, nullptr, nullptr}
	};
	list.validate_and_batch();

		
	OGUIWebGPURenderer* renderer = new OGUIWebGPURenderer();
	renderer->render_primitives(list);
	delete renderer;
	return true;
}

extern "C" int __main__(int /*argc*/, char* /*argv*/[]) {
	if (window::Handle wHnd = window::create()) {
		if ((device = webgpu::create(wHnd))) {
			queue = wgpuDeviceGetDefaultQueue(device);
			swapchain = webgpu::createSwapChain(device);
			createPipelineAndBuffers();

			window::show(wHnd);
			window::loop(wHnd, redraw);

		#ifndef __EMSCRIPTEN__
			wgpuSamplerRelease(sampler);
			wgpuTextureViewRelease(texture_view);
			wgpuBindGroupRelease(bindGroup);
			wgpuRenderPipelineRelease(pipeline);
			wgpuSwapChainRelease(swapchain);
			wgpuQueueRelease(queue);
			wgpuDeviceRelease(device);
		#endif
		}
	#ifndef __EMSCRIPTEN__
	#ifdef __APPLE__
		if(wHnd) window::destroy(wHnd);
	#endif
	#endif
	}
	return 0;
}
