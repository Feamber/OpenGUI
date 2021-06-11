#include "OpenGUI/Style/StyleSelector.h"
#include "utils.h"
#include <string.h>
#include <unordered_map>

#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style/VisualStyleSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/CSSParser/CSSParser.h"
#include "OpenGUI/Xml/XmlAsset.h"
#include "OpenGUI/VisualWindow.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Core/DynamicAtlasResource.h"
#include "efsw/efsw.hpp"

#include "OpenGUI/Text/FontRendering.h"

extern void InstallInput();

using namespace OGUI;
WGPUDevice device;
WGPUQueue queue;
WGPUSwapChain swapchain;
WGPURenderPipeline pipeline;
WGPUBindGroupLayout bindGroupLayout;
WGPUSampler sampler;
std::unique_ptr<Font::TextureFont> white_tex = std::unique_ptr<Font::TextureFont>(
	Font::TextureFont::TextureFontFromFile(1024, 1024, PF_R8, "Vera.ttf", 128)
);
WGPU_OGUI_Texture* default_ogui_texture;

std::unordered_map<TextureInterface*, WGPU_OGUI_Texture> ogui_textures;
window::Handle hWnd;

struct BitmapParser final : public OGUI::BitmapParserInterface
{
    inline Bitmap LoadFromFile(const FileHandle file)
    {
		Bitmap bm = {};
		int x, y, n;
		stbi_info_from_file((FILE*)file, &x, &y, &n);
		bm.format = n == 1 ? PF_R8 : OGUI::PF_R8G8B8A8;
		const auto channels = (n == 1) ? 1 : 4;

		auto data = stbi_load_from_file((FILE*)file, &x, &y, &n, channels);
		bm.resource.bytes = data;
		bm.resource.size_in_bytes = x * y * channels * sizeof(*data);
		bm.height = y;
		bm.width = x;

		return bm;
    }
    inline void Free(Bitmap bm)
    {
        stbi_image_free(bm.resource.bytes);
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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include <Windows.h>
struct SpdlogLogger : LogInterface
{
	SpdlogLogger()
	{
		auto console_sink = get_console_sink();
		console_sink->set_level(spdlog::level::info);
		console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");

		auto file_sink = get_file_sink();
		file_sink->set_level(spdlog::level::trace);
		file_sink->set_pattern("[%Y-%m-%d %z %X.(%F)] [%^%l%$] [%P.%t] %v");

		auto logger = std::make_shared<spdlog::logger>("logger_default", spdlog::sinks_init_list{ console_sink, file_sink });
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::info);
		spdlog::set_default_logger(logger);

		SetConsoleOutputCP(65001);
	}

	virtual void Log(olog::Level l, ostr::string_view msg)
	{
		std::wstring str(msg.raw().cbegin(), msg.raw().cend());
		spdlog::level::level_enum spdlevel = 
		[l]
		{
			if (l == olog::Level::None) return spdlog::level::off;
			return (spdlog::level::level_enum)((uint8_t)l - 1);
		}();
		spdlog::log(spdlevel, std::wstring_view(str));
	}

	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> get_console_sink()
	{
		static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _static = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		return _static;
	}

	std::shared_ptr<spdlog::sinks::daily_file_sink_mt> get_file_sink()
	{
		static std::shared_ptr<spdlog::sinks::daily_file_sink_mt> _static = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
			"Daily.log",        // file name
			0,                  // hour
			0,                  // minute
			false,              // truncate
			7                   // max files
			);
		return _static;
	}
};

/**
 * Bare minimum pipeline to draw a triangle using the above shaders.
 */
 uint8_t white_tex0[1024 * 1024 * 4];
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

	// Font Test
	{
		white_tex->CreateGlyph(u8"a");
		white_tex->CreateGlyph(u8"b");
		white_tex->CreateGlyph(u8"c");
		white_tex->CreateGlyph(u8"d");
		white_tex->CreateGlyph(u8"e");
		white_tex->CreateGlyph(u8"f");
		white_tex->CreateGlyph(u8"g");
		white_tex->CreateGlyph(u8"1");
		white_tex->CreateGlyph(u8"2");
		white_tex->CreateGlyph(u8"3");
		white_tex->CreateGlyph(u8"4");
		white_tex->CreateGlyph(u8",");
		white_tex->CreateGlyph(u8".");
	}
	//Bitmap bitmap = white_tex->GetAtlas()->GetBitmap();
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

bool ReloadCSS = false;
bool ReloadXML = false;
void OnReloaded();
/**
 * Draws using the above pipeline and buffers.
 */
static bool redraw() {
	static std::chrono::time_point prev = std::chrono::high_resolution_clock::now();
	auto& ctx = OGUI::Context::Get();
	std::chrono::time_point now = std::chrono::high_resolution_clock::now();
	using namespace ostr::literal;
	if (ReloadXML)
	{
		std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
		//auto asset = XmlAsset::LoadXmlFile("res/test.xml");
		auto asset = XmlAsset::LoadXmlFile("res/test_nav.xml");

		if(asset)
		{
			auto newVe = asset->Instantiate();
			if (newVe)
			{
				auto ve = ctx.desktops->_children[0];
				ctx.desktops->RemoveChild(ve);
				VisualElement::DestoryTree(ve);
				ctx.desktops->PushChild(newVe);
				OnReloaded();
				ctx._keyboardFocused = ctx.desktops;
				ctx._layoutDirty = true;
			}
		}
		ReloadXML = false;
		ReloadCSS = false;
		olog::Info(u"xml reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
	}
	else if(ReloadCSS)
	{
		std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
		//auto asset = ParseCSSFile("res/test.css");
		auto asset = ParseCSSFile("res/test_nav.css");
		if (asset)
		{
			auto ve = ctx.desktops->_children[0];
			*ve->_styleSheets[0] = asset.value();
			ve->_styleSheets[0]->Initialize();
			ctx._layoutDirty = true;
			ctx.styleSystem.InvalidateCache();
			ve->_selectorDirty = true;
		}
		olog::Info(u"css reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
		ReloadCSS = false;
	}
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


#define SDL_MAIN_NEEDED
#include <SDL2/SDL_main.h>

#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#include "OpenGUI/Core/olog.h"
#if defined(_WIN32) || defined(_WIN64)
#define SDL_MAIN_HANDLED
#ifndef __WIN32__
#define __WIN32__
#endif // __WIN32__
#endif

class UpdateListener : public efsw::FileWatchListener
{
public:
	UpdateListener() {}

	void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
	{
		switch (action)
		{
			case efsw::Actions::Add:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
				break;
			case efsw::Actions::Delete:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
				break;
			case efsw::Actions::Modified:
				if (filename == "test_nav.css")
				{
					ReloadCSS = true;
				}
				if (filename == "test_nav.xml" )
				{
					ReloadXML = true;
				}
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
				break;
			case efsw::Actions::Moved:
				std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
				break;
			default:
				std::cout << "Should never happen!" << std::endl;
		}
	}
};

void OnReloaded()
{
	auto ve = Context::Get().desktops->_children[0];
	if (auto child2 = QueryFirst(ve, "#Child2"))
	{
		constexpr auto handler = +[](PointerDownEvent& event, VisualElement& element)
		{
			if(event.currentPhase == EventRoutePhase::Reach)
				Context::Get().SetFocus(&element);

			using namespace ostr::literal;
			olog::Info(u"Oh ♂ shit! Child2"_o);
			return true;
		};

		child2->_eventHandler.Register<PointerDownEvent, handler>(*child2);
	}

	{
		std::vector<VisualElement*> tests;
		QueryAll(ve, ".Element", tests);
		for (auto [i, test] : ipair(tests))
		{
			constexpr auto handler = +[](PointerDownEvent& event, VisualElement& element)
			{
				if(event.currentPhase == EventRoutePhase::Reach)
					Context::Get().SetFocus(&element);
				return true;
			};
			test->_eventHandler.Register<PointerDownEvent, handler>(*test);
		}
	}

	if (auto child1 = QueryFirst(ve, "#Child1"))
	{
		constexpr auto handler = +[](PointerDownEvent& event, VisualElement& element)
		{
			if(event.currentPhase == EventRoutePhase::Reach)
				Context::Get().SetFocus(&element);

			using namespace ostr::literal;
			olog::Info(u"Oh ♂ shit!"_o);
			return true;
		};
		constexpr auto handlerDown = +[](KeyDownEvent& event)
		{
			using namespace ostr::literal;
			if (event.key == EKeyCode::W)
			{
				olog::Info(u"W is Down!"_o);
			}
			return false;
		};
		constexpr auto handlerUp = +[](KeyUpEvent& event)
		{
			using namespace ostr::literal;
			if (event.key == EKeyCode::W)
			{
				olog::Info(u"W is up!"_o);
			}
			return false;
		};
		constexpr auto handlerHold = +[](KeyHoldEvent& event)
		{
			using namespace ostr::literal;
			if (event.key == EKeyCode::W)
			{
				olog::Info(u"W is Holding!"_o);
			}
			return false;
		};
		child1->_eventHandler.Register<PointerDownEvent, handler>(*child1);
		child1->_eventHandler.Register<KeyDownEvent, handlerDown>();
		child1->_eventHandler.Register<KeyUpEvent, handlerUp>();
		child1->_eventHandler.Register<KeyHoldEvent, handlerHold>();
	}
	{
		std::vector<VisualElement*> tests;
		QueryAll(ve, ".Test", tests);
		for (auto [i, test] : ipair(tests))
			if (i % 2 == 0)
				test->_styleClasses.push_back("Bigger");
	}

	ve->_pseudoMask |= (int)PseudoStates::Root;
}

void LoadResource()
{
	using namespace OGUI;
	auto& ctx = Context::Get();
	//auto asset = XmlAsset::LoadXmlFile("res/test.xml");
	auto asset = XmlAsset::LoadXmlFile("res/test_nav.xml");
	auto ve = asset->Instantiate();
	ctx.desktops->PushChild(ve);
	OnReloaded();
	static efsw::FileWatcher fileWatcher;
	static UpdateListener listener;
	efsw::WatchID watchID = fileWatcher.addWatch("res", &listener, true);
	fileWatcher.watch();
}

int main(int /*argc*/, char* /*argv*/[]) {
	int window_width = WINDOW_WIN_W;
	int window_height = WINDOW_WIN_H;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Demo",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, 0
	);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
#if defined(_WIN32) || defined(_WIN64)
	hWnd = (window::Handle)wmInfo.info.win.window;
#endif
	if (hWnd) {
		if (device = webgpu::create(hWnd);device) {
			queue = wgpuDeviceGetDefaultQueue(device);
			swapchain = webgpu::createSwapChain(device, window_width, window_height);
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
				ctx.logImpl = std::make_unique<SpdlogLogger>();
				LoadResource();

				ctx.ActivateWindow(ctx.desktops);
				BuildSDLMap();
			}

			// main loop
			bool done = false;
			while(!done)
			{
				using namespace ostr::literal;

				SDL_Event event;
				while (SDL_PollEvent(&event)) 
				{
					done = !SDLEventHandler(event, window);
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
