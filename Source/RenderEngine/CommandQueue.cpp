#include "CommandQueue.h"
#include "Device.h"
#include "CommandList.h"
#include "CommandListManager.h" // Manager will actually execute on queues
#include "Macros.h"
#include <span>
#include <vector>
namespace WaxGourd {
	CommandQueue::CommandQueue(Device* _device, CommandListType _command_list_type, std::string _name = "") {
		ID3D12Device* device = _device->GetDevice();
		
		D3D12_COMMAND_QUEUE_DESC queue_desc{};
		
		queue_desc.Type = (D3D12_COMMAND_LIST_TYPE)_command_list_type;
		queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.NodeMask = 0;

		ASSERT_HRESULT(device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(m_command_queue.GetAddressOf())));
		
		m_command_queue->SetName((LPCWSTR)_name.c_str()); // only works with ASCII
	}

	void CommandQueue::ExecuteCommandLists(std::span<CommandList*> _command_lists) {
		if (_command_lists.empty()) return;

		for (CommandList* command_list : _command_lists) command_list->WaitAll();

		std::vector<ID3D12CommandList*> d3d12_cmd_lists(_command_lists.size());
		for (uint32 i = 0; i < d3d12_cmd_lists.size(); ++i) 
			d3d12_cmd_lists[i] = (ID3D12GraphicsCommandList6*)_command_lists[i];
		
		m_command_queue->ExecuteCommandLists((uint32)d3d12_cmd_lists.size(), d3d12_cmd_lists.data());

		for (CommandList* command_list : _command_lists) command_list->SignalAll();
	}

	void CommandQueue::Signal(Fence* _fence, FenceValue _fence_value) {
		m_command_queue->Signal(_fence->GetFence(), _fence_value);
	}

	void CommandQueue::Wait(Fence* _fence, FenceValue _fence_value) {
		m_command_queue->Wait(_fence->GetFence(), _fence_value);
	}

}

