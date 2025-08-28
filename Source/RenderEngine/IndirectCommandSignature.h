#pragma once
#include "Macros.h"
#include "MathDefines.h"
#include "ComPointer.h"

namespace WaxGourd {
enum class IndirectCommandType : uint8 {
	Draw,
	DrawIndexed,
	Dispatch,
	DispatchMesh,
	NumIndirectCommandTypes
};
class Device;

class IndirectCommandSignature {
public:
	IndirectCommandSignature(Device* device, IndirectCommandType command_type);
	operator ID3D12CommandSignature* () const {
		return m_command_signature.Get();
	}
private:
	ComPointer<ID3D12CommandSignature> m_command_signature;
};

class DrawIndirectSignature : public IndirectCommandSignature {
public:
	explicit DrawIndirectSignature(Device* gfx) : IndirectCommandSignature(gfx, IndirectCommandType::Draw) {}
};

class DrawIndexedIndirectSignature : public IndirectCommandSignature {
public:
	explicit DrawIndexedIndirectSignature(Device* gfx) : IndirectCommandSignature(gfx, IndirectCommandType::DrawIndexed) {}
};

class DispatchIndirectSignature : public IndirectCommandSignature {
public:
	explicit DispatchIndirectSignature(Device* gfx) : IndirectCommandSignature(gfx, IndirectCommandType::Dispatch) {}
};

class DispatchMeshIndirectSignature : public IndirectCommandSignature {
public:
	explicit DispatchMeshIndirectSignature(Device* gfx) : IndirectCommandSignature(gfx, IndirectCommandType::DispatchMesh) {}
};
}