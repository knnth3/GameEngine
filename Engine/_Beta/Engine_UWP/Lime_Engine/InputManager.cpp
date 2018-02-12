#include "InputManager.h"
#include "EngineResources.h"

LIME_ENGINE::InputManager::InputManager()
{
	m_MouseCoords = glm::vec2(0.0f, 0.0f);
	m_Camera = nullptr;
	m_rmbPressed = false;
	m_lmbPressed = false;
	m_lmbDown = false;
	m_rmbDown = false;
}

void LIME_ENGINE::InputManager::Initialize(std::shared_ptr<Graphics::Camera>& camera)
{
	m_Camera = camera;
}

const glm::vec2 LIME_ENGINE::InputManager::GetMouseCoords()
{
	return m_MouseCoords;
}

glm::vec3 LIME_ENGINE::InputManager::GetMouseCoordsWorldSpace()
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

bool LIME_ENGINE::InputManager::GetMouse3DPosition(_Inout_ glm::vec3& pos)
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

bool LIME_ENGINE::InputManager::GetMouse3DPosition2(_Inout_ glm::vec3& pos)
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

void LIME_ENGINE::InputManager::SetMouseCoords(float x, float y)
{
	m_MouseCoords.x = (float)x;
	m_MouseCoords.y = (float)y;
}

void LIME_ENGINE::InputManager::SetLMBStatus(bool val)
{
	if (m_lmbDown && !val)
	{
		m_lmbPressed = true;
	}
	m_lmbDown = val;
}

void LIME_ENGINE::InputManager::SetRMBStatus(bool val)
{
	if (m_rmbDown && !val)
	{
		m_rmbPressed = true;
	}
	m_rmbDown = val;
}

glm::vec2 LIME_ENGINE::InputManager::NormalizeDeviceMouseCoords(glm::vec2 & mousePos)
{
	glm::vec2 mousePosition;
	if (m_Camera)
	{
		mousePosition.x = (2.0f * mousePos.x) / m_Camera->GetWindowWidth() - 1.0f;
		mousePosition.y = 1.0f - (2.0f * mousePos.y) / m_Camera->GetWindowHeight();
	}
	return mousePosition;
}

glm::vec4 LIME_ENGINE::InputManager::GetEyeSpaceCoords(glm::vec4 mouseCoords)
{
	glm::vec4 EyeSpaceCoords;
	if (m_Camera)
	{
		EyeSpaceCoords = glm::inverse(m_Camera->Get3DProjectionMatrix()) * mouseCoords;
		EyeSpaceCoords = glm::vec4(EyeSpaceCoords.x, EyeSpaceCoords.y, -1.0f, 0.0f);
	}
	return EyeSpaceCoords;
}

glm::vec3 LIME_ENGINE::InputManager::GetWorldSpaceCoords(glm::vec4 eyeCoords)
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

void LIME_ENGINE::InputManager::SetKeyUp(const int keyID)
{
	if (m_KeysDown[keyID] == true)
		m_KeysPressed[keyID] = true;

	m_KeysDown[keyID] = false;
}

void LIME_ENGINE::InputManager::SetKeyDown(const int iKeyCode)
{
	m_KeysDown[iKeyCode] = true;
}

bool LIME_ENGINE::InputManager::KeyStatus(const int iKeyCode)
{
	auto iterator = m_KeysDown.find((int)iKeyCode);
	if (iterator != m_KeysDown.end())
	{
		return iterator->second;
	}
	return false;
}

bool LIME_ENGINE::InputManager::KeyPressed(const int iKeyCode)
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

bool LIME_ENGINE::InputManager::LMBPressed()
{
	bool val = m_lmbPressed;
	m_lmbPressed = false;
	return val;
}

bool LIME_ENGINE::InputManager::LMBStatus()
{
	return m_lmbDown;
}

bool LIME_ENGINE::InputManager::RMBPressed()
{
	bool val = m_rmbPressed;
	m_rmbPressed = false;
	return val;
}

bool LIME_ENGINE::InputManager::RMBStatus()
{
	return m_rmbDown;
}

void LIME_ENGINE::InputManager::Reset()
{
	m_KeysDown.clear();
	m_KeysPressed.clear();
}
