#include "IndirectCommandSignature.h"
#include "Device.h"

namespace WaxGourd {
namespace {
D3D12_INDIRECT_ARGUMENT_TYPE GetArgumentType(IndirectCommandType command_type) {
	switch (command_type) {
		case IndirectCommandType::Draw:			return D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
		case IndirectCommandType::DrawIndexed:	return D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
		case IndirectCommandType::Dispatch:		return D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
		case IndirectCommandType::DispatchMesh:	return D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH;
	}
	assert(false);
	return D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
}
uint32 GetArgumentStride(IndirectCommandType command_type) {
	switch (command_type) {
		case IndirectCommandType::Draw:			return sizeof(D3D12_DRAW_ARGUMENTS);
		case IndirectCommandType::DrawIndexed:	return sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
		case IndirectCommandType::Dispatch:		return sizeof(D3D12_DISPATCH_ARGUMENTS);
		case IndirectCommandType::DispatchMesh:	return sizeof(D3D12_DISPATCH_MESH_ARGUMENTS);
	}
	assert(false);
	return 0;
}
}
IndirectCommandSignature::IndirectCommandSignature(Device* device, IndirectCommandType command_type) {
	D3D12_COMMAND_SIGNATURE_DESC desc{};
	D3D12_INDIRECT_ARGUMENT_DESC argument_desc{};
	desc.NumArgumentDescs = 1;
	desc.pArgumentDescs = &argument_desc;
	desc.ByteStride = GetArgumentStride(command_type);
	argument_desc.Type = GetArgumentType(command_type);
	ASSERT_HRESULT(device->GetDevice()->CreateCommandSignature(&desc, nullptr, IID_PPV_ARGS(m_command_signature.GetAddressOf())));
}
}

