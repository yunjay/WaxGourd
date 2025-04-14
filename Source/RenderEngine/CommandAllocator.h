#pragma once
#include "CommandList.h"
#include "ComPointer.h"
#include "Device.h"
#include "MathDefines.h"
#include <directx/d3d12.h>
namespace WaxGourd {
class CommandAllocator {
public:
	CommandAllocator(Device* device, CommandListType type);
	~CommandAllocator() = default;

	void Reset();

	operator ID3D12CommandAllocator* () const { return m_command_allocator.Get(); }
	ID3D12CommandAllocator* GetCommandAlloator() const { return m_command_allocator.Get(); }
	CommandListType m_command_list_type;
private:
	ComPointer<ID3D12CommandAllocator> m_command_allocator;
};
}