#pragma once
#include "CommandList.h"
#include "ComPointer.h"
#include "ConcurrentQueue.h"
#include "Device.h"
#include "Fence.h"
#include "MathDefines.h"
#include <directx/d3d12.h>
namespace WaxGourd {
class CommandAllocator {
private:
	ComPointer<ID3D12CommandAllocator> m_command_allocator;
	const CommandListType m_command_list_type;
public:
	CommandAllocator(Device* device, CommandListType type);
	~CommandAllocator() = default; // ComPointer calls release

	void Reset();

	operator ID3D12CommandAllocator* () const { return m_command_allocator.Get(); }
	ID3D12CommandAllocator* GetD3DCommandAlloator() const { return m_command_allocator.Get(); }
	CommandListType GetCommandListType() const { return m_command_list_type; }
};

struct CommandAllocatorPtrWithFenceValue {
	CommandAllocatorPtrWithFenceValue(CommandAllocator* _command_allocator, FenceValue _fence_value)
		: command_allocator{ _command_allocator }, fence_value{ _fence_value } {
	};
	CommandAllocatorPtrWithFenceValue() : command_allocator{ nullptr }, fence_value{ uint64_MAX } {};
	CommandAllocator* command_allocator;
	FenceValue fence_value;
};

class CommandAllocatorPool {
private:
	const CommandListType m_command_list_type;
	Device* m_device;

	ConcurrentQueue<std::unique_ptr<CommandAllocator>> m_allocator_pool;	
	ConcurrentQueue<CommandAllocatorPtrWithFenceValue> m_free_allocators; // Never shrinks in size.

	void addAllocatorToPool();
public:
	CommandAllocatorPool(Device* device, CommandListType type, uint32 initial_size = 18u);
	~CommandAllocatorPool() = default; // CommandAllocator handles release (RAII)

	CommandAllocator* GetAllocator(FenceValue completed_fence_value); // Fetches and resets available allocator
	void ReturnAllocator(FenceValue fence_value, CommandAllocator* allocator);

	inline uint64 GetSize();
	CommandListType GetCommandListType() const { return m_command_list_type; }
}


} // namespace WaxGourd