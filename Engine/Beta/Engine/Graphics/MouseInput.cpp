#include "MouseInput.h"
#include <glm\gtx\transform.hpp>

using namespace Graphics;


glm::vec2 Graphics::MouseInput::GetPositon()
{
	std::lock_guard<std::mutex> lock(m_mouse_lock);
	return m_position;
}

glm::vec3 Graphics::MouseInput::Get3DPosition()
{
	glm::vec3 position;
	if (m_camera)
	{
		std::lock_guard<std::mutex> lock(m_mouse_lock);
		glm::vec3 origin = m_camera->GetPosition();
		glm::vec3 mousePosition = GetWorldSpaceCoords();
		glm::vec3 planeNormal = glm::vec3(0, 1, 0);
		float farDistance = m_camera->GetFarPlane() / 2;
		float DotPt1 = glm::dot(origin, planeNormal);
		float DotPt2 = glm::dot(mousePosition, planeNormal);
		float distanceToPlane = 0.0f - (DotPt1 / DotPt2);
		glm::vec3 mouse3Dposition = origin + (mousePosition * distanceToPlane);
		position = mouse3Dposition;
	}
	return position;
}

glm::vec3 Graphics::MouseInput::Get3DPosition_2()
{
	glm::vec3 position;
	if (m_camera)
	{
		std::lock_guard<std::mutex> lock(m_mouse_lock);
		glm::vec3 cameraPos = m_camera->GetPosition();
		glm::vec3 mousePos = GetWorldSpaceCoords();
		float t = -cameraPos.y / mousePos.y;
		glm::vec3 mousePosition = cameraPos + (mousePos * t);
		mousePosition.y = 0.0f;
		position = mousePosition;
	}
	return position;
}

void Graphics::MouseInput::AttatchCamera(std::shared_ptr<Graphics::Camera>& camera)
{
	std::lock_guard<std::mutex> lock(m_mouse_lock);
	m_camera = camera;
}

void Graphics::MouseInput::SetPosition(float x, float y)
{
	std::lock_guard<std::mutex> lock(m_mouse_lock);
	m_position.x = x;
	m_position.y = y;
}

glm::vec2 Graphics::MouseInput::GetTranslatedCoords()
{
	glm::vec2 mousePosition;
	if (m_camera)
	{
		auto mousePos = GetPositon();
		mousePosition.x = (2.0f * mousePos.x) / m_camera->GetWindowWidth() - 1.0f;
		mousePosition.y = 1.0f - (2.0f * mousePos.y) / m_camera->GetWindowHeight();
	}
	return mousePosition;
}

glm::vec4 Graphics::MouseInput::GetEyeSpaceCoords(glm::vec4 position)
{
	glm::vec4 EyeSpaceCoords;
	if (m_camera)
	{
		EyeSpaceCoords = glm::inverse(m_camera->Get3DProjectionMatrix()) * position;
		EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
	}
	return EyeSpaceCoords;
}

glm::vec3 Graphics::MouseInput::GetWorldSpaceCoords()
{
	glm::vec3 worldCoords;
	if (m_camera)
	{
		//Eye space
		glm::vec2 mousePos = GetPositon();
		glm::vec2 normalized = GetTranslatedCoords();
		glm::vec4 clipCoords = glm::vec4(normalized, -1.0f, 1.0f);
		glm::vec4 eyeCoords = GetEyeSpaceCoords(clipCoords);

		//World space
		glm::vec4 mouseCoords = glm::inverse(m_camera->GetViewMatrix()) * eyeCoords;
		worldCoords = glm::vec3(mouseCoords.x, mouseCoords.y, mouseCoords.z);
		worldCoords = glm::normalize(worldCoords);
	}
	return worldCoords;
}