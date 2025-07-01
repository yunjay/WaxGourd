#pragma once
#include "MathDefines.h"
namespace WaxGourd {
enum class ShaderStage : uint8 {
	Vertex = 0,
	Pixel,
	Hull,
	Domain,
	Geometry,
	Compute,
	Library, // Shader Library - Ray tracing shaders also go in here
	Mesh,
	Amplification,
	SHADER_STAGE_COUNT
};
enum class ShaderModel : uint8 {
	Unknown,
	SM_6_0,
	SM_6_1,
	SM_6_2,
	SM_6_3,
	SM_6_4,
	SM_6_5,
	SM_6_6,
	SM_6_7,
	SM_6_8
};
enum class RayTracingSupport : uint8 {
	NotSupported,
	Tier1_0,
	Tier1_1
};
enum class VSRSupport : uint8 {
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

}