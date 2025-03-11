#pragma once
#include "MathDefines.h"
#include <d3d12.h>
class DescriptorHandle
{
public:
    DescriptorHandle() {
        m_cpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_gpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }
    DescriptorHandle( D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle )
        : m_cpu_handle(CpuHandle){
        m_gpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }
    // Shader visible
    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle)
        : m_cpu_handle(CpuHandle), m_gpu_handle(GpuHandle) { 
    }
    operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return m_cpu_handle; }
    operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return m_gpu_handle; }

    size_t GetCpuHandlePtr() const { return m_cpu_handle.ptr; }
    uint64_t GetGpuHandlePtr() const { return m_gpu_handle.ptr; }
    bool IsNull() const { return m_cpu_handle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }
    bool IsShaderVisible() const { return m_gpu_handle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }

private:
    D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_handle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_handle;
};