#pragma once
#include "MathDefines.h"
#include <d3d12.h>
#define D3D12_GPU_VIRTUAL_ADDRESS_INVALID ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
namespace WaxGourd {

class DescriptorHandle
{
public:
    DescriptorHandle() {
        m_cpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_INVALID;
        m_gpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_INVALID;
    }
    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle)
        : m_cpu_handle(cpu_handle){
        m_gpu_handle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_INVALID;
    }
    // Shader visible
    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
        : m_cpu_handle(cpu_handle), m_gpu_handle(gpu_handle) {
    }
    operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return m_cpu_handle; }
    operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return m_gpu_handle; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const { return m_cpu_handle; }
    D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return m_gpu_handle; }
    size_t GetCpuHandlePtr() const { return m_cpu_handle.ptr; }
    uint64_t GetGpuHandlePtr() const { return m_gpu_handle.ptr; }
    bool IsNull() const { return m_cpu_handle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_INVALID; }
    bool IsShaderVisible() const { return m_gpu_handle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_INVALID; }
    // Increment
    void operator += (int offsetTimesDescriptorSize){
        if (m_cpu_handle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_INVALID)
            m_cpu_handle.ptr += offsetTimesDescriptorSize;
        if (m_gpu_handle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_INVALID)
            m_gpu_handle.ptr += offsetTimesDescriptorSize;
    }
private:
    D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_handle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpu_handle;
};
}