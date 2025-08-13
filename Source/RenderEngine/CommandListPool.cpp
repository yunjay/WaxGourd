#include "CommandListPool.h"

namespace WaxGourd {
CommandListPool::CommandListPool(
    Device* device,
    CommandQueue* command_queue,
    CommandListType type,
    std::string name_prefix,
    uint32 initial_size ) : 
    m_device(device),
    m_command_queue(command_queue),
    m_command_list_type(type),
    m_name_prefix(name_prefix) {
    // Create initial command lists
    for (uint32 i = 0; i < initial_size; ++i) {
        // Get a fresh command allocator for this command list
        CommandAllocator* allocator = m_device->GetCommandAllocatorPool(m_command_list_type)->GetAllocator(0); // 

        std::string name = m_name_prefix + "_" + std::to_string(i);
        auto cmd_list = std::make_unique<CommandList>(m_device, m_command_queue, allocator, m_command_list_type, name);

        m_available_command_lists.Push(cmd_list.get());
        // ownership pool
        m_command_list_pool.Push(std::move(cmd_list));
    }
}

CommandList* CommandListPool::GetCommandList() {
    CommandList* result = nullptr;
    // Try to get a command list from the available list
    if (m_available_command_lists.TryPop(result)) {
        // Get a fresh command allocator for this command list
        // Command allocator pool handles synchronization and allocation of actual memory (allocators)
        FenceValue completed_fence_value = m_device->GetFenceValue(m_command_list_type);
        CommandAllocator* allocator = m_device->GetCommandAllocatorPool(m_command_list_type)
			->GetAllocator(completed_fence_value); 

        // Reset the command list with the new allocator
        result->Reset(allocator);
    }
    else {
        // No command lists available, create a new one
        FenceValue completed_fence_value = m_device->GetFenceValue(m_command_list_type);
        CommandAllocator* allocator = m_device->GetCommandAllocatorPool(m_command_list_type)
            ->GetAllocator(completed_fence_value);

        std::string name = m_name_prefix + "_" + std::to_string(m_command_list_pool.Size());

        auto cmd_list = std::make_unique<CommandList>(m_device, m_command_queue, allocator, m_command_list_type, name);
        result = cmd_list.get();

        m_command_list_pool.Push(std::move(cmd_list));
    }
    return result;
}

void CommandListPool::ReturnCommandList(CommandList* command_list) {
    if (command_list == nullptr) {
        assert(false && "Attempting to return null command list"); 
        return;
    }
    if (command_list->GetCommandAllocator()->GetCommandListType() != m_command_list_type) {
        assert(false && "Returning command list to the wrong pool type"); 
        return;
    }
    // Add the command list back to the available list. Will be reset with a new allocator on Get.
    m_available_command_lists.Push(command_list);
}
}