#include "MathDefines.h"

// Axis-Aligned Bounding Box
namespace WaxGourd {
class AABB {
public:
	AABB() = default; // 0,0
	// AABB with center and radius
	AABB(const float3& center, float radius) {
		float3 radius_vector(radius, radius, radius);
		m_min_point = glm::min(center - radius_vector, m_min_point);
		m_max_point = glm::max(center + radius_vector, m_max_point);
	}

	// AABB that contains two points
	AABB(const float3& p1, const float3& p2) {
		m_min_point = glm::min(p1, p2);
		m_max_point = glm::max(p1, p2);
	}

	void fit_inside(const float3& p) {
		m_min_point = glm::min(p, m_min_point);
		m_max_point = glm::max(p, m_max_point);
	}

	void transform(float4x4 transform) {
		float3 corners[8] = {
			float3(m_min_point.x, m_min_point.y, m_min_point.z),
			float3(m_min_point.x, m_min_point.y, m_max_point.z),
			float3(m_min_point.x, m_max_point.y, m_min_point.z),
			float3(m_min_point.x, m_max_point.y, m_max_point.z),
			float3(m_max_point.x, m_min_point.y, m_min_point.z),
			float3(m_max_point.x, m_min_point.y, m_max_point.z),
			float3(m_max_point.x, m_max_point.y, m_min_point.z),
			float3(m_max_point.x, m_max_point.y, m_max_point.z)
		};

		float3 new_min = float3(float_INF), new_max = float3(-float_INF);
		for (int i = 0; i < 8; ++i) {
			float3 transformed = transform * float4(corners[i], 1.0);
			new_min = std::min(new_min, transformed);
			new_max = std::max(new_max, transformed);
		}
		m_min_point = new_min;
		m_max_point = new_max;
	}
private:
	float3 m_min_point(0.f);
	float3 m_max_point(0.f);
}
}