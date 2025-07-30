#pragma once
#include "CommandAllocator.h"
#include "Macros.h"
namespace WaxGourd {
#pragma region CommandAllocator
CommandAllocator::CommandAllocator(Device* device, CommandListType type) : m_command_list_type(type) {
	auto d3ddevice = device->GetDevice();
	ASSERT_HRESULT(
		d3ddevice->CreateCommandAllocator(GetD3D12CommandListType(m_command_list_type), IID_PPV_ARGS(m_command_allocator.GetAddressOf()))
	);
}
void CommandAllocator::Reset() {
	ASSERT_HRESULT(m_command_allocator->Reset());
}
#pragma endregion

#pragma region CommandAllocatorPool
CommandAllocatorPool::CommandAllocatorPool(Device* device, CommandListType type, uint32 initial_size) :
	m_device{ device }, m_command_list_type{ type } {
	if (initial_size > 0) {
		for (int i = 0; i < initial_size; i++) {
			addAllocatorToPool();
		}
	}
}
CommandAllocator* CommandAllocatorPool::GetAllocator(FenceValue completed_fence_value) {
	CommandAllocatorPtrWithFenceValue allocator_info;
    // First attempt to get an existing allocator
    while (m_free_allocators.TryPop(allocator_info)) {
        if (allocator_info.fence_value <= completed_fence_value) {
            // Reset allocator
            allocator_info.command_allocator->Reset();
            
            return allocator_info.command_allocator;
        }
        else {
            // Still in use
            m_free_allocators.Push(allocator_info);
            break;
        }
    }

    // If failed to get allocator from free list, create a new one.
    // Create a new allocator directly and don't add it to free list
    std::unique_ptr<CommandAllocator> new_allocator = std::make_unique<CommandAllocator>(m_device, m_command_list_type);
    CommandAllocator* result = new_allocator.get();
    // Add to ownership pool only
    m_allocator_pool.Push(std::move(new_allocator));

    return result; // User is in charge of returning
}
void CommandAllocatorPool::ReturnAllocator(FenceValue fence_value, CommandAllocator* allocator) {
    if (allocator == nullptr) {
        assert(false && "Null allocator returned to pool");
        return;
    }

    // Verify this allocator is of the correct type for this pool
    // This helps catch mismatched allocators being returned to the wrong pool
    if (allocator->GetCommandListType() != m_command_list_type) {
        assert(false && "Returning allocator to the wrong pool type!");
        return;
    }

    // Create a fence/allocator pair and add to free allocators queue
    CommandAllocatorPtrWithFenceValue allocator_entry{ allocator, fence_value };

    // Push the allocator to the free queue with its fence value
    // It will become available again when the GPU completes this fence value
    m_free_allocators.Push(allocator_entry);
}
inline uint64 CommandAllocatorPool::GetSize() { return m_allocator_pool.Size(); }
void CommandAllocatorPool::addAllocatorToPool() {
	std::unique_ptr<CommandAllocator> new_command_allocator = std::make_unique<CommandAllocator>(m_device,m_command_list_type);
	CommandAllocator* new_command_allocator_raw_ptr = new_command_allocator.get();
	m_allocator_pool.Push(std::move(new_command_allocator));
	m_free_allocators.Push(CommandAllocatorPtrWithFenceValue{ new_command_allocator_raw_ptr ,FenceValue(0) }); // 0 for new allocators
}
#pragma endregion
}	