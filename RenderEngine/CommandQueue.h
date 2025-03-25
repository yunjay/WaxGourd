#pragma once
#include <d3d12.h>
#include <span>
#include "ComPointer.h"
#include "CommandList.h"
#include "Device.h"
#include "Fence.h"

namespace WaxGourd
{
enum class CommandQueueType : uint8 {
	Rendering,
	Compute,
	Copy
};
class CommandQueue
{
public:
	CommandQueue(Device* _device, CommandListType _command_list_type, std::string _name = "");
	~CommandQueue() = default;
		
	void ExecuteCommandLists(std::span<CommandList*> cmd_lists);

	void Signal(Fence* _fence, FenceValue _fence_value);
	void Wait(Fence* _fence, FenceValue _fence_value);

	CommandQueueType GetType() const { return m_type; }

	operator ID3D12CommandQueue* () const { return m_command_queue.Get(); }
	ID3D12CommandQueue* GetCommandQueue() const { return m_command_queue.Get(); }
private:
	ComPointer<ID3D12CommandQueue> m_command_queue;
	CommandQueueType m_type;
};
}