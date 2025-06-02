#pragma once
#include <directx/d3d12.h>
#include <string>
#include "CommandQueue.h"
#include "ComPointer.h"
#include "Device.h"
#include "MathDefines.h"

namespace WaxGourd
{
typedef uint64 FenceValue;
class Fence
{
public:
	Fence(Device* _device, std::string _name);
	~Fence() = default;

	void Wait(FenceValue _value);
	void Signal(FenceValue _value);

	bool IsCompleted(FenceValue _value);
	FenceValue GetCompletedValue() const;

	operator ID3D12Fence* () const { return m_fence.Get(); }
	ID3D12Fence* GetFence() const { return m_fence.Get(); }
private:
	ComPointer<ID3D12Fence> m_fence = nullptr;
	HANDLE m_event = nullptr;
};
}