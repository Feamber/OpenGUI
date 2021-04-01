#include "utils.h"
#include <string.h>
#include <unordered_map>

#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/Style/StyleHelpers.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Xml/XmlAsset.h"
#include "OpenGUI/VisualWindow.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"

extern void InstallInput();

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
window::Handle hWnd;

struct BitmapParser final : public OGUI::BitmapParserInterface
{
    inline Bitmap LoadFromFile(const FileHandle file)
    {
		Bitmap bm = {};
		int x, y, n;
		stbi_info_from_file((FILE*)file, &x, &y, &n);
		bm.format = n == 1 ? PF_R8Uint : OGUI::PF_R8G8B8A8;
		const auto channels = (n == 1) ? 1 : 4;

		auto data = stbi_load_from_file((FILE*)file, &x, &y, &n, channels);
		bm.bytes = data;
		bm.size_in_bytes = x * y * channels * sizeof(*data);
		bm.height = y;
		bm.width = x;

		return bm;
    }
    inline void Free(Bitmap bm)
    {
        stbi_image_free(bm.bytes);
    }
};

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
			if(last_cmd.texture != cmd.texture || last_index < 0)
			{
				WGPU_OGUI_Texture* texture = (WGPU_OGUI_Texture*)cmd.texture;
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

	TextureHandle RegisterTexture(const Bitmap& bitmap)
	{
		WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
		ogui_textures[t] = *t;
		return t;
	}

	void ReleaseTexture(TextureHandle h)
	{
		if(h)
			ogui_textures.erase(h);
		ogui_textures[h].Release();
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
	Bitmap bitmap = {};
	bitmap.bytes = white_tex;
	bitmap.size_in_bytes = 4 * 1024 * 1024;
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

/**
 * Draws using the above pipeline and buffers.
 */
static bool redraw() {
	static std::chrono::time_point prev = std::chrono::high_resolution_clock::now();
	auto& ctx = OGUI::Context::Get();
	std::chrono::time_point now = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - prev).count();
	prev = now;
	ctx.Update(hWnd, deltaTime);
	ctx.Render(hWnd);
	return true;
}

struct WGPURenderer : RenderInterface
{
	virtual PersistantPrimitiveHandle RegisterPrimitive(
		Vertex* vertices, uint32_t num_vertices,
		uint16_t* indices, uint32_t num_indices)
	{
		return nullptr;
	};
	virtual void ReleasePrimitive(PersistantPrimitiveHandle primitive) {};

	virtual void RenderPrimitives(const struct PrimDrawList& list)
	{
		((PrimDrawList&)list).ValidateAndBatch();

		OGUIWebGPURenderer* renderer = new OGUIWebGPURenderer();
		renderer->RenderPrimitives(list);
		delete renderer;
	};
	virtual void RenderPrimitives(const struct PersistantPrimDrawList&) {};

	virtual TextureHandle RegisterTexture(const Bitmap& bm) 
	{
		 return createTexture(device, queue, bm); 
	};
	virtual void ReleaseTexture(TextureHandle) {};

	virtual void SetScissor(const Scissor scissor) {};
	virtual void ResetScissor() {};

};

#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "olog/olog.h"
#if defined(_WIN32) || defined(_WIN64)
#define SDL_MAIN_HANDLED
#ifndef __WIN32__
#define __WIN32__
#endif // __WIN32__
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

extern "C" int __main__(int /*argc*/, char* /*argv*/[]) {
	int win_width = 1280;
	int win_height = 720;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		win_width, win_height, 0
	);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
#if defined(_WIN32) || defined(_WIN64)
	hWnd = (window::Handle)wmInfo.info.win.window;
#endif
	olog::init_log_system();
	if (hWnd) {
		if (device = webgpu::create(hWnd);device) {
			queue = wgpuDeviceGetDefaultQueue(device);
			swapchain = webgpu::createSwapChain(device, WINDOW_WIN_W, WINDOW_WIN_H);
			createPipelineAndBuffers();
			InstallInput();
			{
				using namespace OGUI;
				using namespace ostr::literal;
				auto& ctx = Context::Get();
				ctx.renderImpl = std::make_unique<WGPURenderer>();
				ctx.bmParserImpl = std::make_unique<BitmapParser>();
				ctx.fileImpl = std::make_unique<OGUI::FileInterface>();
				ctx.desktops = new VisualWindow;

				std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
				auto asset = XmlAsset::LoadXmlFile("res/test.xml");
				auto ve = XmlAsset::Instantiate(asset.lock()->id);
				if(auto child1 = QueryFirst(ve, "#Child1"))
				{
					constexpr auto handler = +[](PointerDownEvent& event)
					{
						using namespace ostr::literal;
						olog::info(u"Oh ♂ shit!");
						return true;
					};
					child1->_eventHandler.Register<PointerDownEvent, handler>();
				}
				{
					std::vector<VisualElement*> tests;
					QueryAll(ve, ".Test", tests);
					for (auto [i, test] : ipair(tests))
						if (i % 2 == 0)
							test->_styleClasses.push_back("Bigger");
				}

				ve->_pseudoMask |= (int)PseudoStates::Root;
				ctx.desktops->PushChild(ve);
				olog::info(u"initialize completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
			}

			// main loop
			bool done = false;
			while(!done)
			{
				using namespace ostr::literal;
				SDL_Event event;
				auto& ctx = OGUI::Context::Get();
				while (SDL_PollEvent(&event)) 
				{
					switch (event.type)
					{
						case SDL_MOUSEBUTTONDOWN:
						{
							EMouseKey buttonCode;
							switch (event.button.button)
							{
							case SDL_BUTTON_LEFT:
								buttonCode = EMouseKey::LB; break;
							case SDL_BUTTON_RIGHT:
								buttonCode = EMouseKey::RB; break;
							case SDL_BUTTON_MIDDLE:
								buttonCode = EMouseKey::MB; break;
							}
							int width, height;
							SDL_GetWindowSize(window, &width, &height);
							ctx.OnMouseDown((float)width, (float)height, buttonCode, event.button.x, event.button.y);
							break;
						}
						case SDL_MOUSEBUTTONUP:
						{
							EMouseKey buttonCode;
							switch (event.button.button)
							{
							case SDL_BUTTON_LEFT:
								buttonCode = EMouseKey::LB; break;
							case SDL_BUTTON_RIGHT:
								buttonCode = EMouseKey::RB; break;
							case SDL_BUTTON_MIDDLE:
								buttonCode = EMouseKey::MB; break;
							}
							ctx.OnMouseUp(buttonCode, event.button.x, event.button.y);
							break;
						}
						case SDL_MOUSEMOTION:
						{
							olog::info(u"MousePos X:{}, Y:{}"_o.format(event.motion.x, event.motion.y));
							//olog::info(u"MousePos RelX:{}, RelY:{}"o.format(event.motion.xrel, event.motion.yrel));
							ctx.OnMouseMove(true, event.motion.xrel, event.motion.yrel);
							break;
						}
						case SDL_KEYDOWN:
						{
							if (event.key.keysym.sym == SDLK_ESCAPE)
								done = true;
							else
								olog::info(u"KeyDown {}"_o.format(event.key.keysym.sym));
								// ctx.OnKeyDown()
							break;
						}
						case SDL_MOUSEWHEEL:
						{
							olog::info(u"MouseWheel Delta:{}"_o.format(event.wheel.y));
							ctx.OnMouseWheel(event.wheel.y);
							break;
						}
						case SDL_QUIT:
						done = true;
						break;
					}
				}
				redraw();
			}

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
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	return 0;
}
