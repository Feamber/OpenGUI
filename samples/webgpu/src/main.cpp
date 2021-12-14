#include "OpenGUI/Bind/EventArg.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/EventBase.h"
#include "OpenGUI/Style2/Selector.h"
#include "OpenGUI/Text/TextElement.h"
#include "appbase.h"
#include "webgpu.h"
#include "OpenGUI/Bind/Bind.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include <cctype>
#include <ctime>
#include <functional>
#include <assert.h>
#include <memory>
#include "SampleControls.h"
#include "luaBind.hpp"

std::unordered_map<uint32_t, OGUI::EKeyCode> gEKeyCodeLut;
extern void InstallInput();

class SampleWindow : public CSSWindow
{
public:
	SampleWindow(int width, int height, const char *title, ReloadManager* watcher, const char *xmlFile, std::function<void(OGUI::VisualElement*)> onReloadedEvent)
		:CSSWindow(width, height, title, watcher, xmlFile, onReloadedEvent)
	{
	};
};

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
		if(vertex_buffer) wgpuBufferRelease(vertex_buffer);
		if(index_buffer) wgpuBufferRelease(index_buffer);

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
			auto backBufView = wgpuSwapChainGetCurrentTextureView(swapchain);			
			wgpuTextureViewRelease(backBufView);
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
	
	void RenderPrimitives(const PrimDrawList& list, const class WindowContext& wctx) override
	{
		ZoneScopedN("WebGPU Render");
		
		if(list.command_list.size() <= 0) return;
		static size_t old_vb_size = 0;
		static size_t old_ib_size = 0;
		std::vector<Vertex> vertices = list.vertices;
		Vector2f resolution = {wctx.GetWidth(), wctx.GetHeight()};
		for(auto& vertex : vertices) //normalize
			vertex.position /= resolution;
		const auto this_vb_size = vertices.size() * sizeof(OGUI::Vertex);
		const auto this_ib_size = list.indices.size() * sizeof(OGUI::uint16_t);
		{
			ZoneScopedN("Prepare Buffers");
			// upload buffer
			if(old_vb_size < this_vb_size)
			{
				if (vertex_buffer) wgpuBufferRelease(vertex_buffer);
				vertex_buffer = createBuffer(device, queue, vertices.data(), this_vb_size, WGPUBufferUsage_Vertex);
				old_vb_size = this_vb_size;
			}
			else
			{
				writeBuffer(queue, vertex_buffer, vertices.data(), this_vb_size);
			}
			if(old_ib_size < this_ib_size)
			{
				if (index_buffer) wgpuBufferRelease(index_buffer);
				index_buffer = createBuffer(device, queue, list.indices.data(), this_ib_size, WGPUBufferUsage_Index);
				old_ib_size = this_ib_size;
			}
			else
			{
				writeBuffer(queue, index_buffer, list.indices.data(), this_ib_size);
			}
		}
		WGPUCommandEncoder encoder;
		{
			ZoneScopedN("Create Encoder");
			encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);			// create encoder
		}
		auto swapchain = registered_windows.find(wctx.GetWindowHandle())->second;
		{
			ZoneScopedN("Swapchain Presents");
		#ifndef __EMSCRIPTEN__
			/*
			* TODO: wgpuSwapChainPresent is unsupported in Emscripten, so what do we do?
			*/
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
			wgpuQueueSubmit(queue, 1, &commands);
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
		auto iter = ogui_textures.find(t);
		if(iter == ogui_textures.end())
			return;
		updateTexture(device, queue, &iter->second, bitmap);
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
	WGPUBuffer vertex_buffer = nullptr;
	WGPUBuffer index_buffer = nullptr;
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
		WGPUVertexAttribute vertAttrs[5] = {};
		vertAttrs[0].format = WGPUVertexFormat_Float32x2;
		vertAttrs[0].offset = offsetof(Vertex, position);
		vertAttrs[0].shaderLocation = 0;
		vertAttrs[1].format = WGPUVertexFormat_Float32x2;
		vertAttrs[1].offset = offsetof(Vertex, texcoord);
		vertAttrs[1].shaderLocation = 1;
		vertAttrs[2].format = WGPUVertexFormat_Unorm8x4;
		vertAttrs[2].offset = offsetof(Vertex, color);
		vertAttrs[2].shaderLocation = 2;
		vertAttrs[3].format = WGPUVertexFormat_Float32x2;
		vertAttrs[3].offset = offsetof(Vertex, clipUV);
		vertAttrs[3].shaderLocation = 3;
		WGPUVertexBufferLayout vertDesc = {};
		vertDesc.arrayStride = sizeof(Vertex);
		vertDesc.attributeCount = 4;
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

ReloadManager reloader;
SampleWindow* CreateNavigationTestWindow()
{
	return  new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "FocusNavigationTest", &reloader, "res/test_nav.xml", [](OGUI::VisualElement* ve)
	{
		ve->_pseudoMask |= PseudoStates::Root;
	});
}

SampleWindow* CreateCssTestWindow()
{
	return new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "CssTest", &reloader, "res/test.xml", [](OGUI::VisualElement* ve)
	{
		ve->_pseudoMask |= PseudoStates::Root;
		{
			std::vector<VisualElement*> tests;
			QueryAll(ve, ".Test", tests);
			for (auto [i, test] : ipair(tests))
				if (i % 2 == 0)
					test->_styleClasses.push_back("Bigger");
		}
	});
}

	static Name hour_ = "hour";
	static Name minute_ = "minute";
	static Name second_ = "second";
	static Name count_ = "count";

struct DataBindSample : public Bindable
{
	ostr::string hour = "00";
	ostr::string minute = "00";
	ostr::string second = "00";
	int count = 0;
	DataBindSample()
	{
		AddSource({hour_, &hour});
		AddSource({minute_, &minute});
		AddSource({second_, &second});
		AddSource({count_, &count});
		AddEventBind("Add", 
		[&](IEventArg& arg)
		{
			auto phase = arg.TryGet<EventRoutePhase>("currentPhase");
			if(!phase.has_value())
				return false;
			if(*phase == EventRoutePhase::Reach || *phase == EventRoutePhase::BubbleUp)
			{
				++count;
				Notify(count_);
				return true;
			}
			return false;
		});
	}

	SampleWindow* MakeWindow()
	{
		return new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "DataBindTest", &reloader, "res/DataBind.xml", [&](OGUI::VisualElement* ve)
		{
			ve->_pseudoMask |= PseudoStates::Root;
			VisualElement* test = QueryFirst(ve, "#AddButton");
			Bind(*test);
			//所有文字绑定到数据源上
			std::vector<VisualElement*> Texts;
			QueryAll(ve, "TextElement", Texts);
			for(auto Text : Texts)
				((TextElement*)Text)->Bind(*this);
		});
	}

	void Update()
	{
		char buffer[4];
		time_t t = time(0);

		strftime(buffer, sizeof(buffer), "%H", localtime(&t));
		hour = buffer;
		Notify(hour_);

		strftime(buffer, sizeof(buffer), "%M", localtime(&t));
		minute = buffer;
		Notify(minute_);

		strftime(buffer, sizeof(buffer), "%S", localtime(&t));
		second = buffer;
		Notify(second_);
	}
};
	static Name value_ = "value";

struct ExternalControlSample : public Bindable
{
	float value = 0.f;
	ExternalControlSample()
	{
		AddSource({value_, &value});
		AddBind({value_, &value, [&](bool valid)
		{
			olog::Info(u"value updated: {}"_o.format(value));
		}});
	}

	SampleWindow* MakeWindow()
	{
		return new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "ExternalControlTest", &reloader, "res/sample.xml", [&](OGUI::VisualElement* ve)
		{
			ve->_pseudoMask |= PseudoStates::Root;
			VisualElement* test = QueryFirst(ve, "#TestSlider");
			Bind(*test);
			std::vector<VisualElement*> allElement;
			QueryAll(ve, "*",  allElement);
			for(auto element : allElement)
				element->Bind(*this);
		});
	}
};

struct LuaSample
{
	sol::state lua;
	sol::table object;
	AppWindow* win;
	std::vector<std::shared_ptr<ReloadableXml>> xmls;
	Bindable cppDataModel;
	LuaSample()
	{
		lua.open_libraries(sol::lib::base, sol::lib::package);
		BindLua(lua.lua_state());
		lua.set_function("LoadXml", [this](const char *xmlFile, std::function<void(OGUI::VisualElement*)> onReloadedEvent)
		{
			LoadXml(xmlFile, onReloadedEvent);
		});
		cppDataModel.AddEventBind("Test", [](IEventArg& arg)
		{
			OGUI::any element = arg.TryGet("element");
			if(element.type() == typeid(VisualElement*))
				olog::Info(u"cpp event:{}"_o.format(OGUI::any_cast<VisualElement*>(element)->GetTypeName()));
			else
			 	olog::Info(u"cpp event: invalid argument(\"element\") type{}"_o.format(element.type().name()));
			return false;
		});
	}

	void LoadXml(const char *xmlFile, std::function<void(OGUI::VisualElement*)> onReloadedEvent)
	{
		xmls.push_back(std::make_shared<ReloadableXml>(&reloader, win->cWnd->GetWindowUI(), xmlFile, onReloadedEvent));
	}

	AppWindow* MakeWindow()
	{
		win = new AppWindow(WINDOW_WIN_W, WINDOW_WIN_H, "LuaBind");
		lua.script_file("res/LuaBind/main.lua");
		lua["main"](&cppDataModel);
		return win;
	}
};

struct XmlFiltersSample : public Bindable
{
	std::time_t a;
	std::time_t b;
	XmlFiltersSample()
	{
		
	}

	SampleWindow* MakeWindow()
	{
		return new SampleWindow(WINDOW_WIN_W, WINDOW_WIN_H, "xmlFiltersSample", &reloader, "res/xmlFilters.xml", [&](OGUI::VisualElement* ve)
		{
			ve->_pseudoMask |= PseudoStates::Root;
		});
	}

	void Update()
	{
		std::time_t now = std::time(0);

		auto& context = Context::Get();
		if(now != a && now % 5 == 0)
		{
			a = now;
			if(context.HasFilterTag("zh"))
				context.SetXmlFilter("language", "en");
			else
				context.SetXmlFilter("language", "zh");
		}

		if(now != b && now % 2 == 0)
		{
			b = now;
			if(context.HasFilterTag("pc"))
				context.SetXmlFilter("platform", "xbox");
			else
				context.SetXmlFilter("platform", "pc");
		}
	}
};

int main(int , char* []) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}
    RegisterBuiltinStructs();
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
	}

	std::vector<AppWindow*> windows;
	SampleControls::Install();
	ExternalControlSample sample;
	windows.push_back(sample.MakeWindow());
	LuaSample lsample;
	windows.push_back(lsample.MakeWindow());
	DataBindSample sample2;
	windows.push_back(sample2.MakeWindow());
	windows.push_back(CreateNavigationTestWindow());
	windows.push_back(CreateCssTestWindow());
	// main loop
	reloader.Watch();
	while(!windows.empty())
	{
		// sample3.Update();
		using namespace ostr::literal;
		
		ZoneScopedN("LoopBody");

		SDL_Event event;
		while (SDL_PollEvent(&event) && !windows.empty()) 
		{
			sample2.Update();
			//olog::Info(u"event type: {}  windowID: {}"_o, (int)event.type, (int)event.window.windowID);
			
			auto iter = std::remove_if(windows.begin(), windows.end(), [&](AppWindow* win)
			{
					if(SDL_GetWindowID(win->window) == event.window.windowID)
					{
						if(!SDLEventHandler(event, win->window, win))
						{
							delete win;
							return true;
						}
					}
					return false;
			});
			if(iter != windows.end())
				windows.erase(iter, windows.end());
		}
		for(auto win : windows)
			win->Update();
		reloader.Tick();
	}
	SDL_Quit();
	return 0;
}