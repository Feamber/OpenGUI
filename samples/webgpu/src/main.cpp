#include "utils.h"
#include <string.h>
#include <unordered_map>
#include "OpenGUI/Core/PrimitiveDraw.h"

WGPUDevice device;
WGPUQueue queue;
WGPUSwapChain swapchain;
WGPURenderPipeline pipeline;
WGPUBindGroupLayout bindGroupLayout;
WGPUSampler sampler;
uint8_t white_tex[4 * 1024 * 1024];
WGPU_OGUI_Texture* default_ogui_texture;

using namespace OGUI;
std::unordered_map<TextureInterface*, WGPU_OGUI_Texture> ogui_textures;

class OGUIWebGPURenderer : public OGUI::RenderInterface
{
public:
	virtual ~OGUIWebGPURenderer()
	{
		if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		if(index_buffer) wgpuBufferRelease(index_buffer);
	}
	PersistantPrimitiveHandle RegisterPrimitive(
		Vertex* vertices, uint32_t num_vertices,
		uint16_t* indices, uint32_t num_indices)
	{
		return nullptr;
	}
	
	void ReleasePrimitive(PersistantPrimitiveHandle primitive)
	{
		
	}

	void RenderPrimitives(const PrimDrawList& list)
	{
		if(list.command_list.size() <= 0) return;
		// upload buffer
		if(vertex_buffer) 
			wgpuBufferRelease(vertex_buffer);
		if(index_buffer) 
			wgpuBufferRelease(index_buffer);

		vertex_buffer = createBuffer(device, queue,
			list.vertices.data(), list.vertices.size() * sizeof(OGUI::Vertex), WGPUBufferUsage_Vertex);
		index_buffer = createBuffer(device, queue,
			list.indices.data(), list.indices.size() * sizeof(uint16_t), WGPUBufferUsage_Index);

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
				WGPU_OGUI_Texture* texture = (WGPU_OGUI_Texture*)last_cmd.texture.value;
				if(!texture)
					texture = default_ogui_texture;
				if(!texture->bind_group)
				{
					// update texture binding
					WGPUBindGroupEntry bgEntry[2] = {{}, {}};
					bgEntry[0].binding = 0;
					bgEntry[0].textureView 
						= texture->texture? texture->texture_view : default_ogui_texture->texture_view;
					bgEntry[1].binding = 1;
					bgEntry[1].sampler = sampler;
					WGPUBindGroupDescriptor bgDesc = {};
					bgDesc.layout = bindGroupLayout;
					bgDesc.entryCount = 2;
					bgDesc.entries = bgEntry;
					texture->bind_group = wgpuDeviceCreateBindGroup(device, &bgDesc);
				}
				wgpuRenderPassEncoderSetBindGroup(pass, 0, texture->bind_group, 0, 0);
			}
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

	void RenderPrimitives(const PersistantPrimDrawList&)
	{
		
	}

	TextureHandle RegisterTexture(const BitMap& bitmap)
	{
		WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
		ogui_textures[t] = *t;
		return t;
	}

	void ReleaseTexture(TextureHandle h)
	{
		if(h.value)
			ogui_textures.erase(h.value);
		ogui_textures[h.value].Release();
	}

	void SetScissor(const Scissor scissor)
	{

	}

	void ResetScissor()
	{

	}
	WGPUBuffer vertex_buffer;
	WGPUBuffer index_buffer;
};

/**
 * Bare minimum pipeline to draw a triangle using the above shaders.
 */

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
	bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);
	
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

	memset(white_tex, 255, 4 * 1024 * 1024 * sizeof(uint8_t)); // pure white
	BitMap bitmap = {};
	bitmap.bytes = white_tex;
	bitmap.bytes_size = 4 * 1024 * 1024;
	bitmap.width = 1024; bitmap.height = 1024;
	bitmap.format = PF_R8G8B8A8;
	WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
	ogui_textures[t] = *t;
	default_ogui_texture = t;

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
}


#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/VisualElement.h"
#include "WidgetSample.h"

static WidgetSample sample;

void RenderRec(VisualElement* element, PrimitiveDraw::DrawContext& ctx)
{
	element->DrawBackgroundPrimitive(ctx);
	element->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
}

/**
 * Draws using the above pipeline and buffers.
 */
static bool redraw() {
	PrimitiveDraw::DrawContext ctx;
	ctx.resolution = Vector2f(800.f, 450.f);
	PrimDrawList& list = ctx.prims;
	RenderRec(sample.tree.get(), ctx);

	/*PrimitiveDraw::BoxParams box = {};
	box.uv = {Vector2f(0.f, 0.f), Vector2f(1.f, 1.f)};
	
	box.rect = {Vector2f(+0.145f, +0.145f), Vector2f(+0.855f, +0.855f)};
	box.color = Color4f(.6f, .6f, .6f, 1.f);
	PrimitiveDraw::DrawBox(list, box);
	box.rect = {Vector2f(+0.1475f, +0.1475f), Vector2f(+0.8525f, +0.8525f)};
	box.color = Color4f(.75f, .75f, .75f, 1.f);
	PrimitiveDraw::DrawBox(list, box);
	box.rect = {Vector2f(+0.15f, +0.15f), Vector2f(+0.85f, +0.85f)};
	box.color = Color4f(.9f, .9f, .9f, 1.f);
	PrimitiveDraw::DrawBox(list, box);
	box.rect = {Vector2f(+0.145f, +0.8f), Vector2f(+0.855f, +0.855f)};
	box.color = Color4f(.3f, .3f, .3f, .8f);
	PrimitiveDraw::DrawBox(list, box);

	// circle 
	PrimitiveDraw::CircleParams circle = {};
	circle.color = Color4f(1, 0, 0, 1);
	circle.pos = Vector2f(0.5f, 0.5f);
	circle.radius = 0.1f;
	PrimitiveDraw::DrawCircle(list, circle);

	// fan 
	PrimitiveDraw::FanParams fan = {};
	fan.color = Color4f(0, 0, 1, 1);
	fan.beginDegree = math::PI / 4;
	fan.degree = math::PI / 2;
	fan.pos = Vector2f(0.5f, 0.5f);
	fan.radius = 0.08f;
	PrimitiveDraw::DrawFan(list, fan);

	// round box 
	PrimitiveDraw::RoundBoxParams roundBox;
	roundBox.rect = { Vector2f(+0.05f, +0.05f), Vector2f(+0.4f, +0.4f) };
	roundBox.color = Color4f(0, 1, 0, 1);
	roundBox.radius = 0.1f;
	PrimitiveDraw::DrawRoundBox(list, roundBox);
*/
	list.ValidateAndBatch();

	OGUIWebGPURenderer* renderer = new OGUIWebGPURenderer();
	renderer->RenderPrimitives(list);
	delete renderer;

	return true;
}

extern "C" int __main__(int /*argc*/, char* /*argv*/[]) {
	if (window::Handle wHnd = window::create()) {
		if ((device = webgpu::create(wHnd))) {
			queue = wgpuDeviceGetDefaultQueue(device);
			swapchain = webgpu::createSwapChain(device);
			createPipelineAndBuffers();

			sample.Initialize();
			window::show(wHnd);
			window::loop(wHnd, redraw);

		#ifndef __EMSCRIPTEN__
			// last bit of clean-up
			wgpuSamplerRelease(sampler);
			for(auto& tex : ogui_textures)
			{
				tex.second.Release();
			}
			wgpuBindGroupLayoutRelease(bindGroupLayout);
			wgpuRenderPipelineRelease(pipeline);
			wgpuSwapChainRelease(swapchain);
			wgpuQueueRelease(queue);
			//wgpuDeviceRelease(device);
		#endif
		}
	#ifndef __EMSCRIPTEN__
	#ifdef __APPLE__
		if(wHnd) 
			window::destroy(wHnd);
	#endif
	#endif
	}
	return 0;
}
