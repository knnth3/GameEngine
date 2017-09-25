#include "DX11Camera.h"



Lime::DX11Camera::DX11Camera(int width, int height)
{
	m_width = width;
	m_height = height;
	m_fov = 3.14f / 2.0f;
	m_aspectRatio = (float)m_width / m_height;
	m_farPlane = 1000.0f;
	m_nearPlane = 1.0f;
	m_position = glm::vec3(0.0f, 3.0f, -8.0f);
	m_targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	m_projection = glm::perspectiveFovLH(m_fov, (float)m_width, (float)m_height, m_nearPlane, m_farPlane);
}


Lime::DX11Camera::~DX11Camera()
{
}

glm::mat4 Lime::DX11Camera::GetWorldMaxrix()
{
	return glm::mat4();
}

glm::mat4 Lime::DX11Camera::GetViewMatrix()
{
	m_view = glm::lookAtLH(m_position, m_targetPos, m_UpDirection);
	return m_view;
}

glm::mat4 Lime::DX11Camera::GetProjectionMatrix()
{
	return m_projection;
}

glm::mat4 Lime::DX11Camera::GetWVPnMatrix()
{
	return GetViewMatrix() * GetProjectionMatrix();
}