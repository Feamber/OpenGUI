/**
 * \file webgpu.h
 * WebGPU/Dawn wrapper.
 */
#pragma once

#include <webgpu/webgpu.h>

#include "window.h"



namespace webgpu
{
	struct instance;
	instance* create(window::Handle window, WGPUBackendType type = WGPUBackendType_Force32);
    void release(instance* instance);
	WGPUSwapChain createSwapChain(instance* instance, float width, float height);
	WGPUTextureFormat getSwapChainFormat(instance* instance);
	WGPUDevice getDevice(instance* instance);
}
