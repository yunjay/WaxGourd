#pragma once
#include "GpuBuffer.h"
#include "CommandList.h"
#include <format>

namespace WaxGourd {
GpuBuffer::GpuBuffer(Device* device, GpuBufferDesc const& buffer_info) : m_device(device), m_desc(buffer_info) {
	uint64 buffer_size = buffer_info.size;
	if (m_desc.buffer_info_flags.HasFlag(GpuBufferInfoFlagsEnum::ConstantBuffer))
		buffer_size = Align(buffer_size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.Width = buffer_size;
	resource_desc.Height = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.Alignment = 0;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;

	if (m_desc.bind_flags.HasFlag(BindFlags::Unordered_Access))
		resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	if (m_desc.bind_flags.HasFlag(BindFlags::Shader_Resource) == false)
		resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	D3D12_RESOURCE_STATES resource_state = D3D12_RESOURCE_STATE_COMMON;
	if (m_desc.buffer_info_flags.HasFlag(GpuBufferInfoFlagsEnum::RayTracingAccelerationStructure))
		resource_state = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;

	D3D12MA::ALLOCATION_DESC allocation_desc{};

	allocation_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
	if (m_desc.heap_type == ResourceHeapType::Readback) {
		allocation_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
		resource_state = D3D12_RESOURCE_STATE_COPY_DEST;
		resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}
	else if (m_desc.heap_type == ResourceHeapType::Upload) {
		allocation_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
		resource_state = D3D12_RESOURCE_STATE_GENERIC_READ;
	}

	auto device = device->GetDevice();
	auto allocator = device->GetDmaAllocator();

	// Actually allocate
	D3D12MA::Allocation* allocation = nullptr;
	HRESULT hr = allocator->CreateResource(
		&allocation_desc,
		&resource_desc,
		resource_state,
		nullptr,
		&allocation,
		IID_PPV_ARGS(m_gpu_resource.GetAddressOf())
	);
	ASSERT_HRESULT(hr);
	m_allocation = allocation;

	// Shared handle
	/*
	if (m_desc.buffer_info_flags.HasFlag(GpuBufferInfoFlagsEnum::Shared)) {
		hr = m_device->GetDevice()->CreateSharedHandle(m_gpu_resource.Get(), nullptr, GENERIC_ALL, nullptr, &m_shared_handle);
		ASSERT_HRESULT(hr);
	}
	*/

	if (m_desc.heap_type == ResourceHeapType::Readback)
	{
		hr = m_gpu_resource->Map(0, nullptr, &m_mapped_data);
		ASSERT_HRESULT(hr);
	}
	else if (m_desc.heap_type == ResourceHeapType::Upload)
	{
		D3D12_RANGE read_range{};
		hr = m_gpu_resource->Map(0, &read_range, &m_mapped_data);
		ASSERT_HRESULT	(hr);
	}
}

GpuBuffer::~GpuBuffer() {
	if (IsMapped()) {
		assert(m_gpu_resource != nullptr);
		Unmap();
	}
	m_allocation->Release();
}

void* GpuBuffer::Map() {
	if (m_mapped_data) 
		return m_mapped_data;

	if (m_desc.heap_type == ResourceHeapType::Readback) {
		ASSERT_HRESULT(m_gpu_resource->Map(0, nullptr, &m_mapped_data));
	}
	else if (m_desc.heap_type == ResourceHeapType::Upload)
	{
		D3D12_RANGE read_range{};
		ASSERT_HRESULT(m_gpu_resource->Map(0, &read_range, &m_mapped_data));
	}
	return m_mapped_data;
}

void GpuBuffer::Unmap() {
	m_gpu_resource->Unmap(0, nullptr);
	m_mapped_data = nullptr;
}

void GpuBuffer::Update(std::span<std::byte> data, uint64 offset /*= 0*/) {
	ASSERT_HRESULT(m_desc.heap_type == ResourceHeapType::Upload);
	if (m_mapped_data) {
		memcpy((uint8*)m_mapped_data + offset, data.data(), data.size());
	}
	else {
		Map();
		ASSERT_HRESULT(m_mapped_data);
		memcpy((uint8*)m_mapped_data + offset, data.data(), data.size());
	}
}
}