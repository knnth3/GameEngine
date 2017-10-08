#include "Camera.h"
#include <glm\gtx\vector_angle.hpp>

#undef near
#undef far

namespace Lime
{

	void Camera::Initialize(UINT windowWidth, UINT windowHeight)
	{
		m_info.m_distanceFromObject = 20.0f;
		m_info.m_targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_info.m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		SetResolution(windowWidth, windowHeight);
		CreateProjectionMatrix();
	}

	void Camera::AttachToModel(std::shared_ptr<Model::Model3D>& model)
	{
		m_info.m_model = model;
	}

	void Camera::SetViewDistance(float near, float far)
	{
		m_info.m_nearPlane = near;
		m_info.m_farPlane = far;
	}

	void Camera::SetFOV(float fov)
	{
		m_info.m_fov = fov;
	}

	void Camera::Move(float x, float y, float z)
	{
		float speed = 5.0f;
		m_info.m_position.x += x / speed;
		m_info.m_position.y += y / speed;
		m_info.m_position.z += z / speed;
	}

	void Camera::SetPosition(glm::vec3 position)
	{
		m_info.m_position = position;
	}

	void Camera::Rotate(float x, float y, float z)
	{
		float circle = (2.0f*PI);
		float xLimitUp = (PI / 2.0f);
		float xLimitDown = 0.0f;

		m_info.m_angleAroundPlayer -= x;
		m_info.m_rotation.x += y;

		if (m_info.m_rotation.x > xLimitUp)
			m_info.m_rotation.x = xLimitUp;

		if (m_info.m_rotation.x < xLimitDown)
			m_info.m_rotation.x = xLimitDown;

		if (m_info.m_angleAroundPlayer >= circle)
			m_info.m_angleAroundPlayer -= circle;
	}

	void Camera::SetRotation(glm::vec3 rotation)
	{
		float circle = (2.0f*PI);
		if (rotation.x >= circle)
			rotation.x -= circle;

		if (rotation.y >= circle)
			rotation.y -= circle;

		if (rotation.z >= circle)
			rotation.z -= circle;
		m_info.m_rotation = rotation;
	}

	void Camera::SetResolution(UINT width, UINT height)
	{
        m_info.m_xResolution = width;
        m_info.m_yResolution = height;
		m_info.m_aspectRatio = ((float)m_info.m_xResolution) / m_info.m_yResolution;
    }

    void Camera::Zoom(float x)
    {
		float maxZoomOut = 30.0f;
		float maxZoomIn = 1.5f;

		m_info.m_distanceFromObject += x;
        if (m_info.m_distanceFromObject < maxZoomIn)
			m_info.m_distanceFromObject = maxZoomIn;
   //     else if (m_info.m_distanceFromObject > maxZoomOut)
			//m_info.m_distanceFromObject = maxZoomOut;
    }

	void Camera::AddPitch(float pitch)
	{
		m_info.m_rotation.x += pitch;
	}

	void Camera::AddYaw(float yaw)
	{
		m_info.m_angleAroundPlayer += yaw;
	}

    glm::vec3 Camera::GetPosition()
    {
        return m_info.m_position;
    }

    glm::mat4 Camera::GetViewMatrix()
    {
		float horis = CalculateHorisDistance();
		float vert = CalculateVertDistance();
		CalculatePosition(horis, vert);
		CreateViewMatrix();
        return m_info.m_view;
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        return m_info.m_projection;
    }

    float Camera::GetFarPlane()
    {
        return m_info.m_farPlane;
    }

    unsigned int Camera::GetWindowWidth()
    {
        return m_info.m_xResolution;
    }

    unsigned int Camera::GetWindowHeight()
    {
        return m_info.m_yResolution;
    }

    void Camera::CreateViewMatrix()
    {
		glm::mat4 basicMatrix;
		glm::mat4 rotX = glm::rotate(basicMatrix, m_info.m_rotation.x , glm::vec3(1, 0, 0));
		glm::mat4 rotY = glm::rotate(rotX, m_info.m_rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZ = glm::rotate(rotY, m_info.m_rotation.z, glm::vec3(0, 0, 1));
		glm::vec3 negPos = m_info.m_position * -1.0f;
		m_info.m_view = glm::translate(rotZ, negPos);
    }

    void Camera::CreateProjectionMatrix()
    {
		m_info.m_projection = glm::infinitePerspective(m_info.m_fov, m_info.m_aspectRatio, m_info.m_nearPlane);
    }

    void Camera::CalculatePosition(float horizontalDistance, float verticalDistance)
    {
		if (m_info.m_model != nullptr)
		{
			float theta = m_info.m_angleAroundPlayer;
			float offsetX = horizontalDistance * sinf(theta);
			float offsetZ = horizontalDistance * cosf(theta);
			m_info.m_rotation.y = PI - m_info.m_angleAroundPlayer;
			m_info.m_position.x = m_info.m_model->GetPosition().x - offsetX;
			m_info.m_position.y = m_info.m_model->GetPosition().y + verticalDistance;
			m_info.m_position.z = -m_info.m_model->GetPosition().z - offsetZ;
			if (!m_info.m_bPlayerAttached)
			{
				AddPitch(PI/4);
				AddYaw(PI);
				m_info.m_bPlayerAttached = true;
			}
		}
    }

	float Camera::CalculateVertDistance()
	{
		return m_info.m_distanceFromObject * sinf(m_info.m_rotation.x);
	}

	float Camera::CalculateHorisDistance()
	{
		return m_info.m_distanceFromObject * cosf(m_info.m_rotation.x);
	}

}