#include "DX11Camera.h"



Lime::DX11Camera::DX11Camera(int width, int height)
{
	m_width = width;
	m_height = height;
	m_fov = 3.14f / 2.0f;
	m_aspectRatio = (float)m_width / m_height;
	m_farPlane = 1000.0f;
	m_nearPlane = 1.0f;
	m_position = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	m_targetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_UpDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_projection = XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}


Lime::DX11Camera::~DX11Camera()
{
}

XMMATRIX Lime::DX11Camera::GetWorldMaxrix()
{
	return XMMatrixIdentity();
}

XMMATRIX Lime::DX11Camera::GetViewMatrix()
{
	m_view = XMMatrixLookAtLH(m_position, m_targetPos, m_UpDirection);
	return m_view;
}

XMMATRIX Lime::DX11Camera::GetProjectionMatrix()
{
	return m_projection;
}

XMMATRIX Lime::DX11Camera::GetWVPnMatrix()
{
	return GetViewMatrix() * GetProjectionMatrix();
}
