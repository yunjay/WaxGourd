#include "Camera.h"
using WaxGourd::Camera;
Camera::Camera(Camera::CameraParameters const& _params)
:m_cameraParams(_params) {
	m_cameraParams.lookDirection = normalize(m_cameraParams.lookDirection);
	m_cameraParams.upDirection = normalize(m_cameraParams.upDirection);
	m_cameraParams.rightDirection = normalize(m_cameraParams.rightDirection);

	m_orientationQuat = glm::quatLookAtLH(m_cameraParams.lookDirection, m_cameraParams.upDirection);
	
	m_aspectRatio = m_cameraParams.viewport.z / m_cameraParams.viewport.w;

	m_bChanged = true;
}
void Camera::Translate(float2 _vec) {
	m_cameraParams.position += _vec.x * m_cameraParams.lookDirection + _vec.y * m_cameraParams.upDirection;
	m_bChanged = true;
}
void Camera::Rotate(float _yaw, float _pitch, float _roll) {
	quat yawQuat = glm::angleAxis(_yaw, m_cameraParams.upDirection);
	quat pitchQuat = glm::angleAxis(_pitch, m_cameraParams.rightDirection);
	quat rollQuat = glm::angleAxis(_roll, m_cameraParams.lookDirection);
	m_orientationQuat = yawQuat * pitchQuat * rollQuat * m_orientationQuat;
	m_bChanged = true;
}
float4x4 Camera::GetViewMatrix() {
	if (m_bChanged) {
		// Rotate then translate for camera model matrix
		m_inverseViewMatrix =  glm::translate(glm::mat4_cast(m_orientationQuat),m_cameraParams.position);
		// Inverse for view matrix
		m_viewMatrix = glm::inverse(m_viewMatrix);
		m_bChanged = false;
	}
	return m_viewMatrix;
}
float4x4 Camera::GetProjectionMatrix(bool _bOrthogonal /*= false*/) {
	// TODO : inverse - z
	if (m_bChanged) {
		if (_bOrthogonal) {
			m_projectionMatrix = glm::orthoLH(
				m_cameraParams.viewport.x, m_cameraParams.viewport.x + m_cameraParams.viewport.z,
				m_cameraParams.viewport.y - m_cameraParams.viewport.w, m_cameraParams.viewport.y,
				m_cameraParams.nearPlane, m_cameraParams.farPlane);
		}
		else
			m_projectionMatrix = glm::perspectiveLH(m_cameraParams.fov, m_aspectRatio, m_cameraParams.nearPlane, m_cameraParams.farPlane);
		
		m_inverseProjectionMatrix = glm::inverse(m_projectionMatrix);
		m_bChanged = false;
	}
	return m_projectionMatrix;
}
Camera::CameraParameters Camera::GetCameraParameters() const {
	return m_cameraParams;
}