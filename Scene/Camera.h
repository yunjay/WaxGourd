#pragma once
#include "MathDefines.h"
namespace WaxGourd {
class Camera {
public:
	Camera() = default;
	struct CameraParameters
	{
		float nearPlane;
		float farPlane;
		float fov;
		float4 viewport; // Top left x,y, width,height
		float3 position;
		float3 lookDirection = float3(0.f,0.f,1.f);
		float3 upDirection = float3(0.f,1.f,0.f);
		float3 rightDirection = float3(1.f,0.f,0.f);
	};
	Camera(CameraParameters const&);
	void Translate(float2 _vec); // direction with size
	void Rotate(float _yaw, float _pitch, float _roll); // angles in radians. 
	// yaw : rotation around up vector, pitch : rotation around right vector, roll : rotation around look vector

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix(bool _bOrthogonal = false);
	CameraParameters GetCameraParameters() const;
private:
	CameraParameters m_cameraParams;

	float m_aspectRatio;

	bool m_bChanged = true;

	quat m_orientationQuat;

	float4x4 m_viewMatrix;
	float4x4 m_inverseViewMatrix;
	float4x4 m_projectionMatrix;
	float4x4 m_inverseProjectionMatrix;
};
}