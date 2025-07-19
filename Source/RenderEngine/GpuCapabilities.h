#pragma once
#include "GpuCapabilitiesEnums.h"
#include "MathDefines.h"
namespace WaxGourd {
class Device;
class GpuCapabilities {
private:
	RayTracingSupport ray_tracing_support = RayTracingSupport::NotSupported;
	VRSSupport VRS_support = VRSSupport::NotSupported;
	MeshShaderSupport mesh_shader_support = MeshShaderSupport::NotSupported;
	WorkGraphSupport work_graph_support = WorkGraphSupport::NotSupported;
	ShaderModel shader_model = SM_Unknown;
	bool enhanced_barriers_supported = false;

	bool additional_shading_rates_supported = false;
	uint32 shading_rate_image_tile_size = 0;
public:
	void Initialize(Device* device); // Initializes the capabilities based on the device properties

	bool CheckRayTracingSupport(RayTracingSupport rts) const;
	bool CheckVRSSupport(VRSSupport VRSs) const;
	bool CheckMeshShaderSupport(MeshShaderSupport mss) const;
	bool CheckWorkGraphSupport(WorkGraphSupport wgs) const;
	bool SupportsRayTracing() const;
	bool SupportsMeshShaders() const;
	bool SupportsVRS() const;
	bool SupportsWorkGraphs() const;
	bool SupportsShaderModel(ShaderModel sm) const;
	bool SupportsEnhancedBarriers() const;
	bool SupportsAdditionalShadingRates() const;
	uint32 GetShadingRateImageTileSize() const;
};
}
