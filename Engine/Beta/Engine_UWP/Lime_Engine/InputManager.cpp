#include "InputManager.h"

InputManager::InputManager()
{
	m_MouseCoords = glm::vec2(0.0f, 0.0f);
	m_Camera = nullptr;
}

void InputManager::Initialize(std::shared_ptr<Graphics::Camera>& camera)
{
	m_Camera = camera;
}

const glm::vec2 InputManager::GetMouseCoords()
{
	return m_MouseCoords;
}

glm::vec3 InputManager::GetMouseCoordsWorldSpace()
{
	glm::vec3 worldCoords;
	if (m_Camera)
	{
		glm::vec2 mousePos = GetMouseCoords();
		glm::vec2 normalized = NormalizeDeviceMouseCoords(mousePos);
		glm::vec4 clipCoords = glm::vec4(normalized, -1.0f, 1.0f);
		glm::vec4 eyeCoords = GetEyeSpaceCoords(clipCoords);
		worldCoords = GetWorldSpaceCoords(eyeCoords);
	}
	return worldCoords;
}

bool InputManager::GetMouse3DPosition(_Inout_ glm::vec3& pos)
{
	bool result = false;
	if (m_Camera)
	{
		glm::vec3 origin = m_Camera->GetPosition();
		glm::vec3 mousePosition = GetMouseCoordsWorldSpace();
		glm::vec3 planeNormal = glm::vec3(0, 1, 0);
		float farDistance = m_Camera->GetFarPlane() / 2;
		float DotPt1 = glm::dot(origin, planeNormal);
		float DotPt2 = glm::dot(mousePosition, planeNormal);
		float distanceToPlane = 0.0f - (DotPt1 / DotPt2);
		glm::vec3 mouse3Dposition = origin + (mousePosition * distanceToPlane);
		pos = mouse3Dposition;
		result = true;
	}
	return result;
}

bool InputManager::GetMouse3DPosition2(_Inout_ glm::vec3& pos)
{
	bool result = false;
	if (m_Camera)
	{
		glm::vec3 cameraPos = m_Camera->GetPosition();
		glm::vec3 mousePos = GetMouseCoordsWorldSpace();
		float t = -cameraPos.y / mousePos.y;
		glm::vec3 mousePosition = cameraPos + (mousePos * t);
		mousePosition.y = 0.0f;
		pos = mousePosition;
		result = true;
	}
	return result;
}

void InputManager::SetMouseCoords(float x, float y)
{
	m_MouseCoords.x = (float)x;
	m_MouseCoords.y = (float)y;
}

glm::vec2 InputManager::NormalizeDeviceMouseCoords(glm::vec2 & mousePos)
{
	glm::vec2 mousePosition;
	if (m_Camera)
	{
		mousePosition.x = (2.0f * mousePos.x) / m_Camera->GetWindowWidth() - 1.0f;
		mousePosition.y = 1.0f - (2.0f * mousePos.y) / m_Camera->GetWindowHeight();
	}
	return mousePosition;
}

glm::vec4 InputManager::GetEyeSpaceCoords(glm::vec4 mouseCoords)
{
	glm::vec4 EyeSpaceCoords;
	if (m_Camera)
	{
		EyeSpaceCoords = glm::inverse(m_Camera->Get3DProjectionMatrix()) * mouseCoords;
		EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
	}
	return EyeSpaceCoords;
}

glm::vec3 InputManager::GetWorldSpaceCoords(glm::vec4 eyeCoords)
{
	glm::vec3 worldCoords;
	if (m_Camera)
	{
		glm::vec4 mouseCoords = glm::inverse(m_Camera->GetViewMatrix()) * eyeCoords;
		worldCoords = glm::vec3(mouseCoords.x, mouseCoords.y, mouseCoords.z);
		worldCoords = glm::normalize(worldCoords);
	}
	return worldCoords;
}

void InputManager::SetKeyUp(const int keyID)
{
	if (m_KeysDown[keyID] == true)
		m_KeysPressed[keyID] = true;

	m_KeysDown[keyID] = false;
}

void InputManager::SetKeyDown(const int iKeyCode)
{
	m_KeysDown[iKeyCode] = true;
}

bool InputManager::KeyStatus(const int iKeyCode)
{
	auto iterator = m_KeysDown.find((int)iKeyCode);
	if (iterator != m_KeysDown.end())
	{
		return iterator->second;
	}
	return false;
}

bool InputManager::KeyPressed(const int iKeyCode)
{
	bool bPressed = false;
	auto result = m_KeysPressed.find((int)iKeyCode);
	if (result != m_KeysPressed.end())
	{
		bPressed = result->second;
		result->second = false;
	}
	return bPressed;
}

void InputManager::Reset()
{
	m_KeysDown.clear();
	m_KeysPressed.clear();
}