#pragma once
#include "CpuBuffer.h"
#include "ComPointer.h"
#include "Device.h"
#include "ResourceEnum.h"
#include "MathDefines.h"
#include "Macros.h"
#include <directx/d3d12.h>
#include <span>
#include <cstddef>

namespace WaxGourd {
struct GpuBufferDesc {
	uint64 size = 0;
	ResourceHeapType heap_type = ResourceHeapType::Default;
	BindFlag bind_flags = BindFlags::None;
	GpuBufferInfoFlags buffer_info_flags = GpuBufferInfoFlagsEnum::None;
	uint32 stride = 0;
	ResourceFormat format = ResourceFormat::Unknown;
};
struct BufferDescriptorData {
	uint64 offset = 0; // Bytes
	uint64 size = uint64(-1);
};

class GpuBuffer {
public:
	DELETE_COPY_OPERATORS(GpuBuffer);
	DELETE_MOVE_OPERATORS(GpuBuffer);
	GpuBuffer(Device* device, GpuBufferDesc const& buffer_info);
template <typename Type>
	GpuBuffer(Device* device, GpuBufferDesc const& buffer_info, std::span<Type> initial_data) : GpuBuffer(device, buffer_info) {
		if (IsMapped() && buffer_info.heap_type == ResourceHeapType::Upload) {
			// map directly if upload heap
			memcpy(m_mapped_data, initial_data.data(), initial_data.size());
		}
		if (initial_data.data() != nullptr && buffer_info.heap_type != ResourceHeapType::Upload) {
			// See https://therealmjp.github.io/posts/gpu-memory-pool/
			auto copy_command_list = m_device->GetCommandList();
			auto upload_buffer = device->GetDynamicAllocator();
			copy_command_list->CopyBuffer(
				*this, // destination buffer
				0, // destination offset
				*upload_buffer.,
				0, // source offset
				initial_data.size());

			if (desc.bind_flags.HasAnyFlag(BindFlag::Shader_Resource)) {
				// Sync copy to srv
				copy_command_list->BufferBarrier(*this, ResourceState::Copy_Destination, ResourceState::All_Shader_Resource);
				copy_command_list->FlushBarriers();
			}
		}
	}
template <typename Type>
	GpuBuffer(Device* device, GpuBufferDesc const& buffer_info, const CpuBuffer<Type>& initial_data) : GpuBuffer(device, buffer_info) {
		if (IsMapped()) {
			memcpy(m_mapped_data, initial_data, desc.size);
		}
		if (initial_data != nullptr && desc.resource_usage != GfxResourceUsage::Upload)
		{
			auto cmd_list = device->GetCommandList();
			auto upload_buffer = device->GetDynamicAllocator();
			GpuCpuStreamingPair upload_alloc = upload_buffer->Allocate(buffer_size);
			upload_alloc.Update(initial_data, desc.size);
			cmd_list->CopyBuffer(
				*this,
				0,
				*upload_alloc.buffer,
				upload_alloc.offset,
				desc.size);

			if (desc.bind_flags.HasAnyFlag(BindFlags::Shader_Resource))
			{
				cmd_list->BufferBarrier(*this, GfxResourceState::CopyDst, GfxResourceState::AllSRV);
				cmd_list->FlushBarriers();
			}
		}
	}
	~GpuBuffer();

	Device* GetDevice() const { return m_device; }
	operator ID3D12Resource* () const { return m_gpu_resource.Get(); }
	ID3D12Resource* GetGpuResource() const { return m_gpu_resource.Get(); }
	GpuBufferDesc const& GetDesc() const { return m_desc; }
	uint64 GetGpuAddress() const { return m_gpu_resource->GetGPUVirtualAddress(); }
	uint64 GetSize() const { return m_desc.size; }
	uint32 GetStride() const { return m_desc.stride; }
	uint32 GetCount() const { return static_cast<uint32>(m_desc.size / m_desc.stride); }
	ResourceFormat GetFormat() const { return m_desc.format; }

	bool IsMapped() const { return m_mapped_data != nullptr; }
	void* GetMappedData() const { return m_mapped_data; }
	template<typename Type> Type* GetMappedData() const { return reinterpret_cast<T*>(data); }

	void* Map();
	void Unmap();
	void Update(std::span<std::byte> data, uint64 offset); // size in bytes
	template<typename Type>
	void Update(std::span<Type> data, uint64 offset){
		Update(std::span<std::byte>(reinterpret_cast<std::byte*>(span.data()), span.size_bytes(), offset);
	}

	void SetName(std::string name) { m_gpu_resource->SetName((LPCWSTR)(name).c_str()); }
private:
	ComPointer<ID3D12Resource> m_gpu_resource = nullptr;
	GpuBufferDesc m_desc;
	Device* m_device;
	D3D12MA::Allocation* m_allocation = nullptr;
	void* m_mapped_data = nullptr;
	// HANDLE m_shared_handle = nullptr;
};

struct GpuBufferView {
	explicit GpuBufferView(GpuBuffer* buffer)
		: buffer_location(buffer->GetGpuAddress()), size_in_bytes((uint32)buffer->GetSize()), stride_in_bytes(buffer->GetStride()), format(buffer->GetFormat())
	{}
	uint64 buffer_location = 0;
	uint32 size_in_bytes = 0;
	uint32 stride_in_bytes = 0;
	ResourceFormat format = ResourceFormat::Unknown;
};
}