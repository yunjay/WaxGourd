#include "SwapChain.h"
#include "Window.h"

namespace WaxGourd {
SwapChain::SwapChain(Device* device, SwapChainInfo const& info) : m_device(device), m_info(info){
	DXGI_SWAP_CHAIN_DESC1 swapchain_desc{};
	swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapchain_desc.BufferCount = NUM_BACK_BUFFERS;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.Format = GetDxgiFormat(info.backbuffer_format);
	swapchain_desc.Width = info.width;
	swapchain_desc.Height = info.height;
	swapchain_desc.Scaling = DXGI_SCALING_NONE;
	swapchain_desc.Stereo = FALSE;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc{};
	fullscreen_desc.RefreshRate.Denominator = 60;
	fullscreen_desc.RefreshRate.Numerator = 1;
	fullscreen_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullscreen_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullscreen_desc.Windowed = info.fullscreen_windowed;

	CommandQueue* graphics_queue = device->GetCommandQueue(CommandQueueType::Rendering);
	ComPointer<IDXGISwapChain1> swapchain1 = nullptr;

	ASSERT_HRESULT(
		device->GetFactory()->CreateSwapChainForHwnd(
			graphics_queue->GetCommandQueue(),
			(device->GetWindow()->GetWindowHandle()),
			&swapchain_desc,
			&fullscreen_desc,
			nullptr,
			swapchain1.GetAddressOf())
	);

	m_swap_chain.Reset();
	swapchain1.As(&m_swap_chain); // Get most recent interface

	m_backbuffer_index = m_swap_chain->GetCurrentBackBufferIndex();
	CreateBackbuffers();
}

SwapChain::~SwapChain() {}

void SwapChain::SetAsRenderTarget(CommandList* cmd_list) {
	Descriptor RTVs[] = { GetBackBufferRenderTargetDescriptor() };
	cmd_list->SetRenderTargets(RTVs);
}

void SwapChain::ClearBackbuffer(CommandList* cmd_list) {
	constexpr float clear_color[] = { 0,0,0,0 };
	Descriptor RTV = GetBackBufferRenderTargetDescriptor();
	cmd_list->ClearRenderTarget(RTV, clear_color);
}

bool SwapChain::Present(bool vsync) {
	HRESULT hr = m_swap_chain->Present(vsync, 0);
	m_backbuffer_index = m_swap_chain->GetCurrentBackBufferIndex();
	return SUCCEEDED(hr);
}

void SwapChain::OnResize(uint32 width, uint32 height) {
	m_info.width = width;
	m_info.height = height;

	for (Uint32 i = 0; i < NUM_BACK_BUFFERS; ++i) {
		m_back_buffers[i].reset(nullptr);
	}

	DXGI_SWAP_CHAIN_DESC desc{};
	m_swap_chain->GetDesc(&desc);
	HRESULT hr = m_swap_chain->ResizeBuffers(desc.BufferCount, width, height, desc.BufferDesc.Format, desc.Flags);
	ASSERT_HRESULT(hr);

	m_backbuffer_index = m_swap_chain->GetCurrentBackBufferIndex();
	CreateBackbuffers();
}

void SwapChain::CreateBackbuffers() {
	for (Uint32 i = 0; i < NUM_BACK_BUFFERS; i++) {
		ComPointer<ID3D12Resource> back_buffer = nullptr;
		HRESULT hr = m_swap_chain->GetBuffer(i, IID_PPV_ARGS(back_buffer.GetAddressOf()));
		ASSERT_HRESULT(hr);
		D3D12_RESOURCE_DESC resource_desc = back_buffer->GetDesc();
		TextureDesc texture_info{};
		texture_info.width = (Uint32)resource_desc.Width;
		texture_info.height = (Uint32)resource_desc.Height;
		texture_info.format = GetFormat(resource_desc.Format);
		texture_info.initial_state = ResourceStates::Present;
		texture_info.clear_value = TextureClearValue(TextureClearValue::ClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		texture_info.bind_flags = BindFlags::Render_Target;

		// Uses texture constructor for outside resource management
		m_back_buffers[i] = std::make_unique<Texture>(this, texture_info, back_buffer);
		m_back_buffers[i]->SetName("Swap Chain Back Buffer");
		m_backbuffer_render_target_views[i] = m_device->CreateTextureRTV(m_back_buffers[i].get());
	}
}

Descriptor SwapChain::GetBackBufferRenderTargetDescriptor() const {
	return m_backbuffer_render_target_views[m_backbuffer_index];
}
}
