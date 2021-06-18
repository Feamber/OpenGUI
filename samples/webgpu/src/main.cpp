#include "OpenGUI/Style/StyleSelector.h"
#include "utils.h"
#include <functional>
#include <filesystem>
#include <memory>
#include <string_view>
#include <string.h>
#include <unordered_map>
#include <vector>

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
#include "webgpu.h"


extern void InstallInput();

using namespace OGUI;

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

class Window;



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
	std::function<void(efsw::WatchID, const std::string&, const std::string&, efsw::Action, std::string)> handle;

	UpdateListener() {};

	void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
	{
		handle(watchid, dir, filename, action, oldFilename);
	}
};

class OGUIWebGPURenderer;
std::shared_ptr<OGUIWebGPURenderer> MakeRenderer(class Window& window);

//TODO 现在还是只支持一个窗口，还需要等待渲染接口和hWnd支持多窗口
class Window
{
public:
	SDL_Window* window;
	SDL_SysWMinfo wmInfo;
	WindowContext* cWnd;
	window::Handle hWnd;
	WGPUDevice device;
	WGPUQueue queue;
	WGPUSwapChain swapchain;
	WGPURenderPipeline pipeline;
	WGPUBindGroupLayout bindGroupLayout;
	WGPUSampler sampler;
	WGPU_OGUI_Texture* default_ogui_texture;
	std::unordered_map<TextureInterface*, WGPU_OGUI_Texture> ogui_textures;
	webgpu::instance* wgpu;

	efsw::FileWatcher fileWatcher;
	UpdateListener listener;

	std::string mainXmlFile;
	std::vector<std::string> allCssFile;
    std::vector<std::string> allXmlFile;

	// Bare minimum pipeline to draw a triangle using the above shaders.
 	uint8_t white_tex0[1024 * 1024 * 4];

	Window(int width, int height, const char *title, const char *xmlFile)
	{
		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, 0
		);

		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);

#if defined(_WIN32) || defined(_WIN64)
		hWnd = (window::Handle)wmInfo.info.win.window;
#endif

		if (hWnd)
		{
			if(wgpu = webgpu::create(hWnd); wgpu)
			{
				device = getDevice(wgpu);
				queue = wgpuDeviceGetDefaultQueue(device);
				swapchain = webgpu::createSwapChain(wgpu, width, height);
				createPipelineAndBuffers();

				using namespace OGUI;
				auto &ctx = Context::Get();
				cWnd = &ctx.Create(hWnd);
				cWnd->renderImpl = std::static_pointer_cast<RenderInterface>(MakeRenderer(*this));
				LoadResource(xmlFile);
			}
        }
	};

	~Window()
	{
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
			webgpu::release(wgpu);
		#endif
		
		SDL_DestroyWindow(window);
	}

	bool Update()
	{
		static std::chrono::time_point prev = std::chrono::high_resolution_clock::now();
		auto& ctx = OGUI::Context::Get();
		std::chrono::time_point now = std::chrono::high_resolution_clock::now();
		using namespace ostr::literal;
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - prev).count();
		prev = now;
		ctx.Update(hWnd, deltaTime);
		ctx.Render(hWnd);
		return true;
	};

	void OnReloaded()
	{
		auto ve = cWnd->GetWindowUI()->_children[0];
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

	void LoadResource(const char *xmlFile)
	{
		using namespace OGUI;
		auto& ctx = Context::Get();
		auto asset = XmlAsset::LoadXmlFile(xmlFile);
		auto ve = asset->Instantiate();

		mainXmlFile = xmlFile;
		allCssFile = asset->all_css_file;
		allXmlFile = asset->all_xml_file;

		for(auto child : cWnd->GetWindowUI()->_children)
		{
			cWnd->GetWindowUI()->RemoveChild(child);
			VisualElement::DestoryTree(child);
		}
		cWnd->GetWindowUI()->PushChild(ve);
		OnReloaded();

		listener.handle = [this](efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
		{
			auto& ctx = Context::Get();
			switch (action)
			{
				case efsw::Actions::Add:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
					break;
				case efsw::Actions::Delete:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
					break;
				case efsw::Actions::Modified:
				{
					auto path = std::filesystem::path(dir.substr(0, dir.length() - 1) + filename);
					if (find(allXmlFile.begin(), allXmlFile.end(), path) != allXmlFile.end())
					{
						std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
						auto asset = XmlAsset::LoadXmlFile(mainXmlFile);

						if(asset)
						{
							auto newVe = asset->Instantiate();
							if (newVe)
							{
								for(auto child : cWnd->GetWindowUI()->_children)
								{
									cWnd->GetWindowUI()->RemoveChild(child);
									VisualElement::DestoryTree(child);
								}
								cWnd->GetWindowUI()->PushChild(newVe);
								OnReloaded();
								ctx._layoutDirty = true;
							}
						}
						olog::Info(u"xml reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
					}
					else if (find(allCssFile.begin(), allCssFile.end(), path) != allCssFile.end())
					{
						std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
						auto asset = ParseCSSFile(path.string());
						if (asset)
						{
							std::vector<VisualElement*> current {cWnd->GetWindowUI()->_children};
							std::vector<VisualElement*> next;
							while (current.size() > 0) 
							{
								for (auto element : current)
								{
									for(auto& styleSheet : element->_styleSheets)
									{
										if(styleSheet->path == path)
										{
											*styleSheet = asset.value();
											styleSheet->Initialize();
											element->_selectorDirty = true;
										}
									}

									next.insert(next.end(), element->_children.begin(), element->_children.end());
								}
								current.clear();
								std::swap(current, next);
							}
							ctx._layoutDirty = true;
							ctx.styleSystem.InvalidateCache();
						}
						olog::Info(u"css reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
					}
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
					break;
				}
				case efsw::Actions::Moved:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
					break;
				default:
					std::cout << "Should never happen!" << std::endl;
			}
		};

		for(auto dir : fileWatcher.directories())
			fileWatcher.removeWatch(dir);
		for(auto filePath : allCssFile)
			fileWatcher.addWatch(std::filesystem::path(filePath).remove_filename().string(), &listener, true);
		for(auto filePath : allXmlFile)
			fileWatcher.addWatch(std::filesystem::path(filePath).remove_filename().string(), &listener, true);

		fileWatcher.watch();
	}

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
		colorDesc.format = webgpu::getSwapChainFormat(wgpu);
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


class OGUIWebGPURenderer : public OGUI::RenderInterface
{
public:
	Window& window;

	OGUIWebGPURenderer(Window& window) 
	: window(window)
	{

	}

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
		((PrimDrawList&)list).ValidateAndBatch();
		if(list.command_list.size() <= 0) return;
		// upload buffer
		if(vertex_buffer) 
			wgpuBufferRelease(vertex_buffer);
		if(index_buffer) 
			wgpuBufferRelease(index_buffer);

		vertex_buffer = createBuffer(window.device, window.queue,
			list.vertices.data(), list.vertices.size() * sizeof(OGUI::Vertex), WGPUBufferUsage_Vertex);
		index_buffer = createBuffer(window.device, window.queue,
			list.indices.data(), list.indices.size() * sizeof(uint16_t), WGPUBufferUsage_Index);

		WGPUTextureView backBufView = wgpuSwapChainGetCurrentTextureView(window.swapchain);			// create textureView
		WGPURenderPassColorAttachmentDescriptor colorDesc = {};
		colorDesc.attachment = backBufView;
		colorDesc.loadOp  = WGPULoadOp_Clear;
		colorDesc.storeOp = WGPUStoreOp_Store;
		colorDesc.clearColor = {0.3f, 0.3f, 0.3f, 1.f};
		WGPURenderPassDescriptor renderPass = {};
		renderPass.colorAttachmentCount = 1;
		renderPass.colorAttachments = &colorDesc;
		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(window.device, nullptr);			// create encoder
		WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);	// create pass
		// draw the triangle (comment these five lines to simply clear the screen)
		wgpuRenderPassEncoderSetPipeline(pass, window.pipeline);

		int last_index = -1;
		for(auto& cmd : list.command_list)
		{
			auto& last_cmd = list.command_list[last_index < 0 ? 0 : last_index];
			if(last_cmd.texture != cmd.texture || last_index < 0)
			{
				WGPU_OGUI_Texture* texture = (WGPU_OGUI_Texture*)cmd.texture;
				if(!texture)
					texture = window.default_ogui_texture;
				if(!texture->bind_group)
				{
					// update texture binding
					WGPUBindGroupEntry bgEntry[2] = {{}, {}};
					bgEntry[0].binding = 0;
					bgEntry[0].textureView 
						= texture->texture? texture->texture_view : window.default_ogui_texture->texture_view;
					bgEntry[1].binding = 1;
					bgEntry[1].sampler = window.sampler;
					WGPUBindGroupDescriptor bgDesc = {};
					bgDesc.layout = window.bindGroupLayout;
					bgDesc.entryCount = 2;
					bgDesc.entries = bgEntry;
					texture->bind_group = wgpuDeviceCreateBindGroup(window.device, &bgDesc);
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

		wgpuQueueSubmit(window.queue, 1, &commands);
		wgpuCommandBufferRelease(commands);														// release commands
	#ifndef __EMSCRIPTEN__
		/*
		* TODO: wgpuSwapChainPresent is unsupported in Emscripten, so what do we do?
		*/
		wgpuSwapChainPresent(window.swapchain);
	#endif
		wgpuTextureViewRelease(backBufView);													
	}

	void RenderPrimitives(const PersistantPrimDrawList&)
	{
		
	}

	TextureHandle RegisterTexture(const Bitmap& bitmap)
	{
		WGPU_OGUI_Texture* t = createTexture(window.device, window.queue, bitmap);
		window.ogui_textures[t] = *t;
		return t;
	}

	void ReleaseTexture(TextureHandle h)
	{
		if(h)
			window.ogui_textures.erase(h);
		window.ogui_textures[h].Release();
	}

	void SetScissor(const Scissor scissor)
	{

	}

	void ResetScissor()
	{

	}
	WGPUBuffer vertex_buffer = nullptr;
	WGPUBuffer index_buffer = nullptr;
};


std::shared_ptr<OGUIWebGPURenderer> MakeRenderer(class Window& window)
{
	return std::make_shared<OGUIWebGPURenderer>(window);
}

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


/*
int main(int , char* []) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	Window* win1 = new Window(WINDOW_WIN_W, WINDOW_WIN_H, "FocusNavigationTest", "res/test_nav.xml");

	InstallInput();
	{
		using namespace OGUI;
		using namespace ostr::literal;
		auto& ctx = Context::Get();
		ctx.bmParserImpl = std::make_unique<BitmapParser>();
		ctx.fileImpl = std::make_unique<OGUI::FileInterface>();
		ctx.logImpl = std::make_unique<SpdlogLogger>();
	}
	BuildSDLMap();

	// main loop
	while(win1)
	{
		using namespace ostr::literal;

		SDL_Event event;
		while (SDL_PollEvent(&event) && (win1)) 
		{
			olog::Info(u"event type: {}  windowID: {}"_o, (int)event.type, (int)event.window.windowID);
			// TODO 关闭事件中没有窗口id？ 导致现在关闭没反应
			if(win1 && SDL_GetWindowID(win1->window) == event.window.windowID)
			{
				if(!SDLEventHandler(event, win1->window, win1->hWnd))
				{
					delete win1;
					win1 = nullptr;
				}
			}
		}
		if(win1) win1->Update();
	}

	
	//delete win2;
	SDL_Quit();
	return 0;
}
*/

int main(int , char* []) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	Window* win1 = new Window(WINDOW_WIN_W, WINDOW_WIN_H, "FocusNavigationTest", "res/test_nav.xml");
	// TODO 现在还不能多窗口
	Window* win2 = new Window(WINDOW_WIN_W, WINDOW_WIN_H, "CssTest", "res/test.xml");

	InstallInput();
	{
		using namespace OGUI;
		using namespace ostr::literal;
		auto& ctx = Context::Get();
		ctx.bmParserImpl = std::make_unique<BitmapParser>();
		ctx.fileImpl = std::make_unique<OGUI::FileInterface>();
		ctx.logImpl = std::make_unique<SpdlogLogger>();
	}
	BuildSDLMap();

	// main loop
	while(win1 || win2)
	{
		using namespace ostr::literal;

		SDL_Event event;
		while (SDL_PollEvent(&event) && (win1 || win2)) 
		{
			olog::Info(u"event type: {}  windowID: {}"_o, (int)event.type, (int)event.window.windowID);
			// TODO 关闭事件中没有窗口id？ 导致现在关闭没反应
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
	}

	
	//delete win2;
	SDL_Quit();
	return 0;
}