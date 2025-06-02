#include "Descriptor.h"
#include "DescriptorHeap.h"
#include "Macros.h"

namespace WaxGourd {
namespace {
// https://developer.nvidia.com/blog/advanced-api-performance-descriptors/
// https://www.gamedev.net/forums/topic/686440-d3d12-descriptor-heap-strategies/
constexpr uint32 MAX_ACTIVE_DESCRIPTORS = 1e6; // for CBV_SRV_UAV
constexpr uint32 MAX_ACTIVE_SAMPLERS = 2000;

constexpr D3D12_DESCRIPTOR_HEAP_TYPE GetD3D12DescriptorHeapType(DescriptorHeapType type) {
	switch (type) {
	case DescriptorHeapType::CBV_SRV_UAV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	case DescriptorHeapType::Sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	case DescriptorHeapType::RTV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	case DescriptorHeapType::DSV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	default:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	};
}
// TODO : Manage GPU visible / CPU visible descriptor heaps. Only scene present descriptors should be GPU visible.

} // translation unit namespace

DescriptorHeap::DescriptorHeap(Device* device, DescriptorHeapType type, uint32 descriptor_count, bool shader_visible) :
	m_device(device), m_type(type), descriptor_count(descriptor_count), m_shader_visible(shader_visible) {

	assert(descriptor_count <= UINT32_MAX && "Too many descriptors");
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};
	heap_desc.Flags = shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heap_desc.NumDescriptors = descriptor_count;
	heap_desc.Type = GetD3D12DescriptorHeapType(type);

	ASSERT_HRESULT(device->GetDevice()->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf())));
	
	m_descriptor_handle_size = device->GetDevice()->GetDescriptorHandleIncrementSize(heap_desc.Type);

	m_start_descriptor_handle = m_shader_visible ? 
		Descriptor(m_heap->GetCPUDescriptorHandleForHeapStart(), m_heap->GetGPUDescriptorHandleForHeapStart()) : 
		Descriptor(m_heap->GetCPUDescriptorHandleForHeapStart());

	m_free_list.reserve(descriptor_count);
	for (uint32 i = 0; i < descriptor_count; i++) {
		m_free_list.push_back(i); // free indices
	}
}
Descriptor DescriptorHeap::GetDescriptorHandle(uint32 index = 0) const {
	Descriptor handle = m_start_descriptor_handle;
	handle += (index * m_descriptor_handle_size);
	return handle;
}
Descriptor DescriptorHeap::AllocateDescriptor() {
	assert(m_free_list.size());
	Descriptor handle = m_start_descriptor_handle;
    int free_index = m_free_list.back();
	m_free_list.pop_back();
	handle += (free_index * m_descriptor_handle_size);
	return handle;
}
void DescriptorHeap::FreeDescriptor(Descriptor handle) {
    int cpu_idx = (int)((handle.GetCpuHandlePtr() - m_start_descriptor_handle.GetCpuHandlePtr()) / m_descriptor_handle_size);
	m_free_list.push_back(cpu_idx);
}
uint32 DescriptorHeap::GetHandleOffset(const Descriptor& DHandle) {
	return (uint32_t)(DHandle.GetCpuHandlePtr() - m_start_descriptor_handle.GetCpuHandlePtr()) / m_descriptor_handle_size;
}
}