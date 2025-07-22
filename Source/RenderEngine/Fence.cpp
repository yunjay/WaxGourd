#pragma once
#include "Fence.h"
#include "Device.h"
#include "Macros.h"
#include "CommandQueue.h"

namespace WaxGourd {
Fence::Fence(Device* _device, std::string _name) {
	ID3D12Device* device = (ID3D12Device*)_device->GetDevice();

	ASSERT_HRESULT(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf())));
	m_fence->SetName((LPCWSTR)_name.c_str());
	
	// Event for this fence
	m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void Fence::Wait(FenceValue value) {
	if (!IsCompleted(value)) {
		m_fence->SetEventOnCompletion(value, m_event);
		WaitForSingleObjectEx(m_event, INFINITE, FALSE);
	}
}

void Fence::Signal(FenceValue value) {
	m_fence->Signal(value);
}

bool Fence::IsCompleted(FenceValue value) {
	// Fence values are incremental
	return this->GetCompletedValue() >= value;
}

FenceValue Fence::GetCompletedValue() const {
	return m_fence->GetCompletedValue();
}

}

