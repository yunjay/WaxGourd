#pragma once
#include "DescriptorHeap.h"
#include "Descriptor.h"
#include <directx/d3d12.h>
/*
* Currently using free list allocation for descriptor heaps to use with bindless.
* Could try Ring Buffer allocations.
*/
namespace WaxGourd {
inline constexpr D3D12_DESCRIPTOR_HEAP_TYPE GetD3D12HeapType(DescriptorHeapType type) {
	switch (type) {
	case DescriptorHeapType::CBV_SRV_UAV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	case DescriptorHeapType::Sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	case DescriptorHeapType::RTV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	case DescriptorHeapType::DSV:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	}
	return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}

DescriptorHeap::DescriptorHeap(Device* device, DescriptorHeapType type, uint32 descriptor_count, bool shader_visible) 
	: m_type{ type },
	m_device{ device },
	m_descriptor_count{ descriptor_count },
	m_shader_visible{ shader_visible },
	m_start_descriptor{}, m_end_descriptor{} {

	createD3DHeap();

	m_start_descriptor.m_cpu_handle = m_heap->GetCPUDescriptorHandleForHeapStart();
	if (shader_visible) 
		m_start_descriptor.m_gpu_handle = m_heap->GetGPUDescriptorHandleForHeapStart();
	m_start_descriptor.m_index = 0;
	m_end_descriptor.Increment(m_descriptor_handle_size, descriptor_count - 1);
	m_free_descriptor_ranges.emplace_back(m_start_descriptor, m_end_descriptor);
}
void DescriptorHeap::createD3DHeap() {
	assert(m_descriptor_count <= UINT32_MAX);
	D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};
	heap_desc.Flags = m_shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heap_desc.NumDescriptors = m_descriptor_count;
	heap_desc.Type = GetD3D12HeapType(m_type);
	ASSERT_HRESULT(m_device->GetDevice()->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf())));
	m_descriptor_handle_size = m_device->GetDevice()->GetDescriptorHandleIncrementSize(heap_desc.Type);
}
Descriptor DescriptorHeap::AllocateDescriptor() {
	DescriptorRange& range = m_free_descriptor_ranges.front();
	Descriptor descriptor = range.begin;
	range.begin.Increment(m_descriptor_handle_size);
	if (range.begin == range.end) // if the range is full, remove from list
		m_free_descriptor_ranges.pop_front();
	return descriptor;
}
void DescriptorHeap::FreeDescriptor(Descriptor descriptor) {
	Descriptor incremented_descriptor_handle = descriptor;
	incremented_descriptor_handle.Increment(m_descriptor_handle_size);
	DescriptorRange range{
		.begin = descriptor,
		.end = incremented_descriptor_handle
	};

	bool found = false;
	for (auto free_range = std::begin(m_free_descriptor_ranges);
		free_range != std::end(m_free_descriptor_ranges); free_range++) {
		// Descriptor is right before a free range (extend front)
		if (free_range->begin == incremented_descriptor_handle) {
			free_range->begin = descriptor;
			found = true;
		}
		// Descriptor is right after a free range (extend end)
		else if (free_range->end == descriptor) {
			free_range->end.Increment(m_descriptor_handle_size);
			found = true;
		}
		// Descriptor is before current range
		else if (free_range->begin.GetIndex() > descriptor.GetIndex()) {
			m_free_descriptor_ranges.insert(free_range, range);
			found = true;
		}
		if (found) 
			break;
	}
	if (!found) 
		m_free_descriptor_ranges.push_back(range);
}
Descriptor DescriptorHeap::GetDescriptorHandle(uint32 index = 0) const {
	assert(m_heap != nullptr);
	assert(index < m_descriptor_count);

	Descriptor handle = m_start_descriptor;
	handle.Increment(m_descriptor_handle_size, index); // Walk from start to the desired index
	return handle;
}

}// namespace WaxGourd
