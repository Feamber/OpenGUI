#include "webgpu.h"

/*
 * On Windows x86/x64 Dawn should have been built with the D3D12 and Vulkan
 * support; macOS/iOS it should be Metal only; Linux (and others) should be
 * Vulkan only. None of the platforms are built with OpenGL support.
 * 
 * TODO: are we doing GL?
 */
#if __has_include("d3d12.h") || (_MSC_VER >= 1900)
#define DAWN_ENABLE_BACKEND_D3D12
#elif __has_include("vulkan/vulkan.h") && (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
 #include "vulkan/vulkan.h"
#define DAWN_ENABLE_BACKEND_VULKAN
#endif

//****************************************************************************/
#include <dawn/dawn_proc.h>
#include <dawn/webgpu_cpp.h>
#include <dawn_native/NullBackend.h>
#ifdef DAWN_ENABLE_BACKEND_D3D12
#include <dawn_native/D3D12Backend.h>
#endif
#ifdef DAWN_ENABLE_BACKEND_VULKAN
#include <dawn_native/VulkanBackend.h>
#define WIN_MEAN_AND_LEAN
#include "windows.h"
#include <vulkan/vulkan_win32.h>
#endif

#pragma comment(lib, "dawn_native.lib")
#pragma comment(lib, "dawn_proc.lib")
#pragma comment(lib, "dawn_platform.lib")
#ifdef DAWN_ENABLE_BACKEND_VULKAN
#pragma comment(lib, "vulkan-1.lib")
#endif

namespace webgpu
{
	struct instance
	{

/*
 * Chosen backend type for \c #device.
 */
		WGPUBackendType backend;

/*
 * WebGPU graphics API-specific device, created from a \c dawn_native::Adapter
 * and optional feature requests. This should wrap the same underlying device
 * for the same configuration.
 */
		WGPUDevice device;

		WGPUInstance instance;
/*
 * Something needs to hold onto this since the address is passed to the WebGPU
 * native API, exposing the type-specific swap chain implementation. The struct
 * gets filled out on calling the respective XXX::CreateNativeSwapChainImpl(),
 * binding the WebGPU device and native window, then its raw pointer is passed
 * into WebGPU as a 64-bit int. The browser API doesn't have an equivalent
 * (since the swap chain is created from the canvas directly).
 * 
 * Is the struct copied or does it need holding for the lifecycle of the swap
 * chain, i.e. can it just be a temporary?
 * 
 * After calling wgpuSwapChainRelease() does it also call swapi->Destroy()
 * to delete the underlying NativeSwapChainImpl(), invalidating this struct?
 */
		DawnSwapChainImplementation swapImpl;

/*
 * Preferred swap chain format, obtained in the browser via a promise to
 * GPUCanvasContext::getSwapChainPreferredFormat(). In Dawn we can call this
 * directly in NativeSwapChaini->GetPreferredFormat() (which is hard-coded
 * with D3D, for example, to RGBA8Unorm, but queried for others). For the D3D
 * back-end calling wgpuSwapChainConfigure ignores the passed preference and
 * asserts if it's not the preferred choice.
 */
		WGPUTextureFormat swapPref;
	};
}

namespace impl {
/*
 * NOTE: keeping these here for a single device/window until I work out more.
 */

//********************************** Helpers *********************************/

/**
 * Analogous to the browser's \c GPU.requestAdapter().
 * \n
 * The returned \c Adapter is a wrapper around the underlying Dawn adapter (and
 * owned by the single Dawn instance).
 * 
 * \todo we might be interested in whether the \c AdapterType is discrete or integrated for power-management reasons
 * 
 * \param[in] type1st first choice of \e backend type (e.g. \c WGPUBackendType_D3D12)
 * \param[in] type2nd optional fallback \e backend type (or \c WGPUBackendType_Null to pick the first choice or nothing)
 * \return the best choice adapter or an empty adapter wrapper
 */
static dawn_native::Adapter requestAdapter(WGPUBackendType type1st, WGPUBackendType type2nd = WGPUBackendType_Null) {
	static dawn_native::Instance instance;
	instance.DiscoverDefaultAdapters();
	wgpu::AdapterProperties properties;
	std::vector<dawn_native::Adapter> adapters = instance.GetAdapters();
	for (auto it = adapters.begin(); it != adapters.end(); ++it) {
		it->GetProperties(&properties);
		if (static_cast<WGPUBackendType>(properties.backendType) == type1st) {
			return *it;
		}
	}
	if (type2nd) {
		for (auto it = adapters.begin(); it != adapters.end(); ++it) {
			it->GetProperties(&properties);
			if (static_cast<WGPUBackendType>(properties.backendType) == type2nd) {
				return *it;
			}
		}
	}
	return dawn_native::Adapter();
}

#ifdef DAWN_ENABLE_BACKEND_VULKAN
/**
 * Helper to obtain a Vulkan surface from the supplied window.
 * 
 * \todo what's the lifecycle of this?
 * 
 * \param[in] device WebGPU device
 * \param[in] window window on which the device will be bound
 * \return window surface (or \c VK_NULL_HANDLE if creation failed)
 */
static VkSurfaceKHR createVkSurface(WGPUDevice device, window::Handle window) {
	VkSurfaceKHR surface = VK_NULL_HANDLE;
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR info = {};
	info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.hinstance = GetModuleHandle(NULL);
	info.hwnd      = reinterpret_cast<HWND>(window);
	vkCreateWin32SurfaceKHR(
		dawn_native::vulkan::GetInstance(device),
			&info, nullptr, &surface);
#endif
	return surface;
}
#endif

/**
 * Creates an API-specific swap chain implementation in \c #swapImpl and stores
 * the \c #swapPref.
 */
static void initSwapChain(webgpu::instance* i, WGPUBackendType backend, WGPUDevice device, window::Handle window) {
	switch (backend) {
#ifdef DAWN_ENABLE_BACKEND_D3D12
	case WGPUBackendType_D3D12:
		if (i->swapImpl.userData == nullptr) {
			i->swapImpl = dawn_native::d3d12::CreateNativeSwapChainImpl(
							 i->device, reinterpret_cast<HWND>(window));
			i->swapPref = dawn_native::d3d12::GetNativeSwapChainPreferredFormat(&i->swapImpl);
		}
		break;
#endif
#ifdef DAWN_ENABLE_BACKEND_VULKAN
	case WGPUBackendType_Vulkan:
		if (i->swapImpl.userData == nullptr) {
			i->swapImpl = dawn_native::vulkan::CreateNativeSwapChainImpl(
							 i->device, createVkSurface(device, window));
			i->swapPref = dawn_native::vulkan::GetNativeSwapChainPreferredFormat(&i->swapImpl);
		}
		break;
#endif
	default:
		if (i->swapImpl.userData == nullptr) {
			i->swapImpl = dawn_native::null::CreateNativeSwapChainImpl();
			i->swapPref = WGPUTextureFormat_Undefined;
		}
		break;
	}
}

/**
 * Dawn error handling callback (adheres to \c WGPUErrorCallback).
 * 
 * \param[in] message error string
 */
static void printError(WGPUErrorType /*type*/, const char* message, void*) {
	puts(message);
}
} // impl

//******************************** Public API ********************************/

webgpu::instance* webgpu::create(window::Handle window, WGPUBackendType type) {
	if (type > WGPUBackendType_OpenGLES) {
	#ifdef DAWN_ENABLE_BACKEND_D3D12
		type = WGPUBackendType_D3D12;
	#else
	#ifdef DAWN_ENABLE_BACKEND_VULKAN
		type = WGPUBackendType_Vulkan;
	#endif
	#endif
	}
	webgpu::instance* i = new webgpu::instance{};
	/*
	 * First go at this. We're only creating one global device/swap chain so far.
	 */
	if (dawn_native::Adapter adapter = impl::requestAdapter(type)) {
		wgpu::AdapterProperties properties;
		adapter.GetProperties(&properties);
		i->backend = static_cast<WGPUBackendType>(properties.backendType);
		i->device  = adapter.CreateDevice();
	
		static dawn_native::Instance instance;
		i->instance = instance.Get();

		impl::initSwapChain(i, i->backend, i->device, window);
		DawnProcTable procs(dawn_native::GetProcs());
		procs.deviceSetUncapturedErrorCallback(i->device, impl::printError, nullptr);
		dawnProcSetProcs(&procs);
	}
	return i;
}

WGPUSwapChain webgpu::createSwapChain(webgpu::instance* i, float width, float height, void* hwnd, void* hinstance) {
	WGPUSwapChainDescriptor swapDesc = {};
	/*
	 * Currently failing (probably because the nextInChain needs setting up, and
	 * also with the correct WGPUSType_* for the platform).
	 *
	swapDesc.usage  = WGPUTextureUsage_OutputAttachment;
	swapDesc.width  = 800;
	swapDesc.height = 450;
	swapDesc.presentMode = WGPUPresentMode_Mailbox;
	 */
	//swapDesc.implementation = reinterpret_cast<uintptr_t>(&i->swapImpl);
	swapDesc.format = WGPUTextureFormat_BGRA8Unorm;
	swapDesc.usage = WGPUTextureUsage_RenderAttachment;
	swapDesc.width = width;
	swapDesc.height = height;
	swapDesc.presentMode = WGPUPresentMode_Mailbox;
	WGPUSurfaceDescriptor desc = {};
	WGPUChainedStruct chain = {};
	chain.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
	chain.next = NULL;
	WGPUSurfaceDescriptorFromWindowsHWND HWNDDesc = {};
	HWNDDesc.chain = chain;
	HWNDDesc.hwnd = hwnd;
	HWNDDesc.hinstance = hinstance;
	desc.nextInChain = (const WGPUChainedStruct*)&HWNDDesc;
	auto surface = wgpuInstanceCreateSurface(i->instance, &desc);
	WGPUSwapChain swapchain = wgpuDeviceCreateSwapChain(i->device, surface, &swapDesc);
	// Configure is invalid for surface-based swapchains
	// wgpuSwapChainConfigure(swapchain, i->swapPref, WGPUTextureUsage_RenderAttachment, width, height);
	return swapchain;
}

WGPUTextureFormat webgpu::getSwapChainFormat(webgpu::instance* i) {
	return i->swapPref;
}

WGPUDevice webgpu::getDevice(webgpu::instance* i)
{
	return i->device;
}

void webgpu::release(webgpu::instance* i)
{
	delete i;
}