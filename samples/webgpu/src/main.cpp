#include "OpenGUI/Core/PrimitiveDraw.h"
#include "appbase.h"
#include "webgpu.h"

extern void InstallInput();

class SampleWindow : public CSSWindow
{
public:
	SampleWindow(int width, int height, const char *title, const char *xmlFile)
		:CSSWindow(width, height, title, xmlFile)
	{
		auto&& ctx = Context::Get();
		ctx.renderImpl->RegisterWindow(*cWnd);
	};
};



#define NUM_FRAMES 3
class OGUIWebGPURenderer final : public OGUI::RenderInterface
{
public:
	webgpu::instance* wgpu;
	WGPUDevice device;
	WGPUQueue queue;
	WGPUBindGroupLayout bindGroupLayout;
	WGPURenderPipeline pipeline;

	WGPUSampler sampler;
	WGPU_OGUI_Texture* default_ogui_texture;
	std::unordered_map<TextureInterface*, WGPU_OGUI_Texture> ogui_textures;
	std::unordered_map<WindowHandle, WGPUSwapChain> registered_windows;
	uint8_t white_tex0[1024 * 1024 * 4];

	OGUIWebGPURenderer() 
	{
		const auto WGPUDeviceSet = InitializeWGPUDevice();
		wgpu = std::get<0>(WGPUDeviceSet);
		device = std::get<1>(WGPUDeviceSet);
		queue = std::get<2>(WGPUDeviceSet);

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

		memset(white_tex0, 255, 4 * 1024 * 1024 * sizeof(uint8_t)); // pure white
		Bitmap bitmap = {};
		bitmap.resource.bytes = white_tex0;
		bitmap.resource.size_in_bytes = 4 * 1024 * 1024;
		bitmap.width = 1024; bitmap.height = 1024;
		bitmap.format = PF_R8G8B8A8;
		WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
		ogui_textures[t] = *t;
		default_ogui_texture = t;

		createPipelineAndBuffers();
	}

	virtual ~OGUIWebGPURenderer()
	{
		for(auto vertex_buffer : vertex_buffers)
		{
			if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		}
		for(auto index_buffer : index_buffers)
		{
			if(index_buffer) wgpuBufferRelease(index_buffer);
		}

		#ifndef __EMSCRIPTEN__
			wgpuRenderPipelineRelease(pipeline);
			// last bit of clean-up
			for(auto& tex : ogui_textures)
			{
				tex.second.Release();
			}
			ogui_textures.clear();
			// last bit of clean-up
			wgpuSamplerRelease(sampler);
			wgpuBindGroupLayoutRelease(bindGroupLayout);
			wgpuQueueRelease(queue);
			//wgpuDeviceRelease(device);
			webgpu::release(wgpu);
		#endif
	}
	
	PersistantPrimitiveHandle RegisterPrimitive(
		Vertex* vertices, uint32_t num_vertices,
		uint16_t* indices, uint32_t num_indices) override
	{
		return nullptr;
	}
	
	void RegisterWindow(const class WindowContext& wctx) override
	{
		const auto hdl = wctx.GetWindowHandle();
		if (registered_windows.find(hdl) != registered_windows.end())
		{
			return;
		}
		const SampleWindow* win = (const SampleWindow*)hdl;
		const auto& wmInfo = win->wmInfo;
		const auto width = win->GetWidth();
		const auto height = win->GetHeight();
		auto swapchain = webgpu::createSwapChain(wgpu, width, height, win->wmInfo.info.win.window, wmInfo.info.win.hinstance);
		{
			//auto backBufView = wgpuSwapChainGetCurrentTextureView(swapchain);			
			//wgpuTextureViewRelease(backBufView);
		}
		registered_windows[hdl] = swapchain;
	}

	void ReleaseWindow(const class WindowContext& wctx) override
	{
		const auto hdl = wctx.GetWindowHandle();
		const auto iter = registered_windows.find(hdl);
		if (iter != registered_windows.end())
		{
			wgpuSwapChainRelease(iter->second);
			registered_windows.erase(hdl);
		}
	}

	void ReleasePrimitive(PersistantPrimitiveHandle primitive) override
	{
		
	}
	int this_frame = -2;
	void RenderPrimitives(const PrimDrawList& list, const class WindowContext& wctx) override
	{
		ZoneScopedN("WebGPU Render");
		auto swapchain = registered_windows.find(wctx.GetWindowHandle())->second;

		if(list.command_list.size() <= 0) return;
		static size_t old_vb_sizes[NUM_FRAMES] = {0, 0, 0};
		static size_t old_ib_sizes[NUM_FRAMES] = {0, 0, 0};
		const auto this_vb_size = list.vertices.size() * sizeof(OGUI::Vertex);
		const auto this_ib_size = list.indices.size() * sizeof(OGUI::uint16_t);
		std::atomic_bool render_finish_signals[NUM_FRAMES] = {true, true, true};
		{
			ZoneScopedN("Prepare Buffers");
			const auto upload_frame = (this_frame + 2) % NUM_FRAMES;
			// upload buffer
			if(old_vb_sizes[upload_frame] < this_vb_size)
			{
				if (vertex_buffers[upload_frame]) wgpuBufferRelease(vertex_buffers[upload_frame]);
				vertex_buffers[upload_frame] = createBuffer(device, queue, list.vertices.data(), this_vb_size, WGPUBufferUsage_Vertex);
				old_vb_sizes[upload_frame] = this_vb_size;
			}
			else
			{
				writeBuffer(queue, vertex_buffers[upload_frame], list.vertices.data(), this_vb_size);
			}
			if(old_ib_sizes[upload_frame] < this_ib_size)
			{
				if (index_buffers[upload_frame]) wgpuBufferRelease(index_buffers[upload_frame]);
				index_buffers[upload_frame] = createBuffer(device, queue, list.indices.data(), this_ib_size, WGPUBufferUsage_Index);
				old_ib_sizes[upload_frame] = this_ib_size;
			}
			else
			{
				writeBuffer(queue, index_buffers[upload_frame], list.indices.data(), this_ib_size);
			}
			if(this_frame < 0) 
			{
				this_frame = (this_frame + 1) % NUM_FRAMES;
				return;
			}
		}
		WGPUCommandEncoder encoder;
		{
			ZoneScopedN("Create Encoder");
			encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);			// create encoder
		}
		{
			ZoneScopedN("Swapchain Presents");
		#ifndef __EMSCRIPTEN__
			wgpuSwapChainPresent(swapchain);
		#endif
		}
		WGPUTextureView backBufView;
		{
			ZoneScopedN("Fetch Backview");
			backBufView = wgpuSwapChainGetCurrentTextureView(swapchain);			// create textureView
		}
		WGPURenderPassEncoder pass;
		{
			ZoneScopedN("Begin Pass");

			WGPURenderPassColorAttachment colorDesc = {};
			colorDesc.view = backBufView;
			colorDesc.loadOp  = WGPULoadOp_Clear;
			colorDesc.storeOp = WGPUStoreOp_Store;
			colorDesc.clearColor = {0.3f, 0.3f, 0.3f, 1.f};
			WGPURenderPassDescriptor renderPass = {};
			renderPass.colorAttachmentCount = 1;
			renderPass.colorAttachments = &colorDesc;
			pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);	// create pass
		}
		{
			ZoneScopedN("Set Pipeline");
			// draw the triangle (comment these five lines to simply clear the screen)
			wgpuRenderPassEncoderSetPipeline(pass, pipeline);
		}

		int last_index = -1;
		{
			ZoneScopedN("Prepare DrawCalls");

			for(auto& cmd : list.command_list)
			{
				auto& last_cmd = list.command_list[last_index < 0 ? 0 : last_index];
				if(last_cmd.texture != cmd.texture || last_index < 0)
				{
					WGPU_OGUI_Texture* texture = (WGPU_OGUI_Texture*)cmd.texture;
					if(!texture)
						texture = default_ogui_texture;
					if(texture != default_ogui_texture && 
						ogui_textures.find(texture) == ogui_textures.end())
					{
						olog::Error(u"WTF?"_o);
						continue;
					}

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
				wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertex_buffers[this_frame],
					cmd.vertex_offset * sizeof(Vertex),
					WGPU_WHOLE_SIZE
				);
				wgpuRenderPassEncoderSetIndexBuffer(pass, index_buffers[this_frame], WGPUIndexFormat_Uint16,
					cmd.index_offset * sizeof(uint16_t),
					WGPU_WHOLE_SIZE
				);
				wgpuRenderPassEncoderDrawIndexed(pass, cmd.element_count, 1, 0, 0, 0);
				last_index++;
			}
		}
		WGPUCommandBuffer commands;
		{
			ZoneScopedN("End Pass");
			wgpuRenderPassEncoderEndPass(pass);
		}
		{
			ZoneScopedN("Create Commands");
			commands = wgpuCommandEncoderFinish(encoder, nullptr);				// create commands
		}
		{
			ZoneScopedN("Submit Commands");
			WGPUQueueWorkDoneCallback callback = +[](WGPUQueueWorkDoneStatus status, void * userdata)
			{
				auto render_finish_signal = (std::atomic_bool*)userdata;
				render_finish_signal->store(true);
			};
			wgpuQueueOnSubmittedWorkDone(queue, 0u, callback, &render_finish_signals[this_frame]);
			{
				ZoneScopedN("Wait Last");
				while(!render_finish_signals[this_frame])
				{
					// Wait for last submission.
				}
			}
			render_finish_signals[this_frame].store(false);
			wgpuQueueSubmit(queue, 1, &commands);
			this_frame = (this_frame + 1) % NUM_FRAMES;
		}
		{
			ZoneScopedN("Release Resources");

			wgpuRenderPassEncoderRelease(pass);														// release pass
			wgpuCommandEncoderRelease(encoder);														// release encoder
			wgpuCommandBufferRelease(commands);														// release commands
			wgpuTextureViewRelease(backBufView);
		}

	}

	void RenderPrimitives(const PersistantPrimDrawList&, const class WindowContext&) override
	{
		
	}

	TextureHandle RegisterTexture(const Bitmap& bitmap) override
	{
		WGPU_OGUI_Texture* t = createTexture(device, queue, bitmap);
		ogui_textures[t] = *t;
		return t;
	}

	void UpdateTexture(TextureHandle t, const Bitmap& bitmap) override
	{
		updateTexture(device, queue, &ogui_textures[t], bitmap);
	}

	void ReleaseTexture(TextureHandle h) override
	{
		if(!h)
			return;
		auto iter = ogui_textures.find(h);
		if(iter == ogui_textures.end())
			return;
		iter->second.Release();
		ogui_textures.erase(iter);
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
	WGPUBuffer vertex_buffers[NUM_FRAMES] = {nullptr, nullptr, nullptr};
	WGPUBuffer index_buffers[NUM_FRAMES] = {nullptr, nullptr, nullptr};
protected:
	void createPipelineAndBuffers()
	{
		// compile shaders
		// NOTE: these are now the WGSL shaders (tested with Dawn and Chrome Canary)
		WGPUShaderModule vertMod = createShader(device, triangle_vert_wgsl);
		WGPUShaderModule fragMod = createShader(device, triangle_frag_wgsl);

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
	}
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
	InstallLogger();	
	InstallInput();
	InstallBitmapParser();
	{
		ctx.renderImpl = std::make_unique<OGUIWebGPURenderer>();
		ctx.textureManager = std::make_unique<RenderTextureManager>();
		ctx.fileImpl = std::make_unique<OGUI::FileInterface>();
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest, "GName");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest2, "GName2");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest3, "GName3");
		ctx.propeManager.RegisterProperty(PropertyPtr(), &dataBindTest4, "GName4");
	}

	SampleWindow* win1 = nullptr;//new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "FocusNavigationTest", "res/test_nav.xml");
	SampleWindow* win2 = new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "CssTest", "res/test.xml");

	// main loop
	while(win1 || win2)
	{
		using namespace ostr::literal;
		
		ZoneScopedN("LoopBody");

		SDL_Event event;
		while (SDL_PollEvent(&event) && (win1 || win2)) 
		{
			//olog::Info(u"event type: {}  windowID: {}"_o, (int)event.type, (int)event.window.windowID);
			if(win1 && SDL_GetWindowID(win1->window) == event.window.windowID)
			{
				if(!SDLEventHandler(event, win1->window, win1))
				{
					delete win1;
					win1 = nullptr;
				}
			}
			else if(win2 && SDL_GetWindowID(win2->window) == event.window.windowID)
			{
				if(!SDLEventHandler(event, win2->window, win2))
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
		FrameMark;
	}

	ctx.propeManager.UnRegisterProperty(PropertyPath::Make("GName"));
	SDL_Quit();
	return 0;
}