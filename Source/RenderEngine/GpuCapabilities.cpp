#include "d3dx12_check_feature_support.h"

#include "GpuCapabilities.h"
#include "Device.h"

#include <iostream>

namespace WaxGourd {
namespace {
constexpr RayTracingSupport ConvertRayTracingTier(D3D12_RAYTRACING_TIER tier) {
	switch (tier)
	{
	case D3D12_RAYTRACING_TIER_NOT_SUPPORTED:
		return RayTracingSupport::NotSupported;
	case D3D12_RAYTRACING_TIER_1_0:
		return RayTracingSupport::Tier1_0;
	case D3D12_RAYTRACING_TIER_1_1:
		return RayTracingSupport::Tier1_1;
	}
	return RayTracingSupport::NotSupported;
}
constexpr VRSSupport ConvertVRSTier(D3D12_VARIABLE_SHADING_RATE_TIER tier) {
	switch (tier)
	{
	case D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED:
		return VRSSupport::NotSupported;
	case D3D12_VARIABLE_SHADING_RATE_TIER_1:
		return VRSSupport::Tier1;
	case D3D12_VARIABLE_SHADING_RATE_TIER_2:
		return VRSSupport::Tier2;
	}
	return VRSSupport::NotSupported;
}
constexpr MeshShaderSupport ConvertMeshShaderTier(D3D12_MESH_SHADER_TIER tier) {
	switch (tier)
	{
	case D3D12_MESH_SHADER_TIER_NOT_SUPPORTED:
		return MeshShaderSupport::NotSupported;
	case D3D12_MESH_SHADER_TIER_1:
		return MeshShaderSupport::Tier1;
	}
	return MeshShaderSupport::NotSupported;
}

constexpr WorkGraphSupport ConvertWorkGraphTier(D3D12_WORK_GRAPHS_TIER work_graph_tier) {
	switch (work_graph_tier)
	{
	case D3D12_WORK_GRAPHS_TIER_NOT_SUPPORTED: return WorkGraphSupport::NotSupported;
	case D3D12_WORK_GRAPHS_TIER_1_0:		   return WorkGraphSupport::Tier1_0;
	}
	return WorkGraphSupport::NotSupported;
}

constexpr ShaderModel ConvertShaderModel(D3D_SHADER_MODEL shader_model) {
	switch (shader_model)
	{
	case D3D_SHADER_MODEL_6_0: return ShaderModel::SM_6_0;
	case D3D_SHADER_MODEL_6_1: return ShaderModel::SM_6_1;
	case D3D_SHADER_MODEL_6_2: return ShaderModel::SM_6_2;
	case D3D_SHADER_MODEL_6_3: return ShaderModel::SM_6_3;
	case D3D_SHADER_MODEL_6_4: return ShaderModel::SM_6_4;
	case D3D_SHADER_MODEL_6_5: return ShaderModel::SM_6_5;
	case D3D_SHADER_MODEL_6_6: return ShaderModel::SM_6_6;
	case D3D_SHADER_MODEL_6_7: return ShaderModel::SM_6_7;
	case D3D_SHADER_MODEL_6_8: return ShaderModel::SM_6_8;
	default:
		return ShaderModel::Unknown;
	}
}
}

void GpuCapabilities::Initialize(Device* device)
{
	CD3DX12FeatureSupport feature_support;
	feature_support.Init(device->GetDevice());

	ray_tracing_support = ConvertRayTracingTier(feature_support.RaytracingTier());
	VRS_support = ConvertVRSTier(feature_support.VariableShadingRateTier());
	mesh_shader_support = ConvertMeshShaderTier(feature_support.MeshShaderTier());
	work_graph_support = ConvertWorkGraphTier(feature_support.WorkGraphsTier());
	shader_model = ConvertShaderModel(feature_support.HighestShaderModel());
	enhanced_barriers_supported = feature_support.EnhancedBarriersSupported();

	shading_rate_image_tile_size = feature_support.ShadingRateImageTileSize();
	additional_shading_rates_supported = feature_support.AdditionalShadingRatesSupported();

	if ((uint8)shader_model < (uint8)ShaderModel::SM_6_6) {
		std::cout << "Shader model 6.6 or higher is required.\n";
		assert(false);
	}
}
}