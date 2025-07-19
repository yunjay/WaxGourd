#pragma once
#include "ComPointer.h"
#include "Descriptor.h"
#include "Device.h"
#include "MathDefines.h"

#include <directx/d3d12.h>
#include <list>
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
	NUM_DESCRIPTOR_HEAP_TYPES
}; 
struct DescriptorRange {
	Descriptor begin;
	Descriptor end;
};
struct DescriptorAllocatorInfo {
	DescriptorHeapType type = DescriptorHeapType::CBV_SRV_UAV;
	uint32 descriptor_count = 0;
	bool shader_visible = false;
};
class DescriptorHeap {
public:
	DescriptorHeap(Device* device, DescriptorHeapType type, uint32 descriptor_count, bool shader_visible);
	~DescriptorHeap() = default;
	operator ID3D12DescriptorHeap* () const { return m_heap.Get(); }
	ID3D12DescriptorHeap* GetHeap() const { return m_heap.Get(); }

	Descriptor AllocateDescriptor();
	void FreeDescriptor(Descriptor descriptor);
	Descriptor GetDescriptorHandle(uint32 index = 0) const;
private:
	void createD3DHeap();
	Device* m_device;
	DescriptorHeapType m_type = DescriptorHeapType::CBV_SRV_UAV;
	ComPointer<ID3D12DescriptorHeap> m_heap = nullptr;
	Descriptor m_start_descriptor; // head
	Descriptor m_end_descriptor; // tail
	std::list<DescriptorRange> m_free_descriptor_ranges;
	uint32 m_descriptor_handle_size = 0;
	uint32 m_descriptor_count = 0;
	bool m_shader_visible = false;

	std::vector<uint32> m_free_list;
};
}