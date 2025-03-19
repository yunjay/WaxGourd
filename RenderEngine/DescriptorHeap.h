#pragma once
#include "ComPointer.h"
#include "Device.h"
#include "DescriptorHandle.h"
#include "MathDefines.h"
#include <d3d12.h>

/*
* Currently using free list allocation for descriptor heaps to use with bindless.
* Could try Ring Buffer allocation for more transient allocations.
*/
namespace WaxGourd {
enum class DescriptorHeapType : uint8 {
	CBV_SRV_UAV, // Constant Buffer View, Shader Resource View, Unordered Access View
	Sampler, // Texture Samplers
	RTV, // Render Target View
	DSV, // Depth Stencil View
	Size
};

class DescriptorHeap {
public:
	DescriptorHeap(Device* device, DescriptorHeapType type, uint32 descriptor_count, bool shader_visible);
	~DescriptorHeap() = default;
	operator ID3D12DescriptorHeap* () const { return m_heap.Get(); }
	ID3D12DescriptorHeap* GetHeap() const { return m_heap.Get(); }

	DescriptorHandle AllocateDescriptor();
	void FreeDescriptor(DescriptorHandle handle);
	uint32 GetHandleOffset(const DescriptorHandle& DHandle);
	DescriptorHandle GetDescriptorHandle(uint32 index = 0) const;
private:
	Device* m_device;

	ComPointer<ID3D12DescriptorHeap> m_heap = nullptr;
	DescriptorHandle m_start_descriptor_handle;
	uint32 m_descriptor_handle_size = 0;
	DescriptorHeapType m_type = DescriptorHeapType::CBV_SRV_UAV;
	uint32 descriptor_count = 0;
	bool m_shader_visible = false;

	std::vector<uint32> m_free_list;
};
}