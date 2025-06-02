#pragma once
#include <memory>
#include <array>
#include "CommandList.h"
#include "ComPointer.h"
#include "Device.h"
#include "DescriptorHandle.h"
#include "ResourceEnum.h"
#include "Macros.h"
#include "Texture.h"

namespace WaxGourd {
struct SwapChainInfo {
	uint32 width = 0;
	uint32 height = 0;
	ResourceFormat backbuffer_format = ResourceFormat::R8G8B8A8_UNORM_SRGB;
	bool fullscreen_windowed = false;
};

class SwapChain {
public:
	SwapChain(Device* device, const SwapChainInfo & info);
	~SwapChain();

	void SetAsRenderTarget(CommandList* command_list);
	void ClearBackbuffer(CommandList* command_list);
	bool Present(bool vsync);
	void OnResize(uint32 width, uint32 height);

	Uint32 GetBackbufferIndex() const { return m_backbuffer_index; }
	Texture* GetBackbuffer() const { return m_back_buffers[m_backbuffer_index].get(); }

private:
	Device* m_device = nullptr;
	ComPointer<IDXGISwapChain4> m_swap_chain = nullptr;
		
	// Owns the textures
	std::array<std::unique_ptr<Texture>, NUM_BACK_BUFFERS> m_back_buffers;
	std::array<DescriptorHandle, NUM_BACK_BUFFERS> m_backbuffer_render_target_views;
		
	SwapChainInfo m_info;
	uint32 m_backbuffer_index = 0;

	void CreateBackbuffers();
	DescriptorHandle GetBackBufferRenderTargetDescriptor() const;
};
}