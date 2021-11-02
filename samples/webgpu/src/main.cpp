#include "OpenGUI/Core/PrimitiveDraw.h"
#include "appbase.h"
#include "webgpu.h"

extern void InstallInput();

std::shared_ptr<class OGUIWebGPURenderer> WebGPURenderer;
webgpu::instance* wgpu;
WGPUDevice device;
WGPUQueue queue;
// Bare minimum pipeline to draw a triangle using the above shaders.
uint8_t white_tex0[1024 * 1024 * 4];

class Window : public CSSWindow
{
public:
	WGPUSwapChain swapchain;
	WGPURenderPipeline pipeline;
	WGPUBindGroupLayout bindGroupLayout;
	WGPUSampler sampler;
	WGPU_OGUI_Texture* default_ogui_texture;
	std::unordered_map<TextureInterface*, WGPU_OGUI_Texture> ogui_textures;

	Window(int width, int height, const char *title, const char *xmlFile)
		:CSSWindow(width, height, title, xmlFile)
	{
		if (hWnd)
		{
			{
				swapchain = webgpu::createSwapChain(wgpu, width, height, hWnd, wmInfo.info.win.hinstance);
				createPipelineAndBuffers();

				cWnd->renderImpl = std::static_pointer_cast<RenderInterface>(WebGPURenderer);
			}
        }
	};

	virtual ~Window()
	{
#ifndef __EMSCRIPTEN__
		// last bit of clean-up
		wgpuSamplerRelease(sampler);
		for(auto& tex : ogui_textures)
		{
			tex.second.Release();
		}
		ogui_textures.clear();
		wgpuBindGroupLayoutRelease(bindGroupLayout);
		wgpuRenderPipelineRelease(pipeline);
		wgpuSwapChainRelease(swapchain);
		wgpuQueueRelease(queue);
		//wgpuDeviceRelease(device);
		webgpu::release(wgpu);
#endif
	}

	virtual bool Update() override
	{
        return CSSWindow::Update();
	};

	void createPipelineAndBuffers()
	{
		// compile shaders
		// NOTE: these are now the WGSL shaders (tested with Dawn and Chrome Canary)
		WGPUShaderModule vertMod = createShader(device, triangle_vert_wgsl);
		WGPUShaderModule fragMod = createShader(device, triangle_frag_wgsl);

		// bind group layout (used by both the pipeline layout and uniform bind group, released at the end of this function)
		WGPUBindGroupLayoutEntry bglEntry[2] = {{}, {}};
		bglEntry[0].binding = 0;
		bglEntry[0].visibility = WGPUShaderStage_Fragment;
		bglEntry[0].texture.sampleType = WGPUTextureSampleType_Float;
		bglEntry[0].texture.viewDimension = WGPUTextureViewDimension_2D;
		bglEntry[0].texture.multisampled = false;
		bglEntry[1].binding = 1;
		bglEntry[1].visibility = WGPUShaderStage_Fragment;
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

		// describe buffer layouts
		WGPUVertexAttribute vertAttrs[3] = {};
		vertAttrs[0].format = WGPUVertexFormat_Float32x2;
		vertAttrs[0].offset = 0;
		vertAttrs[0].shaderLocation = 0;
		vertAttrs[1].format = WGPUVertexFormat_Float32x2;
		vertAttrs[1].offset = 2 * sizeof(float);
		vertAttrs[1].shaderLocation = 1;
		vertAttrs[2].format = WGPUVertexFormat_Float32x4;
		vertAttrs[2].offset = 4 * sizeof(float);
		vertAttrs[2].shaderLocation = 2;
		WGPUVertexBufferLayout vertDesc = {};
		vertDesc.arrayStride = 8 * sizeof(float);
		vertDesc.attributeCount = 3;
		vertDesc.attributes = vertAttrs;

		// shader stages
		WGPUVertexState vertexState = {};
		vertexState.module = vertMod;
		vertexState.entryPoint = "main";
		vertexState.bufferCount = 1;
		vertexState.buffers = &vertDesc;
		desc.vertex = vertexState;

		WGPUFragmentState fragStage = {};
		fragStage.module = fragMod;
		fragStage.entryPoint = "main";
		desc.fragment = &fragStage;

		desc.primitive.frontFace = WGPUFrontFace_CCW;
		desc.primitive.cullMode = WGPUCullMode_None;
		desc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
		desc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;

		// describe blend
		WGPUBlendState blendDesc = {};
		blendDesc.color.operation = WGPUBlendOperation_Add;
		blendDesc.color.srcFactor = WGPUBlendFactor_SrcAlpha;
		blendDesc.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
		blendDesc.alpha.operation = WGPUBlendOperation_Add;
		blendDesc.alpha.srcFactor = WGPUBlendFactor_SrcAlpha;
		blendDesc.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;

		WGPUColorTargetState colorTarget = {};
		colorTarget.format = WGPUTextureFormat_BGRA8Unorm;
		colorTarget.blend = &blendDesc;
		colorTarget.writeMask = WGPUColorWriteMask_All;
		fragStage.targetCount = 1;
		fragStage.targets = &colorTarget;

		desc.multisample.count = 1;
		desc.multisample.mask = 0xFFFFFFFF;
		desc.multisample.alphaToCoverageEnabled = false;
		pipeline = wgpuDeviceCreateRenderPipeline(device, &desc);
		// partial clean-up (just move to the end, no?)
		wgpuPipelineLayoutRelease(pipelineLayout);
		wgpuShaderModuleRelease(fragMod);
		wgpuShaderModuleRelease(vertMod);

		memset(white_tex0, 255, 4 * 1024 * 1024 * sizeof(uint8_t)); // pure white
		Bitmap bitmap = {};
		bitmap.resource.bytes = white_tex0;
		bitmap.resource.size_in_bytes = 4 * 1024 * 1024;
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
};

class OGUIWebGPURenderer final : public OGUI::RenderInterface
{
public:
	Window* window;
	OGUIWebGPURenderer() 
	{
		const auto WGPUDeviceSet = InitializeWGPUDevice();
		wgpu = std::get<0>(WGPUDeviceSet);
		device = std::get<1>(WGPUDeviceSet);
		queue = std::get<2>(WGPUDeviceSet);
	}

	virtual ~OGUIWebGPURenderer()
	{
		if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		if(index_buffer) wgpuBufferRelease(index_buffer);
	}
	
	PersistantPrimitiveHandle RegisterPrimitive(
		Vertex* vertices, uint32_t num_vertices,
		uint16_t* indices, uint32_t num_indices) override
	{
		return nullptr;
	}
	
	void ReleasePrimitive(PersistantPrimitiveHandle primitive) override
	{
		
	}

	void RenderPrimitives(const PrimDrawList& list, const class WindowContext&) override
	{
		((PrimDrawList&)list).ValidateAndBatch();
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

		WGPUTextureView backBufView = wgpuSwapChainGetCurrentTextureView(window->swapchain);			// create textureView
		WGPURenderPassColorAttachment colorDesc = {};
		colorDesc.view = backBufView;
		colorDesc.loadOp  = WGPULoadOp_Clear;
		colorDesc.storeOp = WGPUStoreOp_Store;
		colorDesc.clearColor = {0.3f, 0.3f, 0.3f, 1.f};
		WGPURenderPassDescriptor renderPass = {};
		renderPass.colorAttachmentCount = 1;
		renderPass.colorAttachments = &colorDesc;
		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);			// create encoder
		WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);	// create pass
		// draw the triangle (comment these five lines to simply clear the screen)
		wgpuRenderPassEncoderSetPipeline(pass, window->pipeline);

		int last_index = -1;
		for(auto& cmd : list.command_list)
		{
			auto& last_cmd = list.command_list[last_index < 0 ? 0 : last_index];
			if(last_cmd.texture != cmd.texture || last_index < 0)
			{
				WGPU_OGUI_Texture* texture = (WGPU_OGUI_Texture*)cmd.texture;
				if(!texture)
					texture = window->default_ogui_texture;
				if(!texture->bind_group)
				{
					// update texture binding
					WGPUBindGroupEntry bgEntry[2] = {{}, {}};
					bgEntry[0].binding = 0;
					bgEntry[0].textureView 
						= texture->texture? texture->texture_view : window->default_ogui_texture->texture_view;
					bgEntry[1].binding = 1;
					bgEntry[1].sampler = window->sampler;
					WGPUBindGroupDescriptor bgDesc = {};
					bgDesc.layout = window->bindGroupLayout;
					bgDesc.entryCount = 2;
					bgDesc.entries = bgEntry;
					texture->bind_group = wgpuDeviceCreateBindGroup(device, &bgDesc);
				}
				wgpuRenderPassEncoderSetBindGroup(pass, 0, texture->bind_group, 0, 0);
			}
			wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertex_buffer,
				cmd.vertex_offset * sizeof(Vertex),
				WGPU_WHOLE_SIZE
			);
			wgpuRenderPassEncoderSetIndexBuffer(pass, index_buffer, WGPUIndexFormat_Uint16,
				cmd.index_offset * sizeof(uint16_t),
				WGPU_WHOLE_SIZE
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
		wgpuSwapChainPresent(window->swapchain);
	#endif
		wgpuTextureViewRelease(backBufView);													
	}

	void RenderPrimitives(const PersistantPrimDrawList&, const class WindowContext&) override
	{
		
	}

	TextureHandle RegisterTexture(const Bitmap& bitmap) override
	{
		WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
		window->ogui_textures[t] = *t;
		return t;
	}

	void UpdateTexture(TextureHandle t, const Bitmap& bitmap) override
	{
		updateTexture(device, queue, &window->ogui_textures[t], bitmap);
	}

	void ReleaseTexture(TextureHandle h) override
	{
		if(!h)
			return;
		auto iter = window->ogui_textures.find(h);
		if(iter == window->ogui_textures.end())
			return;
		iter->second.Release();
		window->ogui_textures.erase(iter);
	}

	RenderTargetViewHandle RegisterRenderTargetView(const Bitmap &) override
	{
		return nullptr;
	}

	RenderTargetViewHandle RegisterRenderTargetView(const TextureHandle) override
	{
		return nullptr;
	}

	void ReleaseRenderTargetView(RenderTargetViewHandle) override
	{

	}

	Vector2f GetSize(RenderTargetViewHandle) override
	{
		return {0, 0};
	}

	void SetScissor(const Scissor scissor) override
	{

	}
 
	void ResetScissor() override
	{

	}
	WGPUBuffer vertex_buffer = nullptr;
	WGPUBuffer index_buffer = nullptr;
};

int main(int , char* []) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	ostr::string dataBindTest = "test1";
	ostr::string dataBindTest2 = "test2";
	ostr::string dataBindTest3 = "test1";
	ostr::string dataBindTest4 = "test2";
	
	using namespace OGUI;
	using namespace ostr::literal;
	auto& ctx = Context::Get();
	InstallInput();
	{
		ctx.fileImpl = std::make_unique<OGUI::FileInterface>();
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest, "GName");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest2, "GName2");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest3, "GName3");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest4, "GName4");
	}

	WebGPURenderer = std::make_shared<OGUIWebGPURenderer>();
	Window* win1 = nullptr;//new Window(WINDOW_WIN_W, WINDOW_WIN_H, "FocusNavigationTest", "res/test_nav.xml");
	Window* win2 = new Window(WINDOW_WIN_W, WINDOW_WIN_H, "CssTest", "res/test.xml");
	WebGPURenderer->window = win2;

	// main loop
	while(win1 || win2)
	{
		using namespace ostr::literal;

		SDL_Event event;
		while (SDL_PollEvent(&event) && (win1 || win2)) 
		{
			olog::Info(u"event type: {}  windowID: {}"_o, (int)event.type, (int)event.window.windowID);
			if(win1 && SDL_GetWindowID(win1->window) == event.window.windowID)
			{
				if(!SDLEventHandler(event, win1->window, win1->hWnd))
				{
					delete win1;
					win1 = nullptr;
				}
			}
			else if(win2 && SDL_GetWindowID(win2->window) == event.window.windowID)
			{
				if(!SDLEventHandler(event, win2->window, win2->hWnd))
				{
					delete win2;
					win2 = nullptr;
				}
			}
		}
		if(win1) win1->Update();
		if(win2) win2->Update();
		
		dataBindTest = dataBindTest == "test1" ? "test2" : "test1";
		ctx.propeManager.PropertyChange(PropertyPath::Make("GName"));

		dataBindTest2 = dataBindTest2 == "test1" ? "test2" : "test1";
		ctx.propeManager.PropertyChange(PropertyPath::Make("GName2"));

		dataBindTest3 = dataBindTest3 == "test1" ? "test2" : "test1";
		ctx.propeManager.PropertyChange(PropertyPath::Make("GName3"));

		dataBindTest4 = dataBindTest4 == "test1" ? "test2" : "test1";
		ctx.propeManager.PropertyChange(PropertyPath::Make("GName4"));

	}

	ctx.propeManager.UnRegisterProperty(PropertyPath::Make("GName"));
	SDL_Quit();
	return 0;
}