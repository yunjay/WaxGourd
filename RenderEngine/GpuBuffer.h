#pragma once
#include "CpuBuffer.h"
#include "ComPointer.h"
#include "Device.h"
#include "ResourceCommon.h"
#include "MathDefines.h"
#include "Macros.h"
#include <d3d12.h>
#include <span>
#include <cstddef>

namespace WaxGourd {
struct GpuBufferInfo {
	uint64 size = 0;
	ResourceHeapType heap_type = ResourceHeapType::Default;
	BindFlags bind_flags = BindFlags::None;
	ResourceFlags resource_flags = ResourceFlags::None;
	uint32 stride = 0;
	ResourceFormat format = ResourceFormat::Unknown;
};
struct BufferDescriptorData {
	uint64 offset = 0;
	uint64 size = uint64(-1);
};

class GpuBuffer {
public:
	DELETE_COPY_OPERATORS(GpuBuffer);
	DELETE_MOVE_OPERATORS(GpuBuffer);
	GpuBuffer(Device* device, GpuBufferInfo const& buffer_info);
template <typename Type>
	GpuBuffer(Device* device, GpuBufferInfo const& buffer_info, std::span<Type> initial_data) : GpuBuffer(device, buffer_info) {
		if (IsMapped()) {
			memcpy(m_mapped_data, initial_data.data(), initial_data.size());
		}
		if (initial_data.data() != nullptr && buffer_info.heap_type != ResourceHeapType::Upload) {
			auto copy_command_list = m_device->GetCommandList();
			cmd_list->CopyBuffer(
				*this,
				0,
				*upload_alloc.buffer,
				upload_alloc.offset,
				desc.size);

			if (HasAnyFlag(desc.bind_flags, GfxBindFlag::ShaderResource)) {
				cmd_list->BufferBarrier(*this, GfxResourceState::CopyDst, GfxResourceState::AllSRV);
				cmd_list->FlushBarriers();
			}
		}
	}
template <typename Type>
	GpuBuffer(Device* device, GpuBufferInfo const& buffer_info, const CpuBuffer<Type>& initial_data) : GpuBuffer(device, buffer_info) {
		if (IsMapped()) {
			memcpy(m_mapped_data, initial_data, desc.size);
		}
		if (initial_data != nullptr && desc.resource_usage != GfxResourceUsage::Upload)
		{
			auto cmd_list = gfx->GetCommandList();
			auto upload_buffer = gfx->GetDynamicAllocator();
			GpuCpuStreamingPair upload_alloc = upload_buffer->Allocate(buffer_size);
			upload_alloc.Update(initial_data, desc.size);
			cmd_list->CopyBuffer(
				*this,
				0,
				*upload_alloc.buffer,
				upload_alloc.offset,
				desc.size);

			if (HasAnyFlag(desc.bind_flags, GfxBindFlag::ShaderResource))
			{
				cmd_list->BufferBarrier(*this, GfxResourceState::CopyDst, GfxResourceState::AllSRV);
				cmd_list->FlushBarriers();
			}
		}
	}
	~GpuBuffer();

	operator ID3D12Resource* () const { return m_gpu_buffer.Get(); }
	ID3D12Resource* GetResource() const { return m_gpu_buffer.Get(); }
	GpuBufferInfo const& GetInfo() const { return m_info; }
	uint64 GetGpuHandle() const { return m_gpu_buffer->GetGPUVirtualAddress(); }
	uint64 GetSize() const { return m_info.size; }
	uint32 GetStride() const { return m_info.stride; }
	uint32 GetCount() const { return static_cast<uint32>(m_info.size / m_info.stride); }
	ResourceFormat GetFormat() const { return m_info.format; }

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

	void SetName(std::string name) { m_gpu_buffer->SetName((LPCWSTR)(name).c_str()); }

private:
	ComPointer<ID3D12Resource> m_gpu_buffer = nullptr;
	GpuBufferInfo m_info;
	Device* m_device;
	D3D12MA::Allocation* m_allocation = nullptr;
	void* m_mapped_data = nullptr;
};

struct GpuBufferView {
	explicit GpuBufferView(GpuBuffer* buffer)
		: buffer_location(buffer->GetGpuHandle()), size_in_bytes((Uint32)buffer->GetSize()), stride_in_bytes(buffer->GetStride()), format(buffer->GetFormat())
	{}
	uint64 buffer_location = 0;
	uint32 size_in_bytes = 0;
	uint32 stride_in_bytes = 0;
	ResourceFormat format = ResourceFormat::Unknown;
};

struct GpuCpuStreamingPair {
	GpuBuffer* buffer = nullptr;
	void* cpu_buffer = nullptr;
	uint64 gpu_handle = 0;
	uint64 offset = 0;
	uint64 size = 0;
	void Update(void const* data, uint64 size, uint64 offset = 0) {
		memcpy((uint8*)cpu_buffer + offset, data, size);
	}
	template<typename Type>
	void Update(Type const& data) {
		memcpy(cpu_address, &data, sizeof(T));
	}
};

}