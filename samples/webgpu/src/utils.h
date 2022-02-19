#pragma once
#include "OpenGUI/Core/Types.h"
#include "dawn/webgpu.h"
#include "stb_image.h"
#include "webgpu.h"
#include "OpenGUI/Interface/Interfaces.h"

static char const triangle_vert_wgsl[] = R"(
	[[block]]
	struct VertexIn {
		[[location(0)]] aPos : vec2<f32>;
		[[location(1)]] aUV  : vec2<f32>;
		[[location(2)]] aAA  : vec2<f32>;
		[[location(3)]] aCol : vec4<f32>;
		[[location(4)]] aCUV  : vec2<f32>;
	};
	struct VertexOut {
		[[location(0)]] vCol : vec4<f32>;
		[[location(1)]] vUV  : vec2<f32>;
		[[location(2)]] vAA  : vec2<f32>;
		[[location(3)]] vCUV  : vec2<f32>;
		[[builtin(position)]] Position : vec4<f32>;
	};
	[[stage(vertex)]] fn main(input : VertexIn) -> VertexOut {
		var output : VertexOut;
		output.Position = vec4<f32>(input.aPos * 2.0, 1.0, 1.0);
		output.vCol = input.aCol;
        output.vUV = input.aUV;
        output.vUV.y = 1.0 - output.vUV.y;
		output.vAA = input.aAA;
		output.vCUV = input.aCUV;
		return output;
	}
)";


static char const triangle_frag_wgsl[] = R"(
	struct VertexOut {
		[[location(0)]] vCol : vec4<f32>;
		[[location(1)]] vUV  : vec2<f32>;
		[[location(2)]] vAA  : vec2<f32>;
		[[location(3)]] vCUV  : vec2<f32>;
		[[builtin(position)]] Position : vec4<f32>;
	};
    [[group(0), binding(0)]] var myTexture : texture_2d<f32>;
    [[group(0), binding(1)]] var mySampler : sampler;

	[[stage(fragment)]] 
	fn main(input : VertexOut) ->  [[location(0)]] vec4<f32> {
		var visible : f32;
		visible = f32(all(input.vCUV == clamp(input.vCUV, vec2<f32>(-1.0, -1.0), vec2<f32>(1.0, 1.0))));
		var edge : f32;
		edge = min(1.0, (1.0-abs((input.vAA.x * 2.0) - 1.0)) * input.vAA.y) ;
		var color : vec4<f32> = visible * input.vCol * textureSample(myTexture, mySampler, input.vUV);
		color.w = color.w * edge;
		return color;
		//return vec4<f32>(edge, edge, edge, edge);
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

inline static WGPUBuffer createBuffer(WGPUDevice device, WGPUQueue queue,
    const void* data, size_t size, WGPUBufferUsage usage) {
	WGPUBufferDescriptor desc = {};
	desc.usage = WGPUBufferUsage_CopyDst | usage;
	desc.size  = size;
	WGPUBuffer buffer = wgpuDeviceCreateBuffer(device, &desc);
	wgpuQueueWriteBuffer(queue, buffer, 0, data, size);
	return buffer;
}

inline static void writeBuffer(WGPUQueue queue, WGPUBuffer buffer,
    const void* data, size_t size) 
{
	wgpuQueueWriteBuffer(queue, buffer, 0, data, size);
}

inline static WGPUTextureFormat translate(OGUI::PixelFormat format)
{   
    switch(format)
    {
    case OGUI::PixelFormat::PF_R8G8B8A8: return WGPUTextureFormat_RGBA8Unorm;
    case OGUI::PixelFormat::PF_R16G16B16A16: return WGPUTextureFormat_RGBA16Float;
    case OGUI::PixelFormat::PF_R8G8B8A8_SRGB: return WGPUTextureFormat_RGBA8UnormSrgb;
	case OGUI::PixelFormat::PF_R8A8: return WGPUTextureFormat_RG8Unorm;
    case OGUI::PixelFormat::PF_R8: return WGPUTextureFormat_R8Unorm;
    case OGUI::PixelFormat::PF_R16: return WGPUTextureFormat_R16Uint;
    case OGUI::PixelFormat::PF_R32: return WGPUTextureFormat_R32Uint;
    default: return WGPUTextureFormat_Undefined;
    }
}

inline static uint32_t size_in_bytes(OGUI::PixelFormat format)
{
    switch(format)
    {
    case OGUI::PixelFormat::PF_R8G8B8A8: return 4;
    case OGUI::PixelFormat::PF_R16G16B16A16: return 8;
    case OGUI::PixelFormat::PF_R8G8B8A8_SRGB: return 4;
	case OGUI::PixelFormat::PF_R8A8: return 2;
    case OGUI::PixelFormat::PF_R8: return 1;
    case OGUI::PixelFormat::PF_R16: return 2;
    case OGUI::PixelFormat::PF_R32: return 4;
    default: return -1;
    }
}

struct WGPU_OGUI_Texture
{
    WGPUTexture texture;
    WGPUTextureView texture_view;
	WGPUBindGroup bind_group;

    inline void Release()
    {
        wgpuTextureRelease(texture);
		wgpuTextureViewRelease(texture_view);
		if(bind_group) 
            wgpuBindGroupRelease(bind_group);
    }
};

inline static WGPU_OGUI_Texture* createTexture(
    WGPUDevice device, WGPUQueue queue,
    const OGUI::Bitmap& bitmap
)
{
    WGPU_OGUI_Texture* result = new WGPU_OGUI_Texture();

    WGPUTextureDescriptor descriptor = {};
    descriptor.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    descriptor.dimension = WGPUTextureDimension_2D;
    descriptor.size = {bitmap.width, bitmap.height, 1};
    descriptor.mipLevelCount = 1;
    descriptor.sampleCount = 1;
    descriptor.format = translate(bitmap.format);
    auto tex = wgpuDeviceCreateTexture(device, &descriptor);
    result->texture = tex;

    WGPUImageCopyTexture cpyView = {};
    cpyView.texture = tex;
    cpyView.mipLevel = 0;
    cpyView.origin = { 0, 0, 0 };
    cpyView.aspect = WGPUTextureAspect_All;

    WGPUTextureDataLayout dtLayout = {};
    dtLayout.offset = 0;
    dtLayout.bytesPerRow = bitmap.width * size_in_bytes(bitmap.format);
    dtLayout.rowsPerImage = bitmap.height;

    WGPUExtent3D writeSize = descriptor.size;
    wgpuQueueWriteTexture(queue, &cpyView, bitmap.resource.bytes, bitmap.resource.size_in_bytes, &dtLayout, &writeSize);

	WGPUTextureViewDescriptor viewDesc = {};
	viewDesc.format = translate(bitmap.format);
	viewDesc.dimension = WGPUTextureViewDimension_2D;
	viewDesc.baseMipLevel = 0;
	viewDesc.mipLevelCount = 1;
	viewDesc.baseArrayLayer = 0;
	viewDesc.arrayLayerCount = 1;
	viewDesc.aspect = WGPUTextureAspect_All;
	result->texture_view = wgpuTextureCreateView(tex, &viewDesc);

    return result;
}

inline static void updateTexture(
    WGPUDevice device, WGPUQueue queue, WGPU_OGUI_Texture* texture,
    const OGUI::Bitmap& bitmap
)
{
    WGPUImageCopyTexture cpyView = {};
    cpyView.texture = texture->texture;
    cpyView.mipLevel = 0;
    cpyView.origin = { 0, 0, 0 };
    cpyView.aspect = WGPUTextureAspect_All;

    WGPUTextureDataLayout dtLayout = {};
    dtLayout.offset = 0;
    dtLayout.bytesPerRow = bitmap.width * size_in_bytes(bitmap.format);
    dtLayout.rowsPerImage = bitmap.height;

    WGPUExtent3D writeSize = {bitmap.width, bitmap.height, 1};
    wgpuQueueWriteTexture(queue, &cpyView, bitmap.resource.bytes, bitmap.resource.size_in_bytes, &dtLayout, &writeSize);
}

inline static void createBitMapFromJPG(const char* filename, OGUI::Bitmap& bm)
{
    int x, y, n;
    auto data = stbi_load(filename, &x, &y, &n, STBI_rgb_alpha);
    bm.resource.bytes = data;
    bm.resource.size_in_bytes = x * y * n * sizeof(*data);
    bm.format = OGUI::PF_R8G8B8A8;
    bm.height = y;
    bm.width = x;
}

inline static void freeBitMap(OGUI::Bitmap& bm)
{
    stbi_image_free(bm.resource.bytes);
}


#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "OpenGUI/Context.h"
extern std::unordered_map<uint32_t, OGUI::EKeyCode> gEKeyCodeLut;

inline static bool SDLEventHandler(const SDL_Event& event, SDL_Window* window, OGUI::WindowHandle hWnd)
{
    using namespace OGUI;
    auto& ctx = OGUI::Context::Get();
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
            ctx.OnMouseDown(hWnd, buttonCode, event.button.x, event.button.y);
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
            ctx.OnMouseUp(hWnd, buttonCode, event.button.x, event.button.y);
            break;
        }
        case SDL_MOUSEMOTION:
        {
			int width, height;
            SDL_GetWindowSize(window, &width, &height);
            ctx.OnMouseMove(hWnd, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            break;
        }
        case SDL_KEYDOWN:
        {
            //olog::Info(u"KeyDown {}"_o.format(event.key.keysym.sym));
            //olog::Info(u"KeyDown(EKeyCode) {}"_o.format(gEKeyCodeLut[event.key.keysym.sym]));
            ctx.OnKeyDown(hWnd, gEKeyCodeLut[event.key.keysym.sym]);
            break;
        }
        case SDL_KEYUP:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return false;
            }
            else
            {
                //olog::Info(u"KeyDown {}"_o.format(event.key.keysym.sym));
                //olog::Info(u"KeyDown(EKeyCode) {}"_o.format(gEKeyCodeLut[event.key.keysym.sym]));
                ctx.OnKeyUp(hWnd, gEKeyCodeLut[event.key.keysym.sym]);
            }
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            //olog::Info(u"MouseWheel Delta:{}"_o.format(event.wheel.y));
			int x, y;
			SDL_GetRelativeMouseState(&x, &y);
            ctx.OnMouseWheel(hWnd, event.wheel.y, x, y);
            break;
        }
        case SDL_WINDOWEVENT:
			switch(event.window.event)
			{
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					ctx.OnActivateWindow(hWnd);
					break;
				case SDL_WINDOWEVENT_CLOSE:
            		return false;
				case SDL_WINDOWEVENT_ENTER:
					ctx.OnMouseEnter(hWnd);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					ctx.OnMouseLeave(hWnd);
					break;
			}
			break;
    }
    return true;
}


inline static void BuildSDLMap()
{
    using namespace OGUI;
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

FORCEINLINE auto InitializeWGPUDevice() 
	-> std::tuple<webgpu::instance*, WGPUDevice, WGPUQueue>
{
	auto init_window = SDL_CreateWindow("TestWindow",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		50, 50, 0
	);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(init_window, &wmInfo);
#if defined(_WIN32) || defined(_WIN64)
	const auto hWnd = (window::Handle)wmInfo.info.win.window;
#endif
	if(auto wgpu = webgpu::create(hWnd))
	{
		auto device = getDevice(wgpu);
		auto queue = wgpuDeviceGetQueue(device);
		SDL_DestroyWindow(init_window);
		return std::make_tuple(wgpu, device, queue);
	}
	return {nullptr, nullptr, nullptr};
}

bool InstallBitmapParser();
bool InstallLogger();