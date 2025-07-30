#pragma once
#include <directx/d3d12.h>
#include <algorithm>
#include "EnumFlags.h"
#include "MathDefines.h"
#include "Macros.h"
namespace WaxGourd {
#pragma region Formats
enum class ResourceFormat {
	Unknown,
	R32G32B32A32_FLOAT,
	R32G32B32A32_UINT,
	R32G32B32A32_Int,
	R32G32B32_Float,
	R32G32B32_UINT,
	R32G32B32_SINT,
	R16G16B16A16_FLOAT,
	R16G16B16A16_UNORM,
	R16G16B16A16_UINT,
	R16G16B16A16_SNORM,
	R16G16B16A16_SINT,
	R32G32_FLOAT,
	R32G32_UINT,
	R32G32_SINT,
	R32G8X24_TYPELESS,
	D32_FLOAT_S8X24_UINT,
	R10G10B10A2_UNORM,
	R10G10B10A2_UINT,
	R11G11B10_FLOAT,
	R8G8B8A8_UNORM,
	R8G8B8A8_UNORM_SRGB,
	R8G8B8A8_UINT,
	R8G8B8A8_SNORM,
	R8G8B8A8_SINT,
	B8G8R8A8_UNORM,
	B8G8R8A8_UNORM_SRGB,
	R16G16_FLOAT,
	R16G16_UNORM,
	R16G16_UINT,
	R16G16_SNORM,
	R16G16_SINT,
	R32_TYPELESS,
	D32_FLOAT,
	R32_FLOAT,
	R32_UINT,
	R32_SINT,
	R24G8_TYPELESS,
	D24_UNORM_S8_UINT,
	R8G8_UNORM,
	R8G8_UINT,
	R8G8_SNORM,
	R8G8_SINT,
	R16_TYPELESS,
	R16_FLOAT,
	D16_UNORM,
	R16_UNORM,
	R16_UINT,
	R16_SNORM,
	R16_SINT,
	R8_UNORM,
	R8_UINT,
	R8_SNORM,
	R8_SINT,
	BC1_UNORM,
	BC1_UNORM_SRGB,
	BC2_UNORM,
	BC2_UNORM_SRGB,
	BC3_UNORM,
	BC3_UNORM_SRGB,
	BC4_UNORM,
	BC4_SNORM,
	BC5_UNORM,
	BC5_SNORM,
	BC6H_UF16,
	BC6H_SF16,
	BC7_UNORM,
	BC7_UNORM_SRGB,
	R9G9B9E5_SHAREDEXP,
	END
};
constexpr DXGI_FORMAT GetDxgiFormat(ResourceFormat format) {
	switch (format) {
	case ResourceFormat::R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ResourceFormat::R32G32B32A32_UINT:
		return DXGI_FORMAT_R32G32B32A32_UINT;
	case ResourceFormat::R32G32B32A32_Int:
		return DXGI_FORMAT_R32G32B32A32_SINT;
	case ResourceFormat::R32G32B32_Float:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case ResourceFormat::R32G32B32_UINT:
		return DXGI_FORMAT_R32G32B32_UINT;
	case ResourceFormat::R32G32B32_SINT:
		return DXGI_FORMAT_R32G32B32_SINT;
	case ResourceFormat::R16G16B16A16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case ResourceFormat::R16G16B16A16_UNORM:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case ResourceFormat::R16G16B16A16_UINT:
		return DXGI_FORMAT_R16G16B16A16_UINT;
	case ResourceFormat::R16G16B16A16_SNORM:
		return DXGI_FORMAT_R16G16B16A16_SNORM;
	case ResourceFormat::R16G16B16A16_SINT:
		return DXGI_FORMAT_R16G16B16A16_SINT;
	case ResourceFormat::R32G32_FLOAT:
		return DXGI_FORMAT_R32G32_FLOAT;
	case ResourceFormat::R32G32_UINT:
		return DXGI_FORMAT_R32G32_UINT;
	case ResourceFormat::R32G32_SINT:
		return DXGI_FORMAT_R32G32_SINT;
	case ResourceFormat::R32G8X24_TYPELESS:
		return DXGI_FORMAT_R32G8X24_TYPELESS;
	case ResourceFormat::D32_FLOAT_S8X24_UINT:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case ResourceFormat::R10G10B10A2_UNORM:
		return DXGI_FORMAT_R10G10B10A2_UNORM;
	case ResourceFormat::R10G10B10A2_UINT:
		return DXGI_FORMAT_R10G10B10A2_UINT;
	case ResourceFormat::R11G11B10_FLOAT:
		return DXGI_FORMAT_R11G11B10_FLOAT;
	case ResourceFormat::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case ResourceFormat::R8G8B8A8_UNORM_SRGB:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case ResourceFormat::R8G8B8A8_UINT:
		return DXGI_FORMAT_R8G8B8A8_UINT;
	case ResourceFormat::R8G8B8A8_SNORM:
		return DXGI_FORMAT_R8G8B8A8_SNORM;
	case ResourceFormat::R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_SINT;
	case ResourceFormat::R16G16_FLOAT:
		return DXGI_FORMAT_R16G16_FLOAT;
	case ResourceFormat::R16G16_UNORM:
		return DXGI_FORMAT_R16G16_UNORM;
	case ResourceFormat::R16G16_UINT:
		return DXGI_FORMAT_R16G16_UINT;
	case ResourceFormat::R16G16_SNORM:
		return DXGI_FORMAT_R16G16_SNORM;
	case ResourceFormat::R16G16_SINT:
		return DXGI_FORMAT_R16G16_SINT;
	case ResourceFormat::R32_TYPELESS:
		return DXGI_FORMAT_R32_TYPELESS;
	case ResourceFormat::D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;
	case ResourceFormat::R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;
	case ResourceFormat::R32_UINT:
		return DXGI_FORMAT_R32_UINT;
	case ResourceFormat::R32_SINT:
		return DXGI_FORMAT_R32_SINT;
	case ResourceFormat::R8G8_UNORM:
		return DXGI_FORMAT_R8G8_UNORM;
	case ResourceFormat::R8G8_UINT:
		return DXGI_FORMAT_R8G8_UINT;
	case ResourceFormat::R8G8_SNORM:
		return DXGI_FORMAT_R8G8_SNORM;
	case ResourceFormat::R8G8_SINT:
		return DXGI_FORMAT_R8G8_SINT;
	case ResourceFormat::R16_TYPELESS:
		return DXGI_FORMAT_R16_TYPELESS;
	case ResourceFormat::R16_FLOAT:
		return DXGI_FORMAT_R16_FLOAT;
	case ResourceFormat::D16_UNORM:
		return DXGI_FORMAT_D16_UNORM;
	case ResourceFormat::R16_UNORM:
		return DXGI_FORMAT_R16_UNORM;
	case ResourceFormat::R16_UINT:
		return DXGI_FORMAT_R16_UINT;
	case ResourceFormat::R16_SNORM:
		return DXGI_FORMAT_R16_SNORM;
	case ResourceFormat::R16_SINT:
		return DXGI_FORMAT_R16_SINT;
	case ResourceFormat::R8_UNORM:
		return DXGI_FORMAT_R8_UNORM;
	case ResourceFormat::R8_UINT:
		return DXGI_FORMAT_R8_UINT;
	case ResourceFormat::R8_SNORM:
		return DXGI_FORMAT_R8_SNORM;
	case ResourceFormat::R8_SINT:
		return DXGI_FORMAT_R8_SINT;
	case ResourceFormat::BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM;
	case ResourceFormat::BC1_UNORM_SRGB:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case ResourceFormat::BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM;
	case ResourceFormat::BC2_UNORM_SRGB:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case ResourceFormat::BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM;
	case ResourceFormat::BC3_UNORM_SRGB:
		return DXGI_FORMAT_BC3_UNORM_SRGB;
	case ResourceFormat::BC4_UNORM:
		return DXGI_FORMAT_BC4_UNORM;
	case ResourceFormat::BC4_SNORM:
		return DXGI_FORMAT_BC4_SNORM;
	case ResourceFormat::BC5_UNORM:
		return DXGI_FORMAT_BC5_UNORM;
	case ResourceFormat::BC5_SNORM:
		return DXGI_FORMAT_BC5_SNORM;
	case ResourceFormat::B8G8R8A8_UNORM:
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case ResourceFormat::B8G8R8A8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	case ResourceFormat::BC6H_UF16:
		return DXGI_FORMAT_BC6H_UF16;
	case ResourceFormat::BC6H_SF16:
		return DXGI_FORMAT_BC6H_SF16;
	case ResourceFormat::BC7_UNORM:
		return DXGI_FORMAT_BC7_UNORM;
	case ResourceFormat::BC7_UNORM_SRGB:
		return DXGI_FORMAT_BC7_UNORM_SRGB;
	case ResourceFormat::R9G9B9E5_SHAREDEXP:
		return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	}
	return DXGI_FORMAT_UNKNOWN;
}
constexpr ResourceFormat GetFormat(DXGI_FORMAT dxgi_format) {
	switch (dxgi_format) {
	case DXGI_FORMAT_UNKNOWN:
		return ResourceFormat::Unknown;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return ResourceFormat::R32G32B32A32_FLOAT;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		return ResourceFormat::R32G32B32A32_UINT;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return ResourceFormat::R32G32B32A32_Int;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return ResourceFormat::R32G32B32_Float;
	case DXGI_FORMAT_R32G32B32_UINT:
		return ResourceFormat::R32G32B32_UINT;
	case DXGI_FORMAT_R32G32B32_SINT:
		return ResourceFormat::R32G32B32_SINT;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return ResourceFormat::R16G16B16A16_FLOAT;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		return ResourceFormat::R16G16B16A16_UNORM;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		return ResourceFormat::R16G16B16A16_UINT;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		return ResourceFormat::R16G16B16A16_SNORM;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		return ResourceFormat::R16G16B16A16_SINT;
	case DXGI_FORMAT_R32G32_FLOAT:
		return ResourceFormat::R32G32_FLOAT;
	case DXGI_FORMAT_R32G32_UINT:
		return ResourceFormat::R32G32_UINT;
	case DXGI_FORMAT_R32G32_SINT:
		return ResourceFormat::R32G32_SINT;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return ResourceFormat::R32G8X24_TYPELESS;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		return ResourceFormat::D32_FLOAT_S8X24_UINT;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		return ResourceFormat::R10G10B10A2_UNORM;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		return ResourceFormat::R10G10B10A2_UINT;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		return ResourceFormat::R11G11B10_FLOAT;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return ResourceFormat::R8G8B8A8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return ResourceFormat::R8G8B8A8_UNORM_SRGB;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		return ResourceFormat::R8G8B8A8_UINT;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		return ResourceFormat::R8G8B8A8_SNORM;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return ResourceFormat::R8G8B8A8_SINT;
	case DXGI_FORMAT_R16G16_FLOAT:
		return ResourceFormat::R16G16_FLOAT;
	case DXGI_FORMAT_R16G16_UNORM:
		return ResourceFormat::R16G16_UNORM;
	case DXGI_FORMAT_R16G16_UINT:
		return ResourceFormat::R16G16_UINT;
	case DXGI_FORMAT_R16G16_SNORM:
		return ResourceFormat::R16G16_SNORM;
	case DXGI_FORMAT_R16G16_SINT:
		return ResourceFormat::R16G16_SINT;
	case DXGI_FORMAT_R32_TYPELESS:
		return ResourceFormat::R32_TYPELESS;
	case DXGI_FORMAT_D32_FLOAT:
		return ResourceFormat::D32_FLOAT;
	case DXGI_FORMAT_R32_FLOAT:
		return ResourceFormat::R32_FLOAT;
	case DXGI_FORMAT_R32_UINT:
		return ResourceFormat::R32_UINT;
	case DXGI_FORMAT_R32_SINT:
		return ResourceFormat::R32_SINT;
	case DXGI_FORMAT_R24G8_TYPELESS:
		return ResourceFormat::R24G8_TYPELESS;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		return ResourceFormat::D24_UNORM_S8_UINT;
	case DXGI_FORMAT_R8G8_UNORM:
		return ResourceFormat::R8G8_UNORM;
	case DXGI_FORMAT_R8G8_UINT:
		return ResourceFormat::R8G8_UINT;
	case DXGI_FORMAT_R8G8_SNORM:
		return ResourceFormat::R8G8_SNORM;
	case DXGI_FORMAT_R8G8_SINT:
		return ResourceFormat::R8G8_SINT;
	case DXGI_FORMAT_R16_TYPELESS:
		return ResourceFormat::R16_TYPELESS;
	case DXGI_FORMAT_R16_FLOAT:
		return ResourceFormat::R16_FLOAT;
	case DXGI_FORMAT_D16_UNORM:
		return ResourceFormat::D16_UNORM;
	case DXGI_FORMAT_R16_UNORM:
		return ResourceFormat::R16_UNORM;
	case DXGI_FORMAT_R16_UINT:
		return ResourceFormat::R16_UINT;
	case DXGI_FORMAT_R16_SNORM:
		return ResourceFormat::R16_SNORM;
	case DXGI_FORMAT_R16_SINT:
		return ResourceFormat::R16_SINT;
	case DXGI_FORMAT_R8_UNORM:
		return ResourceFormat::R8_UNORM;
	case DXGI_FORMAT_R8_UINT:
		return ResourceFormat::R8_UINT;
	case DXGI_FORMAT_R8_SNORM:
		return ResourceFormat::R8_SNORM;
	case DXGI_FORMAT_R8_SINT:
		return ResourceFormat::R8_SINT;
	case DXGI_FORMAT_BC1_UNORM:
		return ResourceFormat::BC1_UNORM;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		return ResourceFormat::BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_UNORM:
		return ResourceFormat::BC2_UNORM;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		return ResourceFormat::BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_UNORM:
		return ResourceFormat::BC3_UNORM;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		return ResourceFormat::BC3_UNORM_SRGB;
	case DXGI_FORMAT_BC4_UNORM:
		return ResourceFormat::BC4_UNORM;
	case DXGI_FORMAT_BC4_SNORM:
		return ResourceFormat::BC4_SNORM;
	case DXGI_FORMAT_BC5_UNORM:
		return ResourceFormat::BC5_UNORM;
	case DXGI_FORMAT_BC5_SNORM:
		return ResourceFormat::BC5_SNORM;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return ResourceFormat::B8G8R8A8_UNORM;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return ResourceFormat::B8G8R8A8_UNORM_SRGB;
	case DXGI_FORMAT_BC6H_UF16:
		return ResourceFormat::BC6H_UF16;
	case DXGI_FORMAT_BC6H_SF16:
		return ResourceFormat::BC6H_SF16;
	case DXGI_FORMAT_BC7_UNORM:
		return ResourceFormat::BC7_UNORM;
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return ResourceFormat::BC7_UNORM_SRGB;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		return ResourceFormat::R9G9B9E5_SHAREDEXP;
	}
	return ResourceFormat::Unknown;
}
inline constexpr uint32 GetResourceFormatStride(ResourceFormat format) {
	switch (format) {
	case ResourceFormat::Unknown:
		return 0;
	case ResourceFormat::R32G32B32A32_FLOAT:
	case ResourceFormat::R32G32B32A32_UINT:
	case ResourceFormat::R32G32B32A32_Int:
		return 16; // 4 components * 4 bytes
	case ResourceFormat::R32G32B32_Float:
	case ResourceFormat::R32G32B32_UINT:
	case ResourceFormat::R32G32B32_SINT:
		return 12; // 3 components * 4 bytes
	case ResourceFormat::R16G16B16A16_FLOAT:
	case ResourceFormat::R16G16B16A16_UNORM:
	case ResourceFormat::R16G16B16A16_UINT:
	case ResourceFormat::R16G16B16A16_SNORM:
	case ResourceFormat::R16G16B16A16_SINT:
		return 8; // 4 components * 2 bytes
	case ResourceFormat::R32G32_FLOAT:
	case ResourceFormat::R32G32_UINT:
	case ResourceFormat::R32G32_SINT:
		return 8; // 2 components * 4 bytes
	case ResourceFormat::R32G8X24_TYPELESS:
	case ResourceFormat::D32_FLOAT_S8X24_UINT:
		return 8; // 32-bit float + 8-bit stencil + 24-bit padding
	case ResourceFormat::R10G10B10A2_UNORM:
	case ResourceFormat::R10G10B10A2_UINT:
	case ResourceFormat::R11G11B10_FLOAT:
		return 4; // Packed 32-bit formats
	case ResourceFormat::R8G8B8A8_UNORM:
	case ResourceFormat::R8G8B8A8_UNORM_SRGB:
	case ResourceFormat::R8G8B8A8_UINT:
	case ResourceFormat::R8G8B8A8_SNORM:
	case ResourceFormat::R8G8B8A8_SINT:
	case ResourceFormat::B8G8R8A8_UNORM:
	case ResourceFormat::B8G8R8A8_UNORM_SRGB:
		return 4; // 4 components * 1 byte
	case ResourceFormat::R16G16_FLOAT:
	case ResourceFormat::R16G16_UNORM:
	case ResourceFormat::R16G16_UINT:
	case ResourceFormat::R16G16_SNORM:
	case ResourceFormat::R16G16_SINT:
		return 4; // 2 components * 2 bytes
	case ResourceFormat::R32_TYPELESS:
	case ResourceFormat::D32_FLOAT:
	case ResourceFormat::R32_FLOAT:
	case ResourceFormat::R32_UINT:
	case ResourceFormat::R32_SINT:
		return 4; // 1 component * 4 bytes
	case ResourceFormat::R24G8_TYPELESS:
	case ResourceFormat::D24_UNORM_S8_UINT:
		return 4; // 24-bit depth + 8-bit stencil
	case ResourceFormat::R8G8_UNORM:
	case ResourceFormat::R8G8_UINT:
	case ResourceFormat::R8G8_SNORM:
	case ResourceFormat::R8G8_SINT:
		return 2; // 2 components * 1 byte
	case ResourceFormat::R16_TYPELESS:
	case ResourceFormat::R16_FLOAT:
	case ResourceFormat::D16_UNORM:
	case ResourceFormat::R16_UNORM:
	case ResourceFormat::R16_UINT:
	case ResourceFormat::R16_SNORM:
	case ResourceFormat::R16_SINT:
		return 2; // 1 component * 2 bytes
	case ResourceFormat::R8_UNORM:
	case ResourceFormat::R8_UINT:
	case ResourceFormat::R8_SNORM:
	case ResourceFormat::R8_SINT:
		return 1; // 1 component * 1 byte
		// Block compression formats - bytes per block
	case ResourceFormat::BC1_UNORM:
	case ResourceFormat::BC1_UNORM_SRGB:
		return 8; // 8 bytes per 4x4 block (0.5 bytes per pixel)
	case ResourceFormat::BC2_UNORM:
	case ResourceFormat::BC2_UNORM_SRGB:
	case ResourceFormat::BC3_UNORM:
	case ResourceFormat::BC3_UNORM_SRGB:
		return 16; // 16 bytes per 4x4 block (1 byte per pixel)
	case ResourceFormat::BC4_UNORM:
	case ResourceFormat::BC4_SNORM:
		return 8; // 8 bytes per 4x4 block (0.5 bytes per pixel)
	case ResourceFormat::BC5_UNORM:
	case ResourceFormat::BC5_SNORM:
		return 16; // 16 bytes per 4x4 block (1 byte per pixel)
	case ResourceFormat::BC6H_UF16:
	case ResourceFormat::BC6H_SF16:
	case ResourceFormat::BC7_UNORM:
	case ResourceFormat::BC7_UNORM_SRGB:
		return 16; // 16 bytes per 4x4 block (1 byte per pixel)
	case ResourceFormat::R9G9B9E5_SHAREDEXP:
		return 4; // Packed 32-bit format
	case ResourceFormat::END:
		return 0;
	default:
		assert(false);
		return 0;
	}
}
inline constexpr uint32 GetResourceFormatBlockSize(ResourceFormat format) {
	switch (format) {
	case ResourceFormat::BC1_UNORM:
	case ResourceFormat::BC1_UNORM_SRGB:
	case ResourceFormat::BC4_UNORM:
	case ResourceFormat::BC4_SNORM:
	case ResourceFormat::BC2_UNORM:
	case ResourceFormat::BC2_UNORM_SRGB:
	case ResourceFormat::BC3_UNORM:
	case ResourceFormat::BC3_UNORM_SRGB:
	case ResourceFormat::BC5_UNORM:
	case ResourceFormat::BC5_SNORM:
	case ResourceFormat::BC6H_UF16:
	case ResourceFormat::BC6H_SF16:
	case ResourceFormat::BC7_UNORM:
	case ResourceFormat::BC7_UNORM_SRGB:
		return 4; // 4 x 4 block size
	default:
		return 1; // Non-block compressed formats
	}
}
inline constexpr uint32 GetResourceFormatRowPitch(ResourceFormat format, uint32 width, uint32 mip_level) {
	uint32 bytes_per_pixel = GetResourceFormatStride(format);
	uint32 row_pitch = (width * bytes_per_pixel); // + 255) & ~255; // Align to 256 bytes
	if (mip_level > 0) {
		row_pitch >>= mip_level;
	}
	return std::max(row_pitch + (GetResourceFormatBlockSize(format) - 1) / GetResourceFormatBlockSize(format), 1u); // Round up, and ensure at least 1 byte
}
inline constexpr uint32 GetResourceFormatSlicePitch(ResourceFormat format, uint32 width, uint32 height, uint32 mip_level) {
	uint32 bytes_per_pixel = GetResourceFormatStride(format);
	uint32 row_pitch = GetResourceFormatRowPitch(format, width, mip_level);
	uint32 slice_pitch = (row_pitch * height); // + 255) & ~255; // Align to 256 bytes
	if (mip_level > 0) {
		slice_pitch >>= mip_level;
	}
	return std::max(slice_pitch + (GetResourceFormatBlockSize(format) - 1) / GetResourceFormatBlockSize(format), 1u); // Round up, and ensure at least 1 byte
}
inline constexpr bool IsDepthFormat(ResourceFormat format) {
	switch (format) {
	case ResourceFormat::D16_UNORM:
	case ResourceFormat::D32_FLOAT:
	case ResourceFormat::D24_UNORM_S8_UINT:
	case ResourceFormat::D32_FLOAT_S8X24_UINT:
		return true;
	}
	return false;
}
#pragma endregion
enum class ResourceStateEnum : uint64_t { // For barriers - Enhanced barriers only
	None = 0,
	Common = 1ULL << 0,
	Present = 1ULL << 1,
	Render_Target = 1ULL << 2,
	Depth_Stencil = 1ULL << 3,
	Depth_Stencil_Read_Only = 1ULL << 4,
	Vertex_Shader_Resource = 1ULL << 5,
	Pixel_Shader_Resource = 1ULL << 6,
	Compute_Shader_Resource = 1ULL << 7,
	Vertex_Unordered_Access = 1ULL << 8,
	Pixel_Unordered_Access = 1ULL << 9,
	Compute_Unordered_Access = 1ULL << 10,
	Clear_Unordered_Access = 1ULL << 11,
	Copy_Destination = 1ULL << 12,
	Copy_Source = 1ULL << 13,
	Shading_Rate = 1ULL << 14,
	Index_Buffer = 1ULL << 15,
	Indirect_Arguments = 1ULL << 16,
	Acceleration_Structure_Read = 1ULL << 17,
	Acceleration_Structure_Write = 1ULL << 18,
	Discard = 1ULL << 19,
};
using ResourceState = EnumFlags<ResourceStateEnum>;
namespace ResourceStates { // Define flags
	inline constexpr ResourceState None = ResourceStateEnum::None;
	inline constexpr ResourceState Common = ResourceStateEnum::Common;
	inline constexpr ResourceState Present = ResourceStateEnum::Present;
	inline constexpr ResourceState Render_Target = ResourceStateEnum::Render_Target;
	inline constexpr ResourceState Depth_Stencil = ResourceStateEnum::Depth_Stencil;
	inline constexpr ResourceState Depth_Stencil_Read_Only = ResourceStateEnum::Depth_Stencil_Read_Only;
	inline constexpr ResourceState Vertex_Shader_Resource = ResourceStateEnum::Vertex_Shader_Resource;
	inline constexpr ResourceState Pixel_Shader_Resource = ResourceStateEnum::Pixel_Shader_Resource;
	inline constexpr ResourceState Compute_Shader_Resource = ResourceStateEnum::Compute_Shader_Resource;
	inline constexpr ResourceState Vertex_Unordered_Access = ResourceStateEnum::Vertex_Unordered_Access;
	inline constexpr ResourceState Pixel_Unordered_Access = ResourceStateEnum::Pixel_Unordered_Access;
	inline constexpr ResourceState Compute_Unordered_Access = ResourceStateEnum::Compute_Unordered_Access;
	inline constexpr ResourceState Clear_Unordered_Access = ResourceStateEnum::Clear_Unordered_Access;
	inline constexpr ResourceState Copy_Destination = ResourceStateEnum::Copy_Destination;
	inline constexpr ResourceState Copy_Source = ResourceStateEnum::Copy_Source;
	inline constexpr ResourceState Shading_Rate = ResourceStateEnum::Shading_Rate;
	inline constexpr ResourceState Index_Buffer = ResourceStateEnum::Index_Buffer;
	inline constexpr ResourceState Indirect_Arguments = ResourceStateEnum::Indirect_Arguments;
	inline constexpr ResourceState Acceleration_Structure_Read = ResourceStateEnum::Acceleration_Structure_Read;
	inline constexpr ResourceState Acceleration_Structure_Write = ResourceStateEnum::Acceleration_Structure_Write;
	inline constexpr ResourceState Discard = ResourceStateEnum::Discard;
	// ... other individual states

	// Define composite states
	inline constexpr ResourceState Vertex_All = ResourceStateEnum::Vertex_Shader_Resource | ResourceStateEnum::Vertex_Unordered_Access;
	inline constexpr ResourceState Pixel_All = ResourceStateEnum::Pixel_Shader_Resource | ResourceStateEnum::Pixel_Unordered_Access;
	inline constexpr ResourceState Compute_All = ResourceStateEnum::Compute_Shader_Resource | ResourceStateEnum::Compute_Unordered_Access;
	inline constexpr ResourceState Copy_All = ResourceStateEnum::Copy_Destination | ResourceStateEnum::Copy_Source;
	inline constexpr ResourceState All_Shader_Resource = ResourceStateEnum::Vertex_Shader_Resource | ResourceStateEnum::Pixel_Shader_Resource | ResourceStateEnum::Compute_Shader_Resource;
	inline constexpr ResourceState All_Unordered_Access = ResourceStateEnum::Vertex_Unordered_Access | ResourceStateEnum::Pixel_Unordered_Access | ResourceStateEnum::Compute_Unordered_Access;
	inline constexpr ResourceState All_Depth_Stencil = ResourceStateEnum::Depth_Stencil | ResourceStateEnum::Depth_Stencil_Read_Only;
	inline constexpr ResourceState All_Acceleration_Structure = ResourceStateEnum::Acceleration_Structure_Read | ResourceStateEnum::Acceleration_Structure_Write;
	inline constexpr ResourceState All_Read = ResourceStateEnum::Copy_Source | All_Shader_Resource;
	inline constexpr ResourceState All_Write = ResourceStateEnum::Copy_Destination | All_Unordered_Access;
	inline constexpr ResourceState All_Shading_Resources = All_Shader_Resource | All_Unordered_Access | ResourceStateEnum::Shading_Rate | All_Acceleration_Structure;
}
inline D3D12_BARRIER_SYNC GetD3D12BarrierSync(ResourceState flags) {
	using enum ResourceStateEnum;

	D3D12_BARRIER_SYNC sync = D3D12_BARRIER_SYNC_NONE;
	if (flags.HasFlag(Discard) == false && flags.HasFlag(Clear_Unordered_Access))
		sync |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;

	if (flags.HasFlag(Common) || flags.HasFlag(Present)) sync |= D3D12_BARRIER_SYNC_ALL;
	if (flags.HasFlag(Render_Target))					 sync |= D3D12_BARRIER_SYNC_RENDER_TARGET;
	if (flags.HasAnyFlag(ResourceStates::All_Depth_Stencil))			 sync |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
	if (flags.HasAnyFlag(ResourceStates::Vertex_All))					 sync |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
	if (flags.HasAnyFlag(ResourceStates::Pixel_All))					 sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
	if (flags.HasAnyFlag(ResourceStates::Compute_All))					 sync |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
	if (flags.HasAnyFlag(ResourceStates::Copy_All))						 sync |= D3D12_BARRIER_SYNC_COPY;
	if (flags.HasFlag(Shading_Rate))					 sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
	if (flags.HasFlag(Index_Buffer))				     sync |= D3D12_BARRIER_SYNC_INDEX_INPUT;
	if (flags.HasFlag(Indirect_Arguments))				 sync |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
	if (flags.HasAnyFlag(ResourceStates::All_Acceleration_Structure))	 sync |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;
	return sync;
}
inline D3D12_BARRIER_LAYOUT GetD3D12BarrierLayout(ResourceState flags) {
    using enum ResourceStateEnum;
    if (flags.HasFlag(ResourceStateEnum::Copy_Source) && flags.HasAnyFlag(ResourceStates::All_Shader_Resource) && !flags.HasFlag(ResourceStateEnum::Depth_Stencil_Read_Only)) 
        return D3D12_BARRIER_LAYOUT_GENERIC_READ;
    if (flags.HasFlag(ResourceStateEnum::Copy_Destination) && flags.HasAnyFlag(ResourceStates::All_Unordered_Access)) 
        return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COMMON;
    if (flags.HasFlag(ResourceStateEnum::Depth_Stencil_Read_Only) && (flags.HasAnyFlag(ResourceStates::All_Shader_Resource) || flags.HasFlag(ResourceStateEnum::Copy_Source)))
        return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_GENERIC_READ;
    if (flags.HasFlag(ResourceStateEnum::Discard))          return D3D12_BARRIER_LAYOUT_UNDEFINED;
    if (flags.HasFlag(ResourceStateEnum::Present))          return D3D12_BARRIER_LAYOUT_PRESENT;
    if (flags.HasFlag(ResourceStateEnum::Render_Target))    return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
    if (flags.HasFlag(ResourceStateEnum::Depth_Stencil))    return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    if (flags.HasFlag(ResourceStateEnum::Depth_Stencil_Read_Only))  return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
    if (flags.HasAnyFlag(ResourceStates::All_Shader_Resource))      return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
    if (flags.HasAnyFlag(ResourceStates::All_Unordered_Access) || flags.HasFlag(ResourceStateEnum::Clear_Unordered_Access))
        return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
    if (flags.HasFlag(ResourceStateEnum::Copy_Destination))  return D3D12_BARRIER_LAYOUT_COPY_DEST;
    if (flags.HasFlag(ResourceStateEnum::Copy_Source))       return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
    if (flags.HasFlag(ResourceStateEnum::Shading_Rate))      return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
    
    assert(false);
    return D3D12_BARRIER_LAYOUT_UNDEFINED;
}
inline D3D12_BARRIER_ACCESS GetD3D12BarrierAccess(ResourceState flags) {
    using enum ResourceStateEnum;
    if (flags.HasFlag(ResourceStateEnum::Discard)) return D3D12_BARRIER_ACCESS_NO_ACCESS;

    D3D12_BARRIER_ACCESS access = D3D12_BARRIER_ACCESS_COMMON;
    if (flags.HasFlag(ResourceStateEnum::Render_Target))                access |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
    if (flags.HasFlag(ResourceStateEnum::Depth_Stencil))                access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
    if (flags.HasFlag(ResourceStateEnum::Depth_Stencil_Read_Only))      access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
    if (flags.HasAnyFlag(ResourceStates::All_Shader_Resource))          access |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
    if (flags.HasAnyFlag(ResourceStates::All_Unordered_Access))         access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    if (flags.HasFlag(ResourceStateEnum::Clear_Unordered_Access))       access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    if (flags.HasFlag(ResourceStateEnum::Copy_Destination))             access |= D3D12_BARRIER_ACCESS_COPY_DEST;
    if (flags.HasFlag(ResourceStateEnum::Copy_Source))                  access |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
    if (flags.HasFlag(ResourceStateEnum::Shading_Rate))                 access |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
    if (flags.HasFlag(ResourceStateEnum::Index_Buffer))                 access |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
    if (flags.HasFlag(ResourceStateEnum::Indirect_Arguments))           access |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
    if (flags.HasFlag(ResourceStateEnum::Acceleration_Structure_Read))  access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
    if (flags.HasFlag(ResourceStateEnum::Acceleration_Structure_Write)) access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
    return access;
}
enum class ResourceHeapType : uint8 { // usage
	Default, // GPU only
	Upload, // CPU write, GPU read
	Readback, // CPU read, GPU write
};
enum class BindFlagsEnum : uint32{
	None = 0,
	Shader_Resource = 1 << 0,
	Render_Target = 1 << 1,
	Depth_Stencil = 1 << 2,
	Unordered_Access = 1 << 3
};
using BindFlag = EnumFlags<BindFlagsEnum>;
namespace BindFlags { // Define flags
	inline constexpr BindFlag None = BindFlagsEnum::None;
	inline constexpr BindFlag Shader_Resource = BindFlagsEnum::Shader_Resource;
	inline constexpr BindFlag Render_Target = BindFlagsEnum::Render_Target;
	inline constexpr BindFlag Depth_Stencil = BindFlagsEnum::Depth_Stencil;
	inline constexpr BindFlag Unordered_Access = BindFlagsEnum::Unordered_Access;
}

enum class SubresourceType : uint8 {
	SRV, // Shader Resource View
	UAV, // Unordered Access View
	RTV, // Render Target View
	DSV, // Depth Stencil View
	Invalid
};

enum class ResourceFlagsEnum : uint32 {
	None = 0,
	Allow_Render_Target = 0x1,
	Allow_Depth_Stencil = 0x2,
	Allow_Unordered_Access = 0x4,
	Deny_Shader_Resource = 0x8,
	Allow_Cross_Adapter = 0x10,
	Allow_Simultaneous_Access_Adapter = 0x20,
	Decode_Ref_Only = 0x40,
	Encoode_Ref_Only = 0x80,
	Ray_Tracing_Acceleration_Structure = 0x100,
	Constant_Data_Buffer = 0x200
};
using ResourceFlag = EnumFlags<ResourceFlagsEnum>;
enum class GpuBufferInfoFlagsEnum : uint32 {
	None = 0,
	IndirectArgs = BIT(0),
	BufferRaw = BIT(1),
	BufferStructured = BIT(2),
	ConstantBuffer = BIT(3),
	VertexBuffer = BIT(4),
	IndexBuffer = BIT(5),
	RayTracingAccelerationStructure = BIT(6)
	//,Shared = BIT(7)
};
using GpuBufferInfoFlags = EnumFlags<GpuBufferInfoFlagsEnum>;
struct ClearValue {
	struct ClearColor {
		ClearColor(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) : color{ r, g, b, a } {}
		ClearColor(float4 _color) : color{ _color } {}
		ClearColor(ClearColor const& other) : color{ other.color.r, other.color.g, other.color.b, other.color.a } {}
		bool operator==(ClearColor const& other) const {
			return color.r == other.color.r && color.g == other.color.g && color.b == other.color.b && color.a == other.color.a;
		}
		float4 color;
	};
	struct ClearDepthStencil {
		ClearDepthStencil(float depth = 0.0f, Uint8 stencil = 1)
			: depth(depth), stencil(stencil)
		{}
		float depth;
		uint8 stencil;
		bool operator==(ClearDepthStencil const& other) const {
			return depth == other.depth && stencil == other.stencil;
		}
	};

	union {
		ClearColor color;
		ClearDepthStencil depth_stencil;
	};
	enum class ClearValueType : uint8 {
		Color,
		Depth_Stencil
	} value_type = ClearValueType::Color;

	ClearValue(ClearColor const& _color) : value_type(ClearValueType::Color), color(_color) {};
	ClearValue(ClearDepthStencil const& _depth_stencil) : value_type(ClearValueType::Depth_Stencil), depth_stencil(_depth_stencil) {}
	bool operator==(ClearValue const& other) const {
		if (value_type != other.value_type) return false;
		switch (value_type) {
		case ClearValueType::Color:
			return color == other.color;
		case ClearValueType::Depth_Stencil:
			return depth_stencil == other.depth_stencil;
		default:
			return false;
		}
	}
};
}