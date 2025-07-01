#pragma once
#include "GpuCapabilitiesEnums.h"
#include "MathDefines.h"
namespace WaxGourd {
class Device;
enum class RayTracingSupport : uint8 {
	NotSupported,
	Tier1_0,
	Tier1_1
};
enum class VRSSupport : uint8 {
	NotSupported,
	Tier1,
	Tier2
};
enum class MeshShaderSupport : uint8 {
	NotSupported,
	Tier1
};

enum class WorkGraphSupport : uint8 {
	NotSupported,
	Tier1_0
};

class GpuCapabilities {
public:
	void Initialize(Device* device);

	bool SupportsRayTracing() const {
		return CheckRayTracingSupport(RayTracingSupport::Tier1_0);
	}
	bool SupportsMeshShaders() const {
		return CheckMeshShaderSupport(MeshShaderSupport::Tier1);
	}
	bool SupportsVRS() const { // Variable Rate Shading
		return CheckVRSSupport(VRSSupport::Tier1);
	}
	bool SupportsWorkGraphs() const {
		return CheckWorkGraphSupport(WorkGraphSupport::Tier1_0);
	}
	bool CheckRayTracingSupport(RayTracingSupport rts) const {
		return ray_tracing_support >= rts;
	}
	bool CheckVRSSupport(VRSSupport VRSs) const {
		return VRS_support >= VRSs;
	}
	bool CheckMeshShaderSupport(MeshShaderSupport mss) const {
		return mesh_shader_support >= mss;
	}
	bool CheckWorkGraphSupport(WorkGraphSupport wgs) const {
		return work_graph_support >= wgs;
	}
	bool SupportsShaderModel(ShaderModel sm) const {
		return shader_model >= sm;
	}
	bool SupportsEnhancedBarriers() const {
		return enhanced_barriers_supported; // falls back to legacy barriers if false
	}

	bool SupportsAdditionalShadingRates() const { return additional_shading_rates_supported; }
	uint32 GetShadingRateImageTileSize() const { return shading_rate_image_tile_size; }

private:
	RayTracingSupport ray_tracing_support = RayTracingSupport::NotSupported;
	VRSSupport VRS_support = VRSSupport::NotSupported;
	MeshShaderSupport mesh_shader_support = MeshShaderSupport::NotSupported;
	WorkGraphSupport work_graph_support = WorkGraphSupport::NotSupported;
	ShaderModel shader_model = SM_Unknown;
	bool enhanced_barriers_supported = false;

	bool additional_shading_rates_supported = false;
	uint32 shading_rate_image_tile_size = 0;
};
}
