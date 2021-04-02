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
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
#include "efsw/efsw.hpp"

#include "OpenGUI/Event/KeyEvent.h"

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
		auto asset = XmlAsset::LoadXmlFile("res/test.xml");
		
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
		auto asset = ParseCSSFile("res/test.css");
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

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#define SDL_MAIN_NEEDED
#include <SDL2/SDL_main.h>

std::unordered_map<uint32, EKeyCode> gEKeyCodeLut;
static void BuildSDLMap()
{
	gEKeyCodeLut[SDLK_BACKSPACE] = EKeyCode::Backspace;
	gEKeyCodeLut[SDLK_TAB] = EKeyCode::Tab;
	gEKeyCodeLut[SDLK_CLEAR] = EKeyCode::Clear;
	gEKeyCodeLut[SDLK_LSHIFT] = EKeyCode::LShift;
	gEKeyCodeLut[SDLK_RSHIFT] = EKeyCode::RShift;
	gEKeyCodeLut[SDLK_LCTRL] = EKeyCode::LCtrl;
	gEKeyCodeLut[SDLK_RCTRL] = EKeyCode::RCtrl;
	gEKeyCodeLut[SDLK_LALT] = EKeyCode::LAlt;
	gEKeyCodeLut[SDLK_RALT] = EKeyCode::RAlt;
	gEKeyCodeLut[SDLK_PAUSE] = EKeyCode::Pause;
	gEKeyCodeLut[SDLK_CAPSLOCK] = EKeyCode::CapsLock;
	gEKeyCodeLut[SDLK_ESCAPE] = EKeyCode::Esc;
	gEKeyCodeLut[SDLK_SPACE] = EKeyCode::SpaceBar;
	gEKeyCodeLut[SDLK_CAPSLOCK] = EKeyCode::CapsLock;
	gEKeyCodeLut[SDLK_PAGEUP] = EKeyCode::PageUp;
	gEKeyCodeLut[SDLK_PAGEDOWN] = EKeyCode::PageDown;
	gEKeyCodeLut[SDLK_END] = EKeyCode::End;
	gEKeyCodeLut[SDLK_HOME] = EKeyCode::Home;
	gEKeyCodeLut[SDLK_LEFT] = EKeyCode::Left;
	gEKeyCodeLut[SDLK_UP] = EKeyCode::Up;
	gEKeyCodeLut[SDLK_RIGHT] = EKeyCode::Right;
	gEKeyCodeLut[SDLK_DOWN] = EKeyCode::Down;
	gEKeyCodeLut[SDLK_SELECT] = EKeyCode::Select;
	gEKeyCodeLut[SDLK_PRINTSCREEN] = EKeyCode::Print_screen;
	gEKeyCodeLut[SDLK_EXECUTE] = EKeyCode::Execute;
	gEKeyCodeLut[SDLK_INSERT] = EKeyCode::Insert;
	gEKeyCodeLut[SDLK_DELETE] = EKeyCode::Del;
	gEKeyCodeLut[SDLK_HELP] = EKeyCode::Help;

	gEKeyCodeLut[SDLK_0] = EKeyCode::Zero;
	gEKeyCodeLut[SDLK_1] = EKeyCode::One;
	gEKeyCodeLut[SDLK_2] = EKeyCode::Two;
	gEKeyCodeLut[SDLK_3] = EKeyCode::Three;
	gEKeyCodeLut[SDLK_4] = EKeyCode::Four;
	gEKeyCodeLut[SDLK_5] = EKeyCode::Five;
	gEKeyCodeLut[SDLK_6] = EKeyCode::Six;
	gEKeyCodeLut[SDLK_7] = EKeyCode::Seven;
	gEKeyCodeLut[SDLK_8] = EKeyCode::Eight;
	gEKeyCodeLut[SDLK_9] = EKeyCode::Nine;

	gEKeyCodeLut[SDLK_APP1] = EKeyCode::App;
	gEKeyCodeLut[SDLK_SLEEP] = EKeyCode::Sleep;

	gEKeyCodeLut[SDLK_KP_0] = EKeyCode::Numpad0;
	gEKeyCodeLut[SDLK_KP_1] = EKeyCode::Numpad1;
	gEKeyCodeLut[SDLK_KP_2] = EKeyCode::Numpad2;
	gEKeyCodeLut[SDLK_KP_3] = EKeyCode::Numpad3;
	gEKeyCodeLut[SDLK_KP_4] = EKeyCode::Numpad4;
	gEKeyCodeLut[SDLK_KP_5] = EKeyCode::Numpad5;
	gEKeyCodeLut[SDLK_KP_6] = EKeyCode::Numpad6;
	gEKeyCodeLut[SDLK_KP_7] = EKeyCode::Numpad7;
	gEKeyCodeLut[SDLK_KP_8] = EKeyCode::Numpad8;
	gEKeyCodeLut[SDLK_KP_9] = EKeyCode::Numpad9;

	gEKeyCodeLut[SDLK_KP_MULTIPLY] = EKeyCode::Multiply;
	gEKeyCodeLut[SDLK_KP_PLUS] = EKeyCode::Add;
	gEKeyCodeLut[SDLK_KP_DIVIDE] = EKeyCode::Divide;
	gEKeyCodeLut[SDLK_KP_MINUS] = EKeyCode::Subtract;

	gEKeyCodeLut[SDLK_F1] = EKeyCode::F1;
	gEKeyCodeLut[SDLK_F2] = EKeyCode::F2;
	gEKeyCodeLut[SDLK_F3] = EKeyCode::F3;
	gEKeyCodeLut[SDLK_F4] = EKeyCode::F4;
	gEKeyCodeLut[SDLK_F5] = EKeyCode::F5;
	gEKeyCodeLut[SDLK_F6] = EKeyCode::F6;
	gEKeyCodeLut[SDLK_F7] = EKeyCode::F7;
	gEKeyCodeLut[SDLK_F8] = EKeyCode::F8;
	gEKeyCodeLut[SDLK_F9] = EKeyCode::F9;
	gEKeyCodeLut[SDLK_F10] = EKeyCode::F10;
	gEKeyCodeLut[SDLK_F11] = EKeyCode::F11;
	gEKeyCodeLut[SDLK_F12] = EKeyCode::F12;
	gEKeyCodeLut[SDLK_F13] = EKeyCode::F13;
	gEKeyCodeLut[SDLK_F14] = EKeyCode::F14;
	gEKeyCodeLut[SDLK_F15] = EKeyCode::F15;
	gEKeyCodeLut[SDLK_F16] = EKeyCode::F16;
	gEKeyCodeLut[SDLK_F17] = EKeyCode::F17;
	gEKeyCodeLut[SDLK_F18] = EKeyCode::F18;
	gEKeyCodeLut[SDLK_F19] = EKeyCode::F19;
	gEKeyCodeLut[SDLK_F20] = EKeyCode::F20;
	gEKeyCodeLut[SDLK_F21] = EKeyCode::F21;
	gEKeyCodeLut[SDLK_F22] = EKeyCode::F22;
	gEKeyCodeLut[SDLK_F23] = EKeyCode::F23;
	gEKeyCodeLut[SDLK_F24] = EKeyCode::F24;

	gEKeyCodeLut[SDLK_NUMLOCKCLEAR] = EKeyCode::Num_lock;

	gEKeyCodeLut[SDLK_SEMICOLON] = EKeyCode::Semicolon;
	gEKeyCodeLut[SDLK_PLUS] = EKeyCode::Plus;
	gEKeyCodeLut[SDLK_COMMA] = EKeyCode::Comma;
	gEKeyCodeLut[SDLK_MINUS] = EKeyCode::Minus;
	gEKeyCodeLut[SDLK_SLASH] = EKeyCode::Slash;

	gEKeyCodeLut[SDLK_a] = EKeyCode::A;
	gEKeyCodeLut[SDLK_b] = EKeyCode::B;
	gEKeyCodeLut[SDLK_c] = EKeyCode::C;
	gEKeyCodeLut[SDLK_d] = EKeyCode::D;
	gEKeyCodeLut[SDLK_e] = EKeyCode::E;
	gEKeyCodeLut[SDLK_f] = EKeyCode::F;
	gEKeyCodeLut[SDLK_g] = EKeyCode::G;
	gEKeyCodeLut[SDLK_h] = EKeyCode::H;
	gEKeyCodeLut[SDLK_i] = EKeyCode::I;
	gEKeyCodeLut[SDLK_j] = EKeyCode::J;
	gEKeyCodeLut[SDLK_k] = EKeyCode::K;
	gEKeyCodeLut[SDLK_l] = EKeyCode::L;
	gEKeyCodeLut[SDLK_m] = EKeyCode::M;
	gEKeyCodeLut[SDLK_n] = EKeyCode::N;
	gEKeyCodeLut[SDLK_o] = EKeyCode::O;
	gEKeyCodeLut[SDLK_p] = EKeyCode::P;
	gEKeyCodeLut[SDLK_q] = EKeyCode::Q;
	gEKeyCodeLut[SDLK_r] = EKeyCode::R;
	gEKeyCodeLut[SDLK_s] = EKeyCode::S;
	gEKeyCodeLut[SDLK_t] = EKeyCode::T;
	gEKeyCodeLut[SDLK_u] = EKeyCode::U;
	gEKeyCodeLut[SDLK_v] = EKeyCode::V;
	gEKeyCodeLut[SDLK_w] = EKeyCode::W;
	gEKeyCodeLut[SDLK_x] = EKeyCode::X;
	gEKeyCodeLut[SDLK_y] = EKeyCode::Y;
	gEKeyCodeLut[SDLK_z] = EKeyCode::Z;
}

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
				if (filename == "test.css")
				{
					ReloadCSS = true;
				}
				if (filename == "test.xml" )
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
	if (auto child1 = QueryFirst(ve, "#Child1"))
	{
		constexpr auto handler = +[](PointerDownEvent& event)
		{
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
			return true;
		};
		constexpr auto handlerUp = +[](KeyUpEvent& event)
		{
			using namespace ostr::literal;
			if (event.key == EKeyCode::W)
			{
				olog::Info(u"W is up!"_o);
			}
			return true;
		};
		constexpr auto handlerHold = +[](KeyHoldEvent& event)
		{
			using namespace ostr::literal;
			if (event.key == EKeyCode::W)
			{
				olog::Info(u"W is Holding!"_o);
			}
			return true;
		};
		child1->_eventHandler.Register<PointerDownEvent, handler>();
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
	auto asset = XmlAsset::LoadXmlFile("res/test.xml");
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

				BuildSDLMap();
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
							//olog::info(u"Width: {}, Height: {}"_o.format(width, height));
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
							int width, height;
							SDL_GetWindowSize(window, &width, &height);
							ctx.OnMouseUp((float)width, (float)height, buttonCode, event.button.x, event.button.y);
							break;
						}
						case SDL_MOUSEMOTION:
						{
							olog::Info(u"MousePos X:{}, Y:{}"_o.format(event.motion.x, event.motion.y));
							olog::Info(u"MousePos RelX:{}, RelY:{}"_o.format(event.motion.xrel, event.motion.yrel));
							ctx.OnMouseMove(true, event.motion.xrel, event.motion.yrel);
							break;
						}
						case SDL_KEYDOWN:
						{
							//olog::Info(u"KeyDown {}"_o.format(event.key.keysym.sym));
							//olog::Info(u"KeyDown(EKeyCode) {}"_o.format(gEKeyCodeLut[event.key.keysym.sym]));
							ctx.OnKeyDown(gEKeyCodeLut[event.key.keysym.sym]);
							break;
						}
						case SDL_KEYUP:
						{
							if (event.key.keysym.sym == SDLK_ESCAPE)
							{
								done = true;
							}
							else
							{
								//olog::Info(u"KeyDown {}"_o.format(event.key.keysym.sym));
								//olog::Info(u"KeyDown(EKeyCode) {}"_o.format(gEKeyCodeLut[event.key.keysym.sym]));
								ctx.OnKeyUp(gEKeyCodeLut[event.key.keysym.sym]);
							}
							break;
						}
						case SDL_MOUSEWHEEL:
						{
							//olog::Info(u"MouseWheel Delta:{}"_o.format(event.wheel.y));
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
