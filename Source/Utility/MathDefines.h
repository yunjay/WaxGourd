#pragma once

// GLM Defines
#define GLM_FORCE_MESSAGES
#define GLM_FORCE_LEFT_HANDED		// Direct3D uses left handed coordinate system
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Direct3D uses depth range [0.0,1.0]
#define GLM_FORCE_RADIANS			// Radians for angles

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <cstdint>

typedef glm::vec2 float2;
typedef glm::vec3 float3;	
typedef glm::vec4 float4;

typedef glm::mat3 float3x3;
typedef glm::mat4 float4x4;

typedef glm::uvec2 uint2;
typedef glm::uvec3 uint3;
typedef glm::uvec4 uint4;

typedef glm::ivec2 int2;
typedef glm::ivec3 int3;
typedef glm::ivec4 int4;

typedef glm::bvec2 bool2;
typedef glm::bvec3 bool3;
typedef glm::bvec4 bool4;

typedef glm::quat quat;

typedef std::uint8_t uint8;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;
typedef std::int32_t int32;
typedef std::int64_t int64;
typedef std::uintptr_t uintPointer;

inline constexpr uint64 Align(uint64 address, uint64 alignment){
	if (alignment == 0 || alignment == 1) return address;
	uint64 r = address % alignment;
	return r ? address + (alignment - r) : address;
}