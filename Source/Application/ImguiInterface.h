#pragma once
#include "ResourceEnum.h"
#include "Device.h"
#include "CommandQueue.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_dx12.h"
#include <SDL3/SDL.h>
namespace WaxGourd {
namespace ImguiInterface {
void Init(SDL_Window* window, Device* device, CommandQueue* command_queue, int frames_in_flight, ResourceFormat render_target_format) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Platform/Renderer backends
	ImGui_ImplDX12_InitInfo init_info = {};
	init_info.Device = device->GetDevice();
	init_info.CommandQueue = command_queue->GetCommandQueue();
	init_info.NumFramesInFlight = frames_in_flight;
	init_info.RTVFormat = GetDxgiFormat(render_target_format); // render target format.

	// Allocating SRV descriptors (for textures) is up to the application, so we provide callbacks.
	//init_info.SrvDescriptorHeap = device->GetDescriptorHeap();
	init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) 
		{ return YOUR_ALLOCATOR_FUNCTION_FOR_SRV_DESCRIPTORS(...); };
	init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) 
		{ return YOUR_FREE_FUNCTION_FOR_SRV_DESCRIPTORS(...); };

	// (before 1.91.6 the DirectX12 backend required a single SRV descriptor passed)
	// (there is a legacy version of ImGui_ImplDX12_Init() that supports those, but a future version of Dear ImGuii will requires more descriptors to be allocated)
}
void NewFrame();
void Render(ID3D12GraphicsCommandList* commandList);
void Shutdown();
	
}
}