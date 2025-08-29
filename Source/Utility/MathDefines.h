#pragma once

// GLM Defines
#define GLM_FORCE_MESSAGES
#define GLM_FORCE_LEFT_HANDED		// left handed coordinate system is more traditional for Direct3D
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Direct3D uses depth range [0.0,1.0]
#define GLM_FORCE_RADIANS			// Radians for angles

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <limits>
namespace WaxGourd {

using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;

using float3x3 = glm::mat3;
using float4x4 = glm::mat4;

using uint2 = glm::uvec2;
using uint3 = glm::uvec3;
using uint4 = glm::uvec4;

using int2 = glm::ivec2;
using int3 = glm::ivec3;
using int4 = glm::ivec4;

using bool2 = glm::bvec2;
using bool3 = glm::bvec3;
using bool4 = glm::bvec4;

using quat = glm::quat;

using uint8 = std::uint8_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int32 = std::int32_t;
using int64 = std::int64_t;

using uintPointer = std::uintptr_t;

using byteArray = std::vector<uint8>;

using TypeIndexedPodStorage = std::unordered_map<std::type_index, std::unique_ptr<uint8[]>>;

inline constexpr uint64 Align(uint64 address, uint64 alignment){
	if (alignment == 0 || alignment == 1) return address;
	uint64 r = address % alignment;
	return r ? address + (alignment - r) : address;
}
#include <type_traits>

inline constexpr auto AlignBitOp(uint32_t size, uint32_t alignment) {

	// alignment must be a power of two
	const auto alignment_mask = (alignment) - 1;
	return (size + alignment_mask) & ~alignment_mask;
}

constexpr uint64 uint64_MAX = std::numeric_limits<uint64_t>::max();
constexpr uint32 uint32_MAX = std::numeric_limits<uint32_t>::max();
constexpr float float_INF = std::numeric_limits<float>::infinity();

}