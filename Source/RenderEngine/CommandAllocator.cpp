#pragma once
#include "CommandAllocator.h"
#include "Macros.h"
namespace WaxGourd {
CommandAllocator::CommandAllocator(Device* device, CommandListType type) : m_command_list_type(type) {
	auto d3ddevice = device->GetDevice();
	ASSERT_HRESULT(
		d3ddevice->CreateCommandAllocator(GetD3D12CommandListType(m_command_list_type), IID_PPV_ARGS(m_command_allocator.GetAddressOf()))
	);
}
void CommandAllocator::Reset() {
	ASSERT_HRESULT(m_command_allocator->Reset());
}
}