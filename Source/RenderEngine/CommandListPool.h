#pragma once
#include "ConcurrentQueue.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "Device.h"

namespace WaxGourd {
class CommandListPool {
public:
	CommandListPool(
		Device* device,
		CommandQueue* command_queue,
		CommandListType type,
		std::string name_prefix,
		uint32 initial_size = 5
	);
	~CommandListPool() = default;

	DELETE_COPY_OPERATORS(CommandListPool);
	DELETE_MOVE_OPERATORS(CommandListPool);

	// Get a command list from the pool. Resets allocator on Get, so must return to pool after execution.
	CommandList* GetCommandList();
	// Return a command list to the pool
	void ReturnCommandList(CommandList* command_list);

	// Get the command list type
	CommandListType GetCommandListType() const { return m_command_list_type; }
private:
	const Device* m_device;
	const CommandListType m_command_list_type;
	// Could remove, just here for command list creation convenience. To remove, would need to move command list functions out of command list and into Device.
	const CommandQueue* m_command_queue; 

	std::string m_name_prefix;
	// Available command lists (not owning)
	ConcurrentQueue<CommandList*> m_available_command_lists;
	// Owns all command lists in the pool
	ConcurrentQueue<std::unique_ptr<CommandList>> m_command_list_pool;
};
}